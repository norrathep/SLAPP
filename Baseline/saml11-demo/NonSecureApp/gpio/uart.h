/*
 * uart.h
 *
 * Created: 2/15/2024 3:20:46 PM
 *  Author: Oak
 */ 


#ifndef UART_H_
#define UART_H_

#include "hal_gpio.h"

void uart_init(uint32_t baud);

void uart_putc(char c);

void uart_puts(char *s);

void uart_puti(int i);

void uart_putui64(uint64_t i);

char uart_getc();

void uart_gets(char* s);

int uart_geti();

void uart_get_bytes(uint8_t* output, int size);

void uart_put_bytes(uint8_t* output, int size);



#endif /* UART_H_ */