/**
  ******************************************************************************
  * @file    PowerManage.c
  * @author  xiaobing19851019@163.com
  * @version V1.0
  * @date    2015.09.16
  * @brief   外设模块电源管理
  ******************************************************************************
  * @attention
  *
  * 平台		:STM32F103ZET6
  * 公司    :http://www.jjc12320.com
  *
  ******************************************************************************
  */ 
	
#include "PowerManage.h"

void PowerManage_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOF,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_Out_PP;
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_Out_PP;
	
	GPIO_Init(GPIOF,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_1 ;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_IN_FLOATING;
	
	GPIO_Init(GPIOC,&GPIO_InitStructure);
}



//******************************END OF FILE******************************************
