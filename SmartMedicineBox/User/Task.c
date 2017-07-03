/**
  ******************************************************************************
  * @file    Task.c
  * @author  xiaobing19851019@163.com
  * @version V1.0
  * @date    2016.04.13
  * @brief   任务
  ******************************************************************************
  * @attention
  *
  * 平台		:STM32F103ZET6
  * 公司    :http://www.jjc12320.com
  *
  ******************************************************************************
  */ 
	
#include "Task.h"


	
	
__IO u8 RefreshPlan = 0;
__IO u8 PefreshVoice = 0;
static u8 SmartlinkSuccess = 0;
static u8 DebugCommCtrl = 0;

static u8 SmartlinkStatus = 0;

osThreadId RemindTaskId;
osThreadId LcdDisplayTimeAndVoiceCtrlTaskId;
osThreadId ESP8266ResetCtrlTaskId;


osThreadDef(RemindTask, osPriorityNormal, 1, 2500);       // define RemindTask as thread function
osThreadDef(LcdDisplayTimeAndVoiceCtrlTask, osPriorityNormal, 1, 800); 
osThreadDef(ESP8266ResetCtrlTask, osPriorityNormal, 1, 800); 
osThreadDef(ProgramFlashTask, osPriorityRealtime, 1, 0); 
//osThreadDef(IdleTask,osPriorityIdle,1,0);




char wifi_keep_connect = 0;


/**
  * @brief  变量初始化
  * @param  None
  * @retval : None
  */
static void VariableInit(void)
{
	int i = 0;
	memset(&protocolstu.takemedicine,0,sizeof(protocolstu.takemedicine));
	protocolstu.GetPlan = 0;
	SysStart = 1;
	for(i = 0; i < 28;i++)
	{
			protocolstu.TakeMedicinePlanArray[i] = -1;
	}

	ReadTokenCode();
	DebugCommCtrl = 0;
	SmartlinkSuccess = 0;
	protocolstu.bBanding = 1;
	if(protocolstu.bAuthorize == 1)
	{
		if(protocolstu.bBanding == 1) //绑定则获取一次时间同时强制获取一周服药计划
		{
			
			protocolstu.bNeedGetTMPlan = 1;
			protocolstu.bNeedGetUTC = 1;
		}  
		else
		{
			printf("protocolstu.bBanding != 1\r\n");
			protocolstu.bNeedGetUTC = 1;  //未绑定则只获取时间
		}
			
	}else
	{
		printf("Device start set ID! \r\n");
		SetDeviceID();
		StartRequestAuthorize();
	}
	
	eventdelay.EventSet |= 0x14;
	eventdelay.EventDelay[4] = 0;
}


void wifi_test()
{
	int i,j,len = 0;
	u8 Uart2Buf[256];
	u8 Uart1Buf[256];
	

	
	printf("in test function\r\n");	
//	ESP8266(ON)
	// test wifi
	while(0)
	{

		len = Usart_Read(1,Uart1Buf,256); // rev pc data 

		if(len > 0)
		{

			Usart_Send(2,(unsigned char*)Uart1Buf,len); //send to wifi

		}
		
		len = 0;
		len = Usart_Read(2,Uart2Buf,256); // read wifi 
		if(len > 1)
		{

			Usart_Send(1,(unsigned char*)Uart2Buf,len);  // send to pc

		}
			
		
		Delay_ms(20);
	}
	// test led 
	while(0)
	{
		printf("\r\nstart test led\r\n");
		GPIO_SetBits(GPIOA,GPIO_Pin_5);
		Delay_ms(1000);
		GPIO_SetBits(GPIOA,GPIO_Pin_4);
		Delay_ms(1000);
		GPIO_ResetBits(GPIOA,GPIO_Pin_4 | GPIO_Pin_5);
		Delay_ms(1000);
		
		// set 0  all led
		for(i = 0; i < 28; i++)
		{
			Medicine_LED_CtrlByPosition(i,0);	
		}
		
		for(i = 0; i < 28; i++)
		{
			Medicine_LED_CtrlByPosition(i,1);	
			Delay_ms(1000);	
			Medicine_LED_CtrlByPosition(i,0);	
			Delay_ms(1000);			
		}		
		if( 3 == j++)
			break;
	}
	// test key
	while(0)
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_15);
		printf("\r\nstart test key\r\n");
		for(i = 0; i < 4; i++)
		{
			for(j = 0; j < 7; j++)
			{
				if(Medicine_Status_Detec(i,j) == 1)
				{
					printf("week[%d][%d] near\r\n",i,j);
				}
			}
			
		}
	}
	
	while(1)
	{
		printf("is runing\r\n");
		GPIO_ResetBits(GPIOA,GPIO_Pin_15);
//		printf("read KEY1 = %d TakeMedicine_status = %d\r\n",GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1),Medicine_Status_Detec(0,0));
		j = 0;
		for(i = 0; i < 4; i++)
		{
			for(j = 0; j < 7; j++)
			{
				printf("\r\nready to read 		weed[%d][%d]\r\n",i,j);
				Delay_ms(1000);

				printf("read  weed[%d][%d]TakeMedicine_status = %d\r\n",i,j,Medicine_Status_Detec(j,i));

			}
			


		}
		for(i = 0; i < 28; i++)
		{
			printf("\r\n read key[%d] = %d ",i,Detect_MedicineInfo(i));
			Delay_ms(1000);			
		}
		Delay_ms(1000);
		Delay_ms(1000);
		Delay_ms(1000);
	}
		
	
	
}



