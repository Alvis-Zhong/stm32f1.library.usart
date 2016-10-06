#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"
#include "stdio.h"

#define			USART_BAUD		115200
#define			USART_PORT		USART1
#define			USART_CLK_FUN		RCC_APB2PeriphClockCmd
#define			USART_GPIO_CLK_FUN	RCC_APB2PeriphClockCmd
#define			GPIO_CLK		RCC_APB2Periph_GPIOA
#define			USART_CLK		RCC_APB2Periph_USART1
#define 		GPIO_PORT		GPIOA
#define 		GPIO_PIN_TX		GPIO_Pin_9
#define			GPIO_PIN_RX		GPIO_Pin_10
#define 		USART_IRQ		USART1_IRQn 

void usart_config(void) ; 

#endif

