#ifndef __BSP_M35_H__
#define __BSP_M35_H__

#include "stm32f10x.h"
#include "AT_Protocol.h"

#define M35_POWERCTRL(a)		if(a)	\
															GPIO_SetBits(GPIOC,GPIO_Pin_6);	\
														else	\
															GPIO_ResetBits(GPIOC,GPIO_Pin_6)

#define M35_POWERKEY(a)		if(a)	\
															GPIO_SetBits(GPIOB,GPIO_Pin_2);	\
														else	\
															GPIO_ResetBits(GPIOB,GPIO_Pin_2)
														
#define AT_KEY	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1)
#define M35_STATUS GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)

														
										
#define USB_POWERDETEC GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)
														

void BSP_M35Init(void);
void ResetM35Module(void);
u8 RebootM35Module(void);
u8 M35ModuleShutdown(u8 type);


#endif
