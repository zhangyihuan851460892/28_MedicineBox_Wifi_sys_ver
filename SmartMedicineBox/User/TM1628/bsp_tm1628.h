#ifndef __BSP_TM1628_H__
#define __BSP_TM1628_H__

#include "stm32f10x.h"
#include "bsp_SysTick.h"

#define ON		1
#define OFF		0

#define TM1628_CTRL(a) 	if(a)	GPIO_SetBits(GPIOE,GPIO_Pin_5); \
												else 	GPIO_ResetBits(GPIOE,GPIO_Pin_5)

#define TM1628_STB(a) 	if(a)	GPIO_SetBits(GPIOE,GPIO_Pin_2); \
												else 	GPIO_ResetBits(GPIOE,GPIO_Pin_2)

#define TM1628_CLK(a) 	if(a)	GPIO_SetBits(GPIOE,GPIO_Pin_3); \
												else 	GPIO_ResetBits(GPIOE,GPIO_Pin_3)

#define TM1628_DIO(a) 	if(a)	GPIO_SetBits(GPIOE,GPIO_Pin_4); \
												else 	GPIO_ResetBits(GPIOE,GPIO_Pin_4)
									

void BSP_TM1628_Init(void);
void TM1628_TxByte(u8 data);
void TM1628_WriteCmd(u8 cmd);		
void TM1628_WriteDat(u8 dat);												

void TM1628_DisplayTime(u8 hour,u8 min,u8 dot);

						
#endif
