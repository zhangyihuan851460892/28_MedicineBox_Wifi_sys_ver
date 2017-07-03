/**
  ******************************************************************************
  * @file    MedicineBoxOperation.c
  * @author  xiaobing19851019@163.com
  * @version V1.0
  * @date    2015.12.5
  * @brief   药盒服药提醒流程操作
  ******************************************************************************
  * @attention
  *
  * 平台		:STM32F103ZET6 
  * 公司    :http://www.jjc12320.com
  * 
  *
  ******************************************************************************
  */ 
#include "MedicineBoxOperation.h"


/*时间结构体*/
extern rtc_time systmtime;


//是否进行过提醒
u8 bWarn = 0;
//是否被拿起
u8 bTaken = 0;
//提醒次数
u8 WarnCnt = 0;
//是否上报服药记录
u8 bUpdateRecord = 0;  //每一个药盒是否进行上报，如果上报则对第一次上报将 bit0置一，当六次全部上报了，bUpdateRecord = 0x3f
u32 MedicineBoxStatus = 0;  //28个药盒状态 每个BIT代表一个0在大盒子上，1不在大盒子上
u8 SysStart = 0;


short warn_time = 0;


int find_id(int needtime)
{
	int i = 0,ret = -1;
	// find id
	for(i = 0; i < protocolstu.isize; i++)
	{
		if(needtime == protocolstu.AppTakeWarn[i].time)
		{
			protocolstu.id = protocolstu.AppTakeWarn[i].id;
			ret = protocolstu.id;
		}
	}
	
	return ret;
}



