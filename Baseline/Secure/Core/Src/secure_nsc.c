/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    Secure/Src/secure_nsc.c
  * @author  MCD Application Team
  * @brief   This file contains the non-secure callable APIs (secure world)
  ******************************************************************************
    * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* USER CODE BEGIN Non_Secure_CallLib */
/* Includes ------------------------------------------------------------------*/
#include "secure_nsc.h"
#include "cfa_engine.h"
#include "usart.h"

/** @addtogroup STM32L5xx_HAL_Examples
  * @{
  */

/** @addtogroup Templates
  * @{
  */



/* Global variables ----------------------------------------------------------*/
void *pSecureFaultCallback = NULL;   /* Pointer to secure fault callback in Non-secure */
void *pSecureErrorCallback = NULL;   /* Pointer to secure error callback in Non-secure */
void *pAttestationFunctionCallback = NULL;
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Secure registration of non-secure callback.
  * @param  CallbackId  callback identifier
  * @param  func        pointer to non-secure function
  * @retval None
  */

CMSE_NS_ENTRY void SECURE_RegisterCallback(SECURE_CallbackIDTypeDef CallbackId, void *func){
  if(func != NULL)
  {
    switch(CallbackId)
    {
      case SECURE_FAULT_CB_ID:           /* SecureFault Interrupt occurred */
			pSecureFaultCallback = func;
			break;
      case GTZC_ERROR_CB_ID:             /* GTZC Interrupt occurred */
			pSecureErrorCallback = func;
			break;
      default:
        break;
    }
  }
}

CMSE_NS_ENTRY void SECURE_send(uint8_t* message, int size){
	HAL_UART_Transmit(&hlpuart1, message, size, HAL_MAX_DELAY);
};

CMSE_NS_ENTRY void SECURE_recv(uint8_t* message, int size){
	HAL_UART_Receive(&hlpuart1, message, size, HAL_MAX_DELAY);
};

CMSE_NS_ENTRY int SECURE_avg(int a, int b){
	return (a+b)/2;
};

#include <stdbool.h>

typedef void (*void_function) () __attribute__((cmse_nonsecure_call));
typedef uint8_t* (*getsig_function) () __attribute__((cmse_nonsecure_call));
typedef void (*printsig_function) (uint8_t*, int) __attribute__((cmse_nonsecure_call));

//#define BENCHMARK_STACK

//#define CRYPTO_HMAC_SHA256
//#define CRYPTO_ED25519
//#define CRYPTO_ECC
#define CRYPTO_SPHINCS

#define SHA256_DIGEST_SIZE 32
#include "./Crypto/hmac-sha256/hmac-sha256.h"

#ifdef CRYPTO_HMAC_SHA256
#define SYM_KEY_SIZE 16
static uint8_t symkey[SYM_KEY_SIZE] = {0};

#elif defined(CRYPTO_ED25519)
#include "./Crypto/ed25519/edsign.h"
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

#elif defined(CRYPTO_ECC)
#include "./Crypto/uecc/uECC.h"
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

#elif defined(CRYPTO_SPHINCS)
#include "./Crypto/sphincs/tiny_sphincs.h"
static unsigned char prv_sk[64] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
	0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
	0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
	0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x3b, 0x56, 0xe8, 0x16, 0x84, 0x7f, 0x00, 0x03,
0x86, 0xae, 0xec, 0x2e, 0x2b, 0xb9, 0xe1, 0xb5};
static unsigned char vrf_pk[32] = {0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
	0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x3b, 0x56, 0xe8, 0x16, 0x84, 0x7f, 0x00, 0x03,
0x86, 0xae, 0xec, 0x2e, 0x2b, 0xb9, 0xe1, 0xb5};
uint8_t optrand[32] = { 0x9e, 0x9c, 0x88, 0x01, 0xbb, 0x10, 0x31, 0x83,
0xbf, 0xd6, 0x2a, 0xc9, 0xed, 0x12, 0x65, 0x1f};
unsigned char optrand_buffer[32];
#endif

uint32_t my_counter = 0;

#if defined(CRYPTO_SPHINCS)
const struct ts_parameter_set *ps = &ts_ps_sha2_128f_simple;
#endif

extern unsigned get_sphinc_sig(uint8_t *out);

