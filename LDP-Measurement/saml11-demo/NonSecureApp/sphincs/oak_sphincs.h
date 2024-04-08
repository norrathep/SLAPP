/*
 * oak_sphincs.h
 *
 * Created: 2/15/2024 3:43:32 PM
 *  Author: Oak
 */ 


#ifndef OAK_SPHINCS_H_
#define OAK_SPHINCS_H_

#include "tiny_sphincs.h"
#include "../gpio/uart.h"


void oakSphinc(uint8_t *message, int msgsize);
void oakSphincVerify(uint8_t *message, int msgsize);

#endif /* OAK_SPHINCS_H_ */