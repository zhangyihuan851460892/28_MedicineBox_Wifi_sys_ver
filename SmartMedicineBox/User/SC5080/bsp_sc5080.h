#ifndef __BSP_SC5080_H__
#define __BSP_SC5080_H__


#include "stm32f10x.h"
#include "define.h"
#include "bsp_isd1700.h"


#define SC5080_BUSY(a) 	if(a) GPIO_SetBits(GPIOE,GPIO_Pin_13); \
												else	GPIO_ResetBits(GPIOE,GPIO_Pin_13)

#define SC5080_SDA(a)		if(a) GPIO_SetBits(GPIOE,GPIO_Pin_15); \
												else	GPIO_ResetBits(GPIOE,GPIO_Pin_15)												
							
#define SC5080_POWER(a)	//	if(a) GPIO_SetBits(GPIOA,GPIO_Pin_5); \
												else	GPIO_ResetBits(GPIOA,GPIO_Pin_5)		
													
												
void BSP_SC5080_Init(void);
void BSP_SC5080_Ctrl(u8 data);
void VoiceReminder(u8 voice);
void VoiceLoop(void);
void SetVoiceQueue(u8 voice);
void VoiceLevelCtrl(void);
											

#endif
