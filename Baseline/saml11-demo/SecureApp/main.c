/**
 * Copyright (c) 2019 Microchip Technology Inc.
 *
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "trustzone_config.h"
//#include "edsign.h"

//-----------------------------------------------------------------------------

/* TZ_START_NS: Start address of non-secure application */
#define TZ_START_NS 0x00008000
 	 	
/* typedef for non-secure callback functions */
typedef void (*funcptr_void) (void) __attribute__((cmse_nonsecure_call));
	
/*
	The Secure Application:
	* Sets the non-secure main stack (MSP_NS) 
	* Gets the non-secure reset handler
	* Jumps to the non-secure software application
*/

/* Secure main() */
int main(void)
{
	funcptr_void NonSecure_ResetHandler;
  	
	/* Initialize the SAM system */
    SystemInit();
	
	// Switch to 16MHz clock (disable prescaler)
	OSCCTRL->OSC16MCTRL.reg = OSCCTRL_OSC16MCTRL_ENABLE | OSCCTRL_OSC16MCTRL_FSEL_16;
	
	/* Setup Port Security Attribution of I/O Pins */
	PORT_SEC->Group[0].NONSEC.reg = CONF_PORT_INIT_NONSEC0_VAL;
	NVMCTRL_SEC->NONSEC.bit.WRITE = 1;

	/* Setup NVIC Interrupt Target Non-Secure state */
	NVIC->ITNS[0] = CONF_NVIC_INIT_ITNS0_VAL;
	NVIC->ITNS[1] = CONF_NVIC_INIT_ITNS1_VAL;

	/* Set non-secure main stack (MSP_NS) */
	__TZ_set_MSP_NS(*((uint32_t *)(TZ_START_NS)));
  
	/* Get non-secure reset handler */
	NonSecure_ResetHandler = (funcptr_void)(*((uint32_t *)((TZ_START_NS) + 4U)));
  
	/* Start non-secure state software application */
	NonSecure_ResetHandler();
  
    /* Replace with your SECURE application code */
	
	/*unsigned char sk[200];  // secret key plus public key (64 octets)
	unsigned char pk[100];  // public key (32 octets)
	unsigned char m[10];  // message
	unsigned char nsig[64]; // newly generated signature
    while (1) 
    {
		edsign_sign(nsig, pk, sk, m, 10);
		__NOP();
    }*/
}
