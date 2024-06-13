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

#define SYM_KEY_SIZE 16
static uint8_t symkey[SYM_KEY_SIZE] = {0};

// Correspond to the initial state: empty Map (i.e., all zeroes);
// 0x8b 38 27 b7 ba e2 2f 97 6e 2a 59 e9 95 7b a8 b3 b9 ce e5 7a 4c f9 23 a4 da 97 0a 8f 3c 1e 79 c5
uint8_t secure_state[32] = {0x8b, 0x38, 0x27, 0xb7, 0xba, 0xe2, 0x2f, 0x97, 0x6e, 0x2a, 0x59, 0xe9, 0x95, 0x7b,
	0xa8, 0xb3, 0xb9, 0xce, 0xe5, 0x7a, 0x4c, 0xf9, 0x23, 0xa4, 0xda, 0x97, 0x0a, 0x8f, 0x3c, 0x1e, 0x79, 0xc5};
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

CMSE_NS_ENTRY void SECURE_checkState(uint8_t *state, int state_size) {
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

CMSE_NS_ENTRY uint8_t SECURE_energy_read() {
	uint8_t message[5] = "BBBB!\n";
	HAL_UART_Transmit(&huart2, "\n", 5, HAL_MAX_DELAY);
	HAL_UART_Receive(&huart2, message, 1, HAL_MAX_DELAY);
	return (uint8_t) message[0];
}

CMSE_NS_ENTRY void SECURE_pox(pox_call_t *pox_fn, uint8_t *pox_token) {
	HAL_UART_Transmit(&hlpuart1, "Phase2 start\n", 13, HAL_MAX_DELAY);

	/*uint8_t message[20] = "BBBB!\n";
	HAL_UART_Transmit(&huart2, "\n", 5, HAL_MAX_DELAY);
	HAL_UART_Receive(&huart2, message, 1, HAL_MAX_DELAY);
	sprintf(message, "%d\n", message[0]);
	HAL_UART_Transmit(&hlpuart1, message, 4, HAL_MAX_DELAY);*/


	uint8_t *mem = (uint8_t*) 0x80401f8;
	int mem_len = 0x1000; //0x1000;

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

	//sha256_finalize(&hash, mem, mem_len);


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
		return;
	}

	// h <- Hash(h, O)
	sha256_finalize(&hash, pox_fn->output, pox_fn->output_size);

	//return;

	// Sign(h)
	hmac_sha256 hmac;
	hmac_sha256_initialize(&hmac, symkey, SYM_KEY_SIZE);
	hmac_sha256_finalize(&hmac, hash.hash, SHA256_DIGEST_SIZE);
	memcpy(pox_token, hmac.digest, SHA256_DIGEST_SIZE);

	reset_flags();
	__enable_irq();
	//HAL_UART_Transmit(&hlpuart1, "Success\n", 8, HAL_MAX_DELAY);
	HAL_UART_Transmit(&hlpuart1, "Phase5 start\n", 13, HAL_MAX_DELAY);
}

/**
  * @}
  */

/**
  * @}
  */
/* USER CODE END Non_Secure_CallLib */