/**
*	@brief 判断服药提醒 // 上午 00:00 - 10:59(0-659) 中午 11:00 - 13:59(660-839) 下午 14:00 - 18:59(840-1199) 晚上 19:00 - 23:59(1200-1439)	
*	@param 
* @param 						
* @param 
* @return 
**/
void DecideTakeMedicineRemind(void)
{
	u8 weekday = 0,index = 0;  //0 - 6 日~六
	short minuteValue = 0;
	short TimeLimit1,TimeLimit2,TimeLimit3,TimeLimit4;
	int i = 0,j = 0,temp = 0,temp_time[36] = {-1},iTime[36] = {-1};
	
	if(protocolstu.GetPlan == 0) return;
	weekday = systmtime.tm_wday;
	minuteValue = (systmtime.tm_hour)*60 + systmtime.tm_min;
	
	index = 4*weekday -3;
	if(weekday == 0) index = 25;
	
	for(i = 0; i < 28; i++) // 所在天计划 初始化
	{
//		protocolstu.TakeMedicinePlanArray[i] = -1;
	}
	
	for(i = 0; i < protocolstu.isize; i++)
	{
		if(protocolstu.AppTakeWarn[i].week[weekday] == 1)  // 所在天 是否有计划
		{
			iTime[i] = protocolstu.AppTakeWarn[i].time;				
		}
		
	}
	for(i = 0; i < protocolstu.isize; i++)
	{
		temp_time[i] = 	iTime[i];
	}
	// sort
	for(i = 0; i < protocolstu.isize; i++)
	{
		for(j = i; j < protocolstu.isize; j++)
		{
			if(temp_time[i] > temp_time[j])
			{
				temp = temp_time[i];
				temp_time[i] = temp_time[j];
				temp_time[j] = temp;
			}
		}
	}

// 上午 00:00 - 10:59(0-659) 中午 11:00 - 13:59(660-839) 下午 14:00 - 18:59(840-1199) 晚上 19:00 - 23:59(1200-1439)		
	for(i = 0; i < protocolstu.isize; i++)
	{
		if(temp_time[i]> 0 && temp_time[i] <= 659) // 上午
		{
			

				if(minuteValue > (temp_time[i] - 3) && minuteValue < temp_time[i])
				{
					protocolstu.TakeMedicinePlanArray[index-1] = temp_time[i];
				}
		
//			else if(i == 0)
//			{
//				protocolstu.TakeMedicinePlanArray[index-1] = temp_time[i];
//			}
			
		}
		else if(temp_time[i] >= 660 && temp_time[i] <= 839) // 中午
		{

				if(minuteValue > (temp_time[i] - 3) && minuteValue < temp_time[i])
				{
					protocolstu.TakeMedicinePlanArray[index] = temp_time[i];
				}

//			else if(i == 0)
//			{
//				protocolstu.TakeMedicinePlanArray[index] = temp_time[i];
//			}
			
		}
		else if(temp_time[i] >= 840 && temp_time[i] <= 1199) // 下午
		{

				if(minuteValue > (temp_time[i] - 3) && minuteValue < temp_time[i])
				{
					protocolstu.TakeMedicinePlanArray[index+1] = temp_time[i];
				}

//			else if(i == 0)
//			{
//				protocolstu.TakeMedicinePlanArray[index+1] = temp_time[i];
//			}
			
		}
		else if(temp_time[i] >= 1200 && temp_time[i] <= 1439) // 晚上
		{

				if(minuteValue > (temp_time[i] - 3) && minuteValue < temp_time[i])
				{
					protocolstu.TakeMedicinePlanArray[index+2] = temp_time[i];
				}
			
//			else if(i == 0)
//			{
//				protocolstu.TakeMedicinePlanArray[index+2] = temp_time[i];
//			}
				
		}
		
		else if (minuteValue == temp_time[i])
		{
			
//			printf("\r\n	weekday = %d index = %d TakeMedicinedId = %d PlanTime = %d \r\n",weekday,index,find_id(temp_time[i]),temp_time[i]);
		}
		
	}
	


#if 1	
	
	TimeLimit1 = (protocolstu.TakeMedicinePlanArray[index-1] 	== (-1)?(0+60):protocolstu.TakeMedicinePlanArray[index-1]);
	TimeLimit2 = (protocolstu.TakeMedicinePlanArray[index] 		== (-1)?(659+60):protocolstu.TakeMedicinePlanArray[index]);
	TimeLimit3 = (protocolstu.TakeMedicinePlanArray[index+1]	== (-1)?(840+60):protocolstu.TakeMedicinePlanArray[index+1]);
	TimeLimit4 = (protocolstu.TakeMedicinePlanArray[index+2] 	== (-1)?(1200+60):protocolstu.TakeMedicinePlanArray[index+2]);
	
#endif
//	// 上午 00:00 - 10:59(0-659) 中午 11:00 - 13:59(660-839) 下午 14:00 - 18:59(840-1199) 晚上 19:00 - 23:59(1200-1439)		
//	TimeLimit1 = (protocolstu.TakeMedicinePlanArray[index-1] 	== (-1)?(0+60):protocolstu.TakeMedicinePlanArray[index-1]);
//	TimeLimit2 = (protocolstu.TakeMedicinePlanArray[index] 		== (-1)?(660+60):protocolstu.TakeMedicinePlanArray[index]);
//	TimeLimit3 = (protocolstu.TakeMedicinePlanArray[index+1]	== (-1)?(840+60):protocolstu.TakeMedicinePlanArray[index+1]);
//	TimeLimit4 = (protocolstu.TakeMedicinePlanArray[index+2] 	== (-1)?(1199+60):protocolstu.TakeMedicinePlanArray[index+2]);
	
		

	
		
	// 上午 00:00 - 10:59(0-659) 中午 11:00 - 13:59(660-839) 下午 14:00 - 18:59(840-1199) 晚上 19:00 - 23:59(1200-1439)
#if 0  // change by zhang
	if((minuteValue > -1)&&(minuteValue < TimeLimit1))//上午
	{
		find_id(TimeLimit1);
		if(index == 1)
			CurrentMedicineBoxCtrl(27,TimeLimit1);
		else
			CurrentMedicineBoxCtrl(index-2,TimeLimit1);
	}
	else if((minuteValue > -1)&&(minuteValue < TimeLimit2))//上午	TimeLimit2
	{
		find_id(protocolstu.TakeMedicinePlanArray[index -1]);
		CurrentMedicineBoxCtrl(index-1,TimeLimit2);
	}else if((minuteValue > 659)&&(minuteValue < TimeLimit3))//中午	TimeLimit3
	{
		find_id(protocolstu.TakeMedicinePlanArray[index]);
		CurrentMedicineBoxCtrl(index,TimeLimit3);
	}else if((minuteValue > 840)&&(minuteValue < TimeLimit4))//下午 //TimeLimit4
	{
		find_id(protocolstu.TakeMedicinePlanArray[index + 1]);
		CurrentMedicineBoxCtrl(index+1,TimeLimit4);
	}else if((minuteValue > 1200)&&(minuteValue < 1440))//晚上
	{
		find_id(protocolstu.TakeMedicinePlanArray[index + 2]);
		CurrentMedicineBoxCtrl(index+2,1440);
	}
	
#else 
	// 上午 00:00 - 10:59(0-659) 中午 11:00 - 13:59(660-839) 下午 14:00 - 18:59(840-1199) 晚上 19:00 - 23:59(1200-1439)
//	if((minuteValue > -1)&&(minuteValue <= TimeLimit1))//上午
//	{
//		find_id(TimeLimit1);
//		if(index == 1)
//			CurrentMedicineBoxCtrl(27,TimeLimit1);
//		else
//			CurrentMedicineBoxCtrl(index-2,TimeLimit1);

//	}
	 if((minuteValue > -1)&&(minuteValue <= 659))//上午
	{
//		find_id(protocolstu.TakeMedicinePlanArray[index - 1]);
		CurrentMedicineBoxCtrl(index-1,659);

	}else if((minuteValue >= 660)&&(minuteValue <= 839))//中午
	{
//		find_id(protocolstu.TakeMedicinePlanArray[index]);
		CurrentMedicineBoxCtrl(index,839);
	
	}else if((minuteValue >= 840)&&(minuteValue <= 1199))//下午
	{
//		find_id(protocolstu.TakeMedicinePlanArray[index + 1]);
		CurrentMedicineBoxCtrl(index+1,1199);

	}else if((minuteValue >= 1200)&&(minuteValue <= 1440))//晚上
	{
//		find_id(protocolstu.TakeMedicinePlanArray[index + 2]);
		CurrentMedicineBoxCtrl(index+2,1440);


	}
	
	
#endif
}


