/*
 * hash.h
 */

#include <stdint.h>
#include "stm32l5xx_hal.h"

#ifndef INC_HASH_H_
#define INC_HASH_H_

#define HASH_TIMEOUT	0xffff

void MX_HASH_Init(void);
HAL_StatusTypeDef HASH_SHA_265(uint8_t *pInBuffer, uint32_t Size, uint8_t* pOutBuffer);
HAL_StatusTypeDef HMAC_SHA_265(uint8_t *pInBuffer, uint32_t Size, uint8_t* pOutBuffer);
void set_hmac_key(uint8_t * att_key, uint32_t size);

#endif /* INC_HASH_H_ */
