/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "bsp_rtc.h"
//#include "main.h"

extern __IO uint32_t TimeDisplay;
extern void TimingDelay_Decrement(void);

__IO EventDelay eventdelay = {0};

//__IO int RecyclingTime = 0;
//extern struct rtc_time systmtime;

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

UsartStruct UartStu[3];
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}


/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

#if 0
/**
  * @brief  This function handles TIM4_IRQHandler Handler.
  * @param  None
  * @retval None
  */
void TIM4_IRQHandler(void)
{
	if ( TIM_GetITStatus(TIM4 , TIM_IT_Update) != RESET ) 
	{
		TimingDelay_Decrement();	
		TIM_ClearITPendingBit(TIM4 , TIM_FLAG_Update);  		 
	}
}
#endif

/**
  * @brief  This function handles RTC interrupt request.
  * @param  None
  * @retval None
  */
void RTC_IRQHandler(void)
{
	  if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
	  {
	    /* Clear the RTC Second interrupt */
	    RTC_ClearITPendingBit(RTC_IT_SEC);
	
	    /* Enable time update */
	    TimeDisplay = 1;
	    /* Wait until last write operation on RTC registers has finished */

	    RTC_WaitForLastTask();
	  }
}


/* 串口1接收中断 */  // usart1 to pc
void USART1_IRQHandler(void)
{
   uint8_t dat;
	if(USART_GetFlagStatus(USART1,USART_FLAG_ORE) != RESET || USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{ 
		/////////////// 解决数据量大的时候死机问题///////////////
			USART_ClearITPendingBit(USART1,USART_FLAG_ORE);
			USART_ClearITPendingBit(USART1,USART_IT_ORE);
		/////////////////////////////////////////////////////////    

		dat = USART_ReceiveData(USART1);
		//printf("%c \r\n", rec_cmd);
		if(((UartStu[1].RxBuf_In + 1) & UART_BUF_LEN) == UartStu[1].RxBuf_Out);  //缓存区不足，数据丢弃
		else
		{
			UartStu[1].RxBuf[UartStu[1].RxBuf_In] = dat;
			UartStu[1].RxBuf_In = ((++UartStu[1].RxBuf_In) & UART_BUF_LEN);
		}
	}	 
}

/* 串口2接收中断 */  // usart2 to wifi
void USART2_IRQHandler(void)
{
   uint8_t dat;
//	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
		if(USART_GetFlagStatus(USART2,USART_FLAG_ORE) != RESET || USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		/////////////// 解决数据量大的时候死机问题///////////////
			USART_ClearITPendingBit(USART2,USART_IT_RXNE);
			USART_ClearITPendingBit(USART2,USART_FLAG_ORE);
			USART_ClearITPendingBit(USART2,USART_IT_ORE);
		/////////////////////////////////////////////////////////
			dat = USART_ReceiveData(USART2);
      //printf("%c\r\n", dat);
      if(((UartStu[2].RxBuf_In + 1) & UART_BUF_LEN) == UartStu[2].RxBuf_Out);  //缓存区不足，数据丢弃
			else
      {
        UartStu[2].RxBuf[UartStu[2].RxBuf_In] = dat;
				UartStu[2].RxBuf_In = ((++UartStu[2].RxBuf_In) & UART_BUF_LEN);
      }
	}	 
}

#if 0
/* 串口4接收中断 */
void UART4_IRQHandler(void)
{
   uint8_t dat;
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
	{     
			dat = USART_ReceiveData(UART4);
      //printf("%c \r\n", rec_cmd);
      if(((UartStu[0].RxBuf_In + 1) & UART_BUF_LEN) == UartStu[0].RxBuf_Out );  //缓存区不足，数据丢弃
			else
      {
        UartStu[0].RxBuf[UartStu[0].RxBuf_In] = dat;
				UartStu[0].RxBuf_In = ((++UartStu[0].RxBuf_In) & UART_BUF_LEN);
      }
	}	 
}
#endif


#if 0
/**
  * @brief  This function handles TIM2 interrupt request.
  * @param  None
  * @retval None
  */
void TIM2_IRQHandler(void)
{
	u8 i = 0;
	if ( TIM_GetITStatus(TIM2 , TIM_IT_Update) != RESET ) 
	{	
		for(i = 0; i < 32;i++)
		{
			if(((eventdelay.EventSet)>>i) &0x01)	eventdelay.EventDelay[i]++;
		}
		TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);
	}		 	
}

/**
  * @brief  This function handles TIM3 interrupt request.
  * @param  None
  * @retval None
  */
void TIM3_IRQHandler(void)
{
	if ( TIM_GetITStatus(TIM3 , TIM_IT_Update) != RESET ) 
	{
//		RecyclingTime++;
		TIM_ClearITPendingBit(TIM3 , TIM_FLAG_Update);  		 
	}		 	
}
#endif
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