static u8 ATKeyLocker = 0;

/**
*	@brief 绑定手机APP
*	@param 
* @param 
* @param 
* @return 
**/
void AuthorizeLogin(void)
{
#if 0
	if(!AT_KEY) 
	{
//		printf("at down\r\n");
		if(eventdelay.EventDelay[0] == 0)
			eventdelay.EventSet |= 0x01;
		if((eventdelay.EventDelay[0] > 2100) && (eventdelay.EventDelay[0] < 2300))
		{
			if(ATKeyLocker == 0)
			{
				ATKeyLocker = 1;
//				BEEP();
			}
		}
//		if((eventdelay.EventDelay[0] > 4800) && (eventdelay.EventDelay[0] < 5100))
//		{
//			if(ATKeyLocker == 1)
//			{
//				ATKeyLocker = 2;
//				if(WIFI_Ready == 0) {ESP8266(ON)}
//			}
//		}
//		if((eventdelay.EventDelay[0] > 8500) && (eventdelay.EventDelay[0] < 8700))
//		{
//			if(ATKeyLocker == 2)
//			{
//				BEEP();
//			}
//		}
	}
	else 
	{
//		if(eventdelay.EventDelay[0])
//		printf("at up\r\n");
		if((eventdelay.EventDelay[0] > 2100) && (eventdelay.EventDelay[0] < 5500))
		{
//			printf("at cmd 1\r\n");
			if(ATKeyLocker == 1)
			{
				protocolstu.bNeedSendATCmd = 1;
				printf("at cmd\r\n");
			}
		}
		
//		if(eventdelay.EventDelay[0] > 8700)
//		{
//			if(ATKeyLocker == 2)
//			{
////				Esp8266APDisconnect();
//				SmartLinkStart();
//				SetVoiceQueue(CONNECT_WAIT);  //
//			}
//		}
		
		if(eventdelay.EventDelay[0])
		{
			ATKeyLocker = 0;
			eventdelay.EventSet &= (~(0x01));
			eventdelay.EventDelay[0] = 0;
		}
	}
	if(eventdelay.EventDelay[0] > 20000)
	{
		ATKeyLocker = 0;
		eventdelay.EventSet &= (~(0x01));
		eventdelay.EventDelay[0] = 0;
	}
#endif
	
	if(!AT_KEY)
	{
		if(eventdelay.EventDelay[0] == 0)
			eventdelay.EventSet |= 0x01;
		if(eventdelay.EventDelay[0] > 2950) 
		{
			if(ATKeyLocker == 0)
			{
				protocolstu.nNeedATKeyCmd = 1;
				ATKeyLocker = 1;
				printf("\r\nat cmd\r\n");
			}
				
		}

			

		
	}
	else if(AT_KEY)
	{
		ATKeyLocker = 0;
		
		eventdelay.EventSet &= (~(0x01));
		eventdelay.EventDelay[0] = 0;
	}
	
	
}


