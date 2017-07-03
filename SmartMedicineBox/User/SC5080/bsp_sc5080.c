/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  xiaobing19851019@163.com
  * @version V1.0
  * @date    2015.08.27
  * @brief   ����ҩ��LED��������
  ******************************************************************************
  * @attention
  *
  * ƽ̨		:STM32F103ZET6
  * ��˾    :http://www.jjc12320.com
  *
  ******************************************************************************
  */ 
	
#include "bsp_sc5080.h"

int QueueOrder = 0;
VoiceQueue	VQ;



extern __IO EventDelay eventdelay;
/**
* @brief  ��ʼ��SC5080����IO
* @param  ��
* @retval ��
*/
void BSP_SC5080_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//init clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	
	//init gpio
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_13	| GPIO_Pin_15;  //SC5080_BUSY PE13	//SC5080_SDA	PE15
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
//	GPIO_SetBits(GPIOE,GPIO_Pin_13 | GPIO_Pin_15);
	GPIO_ResetBits(GPIOE,GPIO_Pin_13 | GPIO_Pin_15);
	VQ.VoiceOrder = -1;
	VQ.VoiceLevel = 1;

}


/**
* @brief  ��ʼ��SC5080����IO
* @param  data ���Ƶ�ַ
* E0H~E6H
* ����16��������E0H������С��EFH�������Ĭ����� 	// 224 ~ 239
* F2H
* ѭ�����ţ�F2H+ѭ������������ַ
*	F3H
* �˳�ѭ������
* FEH
* ֹͣ����
*	FC �򿪹���
*	FD �رչ���
* @retval ��
*/
void BSP_SC5080_Ctrl(u8 data)
{
	u8 i = 0;
	
	SC5080_SDA(0);
	myDelay_ms(5);
	// ���û�������޸���ʱʱ�����
	for(i = 0; i < 8; i++)
	{
		SC5080_SDA(1); 
		if(data & 0x01)
		{
			myDelay_us(1200);  // delay_us(1200);
			SC5080_SDA(0);
			myDelay_us(400);   // delay_us(400);
		}
		else
		{ 
			myDelay_us(400);  // delay_us(400);
			SC5080_SDA(0);
			myDelay_us(1200);  // delay_us(1200);
		} 
		data >>= 1;
	}
	 SC5080_SDA(1);  
   myDelay_us(100); 
}	

/**
*	@brief ����
*	@param 
* @param 
* @param 
* @return 
**/
void VoiceReminder(u8 voice)
{
	if(voice == 0 || VQ.VoiceLevel == 0) return;
	if(voice == REMIND_MUSIC && bCustomRecord == 1)
	{
//		ISD_Play();  //����
		return;
	}
	SC5080_BUSY(1);
	BSP_SC5080_Ctrl(voice);
	SC5080_BUSY(0);
}

/**
*	@brief ����������ѯ
*	@param 
* @param 
* @param 
* @return 
**/
void VoiceLoop(void)
{
	if((VQ.VoiceOrder >= 0)&&(eventdelay.EventDelay[2] > 3000))
	{
		eventdelay.EventDelay[2] = 0;
		VoiceReminder(VQ.VoiceArray[QueueOrder++]);
		VQ.VoiceOrder--;
	}
	if(VQ.VoiceOrder == -1) QueueOrder = 0;
}

/**
*	@brief ���÷������У�������ʾ��ʱ����
*	@param voice �������
* @param 
* @param 
* @return 
**/
void SetVoiceQueue(u8 voice)
{
	VQ.VoiceOrder++;
	VQ.VoiceArray[VQ.VoiceOrder] = voice;
	if(VQ.VoiceOrder == 0) eventdelay.EventDelay[2] = 5000;
}



u8 VoiceFlag = 1; // ���Ź���
// ��������
void VoiceLevelCtrl(void)
{
	// 16 level E0H ~ EFH //// 224 ~ 239
	u8 voice_value = 0;
	u8 temp = 0;
	
	if(VQ.VoiceLevel == 0)
	{
		printf("\r\n	�ر���������	\r\n");
		VoiceFlag = 0;
//		voice_value = 252;  // FC = 252
		voice_value = 225; // mix 
		return;
	}
	else
	{
		VoiceFlag = 1;
		if(VQ.VoiceLevel == 1)
		{
			voice_value = 225;
		}
		
		if(VQ.VoiceLevel == 2)
		{
			voice_value = 232;
		}
		
		if(VQ.VoiceLevel == 3)
		{
			voice_value = 239;
		}
		
	}

//	// ������
//	if(VoiceFlag == 0 && VQ.VoiceLevel != 0)
//	{
//		temp = 253; // FD = 253
//		SC5080_BUSY(1);
//		BSP_SC5080_Ctrl(temp);
//		SC5080_BUSY(0);				
//	}

	
	SC5080_BUSY(1);
	BSP_SC5080_Ctrl(voice_value);
	SC5080_BUSY(0);	
}


 
//********************************END OF FILE**************************************
