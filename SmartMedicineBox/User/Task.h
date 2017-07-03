#ifndef __TASK_H__
#define __TASK_H__


#include "main.h"
#include "cmsis_os.h"
#include "common.h"

/*时间结构体*/
extern rtc_time systmtime;

void ProgramFlashTask(void const *argument);
void ESP8266ResetCtrlTask(void const *argument);
void LcdDisplayTimeAndVoiceCtrlTask(void const *argument);
void RemindTask(void const *argument);
void Device_InitTask(void const *argument);
void IdleTask(void const *argument);


#endif