//static u8 CurrentIndex = 30;
u8 prev_flag = 0;
short CurrentTime = -1;
short prevTime = -1;
short validdateTime = -1;
/**
*	@brief 监控当前时段的小药盒
*	@param index 药盒编号
* @param validdate 最大截止时间
* @param 
* @return 
**/
void CurrentMedicineBoxCtrl(u8 index,short validdate)
{
	u8 takeCnt = 0,i = 0;  //0 - 6 日~六
	short minuteValue = 0;
	
	DetectionBoxStatus(index);  //清理亮灯
	
	minuteValue = (systmtime.tm_hour)*60 + systmtime.tm_min;

	if(protocolstu.TakeMedicinePlanArray[index] != -1)	takeCnt++;

// change by zhang	
//	if(CurrentIndex != index)
//	{
//		CurrentIndex = index;
//		bUpdateRecord = 0;
//	}
	// add by zhang
	if(CurrentTime != protocolstu.TakeMedicinePlanArray[index])	// 不同的提醒时间
	{
		prevTime = CurrentTime;
		CurrentTime = protocolstu.TakeMedicinePlanArray[index];
		bUpdateRecord = 0;
		if(prevTime)  // 跳过第一次
		{
			prev_flag = 1;	
		}
			
	}
	
	
	if((prevTime != -1) && prev_flag == 1 && (minuteValue > protocolstu.TakeMedicinePlanArray[index] - 2)) // 下一次提醒前一分钟停止闪烁
	{
		prev_flag = 0;
		// no send data
//		find_id(prevTime);
//		bTaken = 3; // 未服药
//		PrepareTakeMedicineStu(index,prevTime,bTaken);
//		protocolstu.takemedicine.needsend = 1;		
		WarnCnt = 0;//清空提醒次数
		bWarn = 0;
		TIMER3(0);
		bUpdateRecord = 0;	
		if(validdateTime == validdate)
		{
			MediceineBox_BigLedFlash(0); // 停止闪烁			
		}
			
			
//		printf("\r\n	停止上一次，准备开始下一次.	\r\n");
	}
	
	if(validdateTime != validdate)
	{
		validdateTime = validdate;
	}
	
	if(SysStart == 1 && protocolstu.GetPlan == 1)
	{
		SysStart = 0;
		if(minuteValue > protocolstu.TakeMedicinePlanArray[index]) bUpdateRecord = 1;
	}
	
	switch(takeCnt)
	{
		case 1:
		{
			if((minuteValue >= protocolstu.TakeMedicinePlanArray[index]) && ((bUpdateRecord&0x01) == 0))
			{
				if(bWarn == 0) //尚未提醒，进行亮灯提醒
				{
					for(i = 0; i < 28;i++)
					{
						if(i != index)
						{
							MedicineBoxStatus |= ((((u32)(Detect_TakeAndPutStatus(i)))&0x01)<<i);  //将其他小药盒的状态记录下来
						}
					}
					
					MediceineBox_BigLedFlash(1);
					Medicine_LED_CtrlByPosition(index,1);
					bWarn = 1;
					TIMER3(1);
//					RecyclingTime = 0;
					SetVoiceQueue(REMIND_MUSIC);  //吃药时间到
				}else //已经进行过第一次提醒
				{
					if(eventdelay.EventDelay[11] > 0x493e0) // 五分钟
					{
						WarnCnt++;
						if(WarnCnt == 3)
						{
//							bTaken = 2; // 未及时服药
//							PrepareTakeMedicineStu(index,protocolstu.TakeMedicinePlanArray[index],bTaken); //强制15分钟后上传一次未按时服药记录
//							protocolstu.takemedicine.needsend = 1;
						}
						
						if(WarnCnt == 6)	// 30 分钟后作最后一次提醒.
						{
//								MediceineBox_BigLedFlash(0); // 关大灯
								WarnCnt = 0;//清空提醒次数
								TIMER3(0);
					//			RecyclingTime = 0;
								find_id(protocolstu.TakeMedicinePlanArray[index]);
								bTaken = 3;
									// change by zhang  not send data 
//								PrepareTakeMedicineStu(index,protocolstu.TakeMedicinePlanArray[index],bTaken);
//								protocolstu.takemedicine.needsend = 1;
								bUpdateRecord |= bWarn;
								bWarn = 0;
						}
//						RecyclingTime = 0;
						eventdelay.EventDelay[11] = 0;
						SetVoiceQueue(REMIND_MUSIC);
//						printf("\r\n next_flag = %d	WarnCnt = %d index = %d	protocolstu.TakeMedicinePlanArray[index] = %d\r\n",prev_flag,WarnCnt,index,protocolstu.TakeMedicinePlanArray[index]);
					}
				}
				CurrentMedicineBoxDetection(index,protocolstu.TakeMedicinePlanArray[index],validdate);
			}
		}break;
		default: //没有提醒数据
		{
			MediceineBox_BigLedFlash(0);	// change by zhang 
			Medicine_LED_CtrlByPosition(index,0);
			bWarn = 0;
			WarnCnt = 0;//清空提醒次数
			TIMER3(0);
//			RecyclingTime = 0;
		}	
		break;
	}

}

