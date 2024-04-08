/*
 * delay.h
 *
 * Created: 2/15/2024 3:38:11 PM
 *  Author: Oak
 */ 


#ifndef DELAY_H_
#define DELAY_H_



inline void delayMicroseconds(uint32_t us){
	us = us;
	volatile uint32_t count = 0;
	for (count=us; count>0; count--);
}

inline void delayMilliseconds(uint32_t ms){
	int i=0;
	for (i=ms; i>0; i--)
	{
		delayMicroseconds(1000);
	}
}

inline void delaySeconds(uint32_t s){
	int i=0;
	for (i=s; i>0; i--)
	{
		delayMilliseconds(1000);
	}
}



#endif /* DELAY_H_ */