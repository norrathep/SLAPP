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
//#include <time.h>
#include "saml11e16a.h"
#include "pox.h"
#include "veneer.h"
#include "gpio/hal_gpio.h"
#include "gpio/uart.h"
#include "gpio/delay.h"
//#include "gpio/timer.h"
#include "Tinn/Tinn.h"
//#include "genann/genann.h"

//#define ATTACK_A1
//#define ATTACK_A2
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
		
		//LDP_DC(energy, f, p, q);
		uart_puts("[E]\n");
		
		break;
	}
	
}

void wrapper_LDP_energy() {
	//secure_checkState(&B, sizeof(Map));
	
	while(!is_pzem_online());
	float f = *(float*)(pox_input);
	float p = *(float*)(pox_input+4);
	float q = *(float*)(pox_input+4*2);
	LDP_energy(f,p,q);
	//secure_setState(&B, sizeof(Map));
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

#define UsingTinn

#ifdef UsingTinn
#define INPUT_SIZE 16
#define HIDDEN_SIZE 8
#define OUTPUT_SIZE 1
#define FEATURE_SIZE 24 // how many readings per row

typedef struct
{ 
	float input[INPUT_SIZE][FEATURE_SIZE];
	float target[INPUT_SIZE][OUTPUT_SIZE];
	int curIdx;
} State;

State s;

int getBit(int num, int i) {
	return (num >> i) & 1;
}

void initialize() {
	char c = 'a';
	int idx = 0;
	for(int j=0; j<INPUT_SIZE; j++) {
		for(int i=0; i<FEATURE_SIZE; i++) {
			s.input[j][i] = (float)(c+idx);
			idx = (idx+1)%26;
			// simulate an anomaly
			if(j%4 == 0 && i==j%FEATURE_SIZE) s.input[j][i] = 100000.0f;
		}
		
		if(j%4==0) s.target[j][0] = 1.0f;
		else s.target[j][0] = 0.0f;
	}
	s.curIdx = INPUT_SIZE-2;
}

void collect() {
	
	char arr[5];
	float energyList[FEATURE_SIZE];
	int idx = 0;
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
		uart_puts("[E]\n");
		
		energyList[idx] = (float) (energy);
		idx++;
		if(idx == 1) break;
		//if(idx == FEATURE_SIZE) break;
	}
	memcpy(s.input[s.curIdx], energyList, FEATURE_SIZE);
	s.target[s.curIdx][0] = 0.0f; // assuming no anomaly
	s.curIdx = (s.curIdx+1)%INPUT_SIZE;
}

Tinn tinn;

void train(float init_val, int epochs, float rate) {
	setWeight(tinn, init_val);
	
	 //const float anneal = 0.99f;
	 for(int i = 0; i < epochs; i++)
	 {
		 //shuffle(data);
		 float error = 0.0f;
		 for(int j = 0; j < INPUT_SIZE; j++)
		 {
			 const float* const in = s.input[j];
			 const float* const tg = s.target[j];
			 error += xttrain(tinn, s.input[j], s.target[j], rate);
		 }
		 char buffer[50] = {0};
		 sprintf(buffer, "error %d :: learning rate %d\n", (int) (100*error / INPUT_SIZE), (int) (rate*100));
		 uart_puts(buffer);
		 //rate *= anneal;
	 }
	 
	 double err = 0;
	 int i;
	 for(int j = 0; j<INPUT_SIZE; j++) {
		 
		 float* in = s.input[j];
		 float* tg = s.target[j];
		 float* pd = xtpredict(tinn, s.input[j]);
		 uart_puts("Input:\n");
		 for(i=0; i<FEATURE_SIZE; i++) {
			 uart_puti((int) s.input[j][i]);
			 uart_puts(" ");
		 }
		 uart_puts("\nPD:");
		 for(i=0; i<OUTPUT_SIZE; i++) {
			 uart_puti((int) (pd[0]*100));
			 uart_puts(" ");
		 }
		 uart_puts("\nGT:");
		 for(i=0; i<OUTPUT_SIZE; i++) {
			 uart_puti(tg[i]*100);
			 uart_puts(" ");
		 }
		 uart_puts("\n");
	 }
	 
	 uart_puts("\n");
	 
}
#endif

void wrapper_train() {
	secure_checkState(&s, sizeof(State));
	float lr = *(float*)(pox_input);
	int numEpochs = (int) (*(float*)(pox_input+4));
	float init_val = *(float*)(pox_input+4*2);
	train(init_val, numEpochs, lr);
	secure_setState(&s, sizeof(State));
}

void wrapper_init() {
	secure_checkState(&s, sizeof(State));
	initialize();
	secure_setState(&s, sizeof(State));
}

void wrapper_collect() {
	secure_checkState(&s, sizeof(State));
	uart_puts("[S]AA[E]\n");
	while(!is_pzem_online());
	uart_puts("[S]BB[E]\n");
	collect();
	secure_setState(&s, sizeof(State));
}


/*int h() {
	memset(&s, 0, sizeof(State));
	sha256 hash;
	sha256_initialize(&hash);
	sha256_finalize(&hash, &s, sizeof(State));
	uart_puts("printing init state\n");
	print_token(hash.hash, SHA256_DIGEST_SIZE);
	uart_puts("done init state\n");
}*/