void at24c256_test(void)
{
	int i = 0;
	u8 write[100];
	u8 read[100];
	
		for(i = 0; i < 100; i++)
			write[i] = 0;
	
	while(1)
	{
		for(i = 0; i < 100; i++)
			write[i] += i;

		AT24CXX_Write(0,write,100);
		Delay_ms(1000);
		AT24CXX_Read(0,read,100);
		for(i = 0; i < 100; i++)
		{
			printf("write[%d] = %d , read[%d] = %d\r\n",i,write[i],i,read[i]);
			Delay_ms(500);
		}
		Delay_ms(1000);
	}

	
	
}


void sc5080_test(void)
{
	int i = 0;
//	int j = 0;
	
	extern VoiceQueue	VQ;
	
	VQ.VoiceLevel = 0;
	while(1)
	{
			// 语音测试
//		for(i = 1; i < 12; i++)
//		{
//			printf("\r\n 第 %d 段语音\r\n",i);
//			VoiceReminder(i);
//			myDelay_ms(1000*5);
//		}
//		// 声音大小测试
//		for(i = 1; i < 12; i++)
//		{
//			for(j = 224; j < 240; j++) // 0XE0 ~ 0XEF
//			{
//				myDelay_ms(5000);
//				printf("\r\n 第 %d 段语音 第 %d 级音量	\r\n",i,j);
//				VoiceReminder(j);
//				myDelay_ms(1000);
//				VoiceReminder(i);
//				myDelay_ms(5000);
//			}
//			
//		}
		VQ.VoiceLevel++;
		if(VQ.VoiceLevel == 4)
			VQ.VoiceLevel = 0;
		// 声音开关测试
		for(i = 1; i < 4; i++)
		{
			
			printf("\r\n	VQ.VoiceLevel %d\r\n",VQ.VoiceLevel);
			VoiceLevelCtrl();
			myDelay_ms(100);
			VoiceReminder(REMIND_MUSIC);
			myDelay_ms(5000);
		}
	}

}

/**
  * @brief  设备初始化
  * @param  None
  * @retval : None
  */
static void DeviceBspInit(void)
{
//	DelayTickInit();  //配置滴答延时
	USART_Config();
	AT24CXX_Init();  // soft i2c
//	at24c256_test();
	
	
	Bsp_ESP8266_Init();
	

//	wifi_test();	
//	ADC1_Init();
	BSP_HT1621B_Init();
	//检测时钟是否设置
	RTC_CheckAndConfig(&systmtime);
	/* 配置RTC秒中断优先级 */
	RTC_NVIC_Config();

	BSP_SC5080_Init();
	LED_GPIO_Config();
//	BSP_M35Init();

	//I2C_EE_Init(); // hardware i2c
	//isd1700初始化
//	Bsp_ISD1700Init();
	
//	sc5080_test();
	ESP8266(ON)
	printf("ready to try test\r\n");
	
	RotationQuerySimStatus();	// 轮流查询SIM卡状态
	printf("智能药盒设备驱动初始化完成！\r\n");
}


