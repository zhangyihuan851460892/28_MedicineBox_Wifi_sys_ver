/**
  ******************************************************************************
  * @file    main.c
  * @author  xiaobing19851019@163.com
  * @version V1.0
  * @date    2015.08.27
  * @brief   ����ҩ��������
  ******************************************************************************
  * @attention
  *
  * ƽ̨		:STM32F103ZET6 
  * ��˾    :http://www.jjc12320.com
  *
  ******************************************************************************
  */
	

#include "main.h"
#include "cmsis_os.h"
#include "common.h"
#include "Task.h"


osThreadDef(Device_InitTask, osPriorityNormal, 1, 800);



// N = 2^32/365/24/60/60 = 136 ��

/*ʱ��ṹ��*/
rtc_time systmtime;

//static u8 DebugCommCtrl = 0;
//__IO u8 RefreshPlan = 0;

	// ϵͳ��λ
//static void SoftReset(void)
//{
//	__set_FAULTMASK(1);		//�ر��ж�
//	NVIC_SystemReset();		//ϵͳ��λ
//}


/**
  * @brief  Main program.
  * @param  None
  * @retval : None
  */
int main()
{
	osKernelInitialize ();                         // initialize RTOS kernel

  osThreadCreate(osThread(Device_InitTask),NULL);

  osKernelStart ();
	
	while(1)
	{
		osDelay(50);
//		osThreadYield();
	}
}


/***********************************END OF FILE*********************************/

