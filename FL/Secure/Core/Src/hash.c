#include "hash.h"

/**
  * @brief HASH Initialization Function
  * @param None
  * @retval None
  */
HASH_HandleTypeDef hhash;

void MX_HASH_Init(void)
{

  /* USER CODE BEGIN HASH_Init 0 */

  /* USER CODE END HASH_Init 0 */

  /* USER CODE BEGIN HASH_Init 1 */
  __HASH_CLK_ENABLE();
  hhash.Init.DataType = HASH_DATATYPE_8B;

  /* USER CODE END HASH_Init 1 */

  /* USER CODE BEGIN HASH_Init 2 */
  if (HAL_HASH_Init(&hhash) != HAL_OK)
  {
	  Error_Handler();
  }
}

HAL_StatusTypeDef HASH_SHA_265(uint8_t *pInBuffer, uint32_t Size, uint8_t* pOutBuffer){
	//	(HASH_HandleTypeDef *hhash, uint8_t *pInBuffer, uint32_t Size, uint8_t* pOutBuffer, uint32_t Timeout)
	return HAL_HASHEx_SHA256_Start(&hhash, pInBuffer, Size, pOutBuffer, HASH_TIMEOUT);
}

HAL_StatusTypeDef HMAC_SHA_265(uint8_t *pInBuffer, uint32_t Size, uint8_t* pOutBuffer){
	return HAL_HMACEx_SHA256_Start(&hhash, pInBuffer, Size, pOutBuffer, HASH_TIMEOUT);
}

void set_hmac_key(uint8_t * att_key, uint32_t size){
	hhash.Init.KeySize = size;
	hhash.Init.pKey = att_key;
}
