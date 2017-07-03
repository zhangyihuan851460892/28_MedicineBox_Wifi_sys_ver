#ifndef __MEDICINEBOXOPERATION_H__
#define __MEDICINEBOXOPERATION_H__


#include "stm32f10x.h"
#include "bsp_usart.h"
#include "bsp_rtc.h"
#include "bsp_TiMbase.h"
#include "bsp_sc5080.h"
#include "bsp_led.h"
#include "bsp_i2c_ee.h"
#include "bsp_TiMbase.h"
#include "AT_Protocol.h"
#include "bsp_adc.h"
#include "bsp_HT1621B.h"
#include "define.h"
#include "DeviceInit.h"
#include "bsp_isd1700.h"
#include "bsp_led.h"
#include "bsp_esp8266.h"


extern u8 SysStart;

//#define TIMER3(a) if(a) {eventdelay.EventSet |= 0x800;RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);}\
									else  {RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , DISABLE)};
									
#define TIMER3(a) if(a) {eventdelay.EventSet |= 0x800;eventdelay.EventDelay[11] = 0;}\
									else  {eventdelay.EventSet &= ~0x800;eventdelay.EventDelay[11] = 0;};
										
									
									
void AuthorizeLogin(void);
void DecideTakeMedicineRemind(void);
void BatteryStatusFlash(void);
void PollsSendHttpsRequest(void);
void DecideTakeMedicineRemind(void);
void CurrentMedicineBoxDetection(u8 index,short alerttm,short validdate);
void CurrentMedicineBoxCtrl(u8 index,short validdate);
void DetectionBoxStatus(u8 index);
void ClearAllLedWarn(void);

#endif
