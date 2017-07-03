/**
  ******************************************************************************
  * @file    bsp_isd1700.c
  * @author  xiaobing19851019@163.com
  * @version V1.0
  * @date    2015.11.28
  * @brief   ISD1700板级驱动
  ******************************************************************************
  * @attention
  *
  * 平台		:STM32F103ZET6 
  * 公司    :http://www.jjc12320.com
  * 
  *
  ******************************************************************************
  */ 
#include "bsp_isd1700.h"



__IO u8 bCustomRecord = 0;


/**
* @brief 初始化ISD1720
* @param 
* @param 
* @return 
*/
void Bsp_ISD1700Init(void)
{
#if 0 // change by zhang
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC  | RCC_APB2Periph_GPIOF,ENABLE);
	
	//PC5 ISD_PLAY
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOC,GPIO_Pin_5);
	//PB0 ISD_REC
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	//PB1 ISD_ERASE
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_0 | GPIO_Pin_1);
	
	//PF6  KEY_REC  PF7 KEY_ERASE
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;
	
	GPIO_Init(GPIOF,&GPIO_InitStructure);
	
	//PC4 BEEP
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOC,GPIO_Pin_4);
#endif

}

//蜂鸣器
void BEEP(void)
{
#if 0 // change by zhang
	GPIO_SetBits(GPIOC,GPIO_Pin_4);
	Delay_ms(150);
	GPIO_ResetBits(GPIOC,GPIO_Pin_4);
#endif	

}
/**
* @brief ISD1720播放
* @param 
* @param 
* @return 
*/
void ISD_Play(void)
{
#if 0 // change by zhang
	GPIO_ResetBits(GPIOC,GPIO_Pin_5);
	Delay_ms(500);
	GPIO_SetBits(GPIOC,GPIO_Pin_5);
#endif 
}

/**
* @brief ISD1720录音
* @param 
* @param 
* @return 
*/
void ISD_Rec(void)
{
#if 0	// change by zhang
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
	Delay_ms(10000);
	GPIO_SetBits(GPIOB,GPIO_Pin_0);
#endif
}

/**
* @brief ISD1720删除
* @param 
* @param 
* @return 
*/
void ISD_Erase(void)
{
#if 0 // change by zhang
	GPIO_ResetBits(GPIOB,GPIO_Pin_1);
	Delay_ms(3000);
	GPIO_SetBits(GPIOB,GPIO_Pin_1);
#endif
}



/**
*	@brief 开始录音
*	@param 
* @param 
* @param 
* @return 
**/
void Process_ISD_REC(void)
{
#if 0 // change by zhnag	
	if(!KEY_ISD_REC) 
	{
//		printf("1\r\n");
		if(eventdelay.EventDelay[5] == 0)
			eventdelay.EventSet |= 0x20;
		if(eventdelay.EventDelay[5] > 2600)
		{
	//		printf("KEY_ISD_REC in\r\n");
			GPIO_ResetBits(GPIOB,GPIO_Pin_0);
		}
	}
	else 
	{
		if(eventdelay.EventDelay[5])
		{
//			printf("KEY_ISD_REC out\r\n");
			eventdelay.EventSet &= (~(0x20));
			GPIO_SetBits(GPIOB,GPIO_Pin_0);
			if((bCustomRecord != 1)&&(eventdelay.EventDelay[5] > 2600))
			{
				bCustomRecord = 1;
				SaveCustomRecordFlag();
			}
			eventdelay.EventDelay[5] = 0;
		}
	}
	if((eventdelay.EventDelay[5] >2400) && (eventdelay.EventDelay[5] < 2600))
	{
//		printf("beep!\r\n");
//		printf("start rec!\r\n");
		BEEP();
	}
#endif
}

/**
*	@brief 删除录音
*	@param 
* @param 
* @param 
* @return 
**/
void Process_ISD_ERASE(void)
{
#if 0 // chang by zhang
	if(!KEY_ISD_ERASE) 
	{
		if(eventdelay.EventDelay[6] == 0)
		{
//			printf("start erase!\r\n");
			eventdelay.EventSet |= 0x40;
			GPIO_ResetBits(GPIOB,GPIO_Pin_1);
		}
		if(eventdelay.EventDelay[6] > 2400 && eventdelay.EventDelay[6] < 2600)
		{
			BEEP();
		}
	}else
	{
		if(eventdelay.EventDelay[6])
		{
//			printf("end erase!\r\n");
			eventdelay.EventSet &= (~(0x40));
			
			GPIO_SetBits(GPIOB,GPIO_Pin_1);
			if((bCustomRecord != 0)&&(eventdelay.EventDelay[6] > 2000))
			{
				bCustomRecord = 0;
				SaveCustomRecordFlag();
			}
			eventdelay.EventDelay[6] = 0;
		}
	}
#endif
}


/**
*	@brief 保存设备自定义录音情况
*	@param 
* @param 
* @param 
* @return 
**/
void SaveCustomRecordFlag(void)
{
#if 0 // chang be zhang
	printf("写入数据\r\n");
    
  //将
	Delay_ms(40);
	I2C_EE_BufferWrite((u8 *)&bCustomRecord, 138, 1);
  Delay_ms(80);
  printf("\n\r写成功\r\n");
#endif
}





//********************************END OF FILE***********************************