int verify_req(pox_call_t *pox_fn, uint8_t *pox_token) {
	if(pox_fn->counter < my_counter) return 0;

	sha256 hash;
	sha256_initialize(&hash);
	sha256_update(&hash, &(pox_fn->counter), 4);
	sha256_update(&hash, &(pox_fn->addr), 4);
	sha256_finalize(&hash, pox_fn->input, pox_fn->input_size);

	#ifdef CRYPTO_HMAC_SHA256
	hmac_sha256 hmac;
	hmac_sha256_initialize(&hmac, symkey, 16);
	hmac_sha256_finalize(&hmac, hash.hash, SHA256_DIGEST_SIZE);
	for (int i = 0; i < SHA256_DIGEST_SIZE; ++i)
		if(pox_token[i] != hmac.digest[i]){
			pox_fn->output[0] = 0xff;
			return 0;
		}
	pox_fn->output[0] = 0xaa;
	return 1;

	#elif defined(CRYPTO_ECC)
	const struct uECC_Curve_t * curve = uECC_secp256r1();
	int success;
	success = uECC_verify(vrf_pk, hash.hash, SHA256_DIGEST_SIZE, pox_token, curve);
	if(success == 1) pox_fn->output[0] = 0xaa;
	else pox_fn->output[0] = 0xff;
	return success;

	#elif defined(CRYPTO_ED25519)
	uint8_t success = edsign_verify(pox_token, vrf_pk, hash.hash, SHA256_DIGEST_SIZE);
	if(success == 1) pox_fn->output[0] = 0xaa;
	else pox_fn->output[0] = 0xff;
	return success;

	#elif defined(CRYPTO_SPHINCS)
	const struct ts_parameter_set *ps = &ts_ps_sha2_128f_simple;
	if (!ps) return 0;

	memcpy( optrand_buffer, optrand, 32 );

	struct ts_context verify_ctx;
	ts_init_verify( &verify_ctx, hash.hash, SHA256_DIGEST_SIZE, ps, vrf_pk );


	unsigned n = 0;
	unsigned numSigs = 0;
	for (;;) {

		//getsig_function fn = (getsig_function)(0x80404a9);
		//uint8_t *sig = fn();

		uint8_t sig[1024]={0};


		int rem = 17088-numSigs;
		if(rem > 1024) n = 1024;
		else n = rem;
		numSigs += n;
		if (rem == 0) {
			break;
		}

		#ifndef BENCHMARK_STACK
		HAL_UART_Transmit(&hlpuart1, "SPHINCSPHINC SED\n", 17, HAL_MAX_DELAY);

		//volatile int ii=0;
		//for(ii=0; ii<1000000; ii++);

		HAL_UART_Receive(&hlpuart1, sig, n, HAL_MAX_DELAY);
		#endif

		if (1 != ts_update_verify( sig, n, &verify_ctx )){
			HAL_UART_Transmit(&hlpuart1, "Fail\n", 5, HAL_MAX_DELAY);
			*(uint32_t*)(pox_fn->output) = n;
			//return 0;
		}
	}

	if (1 != ts_verify( &verify_ctx )) {
		pox_fn->output[0] = 0x88;
		return 1;
	}
	pox_fn->output[0] = 0xaa;
	return 1;

	/*struct ts_context verify_ctx;
	ts_init_verify( &verify_ctx, hash.hash, SHA256_DIGEST_SIZE, ps, vrf_pk);
	for (;;) {
		if (1 != ts_update_verify(pox_token, 100, &verify_ctx )) return 0;
	}
	return ts_verify( &verify_ctx );*/

	#endif
}

int check_request(pox_call_t *pox_fn, uint8_t *pox_token) {
	//return 1;
	return (my_counter < pox_fn->counter) && verify_req(pox_fn, pox_token);
}

#ifdef BENCHMARK_STACK
extern uint8_t _estack;
uint32_t stack_end = (unsigned long) &_estack;
int stack_size = 0x2000;
uint32_t stack_start;
#define MAGIC 0x88
uint32_t stack_pointer = 0;

void fill_stack() {
	stack_start = stack_end - stack_size;
	asm volatile ("mov %0, sp\n\t"
	: "=r" ( stack_pointer)
	);
	//char msg[10] = "AAAAAAAAAA\n";
	//HAL_UART_Transmit(&hlpuart1, msg, 10, HAL_MAX_DELAY);


	for(uint32_t i=stack_pointer; i>=stack_start; i--) {
	//for(uint32_t i=stack_pointer-0x10; i>=stack_base_address; i--) {
		uint8_t *mem = (uint8_t*)i;
		mem[0] = MAGIC;
	}
	asm volatile ("mov %0, sp\n\t"
	: "=r" ( stack_pointer)
	);
}

