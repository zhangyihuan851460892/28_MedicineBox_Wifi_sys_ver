/**
  ******************************************************************************
  * @file    bsp_tm1628.c
  * @author  xiaobing19851019@163.com
  * @version V1.0
  * @date    2015.08.28
  * @brief   TM1628驱动
  ******************************************************************************
  * @attention
  *
  * 平台		:STM32F103ZET6 
  * 公司    :http://www.jjc12320.com
  *
  ******************************************************************************
  */

#include "bsp_tm1628.h"



static void Delay_us(uint32_t time)
{
	while(time*20)
	{
		time--;
	}
}

static void wDelay_ms(uint32_t time)
{
	Delay_us(time*1000);
}

void BSP_TM1628_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	
	//PE2 STB;PE3 CLK ;PE4 DIO; PE5 POWERCTRL
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOE,GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5);
}


void TM1628_TxByte(u8 data)
{
	u8 i = 0;
	
	TM1628_CLK(1);
	Delay_us(2);
	
	for(i = 0; i < 8; i++)
	{
		Delay_us(2);
		TM1628_CLK(0);
		Delay_us(3);
		if(data&0x01)	TM1628_DIO(1);
		else					TM1628_DIO(0);
		Delay_us(5);
		TM1628_CLK(1);
		data >>= 1;
	}
	Delay_us(3);
}


void TM1628_WriteCmd(u8 cmd)
{
	TM1628_STB(1);
	Delay_us(3);
	TM1628_STB(0);
	Delay_us(3);
	TM1628_TxByte(cmd);
	
}

void TM1628_WriteDat(u8 dat)
{
	TM1628_STB(0);
	Delay_us(3);
	TM1628_TxByte(dat);
	Delay_us(3);
	TM1628_STB(1);
}


//数码管显示时间
void TM1628_DisplayTime(u8 hour,u8 min,u8 dot)
{
	u8 dis = 0;
	u8 dat = 0;
	TM1628_WriteCmd(0x03);
  TM1628_STB(1);
  TM1628_WriteCmd(0x44);
  TM1628_STB(1);
	TM1628_WriteCmd(0x82);
  TM1628_STB(1);
	
//hour display
  TM1628_WriteCmd(0xc0);
	dat = hour/10;
	switch(dat)
	{
		case 0:	dis = (0x3F); break;
		case 1:	dis = (0x06); break;
		case 2:	dis = (0x5B); break;
		case 3:	dis = (0x4F); break;
		case 4:	dis = (0x66); break;
		case 5:	dis = (0x6D); break;
		case 6:	dis = (0x7D); break;
		case 7:	dis = (0x07); break;
		case 8:	dis = (0x7F); break;
		case 9:	dis = (0x6F); break;
		default: dis = (0x00); break;
	}
	TM1628_WriteDat(dis);
	
	TM1628_WriteCmd(0xc2);
	dat = hour%10;
	switch(dat)
	{
		case 0:	dis = (0x3F); break;
		case 1:	dis = (0x06); break;
		case 2:	dis = (0x5B); break;
		case 3:	dis = (0x4F); break;
		case 4:	dis = (0x66); break;
		case 5:	dis = (0x6D); break;
		case 6:	dis = (0x7D); break;
		case 7:	dis = (0x07); break;
		case 8:	dis = (0x7F); break;
		case 9:	dis = (0x6F); break;
		default: dis = (0x00); break;
	}
	if(dot)	dis = (dis | 0x80);
  TM1628_WriteDat(dis);
	
//minute display
	TM1628_WriteCmd(0xc4);
	dat = min/10;
	switch(dat)
	{
		case 0:	dis = (0x3F); break;
		case 1:	dis = (0x06); break;
		case 2:	dis = (0x5B); break;
		case 3:	dis = (0x4F); break;
		case 4:	dis = (0x66); break;
		case 5:	dis = (0x6D); break;
		case 6:	dis = (0x7D); break;
		case 7:	dis = (0x07); break;
		case 8:	dis = (0x7F); break;
		case 9:	dis = (0x6F); break;
		default: dis = (0x00); break;
	}
	TM1628_WriteDat(dis);
	
	TM1628_WriteCmd(0xc6);
	dat = min%10;
	switch(dat)
	{
		case 0:	dis = (0x3F); break;
		case 1:	dis = (0x06); break;
		case 2:	dis = (0x5B); break;
		case 3:	dis = (0x4F); break;
		case 4:	dis = (0x66); break;
		case 5:	dis = (0x6D); break;
		case 6:	dis = (0x7D); break;
		case 7:	dis = (0x07); break;
		case 8:	dis = (0x7F); break;
		case 9:	dis = (0x6F); break;
		default: dis = (0x00); break;
	}
	TM1628_WriteDat(dis);
	
	TM1628_WriteCmd(0x8A);
	TM1628_STB(1);
}



//*****************END FILE****************************************//