/**
*	@brief 检查当前时段的小药盒是否被拿起
*	@param index 药盒编号
* @param alerttm 设置的报警时间
* @param 
* @return 
**/
void CurrentMedicineBoxDetection(u8 index,short alerttm,short validdate)
{
	short minuteValue = 0;
	u8 i = 0,dt = 0;
	minuteValue = (systmtime.tm_hour)*60 + systmtime.tm_min;
	
	if(minuteValue >= validdate-2)
	{
//		MediceineBox_BigLedFlash(0);
		WarnCnt = 0;//清空提醒次数
		TIMER3(0);
//		RecyclingTime = 0;
		bTaken = 0;
		PrepareTakeMedicineStu(index,alerttm,bTaken);
		protocolstu.takemedicine.needsend = 1;
		bUpdateRecord |= bWarn;
		bWarn = 0;
	}else
	{
		for(i = 0; i < 28;i++)
		{
			if(i != index)
			{
				dt = Detect_TakeAndPutStatus(i);
				if(dt^(MedicineBoxStatus>>i&0x01))  //如果状态不同，则拿错了
				{
//					printf("error taked = %d,status = %d\r\n",i,Detect_TakeAndPutStatus(i));
					MedicineBoxStatus ^= (0x00000001<<i);
					if(dt==1)
					{
						SetVoiceQueue(MEDICINE_CHECK);
					}
					return;  //拿错了就返回
				}
			}
		}
		if(Detect_TakeAndPutStatus(index) == 1)
		{
			find_id(protocolstu.TakeMedicinePlanArray[index]);
//			printf("\r\n	及时服药	\r\n");
//			printf("\r\n	index = %d id = %d alerttm = %d validete = %d protocolstu.TakeMedicinePlanArray[index]) = %d",index,protocolstu.id, alerttm,validdate,protocolstu.TakeMedicinePlanArray[index]);
			MediceineBox_BigLedFlash(0);
			Medicine_LED_CtrlByPosition(index,0);
			SetVoiceQueue(BOX_TAKED);  //祝您早日康复
			WarnCnt = 0; //清空提醒次数
			TIMER3(0);
//			RecyclingTime = 0;
			bTaken = 1;
			PrepareTakeMedicineStu(index,alerttm,bTaken);
			protocolstu.takemedicine.needsend = 1;
			bTaken = 0;
			bUpdateRecord |= bWarn;
			bWarn = 0;
		}

		if(WarnCnt > 5)  //半小时后提醒未服药
		{
			#if 0	// change by zhang
			MediceineBox_BigLedFlash(0);
			WarnCnt = 0;//清空提醒次数
			TIMER3(0);
//			RecyclingTime = 0;
			bTaken = 3;
			PrepareTakeMedicineStu(index,alerttm,bTaken);
			protocolstu.takemedicine.needsend = 1;
			bUpdateRecord |= bWarn;
			bWarn = 0;
			#endif 
		}
  }
}

