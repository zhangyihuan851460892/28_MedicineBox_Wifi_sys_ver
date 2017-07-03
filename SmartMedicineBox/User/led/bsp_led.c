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
  * 平台		:STM32F103VET6
  * 公司    :http://www.jjc12320.com
  *
  ******************************************************************************
  */ 
	
#include "bsp_led.h"

u8 AddDrugFlag = 0;

//系统时间
extern rtc_time systmtime;


 /**
  * @brief  初始化控制LED的IO
  * @param  无
  * @retval 无
  */
void LED_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//configure clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);  //将JTAG的IO口复用为普通IO口模式

#if 0
	//初始化GPIO
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_2;	//蓝牙模块指示灯
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_0; //KEY_POWER检测，药盒打开关闭
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_11 | GPIO_Pin_12; //PF11 GSM模块指示灯，PF12 吃药提醒指示灯
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOF,&GPIO_InitStructure);
#endif

	/////////////////////////////////////////
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_4 | GPIO_Pin_5; //POWER_LED，PA4 低电量提醒  TIME_LED，PA5 时间到提醒大指示灯
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOA,&GPIO_InitStructure);	
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_4 | GPIO_Pin_5);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_13; //	NETLIGHT_GLED PC13
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOC,&GPIO_InitStructure);	
	
	GPIO_SetBits(GPIOC,GPIO_Pin_13);

	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_14; //	NETLIGHT_RLED PE14
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOE,&GPIO_InitStructure);	
	
	GPIO_SetBits(GPIOE,GPIO_Pin_14);
	//////////////////////////////////////////
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_0; //小药盒，LED1	PE0
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_1; //小药盒，KEY1	PE1
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_8; //小药盒，LED2	PB8
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9; //小药盒，KEY2	PB9
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOB,&GPIO_InitStructure);
#if 0
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);  //将JTAG的PB3口复用为普通IO口模式
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_3; //小药盒，LED3
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_4; //小药盒，KEY3
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;

	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
#endif	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	// 小药盒, LED3 PA1
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_0; //小药盒，KEY3	PA0
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	

	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_3; //小药盒，LED4	PB3
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_4; //小药盒，KEY4	PB4
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_6; //小药盒，LED5	PD6
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_7; //小药盒，KEY5	PD7
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_4; //小药盒， LED6	PD4
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_5; //小药盒，KEY6	PD5
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_2; //小药盒，LED7	PD2
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_3; //小药盒，KEY7	PD3
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_0; //小药盒，LED8	PD0
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_1; //小药盒，KEY8	PD1
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_11; //小药盒，LED9	PC11
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_12; //小药盒，KEY9	PC12
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_15; //小药盒，LED10	PA15
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_10; //小药盒，KEY10	PC10
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_12; //小药盒，LED11	PA12
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_11; //小药盒，KEY11	PA11
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_3; //小药盒，LED12	PC3
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_2; //小药盒，KEY12	PC2
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9; //小药盒，LED13	PC9
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_8; //小药盒，KEY13	PA8
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_7; //小药盒，LED14	PC7
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_8; //小药盒，KEY14	PC8
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_15; //小药盒，LED15	PD15
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_6; //小药盒，KEY15	PC6
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_13; //小药盒，LED16	PD13
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_14; //小药盒，KEY16	PD14
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_11; //小药盒，LED17	PD11
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_12; //小药盒，KEY17	PD12
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9; //小药盒，LED18	PD9
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_10; //小药盒，KEY18	PD10
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_15; //小药盒，LED19	PB15
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_8; //小药盒，KEY19	PD8
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_13; //小药盒，LED20	PB13
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_14; //小药盒，KEY20	PB14
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_10; //小药盒，LED21	PB10
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_12; //小药盒，KEY21	PB12
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_12; //小药盒，LED22	PE12
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_11; //小药盒，KEY22	PB11
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_10; //小药盒，LED23	PE10
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_11; //小药盒，KEY23	PE11
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_8; //小药盒，LED24	PE8
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9; //小药盒，KEY24	PE9
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_2; //小药盒，LED25	PB2
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_7; //小药盒，KEY25	PE7
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_0; //小药盒，LED26	PB0
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_1; //小药盒，KEY26	PB1
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_4; //小药盒，LED27	PC4
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_5; //小药盒，KEY27	PC5
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_6; //小药盒，LED28	PA6
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_7; //小药盒，KEY28	PA7
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
															// LED1	PE0. LED2	PB8. LED3 PA1. LED4	PB3. LED5	PD6. LED6	PD4. LED7	PD2. 
															// LED8	PD0. LED9	PC11. LED10	PA15. LED11	PA12. LED12	PC3. LED13	PC9. LED14	PC7.
															// LED15	PD15. LED16	PD13. LED17	PD11. LED18	PD9. LED19	PB15. LED20	PB13.LED21	PB10.  												
															// LED22	PE12. LED23	PE10. LED24	PE8. LED25	PB2. LED26	PB0. LED27	PC4. LED28	PA6.	


															
															// PA	1.6.12.15.
															// PB 0.2.3.8.10.13.15.
															// PC 3.4.7.9.11.
															// PD 0.2.4.6.9.11.13.15.
															// PE 0.8.10.12
	//close all led
	GPIO_ResetBits(GPIOA,GPIO_Pin_1 | GPIO_Pin_6 | GPIO_Pin_12 | GPIO_Pin_15);
	GPIO_ResetBits(GPIOB,GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_8 | GPIO_Pin_10 | GPIO_Pin_13 | GPIO_Pin_15);
	GPIO_ResetBits(GPIOC,GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_4 | GPIO_Pin_7 | GPIO_Pin_9 | GPIO_Pin_11);
	GPIO_ResetBits(GPIOD,GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_6 | GPIO_Pin_9 | GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_15);
	GPIO_ResetBits(GPIOE,GPIO_Pin_0 | GPIO_Pin_8 | GPIO_Pin_10 | GPIO_Pin_12);
	
}

 /**
  * @brief  吃药亮灯管理函数
  * @param  day 周一到周日（1-7）
	* @param	time 时刻，早、中、晚、睡前， 取值（1-4）
	* @param  flag 开关状态 ON OFF
  * @retval 无
  */
