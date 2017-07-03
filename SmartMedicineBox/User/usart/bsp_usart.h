#ifndef __USART_H__
#define	__USART_H__

#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include <stdio.h>
#include <string.h>


void USART_Config(void);
//int fputc(int ch, FILE *f);
//void USART1_printf(USART_TypeDef* USARTx, uint8_t *Data,...);
int Usart_Read(uint8_t serialNo,unsigned char *buf,int length);
int Usart_Send(uint8_t serialNo,unsigned char *buf,int length);




#endif /* __USART_H__ */
