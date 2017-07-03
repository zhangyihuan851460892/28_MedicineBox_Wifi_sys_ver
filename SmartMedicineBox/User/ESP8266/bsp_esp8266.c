/**
  ******************************************************************************
  * @file    bsp_esp8266.c
  * @author  xiaobing19851019@163.com
  * @version V1.0
  * @date    2015.11.28
  * @brief   ESP8266驱动
  ******************************************************************************
  * @attention
  *
  * 平台		:STM32F103ZET6 
  * 公司    :http://www.jjc12320.com
  * 
  *
  ******************************************************************************
  */ 
#include "bsp_esp8266.h"



/**
* @brief 初始化ESP8266
* @param 
* @param 
* @return 
*/
void Bsp_ESP8266_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
#if 0	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOF,ENABLE);
	
	//PF3 WIFI_POWER PF5 WIFI_SIGNAL
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_3 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOF,&GPIO_InitStructure);
	//PC1 ATKEY
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOF,GPIO_Pin_5 | GPIO_Pin_3);
#endif
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_0;		// WIFI_PD PC0
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	
		//PC1 ATKEY
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_1;								// PC1_AT  PC1
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	
	
	GPIO_ResetBits(GPIOC,GPIO_Pin_0);
	
}

/**
* @brief wifi信号灯闪烁一次
* @param 
* @param 
* @return 
*/
void Wifi_SignalFlash(void)
{
#if 0
	if(GPIO_ReadOutputDataBit(GPIOF,GPIO_Pin_3))
	{
		eventdelay.EventSet |= 0x100;
		if(eventdelay.EventDelay[8] > 0)
		{
			if((eventdelay.EventDelay[8]) > 500)
			{

				GPIOF->ODR ^= GPIO_Pin_5;

				eventdelay.EventDelay[8] = 0;
			}
		}
	}else
	{
		eventdelay.EventSet &= ~0x100;
		eventdelay.EventDelay[8] = 0;
		GPIO_ResetBits(GPIOF,GPIO_Pin_5);
	}
#endif

}

/**
* @brief 软重启esp8266
* @param 
* @param 
* @return 
*/
void ResetEsp8266(void)
{
	int len = 0;
	char uartdata[128];
	
	len = PrepareATCmd(RST,uartdata);
	Delay_ms(20);
	Usart_Send(2,(unsigned char *)uartdata,len); // send to wifi
}

/**
* @brief 设置esp8266进行SMARTLINK链接
* @param 
* @param 
* @return 
*/
void SmartLinkStart(void)
{
	int len = 0;
	char uartdata[128];
	// user usart2 send cmd to wifi
	len = PrepareATCmd(CWMODE,uartdata);
	Delay_ms(20);
	Usart_Send(2,(unsigned char *)uartdata,len);
	Delay_ms(20);
	len = PrepareATCmd(SMARTLINK,uartdata);
	Delay_ms(20);
	Usart_Send(2,(unsigned char *)uartdata,len);
	Delay_ms(20);
	protocolstu.nNeedATKeyCmd = 0;
}

void SmartLinkStop(void)
{
	int len = 0;
	char uartdata[50];
	
	
	len = PrepareATCmd(SMARTLINKSTOP,uartdata);
	Delay_ms(20);
	Usart_Send(2,(unsigned char *)uartdata,len);
	Delay_ms(20);
	len = PrepareATCmd(SMARTLINKSTOP,uartdata);
	Delay_ms(20);
	Usart_Send(2,(unsigned char *)uartdata,len);
	
	
}


/**
* @brief 设置esp8266断开AP链接
* @param 
* @param 
* @return 
*/
void Esp8266APDisconnect(void)
{
	int len = 0;
	char uartdata[32];
	
	len = sprintf(uartdata,"AT+CWQAP\r\n");
	Delay_ms(20);
	Usart_Send(2,(unsigned char *)uartdata,len);
	Delay_ms(20);
}

// clean esp8266 flash
void reconfig_wifi(void)
{
	int len = 0;
	char data[64];
	
	protocolstu.nNeedATKeyCmd = 2;
	len = PrepareATCmd(RESTORE,data);
	Delay_ms(20);
	Usart_Send(2,(unsigned char*)data,len);	
	Delay_ms(1000);
	
}

// NETLIGHT  
// 1 GLED
// 0 RLED
// 2 all close

void wifi_status(char status)
{
	if(status == 1)
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);	// //	NETLIGHT_GLED PC13 turn on network connet success
		GPIO_SetBits(GPIOE,GPIO_Pin_14);	//	NETLIGHT_RLED PE14 turn off		
	}
	else if(status == 2)
	{
		GPIO_SetBits(GPIOE,GPIO_Pin_14); //	NETLIGHT_RLED PE14 turn on network connet fail
		GPIO_SetBits(GPIOC,GPIO_Pin_13);		// NETLIGHT_GLED turn off
	}
	else
	{
		GPIO_ResetBits(GPIOE,GPIO_Pin_14); //	NETLIGHT_RLED PE14 turn on network connet fail
		GPIO_SetBits(GPIOC,GPIO_Pin_13);		// NETLIGHT_GLED turn off
	}
}

//=================================END OF FILE========================================
