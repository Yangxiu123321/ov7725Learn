#ifndef _USART_H
#define _USART_H

#include <stdio.h>
#include "stm32f4xx.h"
#include "config.h"

void USART_Configuration(void);
void USART_NVIC_Config(void);
void UART3_SendChar(int32_t disp);
char *itoa(int value, char *string, int radix);
void USART_OUT(USART_TypeDef* USARTx, const char *Data, ...);
#endif /*_USART_H*/
