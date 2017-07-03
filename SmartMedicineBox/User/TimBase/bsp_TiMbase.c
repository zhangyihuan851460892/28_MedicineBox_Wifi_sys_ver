/**
  ******************************************************************************
  * @file    bsp_TimBase.c
  * @author  xiaobing19851019@163.com
  * @version V1.0
  * @date    2015.09.15
  * @brief   TIM2 1ms ��ʱ������
  ******************************************************************************
  * @attention
  * 
	*	ƽ̨		��STM32F103ZET6
  * ��˾    :http://www.jjc12320.cn
  * 
  *
  ******************************************************************************
  */ 

#include "bsp_TiMbase.h" 
#include "cmsis_os.h"

static __IO u32 TimingDelay;


/// TIM2�ж����ȼ�����
void TIM2_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  													
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 * TIM_Period / Auto Reload Register(ARR) = 1000   TIM_Prescaler--71 
 * �ж�����Ϊ = 1/(72MHZ /72) * 1000 = 1ms
 *
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIM_Period(ARR) --> �ж� ��TIMxCNT����Ϊ0���¼��� 
 */
void TIM2_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
		/* ����TIM2CLK Ϊ 72MHZ */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
    //TIM_DeInit(TIM2);
	
	/* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
    TIM_TimeBaseStructure.TIM_Period=1000;
	
    /* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
	  /* ʱ��Ԥ��Ƶ��Ϊ72 */
    TIM_TimeBaseStructure.TIM_Prescaler= 71;
	
		/* ���ⲿʱ�ӽ��в�����ʱ�ӷ�Ƶ,����û���õ� */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
		
    TIM_Cmd(TIM2, ENABLE);																		
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);		/*ֱ��ʹ��*/    
}


/// TIM2�ж����ȼ�����
void TIM3_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  													
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 * TIM_Period / Auto Reload Register(ARR) = 1000   TIM_Prescaler--71 
 * �ж�����Ϊ = 1/(72MHZ /72) * 1000 = 1ms
 *
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIM_Period(ARR) --> �ж� ��TIMxCNT����Ϊ0���¼��� 
 */
void TIM3_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
		/* ����TIM3CLK Ϊ 72MHZ */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
    //TIM_DeInit(TIM3);
	
	/* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
    TIM_TimeBaseStructure.TIM_Period=1000;
	
    /* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
	  /* ʱ��Ԥ��Ƶ��Ϊ72 */
    TIM_TimeBaseStructure.TIM_Prescaler= 71;
	
		/* ���ⲿʱ�ӽ��в�����ʱ�ӷ�Ƶ,����û���õ� */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
		
    TIM_Cmd(TIM3, ENABLE);																		
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , DISABLE);		/*�ȹرյȴ�ʹ��*/    
}



/// TIM4�ж����ȼ�����
static void TIM4_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    			
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 	
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 * TIM_Period / Auto Reload Register(ARR) = 1000   TIM_Prescaler--71 
 * �ж�����Ϊ = 1/(72MHZ /72) * 1000 = 1ms
 *
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIM_Period(ARR) --> �ж� ��TIMxCNT����Ϊ0���¼��� 
 */
static void TIM4_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
		/* ����TIM4CLK Ϊ 72MHZ */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE);
    //TIM_DeInit(TIM3);
	
	/* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
    TIM_TimeBaseStructure.TIM_Period= 720;  //��ֵ����̫С
	
    /* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
	  /* ʱ��Ԥ��Ƶ��Ϊ72 */
    TIM_TimeBaseStructure.TIM_Prescaler= 0;
	
		/* ���ⲿʱ�ӽ��в�����ʱ�ӷ�Ƶ,����û���õ� */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
    TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	
    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
		
    TIM_Cmd(TIM4, ENABLE);																		
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , DISABLE);		/*�ȹرյȴ�ʹ��*/    
}

/**
  * @brief   ��ʼ����ʱ����
  * @param  
  *		@arg 
  * @retval  ��
  */
void DelayTickInit(void)
{
		/* TIM4 ��ʱ���� */	
  TIM4_Configuration();
	/* ʵս��ʱ�����ж����ȼ� */
	TIM4_NVIC_Configuration();
}

#if 0
/**
  * @brief   us��ʱ����,10usΪһ����λ
  * @param  
  *		@arg nTime: Delay_us( 1 ) ��ʵ�ֵ���ʱΪ 1 * 10us = 10us
  * @retval  ��
  */
void Delay_us(__IO u32 nTime)
{ 
	TimingDelay = nTime;	

	// ʹ��TIM4��ʱ��  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE);	

	while(TimingDelay != 0);
	// �ر�TIM4��ʱ��  
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , DISABLE);	
}
#else
/**
  * @brief   us��ʱ����,10usΪһ����λ
  * @param  
  *		@arg nTime: Delay_us( 1 ) ��ʵ�ֵ���ʱΪ 1 * 10us = 10us
  * @retval  ��
  */
void Delay_us(__IO u32 nTime)
{
	u32 time = 0;
	
	time = nTime*4;
	
	while(time--)
	{
		__ASM("NOP");
	}
}
#endif


/**
  * @brief  ��ȡ���ĳ���
  * @param  ��
  * @retval ��
  * @attention  �� TIM4 �жϺ��� TIM4_IRQHandler()����
  */
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
}



void myDelay_us(u32 nTime)
{
	int tick, delayPeriod;
	
	tick = osKernelSysTick();                      // get start value of the Kernel system tick
	delayPeriod = osKernelSysTickMicroSec(nTime);
	
	do
	{
		// null
	}while((osKernelSysTick() - tick) < delayPeriod);
	
	
}

/*********************************************END OF FILE**********************/
