/*
 * PZEM004Tv30.h
 *
 * Created: 3/21/2024 2:42:18 PM
 *  Author: super
 */ 


#ifndef PZEM004TV30_H_
#define PZEM004TV30_H_

#include "hal_gpio.h"

void PZEM004Tv30_init();
void send_byte(uint8_t c);
uint8_t recv_byte();





#endif /* PZEM004TV30_H_ */