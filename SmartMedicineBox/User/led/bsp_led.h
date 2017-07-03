#ifndef __BSP_LED_H__
#define __BSP_LED_H__

#include "stm32f10x.h"
#include "define.h"
#include "stm32f10x_it.h"
#include "bsp_sc5080.h"

#if 1

#define MEDICINE_LED(a)	if (a)	\
					GPIO_SetBits(GPIOF,GPIO_Pin_12);\
					else		\
					GPIO_ResetBits(GPIOF,GPIO_Pin_12)

#define BATTERY_LED(a)//	if (a)	\
					GPIO_SetBits(GPIOF,GPIO_Pin_11);\
					else		\
					GPIO_ResetBits(GPIOF,GPIO_Pin_11)

#define BT_LED(a)//	if (a)	\
					GPIO_SetBits(GPIOB,GPIO_Pin_2);\
					else		\
					GPIO_ResetBits(GPIOB,GPIO_Pin_2)
					
#define OCKEY_DETEC	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0)
					

#endif
					
void LED_GPIO_Config(void);
void Medicine_LED_Ctrl(u8 day,u8 time,int flag);
void Medicine_LED_CtrlByPosition(u8 index,u8 flag);
u8 Medicine_Status_Detec(u8 day,u8 time);
u8 Detect_MedicineInfo(u8 position);
char Detect_TakeAndPutStatus(u8 position);
void MediceineBox_BigLedFlash(u8 bFlash);
void MedicineBoxLedFlash(void);
					
#endif
