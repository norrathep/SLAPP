/*
 * timer.c
 *
 * Created: 2/15/2024 3:29:17 PM
 *  Author: Oak
 */ 

#include "timer.h"


int cur_period = 0;
uint64_t num_overflows = 0;
//-----------------------------------------------------------------------------
void timer_set_period(uint16_t i)
{
	TC1->COUNT16.CC[0].reg = (F_CPU / 1000ul / 256) * i;

	TC1->COUNT16.COUNT.reg = 0;
}

//-----------------------------------------------------------------------------
void timer_init(int period)
{
	MCLK->APBCMASK.reg |= MCLK_APBCMASK_TC1;

	GCLK->PCHCTRL[TC1_GCLK_ID].reg = GCLK_PCHCTRL_GEN(0) | GCLK_PCHCTRL_CHEN;
	while (0 == (GCLK->PCHCTRL[TC1_GCLK_ID].reg & GCLK_PCHCTRL_CHEN));

	TC1->COUNT16.CTRLA.reg = TC_CTRLA_MODE_COUNT16 | TC_CTRLA_PRESCALER_DIV256 |
	TC_CTRLA_PRESCSYNC_RESYNC;

	TC1->COUNT16.WAVE.reg = TC_WAVE_WAVEGEN_MFRQ;

	TC1->COUNT16.COUNT.reg = 0xfecd;

	timer_set_period(period);

	TC1->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE;

	TC1->COUNT16.INTENSET.reg = TC_INTENSET_MC(1);
	NVIC_EnableIRQ(TC1_IRQn);
	
	cur_period = period;
	num_overflows = 0;
}

void TC1_Handler(void)
{
	num_overflows++;
	if (TC1->COUNT16.INTFLAG.reg & TC_INTFLAG_MC(1))
	{
		//HAL_GPIO_LED_toggle();
		TC1->COUNT16.INTFLAG.reg = TC_INTFLAG_MC(1);
	}
}

// 1k num_overflows = 1 sec
uint64_t timer_time()
{
	return num_overflows;	
}

uint64_t timer_ticks()
{
	return TC1->COUNT16.COUNT.reg;
}

void timer_reset()
{
	num_overflows = 0;	
}