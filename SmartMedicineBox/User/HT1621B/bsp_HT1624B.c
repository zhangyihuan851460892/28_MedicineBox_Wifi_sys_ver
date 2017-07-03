/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  xiaobing19851019@163.com
  * @version V1.0
  * @date    2015.08.27
  * @brief   智能药盒LED控制驱动
  ******************************************************************************
  * @attention
  *
  * 平台		:STM32F103ZET6
  * 公司    :http://www.jjc12320.com
  *
  ******************************************************************************
  */ 
	
#include "bsp_HT1621B.h"


//定义要字符编码
/*0,1,2,3,4,5,6,7,8,9,DOT*/
const char num4_8[11]={0xD7,0x50,0xB5,0xF1,0x72,0xE3,0xE7,0x51,0xF7,0xF3,0x08};

static void HT1621B_GPIOConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 /*| GPIO_Pin_6 */;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //外接了上拉电阻，采用开漏输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOE,GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5);
}

//写数据到1621
void SendBit_1621(u8 sdata,u8 cnt)
{
	u8 i = 0;
	
	for(i = 0;i < cnt; i++)
	{
		if(sdata&0x80)  
		{
			HT1621B_DATA(1);
		}else
		{
			HT1621B_DATA(0);
		}
		Delay_us(10);
		HT1621B_WR(0);
		Delay_us(10);
		HT1621B_WR(1);
		Delay_us(10);
		sdata <<= 1;
	}
}

void SendDataBit_1621(u8 data2,u8 cnt) //data的低cnt位写入HT1621，低位在前 
{ 
 u8 i = 0;
	
 for(i =0; i <cnt; i ++) 
 { 
  if((data2&0x01)==0) 
	{HT1621B_DATA(0);}
  else 
	{HT1621B_DATA(1);}
	Delay_us(10);
  HT1621B_WR(0); 
  Delay_us(10);
  HT1621B_WR(1);  
  data2>>=1; 
 } 
} 

void SendCmd_1621(u8 cmd)
{
	HT1621B_CS(0);
	Delay_us(100);
	SendBit_1621(Ctrl_Cmd,4);  //不使用修改时钟的命令，所以命令高位直接写0，发送100 0下去
	
	SendBit_1621(cmd,8);
	
	HT1621B_CS(1);
	Delay_us(10);
}


void WriteData_1621(u8 addr,u8 wdata)
{
	addr <<= 2;
	
	HT1621B_CS(0);
	Delay_us(100);
	SendBit_1621(Data_Cmd,3);  //write "101" code
	SendBit_1621(addr,6);			//write addr MSB[7:2]
	SendDataBit_1621(wdata,8);		//write wdata LSB[3:0]
	
	HT1621B_CS(1);
	Delay_us(10);
}

void HT1621_all_on(void) 
{
	u8 i;
  u8 addr = 0;
  for(i=0;i<8;i++)
	{
		WriteData_1621(addr,0xff);
		addr+=2;
	}
}

void HT1621_Test(void) 
{ 
	u8 i,j; 
  u8 addr = 0; 
	for(j = 0; j < 11; j++)
	{
		addr = 0;
		for(i=0;i<8;i++) 
		{
			WriteData_1621(addr,num4_8[j]); 
			addr+=2; 
		} 
		Delay_ms(1000);
	}
} 



//lcd 显示 小时
void LCD_DisplayHours(u8 hours,u8 dot)
{
	u8 i = 0;
	if(dot)
		i = num4_8[hours/10]|num4_8[10];
	else
		i = num4_8[hours/10]&(~num4_8[10]);
	WriteData_1621(0,i);
	WriteData_1621(2,num4_8[hours%10]);
}

//LCD 显示 分钟
void LCD_DisplayMinutes(u8 minutes)
{
	WriteData_1621(4,num4_8[minutes/10]);
	WriteData_1621(6,num4_8[minutes%10]);
}




//HT1621B init
void BSP_HT1621B_Init(void)
{
	HT1621B_GPIOConfig();
	Delay_ms(10);

	SendCmd_1621(0x00);
	SendCmd_1621(0x52);
	
	SendCmd_1621(0x30);
	
	SendCmd_1621(WDTDIS);
	SendCmd_1621(0x02);

	SendCmd_1621(0x06);
	Delay_ms(10);

	HT1621_all_on();


}






//***************************end of file**********************************
