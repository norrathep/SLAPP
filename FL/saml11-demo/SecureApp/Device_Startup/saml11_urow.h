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

#ifndef SAML11_UROW_H_
#define SAML11_UROW_H_

#define UROW_SULCK_BS				0x1		/* 0x0 = BS region is locked ; 0x1 = BS region is not locked  */
#define UROW_SULCK_AS				0x1		/* 0x0 = AS region is locked ; 0x1 = AS region is not locked  */
#define UROW_SULCK_DS				0x1		/* 0x0 = DS region is locked ; 0x1 = DS region is not locked  */
#define UROW_NSULCK_BNS				0x1		/* 0x0 = BNS region is locked ; 0x1 = BNS region is not locked  */
#define UROW_NSULCK_ANS				0x1		/* 0x0 = ANS region is locked ; 0x1 = ANS region is not locked  */
#define UROW_NSULCK_DNS				0x1		/* 0x0 = DNS region is locked ; 0x1 = DNS region is not locked  */
#define UROW_BOD33_LEVEL			0x6		/* 0x0 to 0x1F = BOD33 threshold level */
#define UROW_BOD33_DISABLE			0x0		/* 0x0 = BOD33 enabled ; 0x1 = BOD33 disabled */
#define UROW_BOD33_ACTION			0x1		/* 0x0 = NONE ; 0x1 = RESET ; 0x2 = INT */
#define UROW_BOD12_CALIB_PARAMS		0x08F	/* (Do not change) */
#define UROW_WDT_RUNSTDBY			0x0		/* 0x0 = WDT disabled during standby sleep ; 0x1 = WDT enabled during standby sleep */
#define UROW_WDT_ENABLE				0x0		/* 0x0 = WDT enabled ; 0x1 = WDT disabled */
#define UROW_WDT_ALWAYS_ON			0x0		/* 0x0 = WDT enabled/disabled through ENABLE bit ; 0x1 WDT can only be disabled by a POR */
#define UROW_WDT_PER				0xB		/* 0x0 to 0xB = WDT Time-Out Period */
#define UROW_WDT_WINDOW				0xB		/* 0x0 to 0xB = Window Mode Time-Out Period */
#define UROW_WDT_EWOFFSET			0xB		/* 0x0 to 0xA = Early Warning Interrupt Time Offset */
#define UROW_WDT_WEN				0x0		/* 0x0 = WDT enabled ; 0x1 = WDT disabled */
#define UROW_BOD33_HYST				0x0		/* 0x0 = BOD33 hysteresis enabled ; 0x1 = BOD33 hysteresis disabled */
#define UROW_RXN					0x1		/* 0x0 = RAM is executable ; 0x1 = RAM is not executable */
#define UROW_DXN					0x1		/* 0x0 = DataFlash is executable ; 0x1 = DataFlash is not executable */
#define UROW_AS						0x80	/* Secure Flash size (APPLICATION region) = AS*0x100 */
#define UROW_ANSC					0x20	/* Non-Secure Callable Flash size (APPLICATION region) = ANSC*0x20 */
#define UROW_DS						0x8		/* Secure Data Flash size = DS*0x100 */
#define UROW_RS						0x40	/* Secure SRAM size = RS*0x80 */
#define UROW_URWEN					0x1		/* 0x0 = User Row Write Disabled ; 0x1 = User Row Write Enabled */ 
#define UROW_NONSECA_PAC			0x0		/* 0x0 = PAC is secured ; PAC is Non-secured */
#define UROW_NONSECA_PM				0x0		/* 0x0 = PM is secured ; PM is Non-secured */
#define UROW_NONSECA_MCLK			0x1		/* 0x0 = MCLK is secured ; MCLK is Non-secured */
#define UROW_NONSECA_RSTC			0x0		/* 0x0 = RSTC is secured ; RSTC is Non-secured */
#define UROW_NONSECA_OSCCTRL		0x0		/* 0x0 = OSCCTRL is secured ; OSCCTRL is Non-secured */
#define UROW_NONSECA_OSC32KCTRL		0x0		/* 0x0 = OSC32KCTRL is secured ; OSC32KCTRL is Non-secured */
#define UROW_NONSECA_SUPC			0x0		/* 0x0 = SUPC is secured ; SUPC is Non-secured */
#define UROW_NONSECA_GCLK			0x1		/* 0x0 = GCLK is secured ; GCLK is Non-secured */
#define UROW_NONSECA_WDT			0x0		/* 0x0 = WDT is secured ; WDT is Non-secured */
#define UROW_NONSECA_RTC			0x0		/* 0x0 = RTC is secured ; RTC is Non-secured */
#define UROW_NONSECA_EIC			0x0		/* 0x0 = EIC is secured ; EIC is Non-secured */
#define UROW_NONSECA_FREQM			0x0		/* 0x0 = FREQM is secured ; FREQM is Non-secured */
#define UROW_NONSECA_PORT			0x0		/* 0x0 = PORT is secured ; PORT is Non-secured */
#define UROW_NONSECA_AC				0x0		/* 0x0 = AC is secured ; AC is Non-secured */
#define UROW_NONSECB_IDAU			0x0		/* 0x0 = IDAU is secured ; IDAU is Non-secured */
#define UROW_NONSECB_DSU			0x0		/* 0x0 = DSU is secured ; DSU is Non-secured */
#define UROW_NONSECB_NVMCTRL		0x0		/* 0x0 = NVMCTRL is secured ; NVMCTRL is Non-secured */
#define UROW_NONSECB_DMAC			0x0		/* 0x0 = DMAC is secured ; DMAC is Non-secured */
#define UROW_NONSECB_HMATRIXHS		0x0		/* 0x0 = HMATRIXHS is secured ; HMATRIXHS is Non-secured */
#define UROW_NONSECC_EVSYS			0x0		/* 0x0 = EVSYS is secured ; EVSYS is Non-secured */
#define UROW_NONSECC_SERCOM0		0x1		/* 0x0 = SERCOM0 is secured ; SERCOM0 is Non-secured */
#define UROW_NONSECC_SERCOM1		0x0		/* 0x0 = SERCOM1 is secured ; SERCOM1 is Non-secured */
#define UROW_NONSECC_SERCOM2		0x0		/* 0x0 = SERCOM2 is secured ; SERCOM2 is Non-secured */
#define UROW_NONSECC_TC0			0x0		/* 0x0 = TC0 is secured ; TC0 is Non-secured */
#define UROW_NONSECC_TC1			0x1		/* 0x0 = TC1 is secured ; TC1 is Non-secured */
#define UROW_NONSECC_TC2			0x0		/* 0x0 = TC2 is secured ; TC2 is Non-secured */
#define UROW_NONSECC_ADC			0x0		/* 0x0 = ADC is secured ; ADC is Non-secured */
#define UROW_NONSECC_DAC			0x0		/* 0x0 = DAC is secured ; DAC is Non-secured */
#define UROW_NONSECC_PTC			0x0		/* 0x0 = PTC is secured ; PTC is Non-secured */
#define UROW_NONSECC_TRNG			0x0		/* 0x0 = TRNG is secured ; TRNG is Non-secured */
#define UROW_NONSECC_CCL			0x0		/* 0x0 = CCL is secured ; CCL is Non-secured */
#define UROW_NONSECC_OPAMP			0x0		/* 0x0 = OPAMP is secured ; OPAMP is Non-secured */
#define UROW_NONSECC_TRAM			0x0		/* 0x0 = TRAM is secured ; TRAM is Non-secured */

