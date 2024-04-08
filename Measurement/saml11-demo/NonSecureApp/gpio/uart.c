/*
 * uart.c
 *
 * Created: 2/15/2024 3:13:08 PM
 *  Author: Oak
 */ 

#include "uart.h"

void uart_init(uint32_t baud)
{
	uint64_t br = (uint64_t)65536 * (F_CPU - 16 * baud) / F_CPU;

	HAL_GPIO_UART_TX_out();
	HAL_GPIO_UART_TX_pmuxen(HAL_GPIO_PMUX_C);
	HAL_GPIO_UART_RX_in();
	HAL_GPIO_UART_RX_pmuxen(HAL_GPIO_PMUX_C);

	MCLK->APBCMASK.reg |= MCLK_APBCMASK_SERCOM0;

	GCLK->PCHCTRL[SERCOM0_GCLK_ID_CORE].reg = GCLK_PCHCTRL_GEN(0) | GCLK_PCHCTRL_CHEN;
	while (0 == (GCLK->PCHCTRL[SERCOM0_GCLK_ID_CORE].reg & GCLK_PCHCTRL_CHEN));

	SERCOM0->USART.CTRLA.reg =
	SERCOM_USART_CTRLA_DORD | SERCOM_USART_CTRLA_MODE(1/*USART_INT_CLK*/) |
	SERCOM_USART_CTRLA_RXPO(3/*PAD3*/) | SERCOM_USART_CTRLA_TXPO(1/*PAD2*/);

	SERCOM0->USART.CTRLB.reg = SERCOM_USART_CTRLB_RXEN | SERCOM_USART_CTRLB_TXEN |
	SERCOM_USART_CTRLB_CHSIZE(0/*8 bits*/);

	SERCOM0->USART.BAUD.reg = (uint16_t)br;

	SERCOM0->USART.CTRLA.reg |= SERCOM_USART_CTRLA_ENABLE;
	
	while (SERCOM0->USART.SYNCBUSY.reg & SERCOM_USART_SYNCBUSY_ENABLE);
}

//-----------------------------------------------------------------------------
void uart_putc(char c)
{
	while (!(SERCOM0->USART.INTFLAG.reg & SERCOM_USART_INTFLAG_DRE));
	SERCOM0->USART.DATA.reg = c;
}

//-----------------------------------------------------------------------------
void uart_puts(char *s)
{
	while (*s)
	uart_putc(*s++);
}

void uart_puti(int i)
{
	char s[11];
	int last = 0;
	do {
		s[last] = i%10 + '0';
		i = i/10;
		last++;
	} while(i > 0);
	
	for(int j=last-1; j>=0; j--) {
		uart_putc(s[j]);
	}
}

void uart_putui64(uint64_t i)
{
	char s[11];
	int last = 0;
	do {
		s[last] = i%10 + '0';
		i = i/10;
		last++;
	} while(i > 0);
	
	for(int j=last-1; j>=0; j--) {
		uart_putc(s[j]);
	}
}


char uart_getc()
{
	char rx;
	while(!(SERCOM0->USART.INTFLAG.reg & SERCOM_USART_INTFLAG_RXC))   /* wait until Rx full */
	{
		if(SERCOM0->USART.INTFLAG.reg & SERCOM_USART_INTFLAG_ERROR)
		{
			SERCOM0->USART.INTFLAG.reg |= SERCOM_USART_INTFLAG_ERROR; /* clear any errors */
			break;
		}
	}
	rx = (uint8_t)SERCOM0->USART.DATA.reg;
	return rx;
}

void uart_gets(char* s) {
	int idx=0;
	for(;;) {
		char c = uart_getc();
		uart_putc(c);
		// enter key
		if(c==13) {
			uart_putc('A');
			s[idx] = 0;
			break;
		}
		s[idx] = c;
		idx++;
	}
}

void uart_get_bytes(uint8_t* output, int size){
	for (int i = 0; i < size; i++)
	{
		output[i] = uart_getc();
	}
}

void uart_put_bytes(uint8_t* output, int size){
	for (int i = 0; i < size; i++)
	{
		uart_putc(output[i]);
	}
}

int uart_geti() {
	char s[10];
	uart_gets(s);
	int out;
	sscanf(s, "%d", &out);
	return out;
}