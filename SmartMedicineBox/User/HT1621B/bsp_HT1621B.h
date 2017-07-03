#ifndef __BSP_HT1621B_H__
#define __BSP_HT1621B_H__

#include "stm32f10x.h"
#include "bsp_TiMbase.h"



#define LCD_ON		0x06  //
#define LCD_OFF		0x04

#define ComMode		0x52
#define SYS_EN		0x02
#define RC_OSC		0x30
#define WDTDIS		0x0a

#define Ctrl_Cmd	0x80
#define Data_Cmd	0xa0


#define HT1621B_CS(a)  	if(a) GPIO_SetBits(GPIOE,GPIO_Pin_2);	\
												else	GPIO_ResetBits(GPIOE,GPIO_Pin_2)
									
#define HT1621B_RD(a)		if(a) GPIO_SetBits(GPIOE,GPIO_Pin_3);	\
												else	GPIO_ResetBits(GPIOE,GPIO_Pin_3)

#define HT1621B_WR(a)		if(a) GPIO_SetBits(GPIOE,GPIO_Pin_4);	\
												else	GPIO_ResetBits(GPIOE,GPIO_Pin_4)

#define HT1621B_DATA(a)		if(a) GPIO_SetBits(GPIOE,GPIO_Pin_5);	\
													else	GPIO_ResetBits(GPIOE,GPIO_Pin_5)
													

void BSP_HT1621B_Init(void);
void LCD_DisplayHours(u8 hours,u8 dot);
void LCD_DisplayMinutes(u8 minutes);
void HT1621_Test(void);

													
#endif