void Medicine_LED_Ctrl(u8 day,u8 time,int flag)
{
	GPIOInfo medicinematrix[4][7] = {
															{{GPIOE,GPIO_Pin_0},{GPIOG,GPIO_Pin_12},{GPIOD,GPIO_Pin_3},{GPIOG,GPIO_Pin_7},{GPIOD,GPIO_Pin_13},{GPIOB,GPIO_Pin_13},{GPIOE,GPIO_Pin_8}},
															{{GPIOB,GPIO_Pin_8},{GPIOG,GPIO_Pin_10},{GPIOD,GPIO_Pin_1},{GPIOG,GPIO_Pin_5},{GPIOD,GPIO_Pin_11},{GPIOE,GPIO_Pin_14},{GPIOG,GPIO_Pin_1}},
															{{GPIOB,GPIO_Pin_3},{GPIOD,GPIO_Pin_7},{GPIOC,GPIO_Pin_12},{GPIOG,GPIO_Pin_3},{GPIOD,GPIO_Pin_9},{GPIOE,GPIO_Pin_12},{GPIOF,GPIO_Pin_15}},
															{{GPIOG,GPIO_Pin_14},{GPIOD,GPIO_Pin_5},{GPIOC,GPIO_Pin_3},{GPIOD,GPIO_Pin_15},{GPIOB,GPIO_Pin_15},{GPIOE,GPIO_Pin_10},{GPIOF,GPIO_Pin_13}}
															};
	
	if(flag) 	GPIO_SetBits(medicinematrix[time][day].GPIOx,medicinematrix[time][day].GPIO_Pin);
	else			GPIO_ResetBits(medicinematrix[time][day].GPIOx,medicinematrix[time][day].GPIO_Pin);
}

 /**
  * @brief  吃药亮灯管理函数
  * @param  index 周一到周日（1-7）
	* @param	
	* @param  flag 开关状态 ON OFF
  * @retval 无
  */
