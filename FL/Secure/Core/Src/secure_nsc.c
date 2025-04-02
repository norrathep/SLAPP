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

#include <stdbool.h>

typedef void (*void_function) () __attribute__((cmse_nonsecure_call));
typedef uint8_t* (*getsig_function) () __attribute__((cmse_nonsecure_call));
typedef void (*printsig_function) (uint8_t*, int) __attribute__((cmse_nonsecure_call));

//#define BENCHMARK_STACK

#define CRYPTO_HMAC_SHA256
//#define CRYPTO_ED25519
//#define CRYPTO_SPHINCS

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

//uint8_t secure_state[32] = {0x11, 0x5b, 0xad, 0x14, 0xf1, 0xc9, 0xf2, 0xc0, 0x27, 0xa8, 0x4d,
//		0xe2, 0x1b, 0x10, 0x70, 0x15, 0x72, 0x2c, 0xb7, 0x6b, 0xe8, 0xd0, 0xab, 0xf3, 0x76,
//		0x0a, 0xd8, 0xe0, 0x0d, 0x6c, 0x24, 0xa5};
uint8_t secure_state[32] = {0x7b, 0x8e, 0xc8, 0xdd, 0x83, 0x6b, 0x56, 0x4f, 0x0c, 0x85, 0xad, 0x08,
		0x8f, 0xc7, 0x44, 0xde, 0x82, 0x03, 0x45, 0x20, 0x4e, 0x15, 0x4b, 0xc1, 0x50, 0x3e, 0x04, 0xe9,
		0xd6, 0xfd, 0xd9, 0xf1};

uint8_t exec = 0, stateUsed = 0, stateChecked = 0;

CMSE_NS_ENTRY void SECURE_setState(uint8_t *state, int state_size) {
	if(stateChecked != 1 || exec != 1) return;
	//if(state_size > 32) {

		sha256 hash;
		sha256_initialize(&hash);
		sha256_finalize(&hash, state, state_size);
		memcpy(secure_state, hash.hash, 32);

	//}
	//else {
	//	memset(secure_state, 0, 32);
	//	memcpy(secure_state, state, state_size);
	//}
}

void print_token(uint8_t *token, int size) {
	char hex[3]={0};
	HAL_UART_Transmit(&hlpuart1, "0x", 2, HAL_MAX_DELAY);
	for(int i=0; i<size; i++) {
		sprintf(hex, "%.2x", token[i]);
		HAL_UART_Transmit(&hlpuart1, hex, 2, HAL_MAX_DELAY);
	}
	HAL_UART_Transmit(&hlpuart1, "\n", 1, HAL_MAX_DELAY);
}

CMSE_NS_ENTRY void SECURE_checkState(uint8_t *state, int state_size) {
	sha256 hash;
	sha256_initialize(&hash);
	sha256_finalize(&hash, state, state_size);
	uint8_t cmp = (uint8_t) memcmp(secure_state, hash.hash, 32);

	if(cmp != 0) {
		HAL_UART_Transmit(&hlpuart1, "TamperedState: ", 15, HAL_MAX_DELAY);
		print_token(hash.hash, 32);
	}

	stateChecked = (cmp == 0);
	stateUsed = 1;
}

uint32_t my_counter = 0;

extern unsigned get_sphinc_sig(uint8_t *out);

int verify_req(pox_call_t *pox_fn, uint8_t *pox_token) {
	if(pox_fn->counter < my_counter) return 0;

	sha256 hash;
	sha256_initialize(&hash);
	sha256_update(&hash, pox_fn->command, pox_fn->command_size);
	sha256_update(&hash, &(pox_fn->counter), 4);
	sha256_update(&hash, &(pox_fn->addr), 4);
	sha256_finalize(&hash, pox_fn->input, pox_fn->input_size);


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
}

int check_request(pox_call_t *pox_fn, uint8_t *pox_token) {
	//return 1;
	return (my_counter < pox_fn->counter) && verify_req(pox_fn, pox_token);
}

void init_flags() {
	exec = 1; stateUsed = 0; stateChecked = 0;
}

void reset_flags() {
	exec = 0; stateUsed = 0; stateChecked = 0;
}

CMSE_NS_ENTRY void SECURE_pox(pox_call_t *pox_fn, uint8_t *pox_token) {
	HAL_UART_Transmit(&hlpuart1, "Phase2 start\n", 13, HAL_MAX_DELAY);
	uint8_t *mem = (uint8_t*) 0x80401f8;
	int mem_len = 0x1000;

	// Verify request
	if(check_request(pox_fn, pox_token) == 0) {
		//return;
		pox_fn->output[0] = 0x00;
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




	/*HAL_UART_Transmit(&hlpuart1, "Input1: ", 8, HAL_MAX_DELAY);
	HAL_UART_Transmit(&hlpuart1, hash.hash, 32, HAL_MAX_DELAY);
	HAL_UART_Transmit(&hlpuart1, " Output Size: ", 14, HAL_MAX_DELAY);
	HAL_UART_Transmit(&hlpuart1, "\n", 1, HAL_MAX_DELAY);*/


	//memcpy(pox_fn->output, mem, SHA256_DIGEST_SIZE);

	//memcpy(pox_token, hash.hash, SHA256_DIGEST_SIZE);

	__disable_irq();
	void_function fn = (void_function)(pox_fn->addr);
	HAL_UART_Transmit(&hlpuart1, "Phase3 start\n", 13, HAL_MAX_DELAY);
	fn();
	HAL_UART_Transmit(&hlpuart1, "Phase4 start\n", 13, HAL_MAX_DELAY);

	if(exec && stateUsed && !stateChecked) {
		reset_flags();
		__enable_irq();
		//return;
	}

	// h <- Hash(h, O)
	sha256_finalize(&hash, pox_fn->output, pox_fn->output_size);

	/*HAL_UART_Transmit(&hlpuart1, "Output: ", 8, HAL_MAX_DELAY);
	HAL_UART_Transmit(&hlpuart1, pox_fn->output, pox_fn->output_size, HAL_MAX_DELAY);
	HAL_UART_Transmit(&hlpuart1, "\n", 1, HAL_MAX_DELAY);
	//return;


	HAL_UART_Transmit(&hlpuart1, "Input1: ", 8, HAL_MAX_DELAY);
	HAL_UART_Transmit(&hlpuart1, hash.hash, 32, HAL_MAX_DELAY);
	HAL_UART_Transmit(&hlpuart1, " Output Size: ", 14, HAL_MAX_DELAY);*/

	//char tt[3];
	//sprintf(tt, "%d", pox_fn->output_size);
	//HAL_UART_Transmit(&hlpuart1, tt, 3, HAL_MAX_DELAY);

	// Sign(h)
	hmac_sha256 hmac;
	hmac_sha256_initialize(&hmac, symkey, SYM_KEY_SIZE);
	hmac_sha256_finalize(&hmac, hash.hash, SHA256_DIGEST_SIZE);
	memcpy(pox_token, hmac.digest, SHA256_DIGEST_SIZE);

	reset_flags();
	__enable_irq();
	HAL_UART_Transmit(&hlpuart1, "Phase5 start\n", 13, HAL_MAX_DELAY);
	//HAL_UART_Transmit(&hlpuart1, "Success\n", 8, HAL_MAX_DELAY);
}

/**
  * @}
  */

/**
  * @}
  */
/* USER CODE END Non_Secure_CallLib */

