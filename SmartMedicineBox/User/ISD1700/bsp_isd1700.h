#ifndef __BSP_ISD1700_H__
#define __BSP_ISD1700_H__

#include "stm32f10x.h"
#include "bsp_TiMbase.h"
#include "stm32f10x_it.h"
#include "bsp_usart.h"
#include "bsp_i2c_ee.h"
#include "AT_Protocol.h"




#define KEY_ISD_REC 		  GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_6)
#define KEY_ISD_ERASE			GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_7)


extern __IO u8 bCustomRecord;
void Bsp_ISD1700Init(void);

void ISD_Play(void);
void ISD_Rec(void);
void ISD_Erase(void);
void Process_ISD_REC(void);
void Process_ISD_ERASE(void);
void BEEP(void);

void SaveCustomRecordFlag(void);
#endif