__attribute__((section (".userrowsec")))
const uint32_t userRow[7] = { 
						((UROW_WDT_PER<<28)|(UROW_WDT_ALWAYS_ON<<27)|(UROW_WDT_ENABLE<<26)|(UROW_WDT_RUNSTDBY<<25)|(UROW_BOD12_CALIB_PARAMS<<16)|(UROW_BOD33_ACTION<<14)|(UROW_BOD33_DISABLE<<13)|(UROW_BOD33_LEVEL<<7)|(0x1<<6)|(UROW_NSULCK_DNS<<5)|(UROW_NSULCK_ANS<<4)|(UROW_NSULCK_BNS<<3)|(UROW_SULCK_DS<<2)|(UROW_SULCK_AS<<1)|UROW_SULCK_BS) , 
						((0xFF<<24)|(0xFF<<16)|(0x7<<13)|(UROW_DXN<<12)|(UROW_RXN<<11)|(UROW_BOD33_HYST<<9)|(UROW_WDT_WEN<<8)|(UROW_WDT_EWOFFSET<<4)|UROW_WDT_WINDOW), 
						((UROW_RS<<24)|(UROW_DS<<16)|(UROW_ANSC<<8)|UROW_AS), 
						UROW_URWEN, 
						((UROW_NONSECA_AC<<13)|(UROW_NONSECA_PORT<<12)|(UROW_NONSECA_FREQM<<11)|(UROW_NONSECA_EIC<<10)|(UROW_NONSECA_RTC<<9)|(UROW_NONSECA_WDT<<8)|(UROW_NONSECA_GCLK<<7)|(UROW_NONSECA_SUPC<<6)|(UROW_NONSECA_OSC32KCTRL<<5)|(UROW_NONSECA_OSCCTRL<<4)|(UROW_NONSECA_RSTC<<3)|(UROW_NONSECA_MCLK)<<2|(UROW_NONSECA_PM<<1)|UROW_NONSECA_PAC), 
						((UROW_NONSECB_HMATRIXHS<<4)|(UROW_NONSECB_DMAC<<3)|(UROW_NONSECB_NVMCTRL<<2)|(UROW_NONSECB_DSU<<1)|UROW_NONSECB_IDAU), 
						((UROW_NONSECC_TRAM<<13)|(UROW_NONSECC_OPAMP<<12)|(UROW_NONSECC_CCL<<11)|(UROW_NONSECC_TRNG<<10)|(UROW_NONSECC_PTC<<9)|(UROW_NONSECC_DAC<<8)|(UROW_NONSECC_ADC<<7)|(UROW_NONSECC_TC2<<6)|(UROW_NONSECC_TC1<<5)|(UROW_NONSECC_TC0<<4)|(UROW_NONSECC_SERCOM2<<3)|(UROW_NONSECC_SERCOM1<<2)|(UROW_NONSECC_SERCOM0<<1)|UROW_NONSECC_EVSYS) 
};


#endif /* SAML11_UROW_H_ */