/**
  * @brief  定期更新时间和服药计划
  * @param  None
  * @retval : None
  */
static void RefreshParam(void)
{
		if((systmtime.tm_hour == 9)&&(systmtime.tm_min == 50) && (RefreshPlan == 0))
		{
			protocolstu.bNeedGetUTC = 1;
			RefreshPlan = 1;
		}
		if((systmtime.tm_hour == 23) && (systmtime.tm_min == 50) && (RefreshPlan == 0)) 
		{
			protocolstu.bNeedGetUTC = 1;  //每天 23:50分更新一次时间
			RefreshPlan = 1;
		}

		if((systmtime.tm_min%5 == 0)&&(RefreshPlan == 0))
		{
			protocolstu.bNeedGetTMPlan = 1;  //5分钟更新一次
			RefreshPlan = 1;
		}
		if((systmtime.tm_min%5 != 0)&&(RefreshPlan == 1))
		{
			RefreshPlan = 0;
		}
		
		
		
		// fresh voice 
		if((systmtime.tm_min%2 == 1) && (PefreshVoice == 0 ))	// 1 分钟更新一次
		{
			protocolstu.bNeedGetVoice = 1;  //每 1分更新一次时间	
			PefreshVoice = 1;
		}
		if((systmtime.tm_min%2 != 1) && (PefreshVoice == 1 ))	// 1 分钟更新一次
		{
			PefreshVoice = 0;  //每 1分更新一次时间			
		}
}





// u8 bNeedAuthorize;  //1 完整授权 2更新授权
//u8 GetAuthorizeStep;  //药盒获取授权令牌步骤，1 发送请求授权；2 重发请求授权，如果服务器授权后，获得code；3 使用2步骤获得的code传送POST请求，获取access_token和refresh_token 4 使用refresh_token获取access_token

/**
  * @brief  处理M35串口的数据
  * @param  None
  * @retval : None
  */
