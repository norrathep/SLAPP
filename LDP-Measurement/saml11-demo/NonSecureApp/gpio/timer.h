/*
 * timer.h
 *
 * Created: 2/15/2024 3:29:31 PM
 *  Author: Oak
 */ 


#ifndef TIMER_H_
#define TIMER_H_

#include "hal_gpio.h"

#define PERIOD_FAST     100
#define PERIOD_SLOW     500
#define PERIOD_10S		5000
#define PERIOD_1S		500
#define PERIOD_10MS		5
#define PERIOD_2MS		1
#define PERIOD_4MS		2

void timer_init(int period);
//void timer_set_period(uint16_t i);

uint64_t timer_time();
uint64_t timer_ticks();
void timer_reset();


#endif /* TIMER_H_ */