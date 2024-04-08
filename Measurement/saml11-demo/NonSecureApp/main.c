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

#include "sphincs/oak_sphincs.h"
#include "ed25519/edsign.h"

//-----------------------------------------------------------------------------

#define POX_INPUT_SIZE 1
#define POX_OUTPUT_SIZE 1

uint8_t pox_input[POX_INPUT_SIZE] = {0};
uint8_t pox_output[POX_OUTPUT_SIZE] = {0};

uint8_t sum = 10;

static void test(uint8_t val) {
	secure_checkState(&sum, 1);
	uint8_t out;
	if(val == 0) out = HAL_GPIO_LED_read();
	else out = HAL_GPIO_BUTTON_read();
	sum += out;
	pox_output[0] = sum;
	uart_puts("\n PoX: ");
	uart_puti(sum);
	uart_puts("\n");
	secure_setState(&sum, 1);
}



static void wrapper_test() {
	test(pox_input[0]);
}

//#define CRYPTO_HMAC_SHA256
//#define CRYPTO_ED25519
#define CRYPTO_ECC

//#define CRYPTO_SPHINCS

#define SHA256_DIGEST_SIZE 32
#include "hmac-sha256.h"

#ifdef CRYPTO_HMAC_SHA256
#define SYM_KEY_SIZE 16
static uint8_t key[SYM_KEY_SIZE] = {0};
#define TOKEN_SIZE SHA256_DIGEST_SIZE

#elif defined(CRYPTO_ECC)
#define SK_SIZE 32
#define PK_SIZE 64
#define TOKEN_SIZE 64
static uint8_t vrf_pk[PK_SIZE] = {0x74, 0x2F, 0xBE, 0xAF, 0x4F, 0xF4, 0x1D, 0x27, 0x48, 
	0xAD, 0x4E, 0x23, 0x84, 0x79, 0xA4, 0x42, 0x68, 0x8B, 0xE6, 0x18, 0x98, 0x45, 0x99, 
	0xF1, 0x10, 0x03, 0x4E, 0xF6, 0x5E, 0xE1, 0x1E, 0x4F, 0x49, 0xD0, 0x8E, 0xE0, 0x53, 
	0xA0, 0xE2, 0x7D, 0xB5, 0x09, 0xF1, 0xFD, 0x2E, 0xA5, 0x63, 0x70, 0xC0, 0x5E, 0x20, 
	0x31, 0xC5, 0xC0, 0xCB, 0x4B, 0x01, 0x75, 0xD7, 0x38, 0xEB, 0x2B, 0x44, 0x1B};
static uint8_t prv_sk[SK_SIZE] = {0x89, 0x8B, 0x0C, 0xDE, 0xAB, 0xB5, 0x86, 0x97, 0x3C, 
	0x0D, 0xA9, 0x15, 0x8E, 0x01, 0x84, 0xE7, 0x43, 0xF7, 0x7C, 0x35, 0xB9, 0xC2, 0xB8, 
	0xF7, 0x93, 0x62, 0x92, 0x87, 0xCA, 0x59, 0x8B, 0xB2};
static uint8_t prv_pk[PK_SIZE] = {0x74, 0x2F, 0xBE, 0xAF, 0x4F, 0xF4, 0x1D, 0x27, 0x48,
	0xAD, 0x4E, 0x23, 0x84, 0x79, 0xA4, 0x42, 0x68, 0x8B, 0xE6, 0x18, 0x98, 0x45, 0x99,
	0xF1, 0x10, 0x03, 0x4E, 0xF6, 0x5E, 0xE1, 0x1E, 0x4F, 0x49, 0xD0, 0x8E, 0xE0, 0x53,
	0xA0, 0xE2, 0x7D, 0xB5, 0x09, 0xF1, 0xFD, 0x2E, 0xA5, 0x63, 0x70, 0xC0, 0x5E, 0x20,
	0x31, 0xC5, 0xC0, 0xCB, 0x4B, 0x01, 0x75, 0xD7, 0x38, 0xEB, 0x2B, 0x44, 0x1B};

#elif defined(CRYPTO_ED25519)
#define TOKEN_SIZE EDSIGN_SIGNATURE_SIZE
// Use the same keys for vrf and prv for now
static uint8_t vrf_pk[EDSIGN_PUBLIC_KEY_SIZE] = {0x28, 0xed, 0xbd, 0xf2, 0x1a, 0x31, 0xfc, 0xff,
	0x25, 0x8a, 0x90, 0x2b, 0xb5, 0xfe, 0x89, 0x2d,
	0x48, 0x5f, 0x48, 0xee, 0xe7, 0xc3, 0x50, 0x1e,
0xf1, 0x77, 0x7e, 0x28, 0x89, 0x7a, 0x4f, 0x16};
static uint8_t prv_sk[EDSIGN_SECRET_KEY_SIZE] = {0xf5, 0xe9, 0xe2, 0x5e, 0x53, 0x60, 0xaa, 0xd2,
	0xb2, 0xd0, 0x85, 0xfa, 0x54, 0xd8, 0x35, 0xe8,
	0xd4, 0x66, 0x82, 0x64, 0x98, 0xd9, 0xa8, 0x87,
0x75, 0x65, 0x70, 0x5a, 0x8a, 0x3f, 0x62, 0x80};
static uint8_t prv_pk[EDSIGN_PUBLIC_KEY_SIZE] = {0x28, 0xed, 0xbd, 0xf2, 0x1a, 0x31, 0xfc, 0xff,
	0x25, 0x8a, 0x90, 0x2b, 0xb5, 0xfe, 0x89, 0x2d,
	0x48, 0x5f, 0x48, 0xee, 0xe7, 0xc3, 0x50, 0x1e,
0xf1, 0x77, 0x7e, 0x28, 0x89, 0x7a, 0x4f, 0x16};

