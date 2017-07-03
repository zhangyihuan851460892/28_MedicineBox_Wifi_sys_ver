#ifndef TIME_TEST_H
#define TIME_TEST_H

#include "stm32f10x.h"

void TIM2_NVIC_Configuration(void);
void TIM2_Configuration(void);
void TIM3_NVIC_Configuration(void);
void TIM3_Configuration(void);

void DelayTickInit(void);
void Delay_us(__IO u32 nTime);         // ��λ1us
#define Delay_ms(x) Delay_us(1000*x)	 //��λms



void myDelay_us(u32 nTime);								// kernel us
#define myDelay_ms(x) myDelay_us(1000*x)  // kernel ms
#endif	/* TIME_TEST_H */
