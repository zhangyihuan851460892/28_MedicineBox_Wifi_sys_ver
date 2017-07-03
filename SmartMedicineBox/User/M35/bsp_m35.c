/**
  ******************************************************************************
  * @file    bsp_m35.c
  * @author  xiaobing19851019@163.com
  * @version V1.0
  * @date    2013.09.01
  * @brief   m35模块驱动程序
  ******************************************************************************
  * @attention
  *
  * 平台		:STM32F103ZET6 
  * 公司    :http://www.jjc12320.com
  * 
  *
  ******************************************************************************
  */ 
#include "bsp_m35.h"




/**
* @brief 初始化M35模块IO口
* @param 
* @param 
* @return 
*/
void BSP_M35Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC,ENABLE);
	
	//PC6 POWERKEY
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	//PB2 M35POWER
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	//PC1 ATKEY
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	//PA15 M35_STATUS
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOC,GPIO_Pin_6);
	GPIO_SetBits(GPIOB,GPIO_Pin_2);
	
	//PB10 EPOWER_C  外部电源供电检测
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
}


/**
* @brief 上电复位M35模块
* @param 
* @param 
* @return 
*/
void ResetM35Module(void)
{
#if 0
	M35ModuleShutdown(0);  //软件关机
	Delay_ms(500);
	printf("M35_STATUS = %d\r\n",M35_STATUS);
	M35_POWERCTRL(0);
	Delay_ms(1000);
	while(M35_STATUS == 0);
	M35_POWERCTRL(1);
	Delay_ms(500);
#endif
		//重新上电
		M35_POWERKEY(0);
		Delay_ms(100);
		M35_POWERKEY(1);
		Delay_ms(2000);
	printf("M35_STATUS = %d\r\n",M35_STATUS);
//	if(M35_STATUS == 0)
	{
		M35_POWERCTRL(0);
		Delay_ms(1000);
		while(M35_STATUS == 0);
		M35_POWERCTRL(1);
		Delay_ms(500);
	}
}


/**
* @brief 模块关机，关机时序等待12秒，如果关机成功则直接返回
* @param type 类型 0 软件关机 1 硬件关机
* @param 
* @return 1 关机成功 0 关机失败
*/
u8 M35ModuleShutdown(u8 type)
{
	u8 i = 24;
	u8 sendbuf[13] = "AT+QPOWD=1\r\n";
	if(type)
	{
		M35_POWERCTRL(0);
		Delay_ms(900);
		M35_POWERCTRL(1);
		while((M35_STATUS == 1) && (i--))
		{
			Delay_ms(500);
		}
		return M35_STATUS;
	}else
	{
		Usart_Send(1,sendbuf,12);
		while((M35_STATUS == 1) && (i--))
		{
			Delay_ms(500);
		}
		return M35_STATUS;
	}
}

/**
* @brief 模块重启
* @param 
* @param 
* @return 1 重启成功 0 重启失败
*/
u8 RebootM35Module(void)
{
	M35_POWERCTRL(0);
	Delay_ms(900);
	M35_POWERCTRL(1);
	while(M35_STATUS == 1);
	Delay_ms(2000);
	M35_POWERCTRL(0);
	while(M35_STATUS == 0);
	M35_POWERCTRL(1);
	Delay_ms(500);
	
	return M35_STATUS;
}

//**************************END OF FILE*******************************//
