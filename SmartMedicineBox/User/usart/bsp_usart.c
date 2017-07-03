/**
  ******************************************************************************
  * @file    bsp_usart.c
  * @author  xiaobing19851019@163.com
  * @version V1.0
  * @date    2013.09.01
  * @brief   配置串口驱动USART1 USART2 UART4
  ******************************************************************************
  * @attention
  *
  * 平台		:STM32F103ZET6 
  * 公司    :http://www.jjc12320.com
  * 
  *
  ******************************************************************************
  */ 


#include "bsp_usart.h"



 /// 配置USART1接收中断
static void USART1_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

 /// 配置USART2接收中断
static void USART2_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

 /// 配置UART4接收中断
static void UART4_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

 /**
  * @brief  USART GPIO 配置,工作模式配置。115200 8-N-1
  * @param  无
  * @retval 无
  */
// usart1 to pc  
// usart2 to wifi
void USART_Config(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;

#if 0	
		/* config USART1 clock */
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 | RCC_APB1Periph_UART4,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
#endif
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE); // USART2
	
		//USART1_TX   GPIOA.9
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
		GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
		 
		//USART1_RX	  GPIOA.10初始化
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
		GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  
		
		USART_InitStructure.USART_BaudRate = 115200;//串口波特率
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
		USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
		USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
		USART_Init(USART1, &USART_InitStructure); //初始化串口1
		
		USART1_NVIC_Config();
		

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
    
		USART2_NVIC_Config();
		
		
		
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启串口接受中断
		USART_Cmd(USART1, ENABLE);                    //使能串口1 
		USART_Cmd(USART2, ENABLE);                    //使能串口1 

}


/********************************************
读串口接收BUFFER的数据，

usart1 to pc   usart2 to wifi
***********************************************/
int Usart_Read(uint8_t serialNo,unsigned char *buf,int length)
{
	
	uint32_t i=0, count = 0;
//	//  CLI();				/* 关闭总中断 */  
	for(i = 0;i < length; i++)
	{
		if(UartStu[serialNo].RxBuf_In == UartStu[serialNo].RxBuf_Out)
			break;
	    count += 1;
	 	buf[i] = UartStu[serialNo].RxBuf[(UartStu[serialNo].RxBuf_Out)];
		UartStu[serialNo].RxBuf_Out = (UartStu[serialNo].RxBuf_Out + 1) & UART_BUF_LEN;
	}
//	//  SEI();				/* 开放总中断 */ 
	return count;
}

//
int Usart_Send(uint8_t serialNo,unsigned char *buf,int length)
{
	int i = 0;
	switch(serialNo)
	{
		
		case 1:
		{
//			//  CLI();				/* 关闭总中断 */  
			for(i = 0;i < length; i++)
			{
			/* 发送一个字节数据到USART1 */
				USART_SendData(USART1, (uint8_t) buf[i]);
				
				/* 等待发送完毕 */
				while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		
			}
//			//  SEI();				/* 开放总中断 */ 
		}break;

		case 2:
		{
//			//  CLI();				/* 关闭总中断 */  
			for(i = 0;i < length; i++)
			{
			/* 发送一个字节数据到USART2 */
				USART_SendData(USART2, (uint8_t) buf[i]);
				
				/* 等待发送完毕 */
				while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);		
			}	
//			//  SEI();				/* 开放总中断 */ 
		}break;
		case 3:
		{
			for(i = 0;i < length; i++)
			{
			/* 发送一个字节数据到USART3 */
				USART_SendData(USART3, (uint8_t) buf[i]);
				
				/* 等待发送完毕 */
				while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);		
			}			
		}break;

		case 4:
		{
			for(i = 0;i < length; i++)
			{
			/* 发送一个字节数据到USART3 */
				USART_SendData(UART4, (uint8_t) buf[i]);
				
				/* 等待发送完毕 */
				while (USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET);		
			}			
		}break;
		default: return -1;
	}
	return i;
}

// usart to pc    printf  to pc
///重定向c库函数printf到USART1
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到USART1 */
		USART_SendData(USART1, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		
		return (ch);
}

///重定向c库函数scanf到USART1
int fgetc(FILE *f)
{
		/* 等待串口1输入数据 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
}



/*********************************************END OF FILE**********************/
