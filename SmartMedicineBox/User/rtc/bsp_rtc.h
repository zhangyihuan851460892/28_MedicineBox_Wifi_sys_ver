

#ifndef __RTC_H
#define	__RTC_H


#include "stm32f10x.h"
#include "bsp_calendar.h"
#include "bsp_date.h"
#include "bsp_usart.h"
#include "bsp_sc5080.h"


/* 如果定义了下面这个宏的话,PC13就会输出频率为RTC Clock/64的时钟 */   
//#define RTCClockOutput_Enable  /* RTC Clock/64 is output on tamper pin(PC.13) */

void RTC_NVIC_Config(void);
void RTC_Configuration(void);
void Time_Regulate(rtc_time *tm);
void Time_Adjust(rtc_time *tm);
void Time_Display(uint32_t TimeVar,rtc_time *tm);
void Time_Show(rtc_time *tm);
static uint8_t USART_Scanf(uint32_t value);
void RTC_CheckAndConfig(rtc_time *tm);

void Time_RegulateByInternet(u32 timeVar);

#endif /* __XXX_H */
