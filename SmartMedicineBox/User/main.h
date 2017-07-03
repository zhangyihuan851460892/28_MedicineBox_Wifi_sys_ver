#ifndef __MAIN_H__
#define __MAIN_H__


#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "bsp_usart.h"
#include "bsp_rtc.h"
#include "bsp_TiMbase.h"
#include "bsp_sc5080.h"
#include "bsp_led.h"
#include "bsp_i2c_ee.h"
#include "bsp_esp8266.h"
#include "bsp_TiMbase.h"
#include "AT_Protocol.h"
#include "bsp_adc.h"
#include "bsp_HT1621B.h"
#include "define.h"
#include "DeviceInit.h"
#include "bsp_isd1700.h"
#include "stm32f10x_iwdg.h"
#include "MedicineBoxOperation.h"
#include "24cxx.h" 







/**
eventdelay 0 -- 作为AT_KEY的定时器
					 1 -- 作为电源欠压闪烁LED的定时器
					 2 -- 作为语音播报的定时器
					 3 -- 作为接收JSON数据的延时定时器
					 4 -- 发送HTTPS数据超时计时（3分钟0x2BF20）
					 5 -- 录音延时
					 6 -- 录音删除
					 7 -- 药盒外部LED大灯闪烁定时器
					 8 -- wifi信号灯闪烁
					 9 -- 延时关闭WIFI模块
					 10 - 断网重连定时器
					 11 - 重复5分钟服药提醒
					 12 - Smartlink 连接指示灯闪烁
					 13 - check wifi status timer
					 14 - 时间校准
**/


									
#endif
