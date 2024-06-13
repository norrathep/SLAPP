/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "application.h"
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "gpio.h"
#include "secure_nsc.h"
#include "arm_cmse.h"
#include "core_cm33.h"
#include "./kann/examples/textgen.h"


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//this function will raise secure fault by access secure area directly.
void RaiseSecureFault(void)
{
  //read secure SRAM address
  *(volatile uint32_t *)0x30000000UL = 0;
}

//secure fault generated by IDAU/SAU check */
void SecureFault_Callback(void)
{
  HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
  Error_Handler();
}

//secure error generated by GTZC check
void SecureError_Callback(void)
{
  HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
  Error_Handler();
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */

//#define ATTACK_A1
//#define ATTACK_A2
//#define ATTACK_A3


#define CRYPTO_HMAC_SHA256

#define POX_INPUT_SIZE 12
#define POX_OUTPUT_SIZE 256

#define SHA256_DIGEST_SIZE 32

#ifdef CRYPTO_HMAC_SHA256
#define TOKEN_SIZE SHA256_DIGEST_SIZE

#endif


#define PLAINTEXT_SIZE 5+4*5
#define REQUEST_SIZE PLAINTEXT_SIZE+TOKEN_SIZE

uint8_t pox_input[POX_INPUT_SIZE] = {0};
uint8_t pox_output[POX_OUTPUT_SIZE] = {0};

void sendline(char* s) {
	int i=0;
	while(s[i] != '\n') i++;
	SECURE_send((uint8_t*)s, (i+1));

}

void print_token(uint8_t *token, int size) {
	char hex[3]={0};
	SECURE_send("0x",2);
	for(int i=0; i<size; i++) {
		sprintf(hex, "%.2x", token[i]);
		SECURE_send(hex, 2);
	}
	SECURE_send("\n", 1);
}

void single() {

	for(volatile int i=0; i<10000; i++);
		uint8_t req[100]={};
		uint8_t buff[100]="helllo teset\n";
		SECURE_recv(req, REQUEST_SIZE);
		sendline(buff);

		pox_call_t pc;
		pc.input = pox_input;
		pc.output = pox_output;
		pc.command_size = 5; // assume to be fixed
		pc.command = req; // either [stp] or [clt]
		uint8_t type = 0;
		if(memcmp(pc.command, "[stp]", pc.command_size) == 0 ||
				memcmp(pc.command, "[clt]", pc.command_size) == 0) {

			if(memcmp(pc.command, "[clt]", pc.command_size) == 0)
				type = 1;

			/*sha256 hash;
			sha256_initialize(&hash);
			sha256_finalize(&hash, s.input, INPUT_SIZE);
			print_token(hash.hash, 32);*/

			uint32_t counter = *((uint32_t*)(req+5));
			uint32_t func = *((uint32_t*)(req+5+4));

			sprintf(buff, "[Test] Setup/collect: counter=%d, func=%x, done [/Test]\n", counter, func);
			sendline(buff);

			uint8_t *token = (uint8_t*) (req+PLAINTEXT_SIZE);

			pc.counter = counter;
			pc.addr = func;
			pc.input_size = 12;
			pc.output_size = 32;
			// pox_input = b'\x00'*12
			memcpy(pox_input, req+5+4*2, 12);

			SECURE_pox(&pc, token, type);

			// fault tolerance
			//for(int i=0; i<10; i++) {
			SECURE_send("[OS]", 4);
			SECURE_send(pox_output, 32);
			SECURE_send(" ", 1);
			SECURE_send(token, 32);
			sendline("[OE]\n");
			//}
			sendline("[Over]\n");

		} else if(memcmp(pc.command, "[trn]", pc.command_size) == 0) {
			type = 2;
			uint32_t counter = *((uint32_t*)(req+5));
			uint32_t func = *((uint32_t*)(req+5+4));

			pc.counter = counter;
			pc.addr = func;
			pc.input_size = 12;
			pc.output = pox_output;
			pc.output_size = POX_OUTPUT_SIZE;

			float lr = *((float*)(req+5+4*2));
			int numEpochs = *((float*)(req+5+4*3));
			float init_val = *((float*)(req+5+4*4));

			sprintf(buff, "[Test] Training: counter=%d, func=%x, lr=%d, epoch=%d, init_val=%d done [/Test]\n",
					counter, func, (int) (lr*10000), numEpochs, (int) (init_val*10000));
			sendline(buff);

			uint8_t *token = (uint8_t*) (req+PLAINTEXT_SIZE);
			// pox_input = f, p, q
			memcpy(pox_input, req+5+4*2, 12);

			SECURE_pox(&pc, token, type);



			#ifdef ATTACK_AD
			pc.output[0] += 15;
			#endif

			// fault tolerance
			//for(int i=0; i<10; i++) {
			SECURE_send("[OS]", 4);
			SECURE_send(pc.output, pc.output_size);
			SECURE_send(" ", 1);
			SECURE_send(token, 32);
			sendline("[OE]\n");
			//}
			sendline("[Over]\n");
		} else {
			sprintf(buff, "failed to parse command: %s\n", req);
			sendline(buff);
		}

}


int main(void)
{
  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Initialize all configured peripherals */
   MX_GPIO_Init();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  /* USER CODE BEGIN 2 */
  //register error callbacks
  SECURE_RegisterCallback(SECURE_FAULT_CB_ID, (void *)SecureFault_Callback);
  SECURE_RegisterCallback(GTZC_ERROR_CB_ID, (void *)SecureError_Callback);
  /* USER CODE END 2 */

  srand(time(NULL));



  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

	single();
	//train(0.1, 10, 0.1);
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/* USER CODE BEGIN 4 */
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