void Medicine_LED_CtrlByPosition(u8 index,u8 flag)
{
	#if 0
	GPIOInfo medicinematrix[28] = {
															{{GPIOE,GPIO_Pin_0},{GPIOG,GPIO_Pin_12},{GPIOD,GPIO_Pin_3},{GPIOG,GPIO_Pin_7},{GPIOD,GPIO_Pin_13},{GPIOB,GPIO_Pin_13},{GPIOE,GPIO_Pin_8}},
															{{GPIOB,GPIO_Pin_8},{GPIOG,GPIO_Pin_10},{GPIOD,GPIO_Pin_1},{GPIOG,GPIO_Pin_5},{GPIOD,GPIO_Pin_11},{GPIOE,GPIO_Pin_14},{GPIOG,GPIO_Pin_1}},
															{{GPIOB,GPIO_Pin_3},{GPIOD,GPIO_Pin_7},{GPIOC,GPIO_Pin_12},{GPIOG,GPIO_Pin_3},{GPIOD,GPIO_Pin_9},{GPIOE,GPIO_Pin_12},{GPIOF,GPIO_Pin_15}},
															{{GPIOG,GPIO_Pin_14},{GPIOD,GPIO_Pin_5},{GPIOC,GPIO_Pin_3},{GPIOD,GPIO_Pin_15},{GPIOB,GPIO_Pin_15},{GPIOE,GPIO_Pin_10},{GPIOF,GPIO_Pin_13}}
															};
	#endif
															
#if 0		// change by zhang													
		GPIOInfo medicinematrix[28] = {{GPIOE,GPIO_Pin_0},{GPIOB,GPIO_Pin_8},{GPIOB,GPIO_Pin_3},{GPIOG,GPIO_Pin_14},{GPIOG,GPIO_Pin_12},{GPIOG,GPIO_Pin_10},{GPIOD,GPIO_Pin_7},{GPIOD,GPIO_Pin_5},
																		{GPIOD,GPIO_Pin_3},{GPIOD,GPIO_Pin_1},{GPIOC,GPIO_Pin_12},{GPIOC,GPIO_Pin_3},{GPIOG,GPIO_Pin_7},{GPIOG,GPIO_Pin_5},{GPIOG,GPIO_Pin_3},{GPIOD,GPIO_Pin_15},
																		{GPIOD,GPIO_Pin_13},{GPIOD,GPIO_Pin_11},{GPIOD,GPIO_Pin_9},{GPIOB,GPIO_Pin_15},{GPIOB,GPIO_Pin_13},{GPIOE,GPIO_Pin_14},{GPIOE,GPIO_Pin_12},{GPIOE,GPIO_Pin_10},
																		{GPIOE,GPIO_Pin_8},{GPIOG,GPIO_Pin_1},{GPIOF,GPIO_Pin_15},{GPIOF,GPIO_Pin_13}
															};
#endif
															
																		// 0~27 LED1~ LED17			
																		// LED1	PE0. LED2	PB8. LED3 PA1. LED4	PB3. LED5	PD6. LED6	PD4. LED7	PD2. 
																		// LED8	PD0. LED9	PC11. LED10	PA15. LED11	PA12. LED12	PC3. LED13	PC9. LED14	PC7.
																		// LED15	PD15. LED16	PD13. LED17	PD11. LED18	PD9. LED19	PB15. LED20	PB13.LED21	PB10.  												
																		// LED22	PE12. LED23	PE10. LED24	PE8. LED25	PB2. LED26	PB0. LED27	PC4. LED28	PA6.												
	GPIOInfo medicinematrix[28] = {{GPIOE,GPIO_Pin_0},{GPIOB,GPIO_Pin_8},{GPIOA,GPIO_Pin_1},{GPIOB,GPIO_Pin_3},{GPIOD,GPIO_Pin_6},{GPIOD,GPIO_Pin_4},{GPIOD,GPIO_Pin_2},
																	{GPIOD,GPIO_Pin_0},{GPIOC,GPIO_Pin_11},{GPIOA,GPIO_Pin_15},{GPIOA,GPIO_Pin_12},{GPIOC,GPIO_Pin_3},{GPIOC,GPIO_Pin_9},{GPIOC,GPIO_Pin_7},
																	{GPIOD,GPIO_Pin_15},{GPIOD,GPIO_Pin_13},{GPIOD,GPIO_Pin_11},{GPIOD,GPIO_Pin_9},{GPIOB,GPIO_Pin_15},{GPIOB,GPIO_Pin_13},{GPIOB,GPIO_Pin_10},
																	{GPIOE,GPIO_Pin_12},{GPIOE,GPIO_Pin_10},{GPIOE,GPIO_Pin_8},{GPIOB,GPIO_Pin_2},{GPIOB,GPIO_Pin_0},{GPIOC,GPIO_Pin_4},{GPIOA,GPIO_Pin_6}
																};
		
	if(flag) 	GPIO_SetBits(medicinematrix[index].GPIOx,medicinematrix[index].GPIO_Pin);
	else			GPIO_ResetBits(medicinematrix[index].GPIOx,medicinematrix[index].GPIO_Pin);
}

 /**
  * @brief  小药盒拿起放下检测
  * @param  day 周一到周日（1-7）
	* @param	time 时刻，早、中、晚、睡前， 取值（1-4）
  * @retval 0代表放在上面，1 被拿起
  */
