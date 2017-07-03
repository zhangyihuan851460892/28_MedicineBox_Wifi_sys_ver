#ifndef __POWERMANAGE_H__
#define __POWERMANAGE_H__

#include "stm32f10x.h"



void PowerManage_Init(void);

#define GSMPOWER(a) if(a)	\
					GPIO_SetBits(GPIOA,GPIO_Pin_4);\
					else		\
					GPIO_ResetBits(GPIOA,GPIO_Pin_4)
											
#define GSMPOWER(a) if(a)	\
					GPIO_SetBits(GPIOA,GPIO_Pin_4);\
					else		\
					GPIO_ResetBits(GPIOA,GPIO_Pin_4)
					
#define GSMPOWER(a) if(a)	\
					GPIO_SetBits(GPIOA,GPIO_Pin_4);\
					else		\
					GPIO_ResetBits(GPIOA,GPIO_Pin_4)
					

#endif

