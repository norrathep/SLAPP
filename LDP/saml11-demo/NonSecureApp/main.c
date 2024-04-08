/**
 * Copyright (c) 2019 Microchip Technology Inc.
 *
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

//#include "sam.h"

#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "saml11e16a.h"
#include "pox.h"
#include "veneer.h"
#include "gpio/hal_gpio.h"
#include "gpio/uart.h"
#include "gpio/delay.h"
#include "gpio/timer.h"

//#define ATTACK_A1
#define ATTACK_A2
//#define ATTACK_A3

//-----------------------------------------------------------------------------

#define POX_INPUT_SIZE 4*3
#define POX_OUTPUT_SIZE 32

uint8_t pox_input[POX_INPUT_SIZE] = {0};
uint8_t pox_output[POX_OUTPUT_SIZE] = {0};
	
#define SHA256_DIGEST_SIZE 32
#include "hmac-sha256.h"

#define SYM_KEY_SIZE 16
static uint8_t key[SYM_KEY_SIZE] = {0};
#define TOKEN_SIZE SHA256_DIGEST_SIZE
#define PLAINTEXT_SIZE 5+4*5
#define REQUEST_SIZE PLAINTEXT_SIZE+TOKEN_SIZE

void printBits(uint8_t num)
{
   for(int bit=0;bit<(sizeof(uint8_t) * 8); bit++)
   {
      printf("%i", num & 0x01);
      num = num >> 1;
   }
    printf(" ");
}

void printVector(const uint8_t b[32], char *meta) {
    printf("%s: ", meta);
    for(int i=0; i<32; i++) printBits(b[i]);
    printf("\n");
}

bool is_bit_set(uint8_t num, int n) {
	// Create a mask with only the nth bit set
	uint8_t mask = 1 << n;
	// Perform a bitwise AND operation with the original number
	// If the result is non-zero, then the nth bit is set
	return (num & mask) != 0;
}

uint8_t set_bit(uint8_t num, int n) {
	// Left shift 1 by n to create a mask with the nth bit set
	uint8_t mask = 1 << n;
	// Perform bitwise OR operation to set the nth bit
	return num | mask;
}

uint8_t clear_bit(uint8_t num, int n) {
	// Create a mask with only the nth bit set to 1
	uint8_t mask = ~(1 << n);
	// Perform a bitwise AND operation with the original number
	// This will clear the nth bit while leaving other bits unchanged
	return num & mask;
}

#define MAP_SIZE 20 // Maximum number of elements in the map

// Define a structure to represent key-value pair
typedef struct {
	uint8_t key[32];
	uint8_t value[32];
} KeyValuePair;

// Define a map data structure
typedef struct {
	KeyValuePair pairs[MAP_SIZE];
	size_t size; // Current number of elements in the map
} Map;

Map B;

// Function to initialize the map
void initialize_map(Map *map) {
	map->size = 0;
}

// Function to insert a key-value pair into the map
void insert(Map *map, const uint8_t key[32], const uint8_t value[32]) {
	if (map->size < MAP_SIZE) {
		KeyValuePair *pair = &map->pairs[map->size];
		for (int i = 0; i < 32; i++) {
			pair->key[i] = key[i];
			pair->value[i] = value[i];
		}
		map->size++;
	} else {
		uart_puts("Map is full. Cannot insert.\n");
	}
}

// Function to retrieve a value given a key
uint8_t* get(Map *map, const uint8_t key[32]) {
	for (size_t i = 0; i < map->size; i++) {
		int found = 1;
		for (int j = 0; j < 32; j++) {
			if (map->pairs[i].key[j] != key[j]) {
				found = 0;
				break;
			}
		}
		if (found) {
			return map->pairs[i].value;
		}
	}
	return NULL; // Key not found
}

void UE(uint8_t O_prime, uint8_t *b) {
    uint8_t bin = O_prime/8;
	uint8_t bit = O_prime%8;
	b[bin] = 1<<bit;
}

uint8_t* PRR(const uint8_t *b, float f, Map *B, uint8_t *b_prime) {
    memset(b_prime, 0, 32);
	uint8_t *cache = get(B, b);
	if(cache == NULL) {
		for(int i=0; i<256; i++) {
			float r = (float) rand()/(float) RAND_MAX;
			bool isOne = false;
			uint8_t bin = i/8;
			uint8_t bit = i%8;
			if(r<=f/2) {
				b_prime[bin] = set_bit(b[bin], bit);
			} else if(r<=f) {
				b_prime[bin] = clear_bit(b[bin], bit);
			} else if(is_bit_set(b[bin], bit)) {
				b_prime[bin] = set_bit(b[bin], bit);
			} else {
				b_prime[bin] = set_bit(b[bin], bit);
			}
		}
		insert(B, b, b_prime);
        return b_prime;
	} else {
        return cache;
    }
	
}

void IRR(const uint8_t *b_prime, float p, float q, uint8_t *O) {
	for(int i=0; i<256; i++) {
		uint8_t bin = i/8;
		uint8_t bit = i%8;
		float r = (float) rand()/(float) RAND_MAX;
		bool bit_set = is_bit_set(b_prime[bin], bit);
		if((r <= p && bit_set) || (r <= q && bit_set)) {
			O[bin] = set_bit(b_prime[bin], bit);
		} else {
			O[bin] = clear_bit(b_prime[bin], bit);
		}
	}

}

void initB() {
	memset(&B, 0, sizeof(Map));
	#ifdef ATTACK_A1
	memset(B.pairs[0].key, 1, 32);
	memset(B.pairs[0].value, 1, 32);
	#endif
}

void LDP_DC(uint8_t inp, float f, float p, float q) {
	uint8_t O_prime = inp;
	uint8_t b[32] = {0}, b_prime[32] = {0}, O[32] = {0};
	uint8_t bin, bit;
	// 256-bit vector since O_prime ranges from 0 to 2^8-1=255
	// b <- UE(O_prime)
    UE(O_prime, b);
	
	// b' <- PRR(b, f, B)
    uint8_t *b_prime_out = PRR(b, f, &B, b_prime);

    // O <- IRR(b', p, q)
    IRR(b_prime_out, p, q, O);
	
    memcpy(pox_output, O, 32);
}

void print_token(uint8_t *token, int size) {
	char hex[3]={0};
	uart_puts("0x");
	for(int i=0; i<size; i++) {
		sprintf(hex, "%.2x", token[i]);
		uart_puts(hex);
	}
	uart_puts("\n");
}

int is_pzem_online() {
	//uart_puts("not online?\n");
	uint8_t c1 = uart_getc();
	if(c1 != '<') return 0;
	c1 = uart_getc();
	if(c1 != 'p') return 0;
	c1 = uart_getc();
	if(c1 != '>') return 0;
	
	return 1;
}

void LDP_energy(float f, float p, float q) {
	char arr[5];
	for(;;) {
		// Get energy value from PZEM-004T
		// Arduino sends in the form of <p>val</p>
		uint8_t c1 = uart_getc();
		if(c1 != '<') continue;
		c1 = uart_getc();
		if(c1 != 'p') continue;
		c1 = uart_getc();
		if(c1 != '>') continue;
		
		uart_get_bytes(arr,3);
		uart_puts("[S]");
		arr[3] = '\0';
		uint8_t energy = (uint8_t) atoi(arr);
		uart_puti(energy);
		
		#ifdef ATTACK_A2
		energy += 23;
		#endif
		
		LDP_DC(energy, f, p, q);
		uart_puts("[E]\n");
		
		break;
	}
	
}

void wrapper_LDP_energy() {
	secure_checkState(&B, sizeof(Map));
	
	uart_puts("[S]AA[E]\n");
	while(!is_pzem_online());
	uart_puts("[S]BB[E]\n");
	float f = *(float*)(pox_input);
	float p = *(float*)(pox_input+4);
	float q = *(float*)(pox_input+4*2);
	LDP_energy(f,p,q);
	secure_setState(&B, sizeof(Map));
}

void wrapper_initB() {
	secure_checkState(&B, sizeof(Map));
	initB();
	secure_setState(&B, sizeof(Map));
}


int h() {
	initB();
	sha256 hash;
	sha256_initialize(&hash);
	sha256_finalize(&hash, &B, sizeof(Map));
	uart_puts("printing init state\n");
	print_token(hash.hash, SHA256_DIGEST_SIZE);
	uart_puts("done init state\n");
}

void new_single() {
	
	for(volatile int i=0; i<10000; i++);
	uint8_t req[100]={};
	uart_get_bytes(req, REQUEST_SIZE);
	
	pox_call_t pc;
	pc.input = pox_input;
	pc.output = pox_output;
	pc.command_size = 5; // assume to be fixed
	pc.command = req; // either [stp] or [clt]
	if(memcmp(pc.command, "[stp]", pc.command_size) == 0) {
		uart_puts("[Test] Setup: ");
		uint32_t counter = *((uint32_t*)(req+5));
		uint32_t func = *((uint32_t*)(req+5+4));
		uart_puts("returning ");
		uart_put_bytes(req, REQUEST_SIZE);
		uart_puts(", counter=");
		uart_puti(counter);
		uart_puts(", func=");
		uart_puti(func);
		uart_puts(" = done  [/Test] \n");
		
		uint8_t *token = (uint8_t*) (req+PLAINTEXT_SIZE);
		
		pc.counter = counter;
		pc.addr = func;
		pc.input_size = 12;
		pc.output_size = 32;
		// pox_input = b'\x00'*12
		memcpy(pox_input, req+5+4*2, 12);
		
		secure_pox(&pc, token);
		
		// fault tolerance
		for(int i=0; i<10; i++) {
			uart_puts("[OS]");
			uart_put_bytes(pox_output, 32);
			uart_puts(" ");
			uart_put_bytes(token, 32);
			uart_puts("[OE]\n");
		}
		uart_puts("[Over]\n");
		
		#ifdef ATTACK_A3
		memset(&B, 1, sizeof(Map));
		#endif
		
	} else if(memcmp(pc.command, "[clt]", pc.command_size) == 0) {
		uart_puts("[Test] Collect: ");
		uint32_t counter = *((uint32_t*)(req+5));
		uint32_t func = *((uint32_t*)(req+5+4));
		float f = *((float*)(req+5+4*2));
		float p = *((float*)(req+5+4*3));
		float q = *((float*)(req+5+4*4));
		uart_puts("Returning ");
		uart_put_bytes(req, REQUEST_SIZE);
		uart_puts(", counter=");
		uart_puti(counter);
		uart_puts(", func=");
		uart_puti(func);
		uart_puts(", f=");
		uart_puti((int) (f*10000));
		uart_puts(", p=");
		uart_puti((int) (p*10000));
		uart_puts(", q=");
		uart_puti((int) (q*10000));
		uart_puts(" = done  [/Test] \n");
		
		uint8_t *token = (uint8_t*) (req+PLAINTEXT_SIZE);
		
		pc.counter = counter;
		pc.addr = func;
		pc.input_size = 12;
		pc.output_size = 32;
		// pox_input = f, p, q
		memcpy(pox_input, req+5+4*2, 12);
		
		secure_pox(&pc, token);
		
		for(int i=0; i<10; i++) {
			uart_puts("[OS]");
			uart_put_bytes(pox_output, 32);
			uart_puts(" ");
			uart_put_bytes(token, 32);
			uart_puts("[OE]\n");
		}
		uart_puts("[Over]\n");
	} else {
		uart_puts("failed to parse command: ");
		uart_puts(req);
		uart_puts("\n");
	}
	
}

// Discard everything until the message sent by Vrf
void read_until(char c1, char c2, char c3) {
	for(;;) {
		char r = uart_getc();
		if(r!=c1) continue;
		r = uart_getc();
		if(r!=c2) continue;
		r = uart_getc();
		if(r!=c3) continue;
		break;
	}
}

/* Non-Secure main() */
int main(void)
{
	
	//__disable_irq();
	timer_init(PERIOD_1S);
	
	uart_init(115200);
	uart_puts(" A lot of helloooooo ");
	uart_puts("wrapper_initB at: ");
	uart_puti(&wrapper_initB);
	uart_puts(" wrapper_LDP_energy at: ");
	uart_puti(&wrapper_LDP_energy);
	uart_puts("[Start] \n");
	
	for(;;) {
		new_single();
		read_until('O', 'A', 'K');
		uart_puts("[Next]\n");
	}
	
	/*pox_call_t pc;
	pc.counter = c;
	pc.addr = f;
	pox_input[0] = i;
	pc.input = pox_input;
	pc.input_size = 1;
	pc.output = pox_output;
	pc.output_size = 4;
	
	uart_puts("\n");
	
	uart_puts("SPHINCSTART\n");
	secure_pox(&pc, token);
	
	uart_puts("=========== Output ===========\n");
	print_token(pc.output, pc.output_size);
	
	uart_puts("=========== Output token ===========\n");
	print_token(token, TOKEN_SIZE);
	
	uart_puts("Stack used\n");
	uart_puti(*(uint32_t*)(pc.output));
	
	uart_puts("\n");
	uart_puts("Complete\n");
	return;*/
	
	
}
