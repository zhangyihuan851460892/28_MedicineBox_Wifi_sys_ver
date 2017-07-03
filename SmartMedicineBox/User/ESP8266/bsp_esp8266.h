#ifndef __BSP_ESP8266_H__
#define __BSP_ESP8266_H__

#include "stm32f10x.h"
#include "bsp_TiMbase.h"
#include "bsp_usart.h"
#include "AT_Protocol.h"


// WIFI_PD
#define ESP8266(a) 	if(a)  	GPIO_SetBits(GPIOC,GPIO_Pin_0);\
										else		GPIO_ResetBits(GPIOC,GPIO_Pin_0);
					
#define AT_KEY	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1)		// AT KEY
										
void Bsp_ESP8266_Init(void);
void Wifi_SignalFlash(void);
void ResetEsp8266(void);
void SmartLinkStart(void);
void Esp8266APDisconnect(void);

void reconfig_wifi(void);
// network status 
void wifi_status(char status);
void SmartLinkStop(void);


#endif
