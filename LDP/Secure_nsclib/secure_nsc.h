/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    Secure_nsclib/secure_nsc.h
  * @author  MCD Application Team
  * @brief   Header for secure non-secure callable APIs list
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

/* USER CODE BEGIN Non_Secure_CallLib_h */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SECURE_NSC_H
#define SECURE_NSC_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
typedef char flag;
/* Exported types ------------------------------------------------------------*/
/**
  * @brief  non-secure callback ID enumeration definition
  */
typedef enum
{
  SECURE_FAULT_CB_ID     = 0x00U, /*!< System secure fault callback ID */
  GTZC_ERROR_CB_ID       = 0x01U,  /*!< GTZC secure error callback ID */
} SECURE_CallbackIDTypeDef;


/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void SECURE_RegisterCallback(SECURE_CallbackIDTypeDef, void *);

typedef struct pox_call {
	uint32_t addr;
	uint8_t *input;
	int input_size;
	uint8_t *output;
	int output_size;
	uint32_t counter;
	uint8_t *command;
	uint8_t command_size;
} pox_call_t;

void SECURE_send(uint8_t* message, int size);
void SECURE_recv(uint8_t* message, int size);
uint8_t SECURE_energy_read();

void SECURE_checkState(uint8_t *state, int state_size);
void SECURE_setState(uint8_t *state, int state_size);
void SECURE_pox(pox_call_t *pox_fn, uint8_t *pox_token);
#endif /* SECURE_NSC_H */
/* USER CODE END Non_Secure_CallLib_h */

