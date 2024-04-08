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

#include "secure.h"    /* Header file with secure interface API */
#include "saml11e16a.h"
#include <stdbool.h>

//#define BENCHMARK_STACK

//#define CRYPTO_HMAC_SHA256
//#define CRYPTO_ED25519
//#define CRYPTO_SPHINCS

#include "../Crypto/hmac-sha256/hmac-sha256.h"

typedef void (*void_function) () __attribute__((cmse_nonsecure_call));


#define SYM_KEY_SIZE 16
static uint8_t key[SYM_KEY_SIZE] = {0};
	
// Correspond to the initial state: empty Map (i.e., all zeroes);
// 0xc7, 0x57, 0x0c, 0x20, 0x8a, 0xaf, 0x8b, 0xcd, 0x75, 0xbd, 0x89, 0x51, 0xc1, 0xd6, 0x43, 0x9b, 0x7b, 0xf6, 0x2f, 0xaa, 0x1b, 0xcd, 0xc1, 0x6d, 0x5e, 0x30, 0x7f, 0xa0, 0x80, 0x7f, 0x3f, 0xf2
uint8_t secure_state[32] = {0xc7, 0x57, 0x0c, 0x20, 0x8a, 0xaf, 0x8b, 0xcd, 0x75, 0xbd, 0x89, 0x51, 0xc1, 0xd6, 0x43, 
	0x9b, 0x7b, 0xf6, 0x2f, 0xaa, 0x1b, 0xcd, 0xc1, 0x6d, 0x5e, 0x30, 0x7f, 0xa0, 0x80, 0x7f, 0x3f, 0xf2};
uint8_t exec = 0, stateUsed = 0, stateChecked = 0;

int setState(uint8_t *state, int state_size) {
	if(stateChecked != 1 || exec != 1) return 0;
	sha256 hash;
	sha256_initialize(&hash);
	sha256_finalize(&hash, state, state_size);
	memcpy(secure_state, hash.hash, 32);
	return 1;
}

int checkState(uint8_t *state, int state_size) {
	sha256 hash;
	sha256_initialize(&hash);
	sha256_finalize(&hash, state, state_size);
	uint8_t cmp = (uint8_t) memcmp(secure_state, hash.hash, 32);
	stateChecked = (cmp == 0);
	stateUsed = 1;
}
	
uint32_t my_counter = 0;
	
int verify_req(pox_call_t *pox_fn, uint8_t *pox_token) {
	if(pox_fn->counter < my_counter) return 0;
	
	sha256 hash;
	sha256_initialize(&hash);
	sha256_update(&hash, pox_fn->command, pox_fn->command_size);
	sha256_update(&hash, &(pox_fn->counter), 4);
	sha256_update(&hash, &(pox_fn->addr), 4);
	sha256_finalize(&hash, pox_fn->input, pox_fn->input_size);
	
	hmac_sha256 hmac;
	hmac_sha256_initialize(&hmac, key, 16);
	hmac_sha256_finalize(&hmac, hash.hash, SHA256_DIGEST_SIZE);
	for (int i = 0; i < SHA256_DIGEST_SIZE; ++i) 
		if(pox_token[i] != hmac.digest[i]){
			pox_fn->output[0] = 0xff;
			return 0;
		}
	pox_fn->output[0] = 0xaa;
	return 1;
}

int check_request(pox_call_t *pox_fn, uint8_t *pox_token) {
	return (my_counter < pox_fn->counter) && verify_req(pox_fn, pox_token);
}

void init_flags() {
	exec = 1; stateUsed = 0; stateChecked = 0;
}

void reset_flags() {
	exec = 0; stateUsed = 0; stateChecked = 0;
}
	
void pox(pox_call_t *pox_fn, uint8_t *pox_token) {
	
	uint8_t *mem = (uint8_t*) 0x8000;
	int mem_len = 14980; //0x8000;
	//int code_size = 7924;
	//memset(mem+code_size, 0, mem_len-code_size);
	
	// Verify request
	if(check_request(pox_fn, pox_token) == 0) {
		return;
	}
	
	my_counter = pox_fn->counter;
	init_flags();
		
	// h <- Hash(PMEM, F, I, c)
	
	sha256 hash;
	sha256_initialize(&hash);
	sha256_update(&hash, &(pox_fn->counter), 4);
	sha256_update(&hash, &(pox_fn->addr), 4);
	sha256_update(&hash, pox_fn->input, pox_fn->input_size);
	sha256_update(&hash, mem, mem_len);
	
	__disable_irq();
	void_function fn = (void_function)(pox_fn->addr);
	fn();
	
	if(exec && stateUsed && !stateChecked) {
		pox_fn->output[0] = 0xab;
		reset_flags();
		__enable_irq();
		return;
	}
	
	// h <- Hash(h, O)
	sha256_finalize(&hash, pox_fn->output, pox_fn->output_size);
	
	// Sign(h)
	hmac_sha256 hmac;
	hmac_sha256_initialize(&hmac, key, SYM_KEY_SIZE);
	hmac_sha256_finalize(&hmac, hash.hash, SHA256_DIGEST_SIZE);
	memcpy(pox_token, hmac.digest, SHA256_DIGEST_SIZE);
	//memcpy(pox_token, hash.hash, SHA256_DIGEST_SIZE);
	
	reset_flags();
	__enable_irq();
}
