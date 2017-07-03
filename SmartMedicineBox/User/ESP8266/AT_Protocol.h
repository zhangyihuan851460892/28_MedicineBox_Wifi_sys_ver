#ifndef __AT_PROTOCOL_H__
#define __AT_PROTOCOL_H__

#include "stm32f10x.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bsp_date.h"
#include "bsp_usart.h"
#include "bsp_TiMbase.h"
#include "bsp_sc5080.h"
#include "define.h"
#include "bsp_rtc.h"
#include "bsp_i2c_ee.h"
#include "bsp_esp8266.h"


#define HOME_PAGE "caijiyun.cn"
#define HOME_API 	"api.caijiyun.cn"


#define CHANGE	1
#define NEW_EEPROM 0

#define WIFI_POWER 1  // wifi 是否长连.


extern ProtocolStu protocolstu;
//服药5分钟循环提醒 0x493e0
//extern __IO int RecyclingTime;
extern int HttpDataLength;
extern u8 WIFI_Ready;

u8 ParseOneFrameCustomCmd(u8 *pdata);
extern CustomCmdStu cst;
u8 ParseOneFrameATCmd(u8 *pdata);
int ParseServerResponses(u8 *pdata);
AT_Flag GetHeadType(void);
int PrepareATCmd(AT_Cmd cmd,char *strCmd);

void TCP_Close(void);
void WIFI_Status_Check(void);

//授权
void StartRequestAuthorize(void);
int RequestAuthorize(char *data,u8 type);
void ParseGetServerAuthorizeJson(char *pMsg);
void GetServerAuthorizeCode(AT_Flag responses);
void RefreshAccessToken(void);

//解析绑定手机APP的命令
void StartAgreeBanding(void);
void GetAgreeBandingCmd(AT_Flag responses);
//确认绑定手机APP
int BindingConfirm(char *data);

//获取服务器UTC时钟
void StartGetUTCTime(void);
void GetRegulateTimeCmd(AT_Flag responses);
int GetRtcValue(char *data);
void ParseGetServerRtcTimeJson(char *pMsg);

//获取服药管理计划的AT 命令
void StartGetTakeMedicinePlan(u8 type);
void GetTakeMedicinePlanCmd(AT_Flag responses,u8 type);
int GetTakeMedicinePlan(char *data,u8 type);
//解析获取服药管理计划
void ParseGetTakeMedicinePlanJson(char *pMsg);
short ParseTimeString(const char *str,u8 *hours,u8 *minutes);
void ParseTakeMedicinePlanString(const char *str,u8 index);
void ParseTakeMedicinePlanScopeString(const char *str,char *week);

//上传服药情况
void StartSendTakeMedicineSitu(void);
void SendTakeMedicineSituationCmd(AT_Flag responses);
int SendTakeMedicineSituation(char *data);
void PrepareTakeMedicineStu(u8 index,short alerttime,u8 btaken);

//保存数据
void SaveTokenCode(void);
void ReadTokenCode(void);
//void SaveDeviceParam(void);
//void ReadDeviceParam(void);
void SaveTMPlan(void);
void ReadTMPlan(void);

// 获取声音
void StartGetVoice(void);
void GetRegulateVoiceCmd(AT_Flag responses);
int GetVoiceValue(char *data);
void ParseGetServerVoiceJson(char *pMsg);



#endif
