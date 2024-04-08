/**
 * \file
 *
 * \brief Fuses definition for SAML11 device
 *
 * Copyright (c) 2019 Microchip Technology Inc.
 *
 * \license_start
 *
 * \page License
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * \license_stop
 *
 */

#ifndef SAML11_BOCOR_H_
#define SAML11_BOCOR_H_

#define RESERVED 0xFFFFFFFF

/* BOCOR_X: Boot Configuration Row (BOCOR) Word X definitions */

#define BOCOR_BNSC			0x00		/* Non-Secure Callable Flash size (BOOT region) = BNSC*0x20 */
#define BOCOR_BS			0x00		/* Non-Secure Flash size (BOOT region) = BS*0x100 */
#define BOCOR_BOOTOPT		0x00		/* 0x0 = no boot verification ; 0x1 = SHA256 verification on BS ; 0x2 or 0x3 = SHA256 with BOOTKEY on BS */
#define BOCOR_BOOTPROT		0x00		/* Boot Protection size = BOOTPROT*0x100 */
#define BOCOR_BCWEN			0x1			/* 0x0 = Boot Configuration Write disabled ; 0x1 = Boot Configuration Write Enabled*/
#define BOCOR_BCREN			0x1			/* 0x0 = Boot Configuration Read disabled ; 0x1 = Boot Configuration Read Enabled*/ 

#define BOCOR_CEKEY0_WORD_0  0xFFFFFFFF /* Chip erase 0 key (128-bits) */
#define BOCOR_CEKEY0_WORD_1  0xFFFFFFFF /* " */
#define BOCOR_CEKEY0_WORD_2  0xFFFFFFFF /* " */
#define BOCOR_CEKEY0_WORD_3  0xFFFFFFFF /* " */

#define BOCOR_CEKEY1_WORD_0  0xFFFFFFFF /* Chip erase 1 key (128-bits) */
#define BOCOR_CEKEY1_WORD_1  0xFFFFFFFF	/* " */
#define BOCOR_CEKEY1_WORD_2  0xFFFFFFFF	/* " */
#define BOCOR_CEKEY1_WORD_3  0xFFFFFFFF	/* " */

#define BOCOR_CEKEY2_WORD_0  0xFFFFFFFF /* Chip erase 2 key (128-bits) */
#define BOCOR_CEKEY2_WORD_1  0xFFFFFFFF	/* " */
#define BOCOR_CEKEY2_WORD_2  0xFFFFFFFF	/* " */
#define BOCOR_CEKEY2_WORD_3  0xFFFFFFFF	/* " */

#define BOCOR_BOOTKEY_WORD_0 0xFFFFFFFF /* BOOTKEY (256-bits) */
#define BOCOR_BOOTKEY_WORD_1 0xFFFFFFFF	/* " */
#define BOCOR_BOOTKEY_WORD_2 0xFFFFFFFF	/* " */
#define BOCOR_BOOTKEY_WORD_3 0xFFFFFFFF	/* " */
#define BOCOR_BOOTKEY_WORD_4 0xFFFFFFFF /* " */
#define BOCOR_BOOTKEY_WORD_5 0xFFFFFFFF	/* " */
#define BOCOR_BOOTKEY_WORD_6 0xFFFFFFFF	/* " */
#define BOCOR_BOOTKEY_WORD_7 0xFFFFFFFF	/* " */

__attribute__((section (".bocorsec")))
const uint32_t bocor[2] = { 
	((BOCOR_BOOTOPT<<24)|(BOCOR_BNSC<<16)|(BOCOR_BS<<8)) , 
	((BOCOR_BCREN<<17)|(BOCOR_BCWEN<<16)|(BOCOR_BOOTPROT))
};
	
__attribute__((section (".bocorcekeysec")))
const uint32_t bocor_ce_keys[12] = { BOCOR_CEKEY0_WORD_0 , BOCOR_CEKEY0_WORD_1 , BOCOR_CEKEY0_WORD_2, BOCOR_CEKEY0_WORD_3 ,
	BOCOR_CEKEY1_WORD_0 , BOCOR_CEKEY1_WORD_1 , BOCOR_CEKEY1_WORD_2, BOCOR_CEKEY1_WORD_3 ,
	BOCOR_CEKEY2_WORD_0 , BOCOR_CEKEY2_WORD_1 , BOCOR_CEKEY2_WORD_2, BOCOR_CEKEY2_WORD_3
};

__attribute__((section (".bocorbootkeysec"))) 
const uint32_t bocor_bootkey[8] =  { BOCOR_BOOTKEY_WORD_0 , BOCOR_BOOTKEY_WORD_1 , BOCOR_BOOTKEY_WORD_2, BOCOR_BOOTKEY_WORD_3 ,
	BOCOR_BOOTKEY_WORD_4 , BOCOR_BOOTKEY_WORD_5 , BOCOR_BOOTKEY_WORD_6, BOCOR_BOOTKEY_WORD_7 
};


#endif /* SAML11_BOCOR_H_ */