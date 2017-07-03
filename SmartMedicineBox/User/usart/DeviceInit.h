#ifndef __DEVICEINIT_H__
#define __DEVICEINIT_H__

#include "stm32f10x.h"
#include "bsp_usart.h"
#include "AT_Protocol.h"
#include "bsp_esp8266.h"
#include "bsp_led.h"
#include "bsp_isd1700.h"



void SetDeviceID(void);
void RotationQuerySimStatus(void);
void TestLEDandSWITCH(void);
void TestRECandERASE(void);
	
#endif