#endif

void print_token(uint8_t *token, int size) {
	char hex[3]={0};
	uart_puts("0x");
	for(int i=0; i<size; i++) {
		sprintf(hex, "%.2x", token[i]);
		uart_puts(hex);
	}
	uart_puts("\n");
}

int verify_req(pox_call_t *pox_fn, uint8_t *token) {
	
	
	/*uint32_t f, c;
	uint8_t i;
	c = *(uint32_t*)(req);
	f = *(uint32_t*)(req+4);
	i = *(uint32_t*)(req+8);
	
	sha256 hash;
	sha256_initialize(&hash);
	sha256_finalize(&hash, req, req_size);*/
	
	sha256 hash;
	sha256_initialize(&hash);
	sha256_update(&hash, &(pox_fn->counter), 4);
	sha256_update(&hash, &(pox_fn->addr), 4);
	sha256_finalize(&hash, pox_fn->input, pox_fn->input_size);
	
	print_token(hash.hash, SHA256_DIGEST_SIZE);
	print_token(token, TOKEN_SIZE);
	
	#ifdef CRYPTO_HMAC_SHA256
	hmac_sha256 hmac;
	hmac_sha256_initialize(&hmac, key, 16);
	hmac_sha256_finalize(&hmac, hash.hash, SHA256_DIGEST_SIZE);
	print_token(hmac.digest, SHA256_DIGEST_SIZE);
	for (int i = 0; i < SHA256_DIGEST_SIZE; ++i)
		if(token[i] != hmac.digest[i]) {
			return 0;
		}
	return 1;
	
	#elif defined(CRYPTO_ED25519)
	/*uint8_t sig[TOKEN_SIZE] = {0};
	edsign_sign(sig, prv_pk, prv_sk, hash.hash, SHA256_DIGEST_SIZE);
	print_token(sig, TOKEN_SIZE);
	uart_puts("so???\n");*/
	
	return edsign_verify(token, vrf_pk, hash.hash, SHA256_DIGEST_SIZE);
	
	#elif defined(CRYPTO_SPHINCS)
	struct ts_context verify_ctx;
	ts_init_verify( &verify_ctx, hash.hash, SHA256_DIGEST_SIZE, ps, vrf_pk);
	for (;;) {
		if (1 != ts_update_verify(token, 100, &verify_ctx )) return 0;
	}
	return ts_verify( &verify_ctx );
	#endif
}

extern uint8_t buffer[ 1024 ];
extern unsigned numSigs;
static uint8_t *get_sphinc_sig() {
	uart_puts("SPHINCSPHINC\n");
	unsigned n = 0;
	int rem = 17088-numSigs;
	if(rem > sizeof buffer) n = sizeof buffer;
	else n = rem;
	numSigs += n;
	uart_get_bytes(buffer, n);
	//out = buffer;
	uart_puti(n);
	uart_puts("\n");
	return buffer;
}

/* Non-Secure main() */
int main(void)
{
	
	//__disable_irq();
	timer_init(PERIOD_1S);
	
	uart_init(115200);
	uart_puts("A lot of helloooooo\n");
	wrapper_test();
	HAL_GPIO_LED_out();
	timer_reset();
	
	
	uint8_t req[100]={};
	uart_puts("Hi\n");
	uart_get_bytes(req, TOKEN_SIZE+9);
	
	uart_puts("Receive: ");
	uint32_t f, c;
	uint8_t i;
	c = *(uint32_t*)(req);
	f = *(uint32_t*)(req+4);
	i = *(uint32_t*)(req+8);
	uart_puti(c);
	uart_puts(" ");
	uart_puti(f);
	uart_puts(" ");
	uart_puti(i);
	uint8_t *token = (uint8_t*) (req+9);
	
	uart_puts("\n");
	uart_puts("get_sphinc_sig at: ");
	uart_puti(&get_sphinc_sig);
	uart_puts("\n");
	uart_puts("\n");
	uart_puts("Wrapper test at: ");
	uart_puti(&wrapper_test);
	uart_puts("\n");
	
	uart_puts("Start running sphincs\n");
	
	pox_call_t pc;
	pc.counter = c;
	pc.addr = f;
	pox_input[0] = i;
	pc.input = pox_input;
	pc.input_size = 1;
	pc.output = pox_output;
	pc.output_size = 4;
	
	uart_puts("\n");
	/*int pass = verify_req(&pc, token);
	uart_puts("Request passed? ");
	uart_puti(pass);
	uart_puts("\n");*/
	
	#ifdef CRYPTO_SPHINCS
	uart_puts("SPHINCSTART\n");
	#endif
	secure_pox(&pc, token);
	
	uart_puts("=========== Output ===========\n");
	print_token(pc.output, pc.output_size);
	
	uart_puts("=========== Output token ===========\n");
	print_token(token, TOKEN_SIZE);
	
	uart_puts("Stack used\n");
	uart_puti(*(uint32_t*)(pc.output));
	
	uart_puts("\n");
	return;
	
	
}