#define BENCHMARK_STACK
#ifdef BENCHMARK_STACK
// #define STACK_BASE 0x20000260 // HMAC
// #define STACK_BASE 0x200002b0 // ed25519
// #define STACK_BASE 0x200002f0 // sphincs
// #define STACK_BASE 0x20000328 // ecdsa
// TODO: figure out how to use these symbols
extern uint8_t _sstack, _estack;
#define STACK_BASE 0x20002ac8 // ED
#define STACK_SIZE 0x00001400
#define OFFSET 0
#define MAGIC 0xd9
uint32_t stack_pointer = 0;
uint32_t stack_usage = 0;

void fill_stack() {
	asm volatile ("mov %0, sp\n\t"
	: "=r" ( stack_pointer)
	);
	uint8_t* stack_base_address = &_sstack;
	uint8_t* stack_end_address = &_estack;
	for(uint32_t i=stack_pointer-0x10; i>=STACK_BASE; i--) {
		//for(uint32_t i=stack_pointer-0x10; i>=stack_base_address; i--) {
		uint8_t *mem = (uint8_t*)i;
		mem[0] = MAGIC;
	}
}

uint32_t stack_used() {
	uint32_t i;
	// TODO: for some reasons, STACK_BASE is modified...
	uint32_t start = 0x20002c50;
	for(i=start; i<stack_pointer-0x10; i++) {
		uint8_t *mem = (uint8_t*)i;
		if(mem[0] != MAGIC && mem[1] != MAGIC && mem[2] != MAGIC && mem[3] != MAGIC)
		break;
	}
	return (STACK_BASE+STACK_SIZE) - i;
}
#endif

void single() {
	for(volatile int i=0; i<10000; i++);
	uint8_t req[100]={};
	uart_get_bytes(req, REQUEST_SIZE);
	
	pox_call_t pc;
	pc.input = pox_input;
	pc.output = pox_output;
	pc.command_size = 5; // assume to be fixed
	pc.command = req; // either [stp] or [clt]
	if(memcmp(pc.command, "[stp]", pc.command_size) == 0 ||
	memcmp(pc.command, "[clt]", pc.command_size) == 0) {
		uart_puts("[Test] Setup/collect: ");
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
		
		uart_puts("[SStack] ");
		uart_puti(*(uint32_t*)(pc.output));
		uart_puts("[\SStack]\n");
		
		#ifdef BENCHMARK_STACK
		uint32_t stack_usage = stack_used();
		uart_puts("[NSStack] =");
		uart_puti(stack_usage);
		uart_puts("[\NSStack]\n");
		#endif
		
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
		memset(&s, 1, sizeof(State));
		#endif
		
	} else if(memcmp(pc.command, "[trn]", pc.command_size) == 0) {
		uart_puts("[Test] Training: ");
		uint32_t counter = *((uint32_t*)(req+5));
		uint32_t func = *((uint32_t*)(req+5+4));
		
		pc.counter = counter;
		pc.addr = func;
		pc.input_size = 12;
		pc.output = tinn.w;
		pc.output_size = tinn.nw*sizeof(float);
		
		float lr = *((float*)(req+5+4*2));
		int numEpochs = *((float*)(req+5+4*3));
		float init_val = *((float*)(req+5+4*4));
		
		int numAlloc = tinn.nw*4+tinn.nb*4+HIDDEN_SIZE*4+OUTPUT_SIZE*4;
		uart_puts("Returning ");
		uart_put_bytes(req, REQUEST_SIZE);
		uart_puts(", counter=");
		uart_puti(counter);
		uart_puts(", func=");
		uart_puti(func);
		uart_puts(", lr=");
		uart_puti((int) (lr*10000));
		uart_puts(", epoch=");
		uart_puti(numEpochs);
		uart_puts(", init_val=");
		uart_puti((int) (init_val*10000));
		uart_puts(", output size=");
		uart_puti(pc.output_size);
		uart_puts(", alloc size=");
		uart_puti(numAlloc);
		uart_puts(" = done  [/Test] \n");
		
		uint8_t *token = (uint8_t*) (req+PLAINTEXT_SIZE);
		// pox_input = f, p, q
		memcpy(pox_input, req+5+4*2, 12);
		
		secure_pox(&pc, token);
		
		uart_puts("[SStack] ");
		uart_puti(*(uint32_t*)(pc.output));
		uart_puts("[\SStack]\n");
		
		#ifdef BENCHMARK_STACK
		uint32_t stack_usage = stack_used();
		uart_puts("[NSStack] =");
		uart_puti(stack_usage);
		uart_puts("[\NSStack]\n");
		#endif
		
		for(int i=0; i<10; i++) {
			uart_puts("[OS]");
			uart_put_bytes(pc.output, pc.output_size);
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


/* Non-Secure main() */
int main(void)
{
	
	
	#ifdef BENCHMARK_STACK
	fill_stack();
	#endif
	//__disable_irq();
	//timer_init(PERIOD_1S);
	
	uart_init(115200);
	uart_puts(" A lot of hellooooo");
	uart_puts(" wrapper_init at: ");
	uart_puti(&wrapper_init);
	uart_puts(" wrapper_collect at: ");
	uart_puti(&wrapper_collect);
	uart_puts(" wrapper_train at: ");
	uart_puti(&wrapper_train);
	uart_puts("[Start] \n");
	
	//h();
	
	memset(&s, 0, sizeof(State));

	
	tinn = xtbuild(FEATURE_SIZE, HIDDEN_SIZE, OUTPUT_SIZE);
	/*wbrand(tinn);
	initialize();
	execute(0.1f, iterations, rate);
	uart_puti(sizeof(State));
	return;*/
	
	for(;;) {
		single();
		
		read_until('O', 'A', 'K');
		uart_puts("[Next]\n");
	}
	
	
}