u8 Medicine_Status_Detec(u8 day,u8 time)
{

	u8 ret = 1;


															// KEY1	PE1. KEY2	PB9. KEY3	PA0. KEY4	PB4. KEY5	PD7. KEY6	PD5. KEY7	PD3.
															// KEY8	PD1. KEY9	PC12. KEY10	PC10. KEY11	PA11. KEY12	PC2. KEY13	PA8. KEY14	PC8.
															// KEY15	PC6. KEY16	PD14. KEY17	PD12. KEY18	PD10. KEY19	PD8. KEY20	PB14. KEY21	PB12.
															// KEY22	PB11. KEY23	PE11. KEY24	PE9. KEY25	PE7. KEY26	PB1. KEY27	PC5. KEY28	PA7.	
															// 1 PE1 5 PD7 9  PC12 13 PA8  17 PD12 21 PB12 25 PE7
															// 2 PB9 6 PD5 10 PC10 14 PC8  18 PD10 22 PB11 26 PB1
															// 3 PA0 7 PD3 11 PA11 15 PC6  19 PD8  23 PE11 27 PC5
															// 4 PB4 8 PD1 12 PC2  16 PD14 20 PB14 24 PE9  28 PA7
	GPIOInfo medicinematrix[4][7] = {
															{{GPIOE,GPIO_Pin_1},{GPIOD,GPIO_Pin_7},{GPIOC,GPIO_Pin_12},{GPIOA,GPIO_Pin_8},{GPIOD,GPIO_Pin_12},{GPIOB,GPIO_Pin_12},{GPIOE,GPIO_Pin_7}},
															{{GPIOB,GPIO_Pin_9},{GPIOD,GPIO_Pin_5},{GPIOC,GPIO_Pin_10},{GPIOC,GPIO_Pin_8},{GPIOD,GPIO_Pin_10},{GPIOB,GPIO_Pin_11},{GPIOB,GPIO_Pin_1}},
															{{GPIOA,GPIO_Pin_0},{GPIOD,GPIO_Pin_3},{GPIOA,GPIO_Pin_11},{GPIOC,GPIO_Pin_6},{GPIOD,GPIO_Pin_8},{GPIOE,GPIO_Pin_11},{GPIOC,GPIO_Pin_5}},
															{{GPIOB,GPIO_Pin_4},{GPIOD,GPIO_Pin_1},{GPIOC,GPIO_Pin_2},{GPIOD,GPIO_Pin_14},{GPIOB,GPIO_Pin_14},{GPIOE,GPIO_Pin_9},{GPIOA,GPIO_Pin_7}}
															};
															
	ret = GPIO_ReadInputDataBit(medicinematrix[time][day].GPIOx,medicinematrix[time][day].GPIO_Pin);
															
	return ret;
}


