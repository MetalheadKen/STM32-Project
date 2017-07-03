/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MY_UART_H
#define __MY_UART_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
	 
extern char uart_buffer[100];
extern uint16_t uart_length;
	 
void Usart3_Init(void);
void Usart3_Interrupt(void);
void Usart3_Printf(char *);
void Usart3_ReadLine(void);

#ifdef __cplusplus
}
#endif

#endif /* __MY_UART_H */
