/**
  ******************************************************************************
  * @file    DeviceInit.c
  * @author  xiaobing19851019@163.com
  * @version V1.0
  * @date    2015.11.25
  * @brief   设备出厂初始化处理
  ******************************************************************************
  * @attention
  *
  * 平台		:STM32F103ZET6 
  * 公司    :http://www.jjc12320.com
  * 
  *
  ******************************************************************************
  */ 
#include "DeviceInit.h"

u8 ID_Buffer[20] = {0};
u8 headin = 0;

int GetDeviceID(void);


/**
*	@brief 工厂测试程序，测试LED和霍尔开关的状态
*	@param type 0 打开所有灯光 1 关闭所有灯光 2 感应开关，开关灯
* @param 
* @param 
* @return 
**/
void FactoryTestProgram(u8 type)
{
	int i = 0,j = 0;
	
	switch(type)
	{
		case 0:
		{
//			GPIOF->BSRR = GPIO_Pin_11 | GPIO_Pin_12;  //PF11  电池指示灯   PF12   大灯
			for(i = 0; i< 7;i++)
			{
				for(j = 0;j<4;j++)
				{
					Medicine_LED_Ctrl(i,j,1);
				}
			}
			Delay_ms(5000);
		}break;
		case 1:
		{
//			GPIOF->BRR = GPIO_Pin_11 | GPIO_Pin_12;
			for(i = 0; i< 7;i++)
			{
				for(j = 0;j<4;j++)
				{
					Medicine_LED_Ctrl(i,j,0);
				}
			}
	}break;
		case 2:
		{
			for(i = 0; i< 7;i++)
			{
				for(j = 0;j<4;j++)
				{
					if(!(Medicine_Status_Detec(i,j)))
					{
//						GPIOF->BSRR = GPIO_Pin_11 | GPIO_Pin_12;
						Medicine_LED_Ctrl(i,j,1);
					}else	
					{
//						GPIOF->BRR = GPIO_Pin_11 | GPIO_Pin_12;
						Medicine_LED_Ctrl(i,j,0);
					}
				}
			}
		}break;
	}
}

/**
*	@brief 出厂前设置药盒ID和测试药盒硬件设备
*	@param 
* @param 
* @param 
* @return 
**/
void SetDeviceID(void)
{
	int i = 0,len = 0;
	u8 tempbuf[128];
	printf("ready to set device id\r\n");
	while(1)
	{
//		FactoryTestProgram(2);
		len = Usart_Read(1,tempbuf,16);  //rev pc data
		
		
		for(i = 0; i < len; i++)
		{
			if((headin > 0) && (headin < 20))
			{
				if((tempbuf[i] == 0x0d) || (tempbuf[i] == 0x0a))
				{
					ID_Buffer[headin++] = 0;
					headin = 0;
//					if(ID_Buffer[1] == '@') FactoryTestProgram(0);
//					else if(ID_Buffer[1] == '#') FactoryTestProgram(1);
//					else
					{
					protocolstu.Device_ID = GetDeviceID();
					printf("DeviceID = %d \r\n",protocolstu.Device_ID);
					return ;
					}
				}
				else
				{
					ID_Buffer[headin++] = tempbuf[i];
				}
			}
			if(tempbuf[i] == '$')
			{
				headin = 0;
				ID_Buffer[headin++] = tempbuf[i];
			}
		}
		len = Usart_Read(2,tempbuf,128);  // rev wifi feedback

		if(len > 0)
		{
			Usart_Send(1,tempbuf,len); // send to pc
		}
	}
}

/**
  * @brief  获取设置的设备ID
  * @param  无
  * @retval 无
  */
int GetDeviceID(void)
{
	char tempbuf[20] = "";
	int i = 0,j = 1;
	
	while(ID_Buffer[j] != 0)
	{
		tempbuf[i++] = ID_Buffer[j++];
	}
	
	tempbuf[i] = 0;
	
	if(i != 8) return -1;
	
	return atoi(tempbuf);
}
/**
  * @brief  USART2 GPIO 配置,工作模式配置。115200 8-N-1
  * @param  无
  * @retval 无
  */
static void USART2_ReConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
#if 0 // change by zhang	
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* config USART1 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
	
/* USART4 GPIO config */
	/* Configure USART4 Tx (PC.10) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* Configure USART4 Rx (PC.11) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	/* USART4 mode config */
	USART_InitStructure.USART_BaudRate = 19200; // 19200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART4, &USART_InitStructure); 
	
 /* 使能串口4接收中断 */
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);	
	USART_Cmd(UART4, ENABLE);
	USART_ClearFlag(UART4, USART_FLAG_TC);
#endif

	/* config USART2 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能USART2，GPIOA时钟 PA2 PA3
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);	// 使能USART2  clock
	/* USART2 GPIO config */
	/* Configure USART2 Tx (PA.02) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure USART2 Rx (PA.3) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* USART2 mode config */
	USART_InitStructure.USART_BaudRate = 115200;//19200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure); //初始化串口2

	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	

  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART2, ENABLE);                    //使能串口2

}

