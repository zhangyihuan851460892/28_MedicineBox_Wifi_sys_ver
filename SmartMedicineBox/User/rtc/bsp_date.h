/******************** (C) COPYRIGHT 2009 www.armjishu.com ************************
* File Name          : date.h
* Author             : www.armjishu.com Team
* Version            : V1.0
* Date               : 12/1/2009
* Description        : 日期相关函数
*******************************************************************************/
#ifndef __DATE_H
#define __DATE_H

#include "stm32f10x.h"
#include "define.h"

    
void GregorianDay(rtc_time * tm);
uint32_t mktimev(rtc_time *tm);
void to_tm(uint32_t tim, rtc_time * tm);
#endif 