static void ProcessM35Comm(void)
{
	u8 Uart2Buf[256];
	int i = 0,len = 0,atCmdLen = 0;
	AT_Flag atflags = ATDFT;
	static int wifi_connect_step = -1;


	if(protocolstu.GetServerResponsesFlag == 1)
	{
		if(eventdelay.EventDelay[3]> 1000)
		{
			if(protocolstu.GetAuthorizeStep)
				ParseGetServerAuthorizeJson((char*)(protocolstu.ServerResponse.ResponseBuf));  //处理授权请求返回JSON数据包
			if(protocolstu.AgreeBandingStep)  					
			{
				protocolstu.AgreeBandingStep = 0;
				if(protocolstu.bBanding != 1)
				{
					protocolstu.bBanding = 1;
					////  //  //  CLI(); // 关中断
					SaveTokenCode();
					////  SEI(); // 开中断
//					printf("\r\n saveTokenCode\r\n");
				}
			}
			if(protocolstu.GetUtcTimeStep == 1)
				ParseGetServerRtcTimeJson((char*)(protocolstu.ServerResponse.ResponseBuf));
			if(protocolstu.GetTakeMedicinePlanStep)
				ParseGetTakeMedicinePlanJson((char*)(protocolstu.ServerResponse.ResponseBuf));
			if(protocolstu.SendTakeMedicineCaseStep)
				protocolstu.SendTakeMedicineCaseStep = 0;
			
			if(protocolstu.GetVoiceStep == 1)
			{
				ParseGetServerVoiceJson((char*)(protocolstu.ServerResponse.ResponseBuf));
				VoiceLevelCtrl();
			}
				
			
			
			TCP_Close();
			//清理定时器，数据
			protocolstu.HttpsLocker = 0;
			protocolstu.GetServerResponsesFlag = 0;
			protocolstu.ServerResponse.ResponseHead = 0;
			protocolstu.ServerResponse.ResponseBuf[0] = 0;
			eventdelay.EventSet &= (~0x08);
			eventdelay.EventDelay[3] = 0;
		}
	}
	
	
	if(protocolstu.nNeedATKeyCmd == 2)  // AT_KEY
	{
		wifi_status(2);
		printf("\r\n restore  smarlink start\r\n");
		wifi_connect_step++;
		SmartlinkSuccess = 0;
		ESP8266(ON)
		Delay_ms(500);
		reconfig_wifi();
		Delay_ms(1000);
		SmartlinkStatus = 1;
		SmartLinkStart();
		

		eventdelay.EventSet |= (0x1000);
		eventdelay.EventDelay[12] = 0;	
		
	}
	
	if(SmartlinkStatus)
	{
		if(eventdelay.EventDelay[12] % 1000 == 0 && eventdelay.EventDelay[12] > 50)
		{							
			 GPIOE->ODR ^= GPIO_Pin_14; // PC13 netlight   //	NETLIGHT_RLED PE14
		}
		if(eventdelay.EventDelay[12] > 100)
		{
			//SetVoiceQueue(CONNECT_WAIT); //等待网络链接
			if(eventdelay.EventDelay[12] > 100 && eventdelay.EventDelay[12] <= 120*(1000)) // 2 min
			{
				if(eventdelay.EventDelay[12] % (10*1000) == 0) // 10s every time
				{
					SetVoiceQueue(CONNECT_WAIT); //等待网络链接
				}
			}
			else if(eventdelay.EventDelay[12] > (120 * 1000) && eventdelay.EventDelay[12] <= (300 * 1000)) // 2 ~ 5 min
			{
				if(eventdelay.EventDelay[12] % (20*1000) == 0) // 20s every time
				{
					SetVoiceQueue(CONNECT_WAIT); //等待网络链接
				}
			}
			else if(eventdelay.EventDelay[12] > (300 * 1000) && eventdelay.EventDelay[12] <= (600 * 1000)) // 5 ~ 10 min
			{
				if(eventdelay.EventDelay[12] % (30*1000) == 0) // 30s every time
				{
					SetVoiceQueue(CONNECT_WAIT); //等待网络链接
				}
			}
			else if(eventdelay.EventDelay[12] > (600 * 1000) && eventdelay.EventDelay[12] <= (1800 * 1000)) // 10 ~ 30 min
			{
				if(eventdelay.EventDelay[12] % (60*1000) == 0) // 60s every time
				{
					SetVoiceQueue(CONNECT_WAIT); //等待网络链接
				}
			}
		}
		
		if(eventdelay.EventDelay[12] > 2000000000) // 23 day
		{
			eventdelay.EventSet &= (~0x1000);
			eventdelay.EventDelay[12] = 0;
		}
	}	
	
#if 1	

	len = Usart_Read(2,Uart2Buf,256); // rev wifi data

	if(len > 0)
	{
		for(i = 0;i<len;i++)
		{
			if(protocolstu.GetServerResponsesFlag == 0)
			{
				atCmdLen = ParseOneFrameATCmd(Uart2Buf+i);
				if(atCmdLen)
				{
					atflags = GetHeadType();
					

					if(atflags == WIFIGOTIP) 									
					{
						if(SmartlinkSuccess)
						{
//							SetVoiceQueue(CONNECT_SUCCESS);  //网络链接成功
							SmartlinkSuccess = 0;
						}
						SetVoiceQueue(CONNECT_SUCCESS);  //网络链接成功
						WIFI_Ready = 1;
						SmartlinkStatus = 0;	
						eventdelay.EventSet &= (~0x1000);
						eventdelay.EventDelay[12] = 0;
						wifi_status(2);
						wifi_status(1);
						wifi_connect_step = -1; // ready to next connet
						Delay_ms(100);
					}
					if(atflags == WIFIDISCN)
					{
//						SetVoiceQueue(CONNECT_WAIT); //等待网络链接
//						SmartLinkStart();
						if(wifi_connect_step == -1)
						{
							printf("\r\n rest\r\n");
							ResetEsp8266();
							wifi_connect_step++;
							
							eventdelay.EventSet |= (0x1000);
							eventdelay.EventDelay[12] = 0;	
						}
//						eventdelay.EventSet |= 0x400;
//						eventdelay.EventDelay[10] = 0;
						
//						if(SmartlinkStatus == 0)
//							wifi_status(0);
						
							if(eventdelay.EventDelay[12] % 1000 == 0 && eventdelay.EventDelay[12] > 50)
							{		
								 GPIO_ResetBits(GPIOE,GPIO_Pin_13);
								 GPIOE->ODR ^= GPIO_Pin_14; // PC13 NETLIGHT_GLED   //	NETLIGHT_RLED PE14
								
							}
						
					}
					

					
					if(atflags == SMART_SUCCESS)
					{
						SmartLinkStop();
						SmartlinkSuccess = 1;
						
						eventdelay.EventSet &= (~0x400);
						eventdelay.EventDelay[10] = 0;
						
						
						eventdelay.EventSet &= (~0x1000); // 网络灯停止闪烁
						eventdelay.EventDelay[12] = 0;
					}
					

					
					if(protocolstu.GetAuthorizeStep)
						GetServerAuthorizeCode(atflags);

					
					
					if(protocolstu.AgreeBandingStep)
						GetAgreeBandingCmd(atflags);
				
					if(protocolstu.GetUtcTimeStep == 1)
						GetRegulateTimeCmd(atflags);
					if(protocolstu.GetTakeMedicinePlanStep)
						GetTakeMedicinePlanCmd(atflags,protocolstu.GetTakeMedicinePlanStep-1);
					if(protocolstu.SendTakeMedicineCaseStep)
						SendTakeMedicineSituationCmd(atflags);
					if(protocolstu.GetVoiceStep == 1)
					{
						GetRegulateVoiceCmd(atflags);
					}
				}
			}else
			{
				ParseServerResponses(Uart2Buf+i);
			}
		}
		Usart_Send(1,Uart2Buf,len);  // show for pc   send to pc

	}


#else
//	if(protocolstu.GetServerResponsesFlag == 0)
//	{
		if(protocolstu.GetAuthorizeStep)
			GetServerAuthorizeCode(atflags);

		
		
		if(protocolstu.AgreeBandingStep)
			GetAgreeBandingCmd(atflags);
	
		if(protocolstu.GetUtcTimeStep == 1)
			GetRegulateTimeCmd(atflags);
		if(protocolstu.GetTakeMedicinePlanStep)
			GetTakeMedicinePlanCmd(atflags,protocolstu.GetTakeMedicinePlanStep-1);
		if(protocolstu.SendTakeMedicineCaseStep)
			SendTakeMedicineSituationCmd(atflags);		
//	}


		len = Usart_Read(2,Uart2Buf,256); // rev wifi data
		if(len > 0)
		{
			Usart_Send(1,Uart2Buf,len);  // show for pc   send to pc
		}
		
#endif


}