uint32_t stack_used() {
	stack_start = stack_end - stack_size;
	uint32_t i;
	for(i=stack_start; i<stack_pointer-0x10; i++) {
		uint8_t *mem = (uint8_t*)i;
		if(mem[0] != MAGIC && mem[1] != MAGIC && mem[2] != MAGIC && mem[3] != MAGIC)
			break;
	}
	return (stack_start+stack_size) - i;
}
#endif

unsigned char optrand_buffer[32];
int optrand_rng( unsigned char *target, size_t num_bytes ) {
	memcpy( target, optrand_buffer, num_bytes );
	return 1;
}

uint8_t sum = 0x87;

uint8_t read1(){
	return 10;
}

uint8_t read2() {
	return 15;
}

static void test(uint8_t val, uint8_t *pox_output) {
	uint8_t out;
	if(val == 0) out = read1();
	else out = read2();
	sum += out;
	pox_output[0] = sum;
}

CMSE_NS_ENTRY void SECURE_pox(pox_call_t *pox_fn, uint8_t *pox_token) {
	#ifdef BENCHMARK_STACK
	fill_stack();
	#endif

	uint8_t *mem = (uint8_t*) 0x8040000;
	int mem_len = 0x8000;

	// Verify request
	if(check_request(pox_fn, pox_token) == 0) {
		#ifndef BENCHMARK_STACK
		return;
		#endif
	}

	my_counter = pox_fn->counter;

	// h <- Hash(PMEM, F, I, c)

	sha256 hash;
	sha256_initialize(&hash);
	sha256_update(&hash, &(pox_fn->counter), 4);
	sha256_update(&hash, &(pox_fn->addr), 4);
	sha256_update(&hash, pox_fn->input, pox_fn->input_size);
	sha256_update(&hash, mem, mem_len);

	__disable_irq();
	void_function fn = (void_function)(pox_fn->addr);
	test(pox_fn->input[0], pox_fn->output);

	// h <- Hash(h, O)
	sha256_finalize(&hash, pox_fn->output, pox_fn->output_size);

	// Sign(h)
	#ifdef CRYPTO_HMAC_SHA256
	hmac_sha256 hmac;
	hmac_sha256_initialize(&hmac, symkey, SYM_KEY_SIZE);
	hmac_sha256_finalize(&hmac, hash.hash, SHA256_DIGEST_SIZE);
	memcpy(pox_token, hmac.digest, SHA256_DIGEST_SIZE);

	#elif defined(CRYPTO_ECC)
	const struct uECC_Curve_t * curve = uECC_secp256r1();
	uECC_sign(prv_sk, hash.hash, SHA256_DIGEST_SIZE, pox_token, curve);

	#elif defined(CRYPTO_ED25519)
	edsign_sign(pox_token, prv_pk, prv_sk, hash.hash, SHA256_DIGEST_SIZE);

	#elif defined(CRYPTO_SPHINCS)
	struct ts_context ctx;
	struct ts_context verify_ctx;
	ts_init_verify( &verify_ctx, hash.hash, SHA256_DIGEST_SIZE, ps, vrf_pk );

	ts_init_sign( &ctx, hash.hash, SHA256_DIGEST_SIZE, ps, prv_sk, optrand_rng );
	for(;;) {
		unsigned n = ts_sign( pox_token, sizeof pox_token, &ctx );

		if (1 != ts_update_verify( pox_token, n, &verify_ctx )){
			HAL_UART_Transmit(&hlpuart1, "Fail2\n", 6, HAL_MAX_DELAY);
		}

		if (n == 0) break;
	}
	if (1 != ts_verify( &verify_ctx )) {
		HAL_UART_Transmit(&hlpuart1, "Fail3\n", 6, HAL_MAX_DELAY);
	} else {
		HAL_UART_Transmit(&hlpuart1, "YEAHH\n", 6, HAL_MAX_DELAY);
	}
	#endif

	__enable_irq();
	#ifndef BENCHMARK_STACK
	HAL_UART_Transmit(&hlpuart1, "SucceSS\n", 8, HAL_MAX_DELAY);
	#endif

	#ifdef BENCHMARK_STACK
	uint32_t *out = (uint32_t*) (pox_fn->output);
	out[0] = stack_used();
	//out[0] = stack_pointer;
	#endif
}

/**
  * @}
  */

/**
  * @}
  */
/* USER CODE END Non_Secure_CallLib */