//@return 0 放在上面 1 没有放在上面
u8 Detect_MedicineInfo(u8 position)
{
	u8 ret = 1;
#if 0  // change by zhang
	GPIOInfo medicinematrix[28] = {
															{GPIOE,GPIO_Pin_1},{GPIOB,GPIO_Pin_9},{GPIOB,GPIO_Pin_4},{GPIOG,GPIO_Pin_15},{GPIOG,GPIO_Pin_13},{GPIOG,GPIO_Pin_11},{GPIOG,GPIO_Pin_9},{GPIOD,GPIO_Pin_6},
															{GPIOD,GPIO_Pin_4},{GPIOD,GPIO_Pin_2},{GPIOD,GPIO_Pin_0},{GPIOC,GPIO_Pin_2},{GPIOG,GPIO_Pin_8},{GPIOG,GPIO_Pin_6},{GPIOG,GPIO_Pin_4},{GPIOG,GPIO_Pin_2},
															{GPIOD,GPIO_Pin_14},{GPIOD,GPIO_Pin_12},{GPIOD,GPIO_Pin_10},{GPIOD,GPIO_Pin_8},{GPIOB,GPIO_Pin_14},{GPIOB,GPIO_Pin_12},{GPIOE,GPIO_Pin_13},{GPIOE,GPIO_Pin_11},
															{GPIOE,GPIO_Pin_9},{GPIOE,GPIO_Pin_7},{GPIOG,GPIO_Pin_0},{GPIOF,GPIO_Pin_14}
															};
#endif 	
															// KEY1	PE1. KEY2	PB9. KEY3	PA0. KEY4	PB4. KEY5	PD7. KEY6	PD5. KEY7	PD3.
															// KEY8	PD1. KEY9	PC12. KEY10	PC10. KEY11	PA11. KEY12	PC2. KEY13	PA8. KEY14	PC8.
															// KEY15	PC6. KEY16	PD14. KEY17	PD12. KEY18	PD10. KEY19	PD8. KEY20	PB14. KEY21	PB12.
															// KEY22	PB11. KEY23	PE11. KEY24	PE9. KEY25	PE7. KEY26	PB1. KEY27	PC5. KEY28	PA7.	
	GPIOInfo medicinematrix[28] = {
															{GPIOE,GPIO_Pin_1},{GPIOB,GPIO_Pin_9},{GPIOA,GPIO_Pin_0},{GPIOB,GPIO_Pin_4},{GPIOD,GPIO_Pin_7},{GPIOD,GPIO_Pin_5},{GPIOD,GPIO_Pin_3},
															{GPIOD,GPIO_Pin_1},{GPIOC,GPIO_Pin_12},{GPIOC,GPIO_Pin_10},{GPIOA,GPIO_Pin_11},{GPIOC,GPIO_Pin_2},{GPIOA,GPIO_Pin_8},{GPIOC,GPIO_Pin_8},
															{GPIOC,GPIO_Pin_6},{GPIOD,GPIO_Pin_14},{GPIOD,GPIO_Pin_12},{GPIOD,GPIO_Pin_10},{GPIOD,GPIO_Pin_8},{GPIOB,GPIO_Pin_14},{GPIOB,GPIO_Pin_12},
															{GPIOB,GPIO_Pin_11},{GPIOE,GPIO_Pin_11},{GPIOE,GPIO_Pin_9},{GPIOE,GPIO_Pin_7},{GPIOB,GPIO_Pin_1},{GPIOC,GPIO_Pin_5},{GPIOA,GPIO_Pin_7}
															};															
															
	ret = GPIO_ReadInputDataBit(medicinematrix[position].GPIOx,medicinematrix[position].GPIO_Pin);
															
	return ret;
}


//判断小药盒的拿起放下状态
//如果返回0，则没有动作
//1 
char Detect_TakeAndPutStatus(u8 position)
{
//	prevStatus = curStatus;
	return Detect_MedicineInfo(position);
	
//	return (curStatus - prevStatus);
}

 /**
  * @brief  药盒大灯闪烁函数
  * @param  bFlash 0 停止闪烁 1 开始闪烁
	* @param	
  * @retval 
  */
//TIME_LED，PA5 时间到提醒大指示灯
void MediceineBox_BigLedFlash(u8 bFlash)
{
	if(bFlash)
	{
		eventdelay.EventSet |= 0x80;
		eventdelay.EventDelay[7] = 0;

	}else
	{
		eventdelay.EventSet &= (~0x80);
		eventdelay.EventDelay[7] = 0;
#if 0	// change by zhang
		GPIOF->BRR = GPIO_Pin_12;
#endif
		GPIOA->BRR = GPIO_Pin_5;
	}
	
}
	
void MedicineBoxLedFlash(void)
{
	if(eventdelay.EventDelay[7] > 1200)
	{
#if 0
		GPIOF->ODR ^= GPIO_Pin_12;
#endif
		GPIOA->ODR ^= GPIO_Pin_5;
		eventdelay.EventDelay[7] = 0;
		
	}
	if((systmtime.tm_wday == 0) && ((systmtime.tm_hour == 8) || (systmtime.tm_hour == 12)) && (AddDrugFlag == 0))
	{
		AddDrugFlag = 1;
		SetVoiceQueue(MEDICINE_CHECK);  //药物不足，请及时补充
	}
	if(((systmtime.tm_hour != 8) && (systmtime.tm_hour != 12))) AddDrugFlag = 0;
}





//********************************************END OF FILE***************************************************************
	
	
	