/**
*	@brief 电池低电量闪烁显示
*	@param 
* @param         
* @param 
* @return 
**/
void BatteryStatusFlash(void)
{
	eventdelay.EventSet |= 0x02;  //开启两个定时器，两个定时器都是常开状态
	if(eventdelay.EventDelay[1] > 0)
	{
		if((((eventdelay.EventDelay[1])/3000)&0x01) == 1)
		{
//			BATTERY_LED(1);
		}
		if((((eventdelay.EventDelay[1])/3000)&0x01) == 0)
		{
//			BATTERY_LED(0);
		}
		if(eventdelay.EventDelay[1] > 30000) 
		{
			eventdelay.EventDelay[1] = 0;
//			SetVoiceQueue(LOW_POWER);
		}
	}
}

/**
*	@brief 查询药盒状态，并提醒放回和清理亮灯
*	@param 
* @param 
* @param 
* @return 
**/
void DetectionBoxStatus(u8 index)
{
	u8 i = 0;
	for(i = 0; i < 28;i++)
	{
		{
			if(Detect_TakeAndPutStatus(i) == 1)
			{
				if((bWarn == 0) && (i == index))
				{
					MediceineBox_BigLedFlash(0);
				}
				Medicine_LED_CtrlByPosition(i,0);
			}
		}
	}
}


/**
*	@brief 周日晚上23:59分会清理所有灯光提醒
*	@param 
* @param 
* @param 
* @return 
**/
void ClearAllLedWarn(void)
{
	u8 i = 0;
	
	if((systmtime.tm_wday == 0) && (systmtime.tm_hour == 23) && (systmtime.tm_min == 59) && (systmtime.tm_sec == 40))
	{
		MediceineBox_BigLedFlash(0);
//		printf("close led in clearallledwarn\r\n");
		for(i = 0; i < 28;i++)
		{
			Medicine_LED_CtrlByPosition(i,0);
		}
	}
}


