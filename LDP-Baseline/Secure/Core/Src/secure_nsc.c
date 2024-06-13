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

#define CRYPTO_HMAC_SHA256

#define SHA256_DIGEST_SIZE 32
#include "./Crypto/hmac-sha256/hmac-sha256.h"

#define SYM_KEY_SIZE 16
static uint8_t symkey[SYM_KEY_SIZE] = {0};


void sendline(char* s) {
	int i=0;
	while(s[i] != '\n') i++;
	HAL_UART_Transmit(&hlpuart1, (uint8_t*)s, (i+1), HAL_MAX_DELAY);

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
		sendline("Map is full. Cannot insert.\n");
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
}


uint8_t energy_read() {
	uint8_t message[5] = "BBBB!\n";
	HAL_UART_Transmit(&huart2, "\n", 5, HAL_MAX_DELAY);
	HAL_UART_Receive(&huart2, message, 1, HAL_MAX_DELAY);
	return (uint8_t) (message[0]);
}

void LDP_DC(pox_call_t *pox_fn, uint8_t inp, float f, float p, float q) {
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

    memcpy(pox_fn->output, O, 32);
}

void LDP_energy(pox_call_t *pox_fn) {

	float f = *(float*)(pox_fn->input);
	float p = *(float*)(pox_fn->input+4);
	float q = *(float*)(pox_fn->input+4*2);
	char arr[5];

	// TODO: get actual value from PZEM sensor
	uint8_t energy = energy_read();
	//uint8_t energy = (uint8_t)(((float) rand()/RAND_MAX)*255 + 1);
	char buf[20] = {0};
	sprintf(buf, "Energy:%d\n", energy);
	sendline(buf);

	LDP_DC(pox_fn, energy, f, p, q);

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

CMSE_NS_ENTRY void SECURE_pox(pox_call_t *pox_fn, uint8_t *pox_token, uint8_t type) {
	HAL_UART_Transmit(&hlpuart1, "Phase2 start\n", 13, HAL_MAX_DELAY);


	uint8_t *mem = (uint8_t*) 0x8040000; // 0x80401f8;
	int mem_len = 0x40000; //0x1000;

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
	sha256_update(&hash, pox_fn->input, pox_fn->input_size);
	sha256_update(&hash, mem, mem_len);

	//sha256_finalize(&hash, mem, mem_len);


	//memcpy(pox_fn->output, mem, SHA256_DIGEST_SIZE);

	//memcpy(pox_token, hash.hash, SHA256_DIGEST_SIZE);

	__disable_irq();
	void_function fn = (void_function)(pox_fn->addr);
	HAL_UART_Transmit(&hlpuart1, "Phase3 start\n", 13, HAL_MAX_DELAY);

	if(type == 0) {
		initB();
	} else {
		LDP_energy(pox_fn);
	}
	HAL_UART_Transmit(&hlpuart1, "Phase4 start\n", 13, HAL_MAX_DELAY);

	// h <- Hash(h, O)
	sha256_finalize(&hash, pox_fn->output, pox_fn->output_size);

	//return;

	// Sign(h)
	hmac_sha256 hmac;
	hmac_sha256_initialize(&hmac, symkey, SYM_KEY_SIZE);
	hmac_sha256_finalize(&hmac, hash.hash, SHA256_DIGEST_SIZE);
	memcpy(pox_token, hmac.digest, SHA256_DIGEST_SIZE);

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