/**
  * @brief  处理调试串口数据，并将命令通过串口2转发到wifi模块
  * @param  None
  * @retval : None
  */
static void ProcessDebugComm(void)
{
	u8 Uart2Buf[256];
	int len = 0,i = 0,ret = 0;
	
	len = Usart_Read(1,Uart2Buf,256);  //rev pc data
	
	for(i = 0; i < len; i++)
	{
		ret = ParseOneFrameCustomCmd(Uart2Buf+i);
		if(ret != 0)
		{
			if(cst.frmData[1] == '0' && cst.frmData[2] == '0')
			{
				DebugCommCtrl = 0;
				return;
			}
			else if(cst.frmData[1] == '0' && cst.frmData[2] == '1')
			{
				DebugCommCtrl = 1;
				return;
			}
			else if(cst.frmData[1] == '1' && cst.frmData[2] == '1')
			{
				osThreadCreate(osThread(ProgramFlashTask),NULL);	// usart updata bin file.
				return;
			}
			else if(cst.frmData[1] == '2' && cst.frmData[2] == '1')
			{
				ESP8266(ON);
				return;
			}
			// CHANGE by zhang   reset ID
			else if(cst.frmData[1] == '2' && cst.frmData[2] == '2')
			{
//				I2C_EE_BufferWrite(0, 0, 192);  //  change by zhang
				DebugCommCtrl = 1;
				AT24CXX_Write(0,0,192);
				SetDeviceID();
				return ;
			}
			// change by zhang test wifi module
			else if(cst.frmData[1] == '2' && cst.frmData[2] == '3')
			{
				printf("\r\nstart to wifi test\r\n");
				ESP8266(ON);
				wifi_keep_connect = 1;
				DebugCommCtrl = 1; // enable usart1
#if 1		

			while(1)
			{

				len = Usart_Read(1,Uart2Buf,256); // rev pc data 
				if(strstr((char *)Uart2Buf,"quit"))
				{
					printf("close wifi test\r\n");
					wifi_keep_connect = 0;
					break;
				}
				
				if(len > 0)
				{

					Usart_Send(2,(unsigned char*)Uart2Buf,len); //send to wifi
				}
				Delay_ms(20);
				len = 0;
				len = Usart_Read(2,Uart2Buf,256); // read wifi 
				if(len > 0)
				{

					Usart_Send(1,(unsigned char*)Uart2Buf,len);  // send to pc
					
				}
					
				
				Delay_ms(20);
			}
#endif
				return;
			}
				
		}
	}
	if(len > 0)
	{
		if(DebugCommCtrl == 1)
		{
//			if(Uart2Buf[0]=='@')
//			{
//	  		protocolstu.bNeedGetTMPlan = 1;
//				protocolstu.bNeedGetUTC = 1;
//				Uart2Buf[0]= 0;
//			}
//			if(Uart2Buf[0]=='&')
//			{
//				RefreshAccessToken();
//				Uart2Buf[0]= 0;
//			}
//			if(Uart2Buf[0]=='%')
//			{
//				StartRequestAuthorize();
//				Uart2Buf[0]= 0;
//			}
//			else if(Uart2Buf[0]=='$') 
//			{
//				printf("TIME %04d-%02d-%02d %02d:%02d:%02d",systmtime.tm_year,systmtime.tm_mon,systmtime.tm_mday,systmtime.tm_hour,systmtime.tm_min,systmtime.tm_sec);
//			}else 
			Usart_Send(2,Uart2Buf,len);  // send data to wifi
		}
	}
}