/**
*	@brief 轮询是否发送HTTPS请求
*	@param 
* @param 
* @param 
* @return 
**/
// u8 bNeedGetTMPlan;  //1 强制 2 获取更新		
// u8 bNeedAuthorize;  //1 完整授权 2更新授权
// u8 GetAuthorizeStep;  //药盒获取授权令牌步骤，1 发送请求授权；2 重发请求授权，如果服务器授权后，获得code；3 使用2步骤获得的code传送POST请求，获取access_token和refresh_token 4 使用refresh_token获取access_token
void PollsSendHttpsRequest(void)
{
	
#if CHANGE
	if((protocolstu.HttpsLocker == 1)&&(eventdelay.EventDelay[4] > 0x4E20))  //延时20秒，如果20秒未收到完整回应，则重发
	{
		
		protocolstu.HttpsLocker = 0;
		TCP_Close();
		protocolstu.GetServerResponsesFlag = 0;
		if(protocolstu.GetAuthorizeStep == 4) protocolstu.bNeedAuthorize = 2;
		if(protocolstu.GetAuthorizeStep > 0 && protocolstu.GetAuthorizeStep < 4) protocolstu.bNeedAuthorize = 1;
		if(protocolstu.AgreeBandingStep)
		{
			protocolstu.AgreeBandingStep = 0;
			protocolstu.bNeedSendATCmd = 1;
		}
		if(protocolstu.GetUtcTimeStep == 1)
		{
			protocolstu.GetUtcTimeStep = 0;
			protocolstu.bNeedGetUTC = 1;
		}
		if(protocolstu.GetTakeMedicinePlanStep)
		{
			protocolstu.bNeedGetTMPlan = protocolstu.GetTakeMedicinePlanStep;
			protocolstu.GetTakeMedicinePlanStep = 0;
		}
		if(protocolstu.SendTakeMedicineCaseStep)
		{
			protocolstu.takemedicine.needsend = protocolstu.SendTakeMedicineCaseStep;
			protocolstu.SendTakeMedicineCaseStep = 0;
		}
	}
	
	if(protocolstu.bNeedAuthorize == 1)
		StartRequestAuthorize();
	if(protocolstu.bNeedAuthorize == 2)
		RefreshAccessToken();
	if(protocolstu.bNeedGetUTC)
		StartGetUTCTime();
//	if(protocolstu.bNeedSendATCmd)
//		StartAgreeBanding();
	if(protocolstu.takemedicine.needsend)
		StartSendTakeMedicineSitu();
	if(protocolstu.bNeedGetTMPlan)
		StartGetTakeMedicinePlan(protocolstu.bNeedGetTMPlan-1);
	if(protocolstu.nNeedATKeyCmd == 1)
		reconfig_wifi();
	if(protocolstu.bNeedGetVoice)
		StartGetVoice();
	
	
	
#else
	if((protocolstu.HttpsLocker == 1)&&(eventdelay.EventDelay[4] > 0x4E20))  //延时20秒，如果20秒未收到完整回应，则重发
	{	
		protocolstu.HttpsLocker = 0;
		TCP_Close();
		protocolstu.GetServerResponsesFlag = 0;
		if(protocolstu.GetAuthorizeStep == 4) protocolstu.bNeedAuthorize = 2;
		if(protocolstu.GetAuthorizeStep > 0 && protocolstu.GetAuthorizeStep < 4) protocolstu.bNeedAuthorize = 1;
		if(protocolstu.AgreeBandingStep)
		{
			protocolstu.AgreeBandingStep = 0;
			protocolstu.bNeedSendATCmd = 1;
		}
		if(protocolstu.GetUtcTimeStep == 1)
		{
			protocolstu.GetUtcTimeStep = 0;
			protocolstu.bNeedGetUTC = 1;
		}
		if(protocolstu.GetTakeMedicinePlanStep)
		{
			protocolstu.bNeedGetTMPlan = protocolstu.GetTakeMedicinePlanStep;
			protocolstu.GetTakeMedicinePlanStep = 0;
		}
		if(protocolstu.SendTakeMedicineCaseStep)
		{
			protocolstu.takemedicine.needsend = protocolstu.SendTakeMedicineCaseStep;
			protocolstu.SendTakeMedicineCaseStep = 0;
		}
//		if(protocolstu.SendSimInfo)
//		{
//			protocolstu.bNeedSendSimInfo = 1;
//			protocolstu.SendSimInfo = 0;
//		}
	}

	
	if(protocolstu.bNeedAuthorize == 1)
		StartRequestAuthorize();
	if(protocolstu.bNeedAuthorize == 2)
		RefreshAccessToken(); // protocolstu.GetAuthorizeStep = 4;
	if(protocolstu.bNeedGetUTC)
		StartGetUTCTime();
	// change by zhang
//	if(protocolstu.bNeedSendATCmd)        StartAgreeBanding();
	if(protocolstu.takemedicine.needsend)
		StartSendTakeMedicineSitu();
	if(protocolstu.bNeedGetTMPlan)
		StartGetTakeMedicinePlan(protocolstu.bNeedGetTMPlan-1);
//	if(protocolstu.bNeedSendSimInfo)			StartSendSimInfo();
	
#endif
}




//****************************END OF FILE***********************************
