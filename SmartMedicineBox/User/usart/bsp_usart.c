/**
  ******************************************************************************
  * @file    bsp_usart.c
  * @author  xiaobing19851019@163.com
  * @version V1.0
  * @date    2013.09.01
  * @brief   ���ô�������USART1 USART2 UART4
  ******************************************************************************
  * @attention
  *
  * ƽ̨		:STM32F103ZET6 
  * ��˾    :http://www.jjc12320.com
  * 
  *
  ******************************************************************************
  */ 


#include "bsp_usart.h"



 /// ����USART1�����ж�
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

 /// ����USART2�����ж�
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

 /// ����UART4�����ж�
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
  * @brief  USART GPIO ����,����ģʽ���á�115200 8-N-1
  * @param  ��
  * @retval ��
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
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE); // USART2
	
		//USART1_TX   GPIOA.9
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
		GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
		 
		//USART1_RX	  GPIOA.10��ʼ��
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
		GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  
		
		USART_InitStructure.USART_BaudRate = 115200;//���ڲ�����
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
		USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
		USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
		USART_Init(USART1, &USART_InitStructure); //��ʼ������1
		
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
		USART_Init(USART2, &USART_InitStructure); //��ʼ������2
    
		USART2_NVIC_Config();
		
		
		
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
		USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 
		USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ���1 

}


/********************************************
�����ڽ���BUFFER�����ݣ�

usart1 to pc   usart2 to wifi
***********************************************/
int Usart_Read(uint8_t serialNo,unsigned char *buf,int length)
{
	
	uint32_t i=0, count = 0;
//	//  CLI();				/* �ر����ж� */  
	for(i = 0;i < length; i++)
	{
		if(UartStu[serialNo].RxBuf_In == UartStu[serialNo].RxBuf_Out)
			break;
	    count += 1;
	 	buf[i] = UartStu[serialNo].RxBuf[(UartStu[serialNo].RxBuf_Out)];
		UartStu[serialNo].RxBuf_Out = (UartStu[serialNo].RxBuf_Out + 1) & UART_BUF_LEN;
	}
//	//  SEI();				/* �������ж� */ 
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
//			//  CLI();				/* �ر����ж� */  
			for(i = 0;i < length; i++)
			{
			/* ����һ���ֽ����ݵ�USART1 */
				USART_SendData(USART1, (uint8_t) buf[i]);
				
				/* �ȴ�������� */
				while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		
			}
//			//  SEI();				/* �������ж� */ 
		}break;

		case 2:
		{
//			//  CLI();				/* �ر����ж� */  
			for(i = 0;i < length; i++)
			{
			/* ����һ���ֽ����ݵ�USART2 */
				USART_SendData(USART2, (uint8_t) buf[i]);
				
				/* �ȴ�������� */
				while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);		
			}	
//			//  SEI();				/* �������ж� */ 
		}break;
		case 3:
		{
			for(i = 0;i < length; i++)
			{
			/* ����һ���ֽ����ݵ�USART3 */
				USART_SendData(USART3, (uint8_t) buf[i]);
				
				/* �ȴ�������� */
				while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);		
			}			
		}break;

		case 4:
		{
			for(i = 0;i < length; i++)
			{
			/* ����һ���ֽ����ݵ�USART3 */
				USART_SendData(UART4, (uint8_t) buf[i]);
				
				/* �ȴ�������� */
				while (USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET);		
			}			
		}break;
		default: return -1;
	}
	return i;
}

// usart to pc    printf  to pc
///�ض���c�⺯��printf��USART1
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ�USART1 */
		USART_SendData(USART1, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		
		return (ch);
}

///�ض���c�⺯��scanf��USART1
int fgetc(FILE *f)
{
		/* �ȴ�����1�������� */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
}



/*********************************************END OF FILE**********************/