//static int checkend(void)
//{
//	int i = 0x12345678;
//	char *c = (char*)&i;
//	return (*c == 0x12);
//}


#if 0
void job1 (void const *argument)  {              // thread function 'job1'
  while (1)  {
      printf("job1\r\n");                                         // execute some code
    osDelay (1000);                                // delay execution for 10 milliseconds
  }
}
// define job1 as thread function
osThreadDef(job1, osPriorityAboveNormal, 1, 0);  // define job1 as thread function
void job2 (void const *argument)  {              // thread function 'job2'
        // create job1 thread
//  while (1)   {
		osThreadCreate (osThread(job1),NULL);	
		 
//    osDelay(100);                                           // execute some code
//  }
}
osThreadDef(job2, osPriorityNormal, 1, 0);       // define job2 as thread function

/**
  * @brief  设备语音控制任务
  * @param  argument
  * @retval : None
  */
void VoiceCtrlTask(void const *argument)
{
	while(1)
	{
		
		osThreadYield();
	}
}
osThreadDef(VoiceCtrlTask, osPriorityNormal, 1, 0); 
#endif


#if 0
/**
  * @brief  程序空闲任务
  * @param  argument
  * @retval : None
  */
void IdleTask(void const *argument)
{
//	__set_FAULTMASK(1);		//关闭中断
//	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
	while(1)
	{
		osDelay(200);
		printf("\r\nenter idle");
	}
}

#endif
/**
  * @brief  程序下载任务
  * @param  argument
  * @retval : None
  */
void ProgramFlashTask(void const *argument)
{
	osThreadTerminate(RemindTaskId);
	osThreadTerminate(LcdDisplayTimeAndVoiceCtrlTaskId);
	osThreadTerminate(ESP8266ResetCtrlTaskId);
	__set_FAULTMASK(1);		//关闭中断
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
	while(1)
	{
		Main_Menu();
	}
}


/**
  * @brief  WIFI模块复位任务
  * @param  argument
  * @retval : None
  */