/**
* @brief 轮流查询SIM卡状态
* @param 
* @param 
* @return 
*/
void RotationQuerySimStatus(void)
{
	int len = 0,i=0,atCmdLen = 0;
	int RotationQueryCnt = 1;
	char data[64];
	u8 tempbuf[256];
	u8 smartsuccess = 0;
	u8 connect_status = 0;
	
	wifi_status(2);
	VoiceReminder(CONNECT_WAIT); //等待网络链接  开机提醒	
	while(1)
	{
		if(RotationQueryCnt == 10)
		{
			len = PrepareATCmd(RST,data);
			Usart_Send(2,(unsigned char*)data,len);


		}
		

		// config usart
		if(RotationQueryCnt == 50)
		{
			// usart2 115200 n 8 1
			USART2_ReConfig();
			len = PrepareATCmd(UARTSET,data);
			Usart_Send(2,(unsigned char*)data,len);		
		}
		// set mode
		if(RotationQueryCnt == 100) // 200
		{
			len = PrepareATCmd(CWMODE,data);
			Usart_Send(2,(unsigned char*)data,len);
		}
		
		// test AT cmd 
		if(RotationQueryCnt == 500) // 200
		{
			len = PrepareATCmd(AT,data);
			Usart_Send(2,(unsigned char*)data,len);
		}
		// start smartlink
		if((RotationQueryCnt % 5000) == 0) // 2600
		{
			if(smartsuccess == 0)
			{
				SmartLinkStart();
				VoiceReminder(CONNECT_WAIT); //等待网络链接
			}
		}
		Delay_ms(3);
		RotationQueryCnt++;
		


		len = Usart_Read(2,tempbuf,256);  // read wifi feelback.

		for(i = 0;i<len;i++)
		{
			atCmdLen = ParseOneFrameATCmd(tempbuf+i); // parse .........
			if(atCmdLen)
			{
				if(GetHeadType() == WIFIGOTIP) 
				{
					VoiceReminder(CONNECT_SUCCESS); //网络链接成功
					connect_status = 1;
					wifi_status(2);
					wifi_status(1);
					WIFI_Ready = 1;
					Usart_Send(1,tempbuf,len);  // send to pc       printf the message					
					printf("\r\n connect success\r\n");
					
					Delay_ms(1000);
					SmartLinkStop();
					Delay_ms(1000);
					return;
				}
				else
				{
					wifi_status(2);
					wifi_status(0);					
				}
				if(GetHeadType() == SMART_SUCCESS)
				{
					smartsuccess = 1;
				}
			}
		}

		if(connect_status == 0)
		{
			if(RotationQueryCnt % 300 == 0)
			{
				GPIO_ResetBits(GPIOE,GPIO_Pin_13); // green light
				GPIOE->ODR ^= GPIO_Pin_14; // red light
			}
			if(RotationQueryCnt % 3333 == 0)
			{
				VoiceReminder(CONNECT_WAIT); //等待网络链接
			}
		}
			
		if(len > 0)
		{
			Usart_Send(1,tempbuf,len);  // send to pc
		}
	}
}


/**
*	@brief 测试录音 删除
*	@param 
* @param 
* @param 
* @return 
**/
void TestRECandERASE(void)
{
	int i = 1000;
	printf("请按下录音按键，进行录音测试！\r\n");
	while(i--)
	{
		if(!KEY_ISD_REC) 
		{
			printf("BEEP!\r\n");
			BEEP();
			ISD_Rec();
			printf("End of rec!\r\n");
			i = 0;
		}
		Delay_ms(8);
	}
		Delay_ms(400);
		printf("BEEP!\r\n");
		BEEP();
		ISD_Play();
		Delay_ms(5000);
	
		printf("请按下删除按键，进行删除测试！\r\n");
	i = 1000;
	while(i--)
	{
		if(!KEY_ISD_ERASE) 
		{
			BEEP();
			printf("Start of erase!\r\n");
			ISD_Erase();
			i = 0;
		}
		Delay_ms(8);
	}
	Delay_ms(1000);
	ISD_Play();
	Delay_ms(1000);
	
	VoiceReminder(MEDICINE_CHECK);
	Delay_ms(5000);
	VoiceReminder(CONNECT_WAIT);
	Delay_ms(2000);
	VoiceReminder(CONNECT_SUCCESS);
	Delay_ms(2000);
	VoiceReminder(BOX_TAKED);
	Delay_ms(2000);
	VoiceReminder(REMIND_MUSIC);
	Delay_ms(5000);
}


/**
*	@brief 出厂前设置药盒ID和测试药盒硬件设备
*	@param 
* @param 
* @param 
* @return 
**/
void TestLEDandSWITCH(void)
{
	int i = 2000;
	
	FactoryTestProgram(0);
	while(i--)
	{
		FactoryTestProgram(2);
		Delay_ms(10);
	}
}



//*****************************END OF FILE*******************************************
