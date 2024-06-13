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
#include "./kann/examples/textgen.h"

int heapSize = 0;

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

void sendline(char* s) {
	int i=0;
	while(s[i] != '\n') i++;
	HAL_UART_Transmit(&hlpuart1, (uint8_t*)s, (i+1), HAL_MAX_DELAY);

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


#define INPUT_SIZE 24*5

typedef struct
{
	uint8_t input[INPUT_SIZE];
	int curIdx;
} State;

State s;

void initialize() {
	// num uniques correspond to number of possible energy readings
	int num_unique = 24;
	char start_char = 65;
	char current_char = start_char;
	for(int j=0; j<INPUT_SIZE; j++) {
        s.input[j] = current_char;
        current_char++;
        if (current_char > start_char+num_unique) {
            current_char = start_char;
        }
	}
	s.curIdx = 0;
}

void collect() {

	// Simulate data collection for benchmarking purpose
	// for actual implementation, see LDP example
	s.input[s.curIdx] = 15;
	s.curIdx = (s.curIdx+1)%INPUT_SIZE;

}

void train(pox_call_t *pox_fn) {
	float rate = *(float*)(pox_fn->input);
	int epochs = (int) (*(float*)(pox_fn->input+4));
	float init_val = *(float*)(pox_fn->input+4*2);

	//sendline("HELLODD\n");

	int c, seed = 11, ulen = 1, vlen = 1, n_h_layers = 1, n_h_neurons = 8, model = 2, max_epoch = epochs, mbs = 4;
	int use_norm = 1, batch_len = 1000, n_threads = 1, cs = 10;
	float h_dropout = 0.0f, temp = 0.5f, lr = rate, grad_clip = 0.0f;
	tg_data_t *tg;
	kann_t *ann = 0;
	tg = tg_init_with_data(s.input, INPUT_SIZE);
	//sendline("INIT DONE\n");

	int n1 = tg->n_char;
	ann = model_gen(model, tg->n_char, n_h_layers, n_h_neurons, h_dropout, use_norm);
	//sendline("MODEL_GEN DONE\n");

	tg_train(ann, tg, lr, ulen, vlen, cs, mbs, max_epoch, grad_clip, batch_len, n_threads);
	//sendline("TG_TRAIN DONE\n");

	free(tg->data); free(tg);

	memcpy(pox_fn->output, tg->c2i, pox_fn->output_size);
}

CMSE_NS_ENTRY void SECURE_pox(pox_call_t *pox_fn, uint8_t *pox_token, uint8_t type) {
	uint8_t buff[30]={};
	sprintf(buff, "type: %d\n", type);
	sendline(buff);
	HAL_UART_Transmit(&hlpuart1, "Phase2 start\n", 13, HAL_MAX_DELAY);
	uint8_t *mem = (uint8_t*) 0x8040000; //0x80401f8;
	int mem_len = 0x40000; //0x1000

	// Verify request
	if(check_request(pox_fn, pox_token) == 0) {
		return;
	}

	my_counter = pox_fn->counter;

	// h <- Hash(PMEM, F, I, c)

	sha256 hash;
	sha256_initialize(&hash);
	sha256_update(&hash, &(pox_fn->counter), 4);
	sha256_update(&hash, &(pox_fn->addr), 4);
	sha256_update(&hash, mem, mem_len);

	sha256_update(&hash, pox_fn->input, pox_fn->input_size);

	__disable_irq();
	HAL_UART_Transmit(&hlpuart1, "Phase3 start\n", 13, HAL_MAX_DELAY);
	if(type == 0) {
		// Setup
		initialize();
	} else if (type == 1) {
		// Collect
		collect();

	} else {
		// Train
		train(pox_fn);

	}
	HAL_UART_Transmit(&hlpuart1, "Phase4 start\n", 13, HAL_MAX_DELAY);

	// h <- Hash(h, O)
	sha256_finalize(&hash, pox_fn->output, pox_fn->output_size);

	//char tt[3];
	//sprintf(tt, "%d", pox_fn->output_size);
	//HAL_UART_Transmit(&hlpuart1, tt, 3, HAL_MAX_DELAY);

	// Sign(h)
	hmac_sha256 hmac;
	hmac_sha256_initialize(&hmac, symkey, SYM_KEY_SIZE);
	hmac_sha256_finalize(&hmac, hash.hash, SHA256_DIGEST_SIZE);
	memcpy(pox_token, hmac.digest, SHA256_DIGEST_SIZE);

	__enable_irq();
	HAL_UART_Transmit(&hlpuart1, "Phase5 start\n", 13, HAL_MAX_DELAY);
	//HAL_UART_Transmit(&hlpuart1, "Success\n", 8, HAL_MAX_DELAY);
	//sprintf(buff, "Heap Usage: %d\n",heapSize);
	//sendline(buff);
}

/**
  * @}
  */

/**
  * @}
  */
/* USER CODE END Non_Secure_CallLib */