void ESP8266ResetCtrlTask(void const *argument)
{
	while(1)
	{
		if(protocolstu.HttpsLocker == 0)
		{
//			if(eventdelay.EventDelay[9] > 3000)
//			{
//				if(SmartlinkStatus != 1)
//				{
////					ESP8266(OFF)	
//					WIFI_Status_Check();

//				}

//				
//				WIFI_Ready = 0;
//				eventdelay.EventSet &= ~0x200;  //关闭第十个定时器
//				eventdelay.EventDelay[9] = 0;
//				printf("Close WiFi\r\n");
//				
//			}
			
			
		}
		
		if(protocolstu.bNeedAuthorize == 1 || protocolstu.bNeedGetTMPlan == 1 || protocolstu.bNeedGetUTC == 1 || protocolstu.bNeedSendATCmd == 1 || protocolstu.takemedicine.needsend == 1 || protocolstu.nNeedATKeyCmd == 1)
		{
			if(WIFI_Ready == 0 || protocolstu.nNeedATKeyCmd == 1)	
			{
//				eventdelay.EventSet &= ~0x200;  //关闭第十个定时器
//				eventdelay.EventDelay[9] = 0;
//				ESP8266(ON)

			}
		}
		if(wifi_keep_connect == 1)  // 正在调试wifi
		{
			while(wifi_keep_connect);
		}
		

		

		osDelay(100);
//		osThreadYield();
	}
}


/**
  * @brief  设备时间显示任务
  * @param  argument
  * @retval : None
  */
void LcdDisplayTimeAndVoiceCtrlTask(void const *argument)
{
	while(1)
	{
		Time_Show(&systmtime);
		VoiceLoop();
		MedicineBoxLedFlash();
		osDelay(100);
//		osThreadYield();
	}
}


/**
  * @brief  设备提醒任务
  * @param  argument
  * @retval : None
  */
void RemindTask(void const *argument)
{
  while(1)
	{
		if((eventdelay.EventSet&0x10) == 0) eventdelay.EventSet |= 0x10;  
		
//		Process_ISD_REC();
//		Process_ISD_ERASE();

		if(protocolstu.GetUtcTimeStep == 2)  //已经成功获取RTC时间
		{
			if(RTC_GetCounter() > protocolstu.TokenRefreshTimeValue + 560000)
			{
				protocolstu.bNeedAuthorize = 2;
				protocolstu.GetUtcTimeStep = 3;  //置位
			}
		}
		
		ProcessDebugComm();  //处理调试串口数据并通过串口2将命令转发

		ProcessM35Comm();		// 处理M35串口的数据
		
		ClearAllLedWarn();   // 周日晚上23:59分会清理所有灯光提醒


//		if(ADC_ConvertedValue < 1350 && ADC_ConvertedValue > 1000)
//			BatteryStatusFlash();
//		else BATTERY_LED(0);
		
		PollsSendHttpsRequest();  // 轮询是否发送HTTPS请求
		
		if(protocolstu.bAuthorize != 1) continue;
		AuthorizeLogin();  // 绑定手机APP
		if(protocolstu.bBanding != 1) continue;

		DecideTakeMedicineRemind();  //检测小药盒

		RefreshParam();  //更新数据

		osDelay(50);
//		osThreadYield();
	}
}


osTimerId EventTimerId;   //事件定时器基准，100ms


void EventTimerCallback(void const *arg)
{
	int i = 0;
	
	for(i = 0; i < 32;i++)
	{
		if(((eventdelay.EventSet)>>i) &0x01)	eventdelay.EventDelay[i] += 50;
	}
	

	
}

osTimerDef(EventTimer,EventTimerCallback);

/**
  * @brief  设备初始化任务
  * @param  argument
  * @retval : None
  */
void Device_InitTask(void const *argument)
{
	osStatus status;
	DeviceBspInit();
	printf("智能药盒开始启动！\r\n");
	VariableInit();
	printf("智能药盒初始化完成！\r\n");
	

//	printf("Memory endian = %d\r\n",checkend());

	EventTimerId = osTimerCreate(osTimer(EventTimer),osTimerPeriodic,NULL);  //创建事件定时器基准 100ms
	if(EventTimerId == NULL)  printf("EventTimerId error!\r\n");
	status = osTimerStart(EventTimerId,50);
	if(status != osOK)  printf("EventTimerId start error!\r\n");
	

	//启动其他任务
	RemindTaskId = osThreadCreate(osThread(RemindTask),NULL);	
	LcdDisplayTimeAndVoiceCtrlTaskId = osThreadCreate(osThread(LcdDisplayTimeAndVoiceCtrlTask),NULL);	
	ESP8266ResetCtrlTaskId = osThreadCreate(osThread(ESP8266ResetCtrlTask),NULL);
//	osThreadCreate(osThread(IdleTask),NULL);
}




	
	
	
	
	
//===============================END OF FILE====================================

