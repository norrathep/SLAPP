/*
 * PZEM004Tv30.c
 *
 * Created: 3/21/2024 2:42:25 PM
 *  Author: super
 */ 

#include "PZEM004Tv30.h"

Sercom *ser;

void PZEM004Tv30_init()
{
	ser = SERCOM0;
	uint32_t baud = 9600;
	uint64_t br = (uint64_t)65536 * (F_CPU - 16 * baud) / F_CPU;

	HAL_GPIO_UART_TX_out();
	HAL_GPIO_UART_TX_pmuxen(HAL_GPIO_PMUX_C);
	HAL_GPIO_UART_RX_in();
	HAL_GPIO_UART_RX_pmuxen(HAL_GPIO_PMUX_C);

	MCLK->APBCMASK.reg |= MCLK_APBCMASK_SERCOM0;

	GCLK->PCHCTRL[SERCOM0_GCLK_ID_CORE].reg = GCLK_PCHCTRL_GEN(0) | GCLK_PCHCTRL_CHEN;
	while (0 == (GCLK->PCHCTRL[SERCOM0_GCLK_ID_CORE].reg & GCLK_PCHCTRL_CHEN));

	ser->USART.CTRLA.reg =
	SERCOM_USART_CTRLA_DORD | SERCOM_USART_CTRLA_MODE(1/*USART_INT_CLK*/) |
	SERCOM_USART_CTRLA_RXPO(3/*PAD3*/) | SERCOM_USART_CTRLA_TXPO(1/*PAD2*/);

	ser->USART.CTRLB.reg = SERCOM_USART_CTRLB_RXEN | SERCOM_USART_CTRLB_TXEN |
	SERCOM_USART_CTRLB_CHSIZE(0/*8 bits*/);

	ser->USART.BAUD.reg = (uint16_t)br;

	ser->USART.CTRLA.reg |= SERCOM_USART_CTRLA_ENABLE;
	
	while (ser->USART.SYNCBUSY.reg & SERCOM_USART_SYNCBUSY_ENABLE);
}

//-----------------------------------------------------------------------------
void send_byte(uint8_t c)
{
	while (!(ser->USART.INTFLAG.reg & SERCOM_USART_INTFLAG_DRE));
	ser->USART.DATA.reg = c;
}//-----------------------------------------------------------------------------
void send_str(char *s)
{
	while (*s)
	send_byte(*s++);
}


uint8_t recv_byte()
{
	char rx;
	while(!(ser->USART.INTFLAG.reg & SERCOM_USART_INTFLAG_RXC))   /* wait until Rx full */
	{
		if(ser->USART.INTFLAG.reg & SERCOM_USART_INTFLAG_ERROR)
		{
			ser->USART.INTFLAG.reg |= SERCOM_USART_INTFLAG_ERROR; /* clear any errors */
			break;
		}
	}
	rx = (uint8_t)ser->USART.DATA.reg;
	return rx;
}