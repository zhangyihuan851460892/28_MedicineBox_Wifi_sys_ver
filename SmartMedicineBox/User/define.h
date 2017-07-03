#ifndef __DEFINE_H__
#define __DEFINE_H__

#include "stm32f10x.h"

#define __XB_DEBUG
#define DEBUG_COMM 1  //1 COMM2  2 COMM4

#define HIGH 	1
#define LOW		0

#define ON 1
#define OFF 0


//#ifdef CHINESE
////语音IC播放地址
//#define BOX_TAKED						1
//#define LOW_POWER						7
//#define CONNECT_SUCCESS			3
//#define CONNECT_WAIT				5
//#define	MEDICINE_CHECK			9  //改成了取药错误
//#define	REMIND_MUSIC				11
//#else
//#define BOX_TAKED						2
//#define LOW_POWER						8
//#define CONNECT_SUCCESS			4
//#define CONNECT_WAIT				6
//#define	MEDICINE_CHECK			10  //改成了取药错误
//#define	REMIND_MUSIC				11
//#endif


//#define CHINESE 

//#ifdef CHINESE
////语音IC播放地址
////#define BOX_TAKED						1
////#define LOW_POWER						7
////#define CONNECT_SUCCESS			3
////#define CONNECT_WAIT				5
////#define	MEDICINE_CHECK			9  //改成了取药错误
////#define	REMIND_MUSIC				11
////老语音芯片的地址
//#define BOX_TAKED						5
//#define LOW_POWER						11
//#define CONNECT_SUCCESS			0
//#define CONNECT_WAIT				0
//#define	MEDICINE_CHECK			0  //改成了取药错误
//#define	REMIND_MUSIC				14  
//#else
//#define BOX_TAKED						2
//#define LOW_POWER						8
//#define CONNECT_SUCCESS			4
//#define CONNECT_WAIT				6
//#define	MEDICINE_CHECK			10  //改成了取药错误
//#define	REMIND_MUSIC				11
//#endif

//#define chinese

#ifdef chinese							// 中文
#define BOX_TAKED						1
#define LOW_POWER						7
#define CONNECT_SUCCESS			3
#define CONNECT_WAIT				5
#define	MEDICINE_CHECK			9  //改成了取药错误
#define	REMIND_MUSIC				11

#else												// english
#define BOX_TAKED						2
#define LOW_POWER						8
#define CONNECT_SUCCESS			4
#define CONNECT_WAIT				6
#define	MEDICINE_CHECK			10  //改成了取药错误
#define	REMIND_MUSIC				11

#endif


#define CLI()      	__set_PRIMASK(1)				/* 关闭总中断 */  
#define SEI() 			__set_PRIMASK(0)				/* 开放总中断 */ 

//定义事件延时结构体，支持最大32个独立事件延时
typedef struct{
	u32	EventSet;
	u32	EventDelay[32];  //单位ms
}EventDelay,*p_EventDelay;


typedef struct{
	u8 frmHeader;
	u8 frmData[8];
	u8 frmLen;
}CustomCmdStu,*p_CustomCmdStu;

typedef struct{
	u8 VoiceArray[20];
	int VoiceOrder;   //不能大于19，最多20条语音队列
	u8 VoiceLevel;
}VoiceQueue,*p_VoiceQueue;



typedef struct{
	GPIO_TypeDef* GPIOx;
	uint16_t			GPIO_Pin;
}GPIOInfo;


//RTC时钟结构体
typedef struct {
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
	int tm_wday;
}rtc_time;

//at指令反馈数据结构体
typedef struct{
	u8 RxBuf[64];
	u8 RxBuf_In;
}AT_Frame,*pAT_Frame;

typedef struct{
	int ResponseLen;	//	response length
	u8	ResponseBuf[1024];	//response buffer
	u8	ResponseHead;			//response head
}Server_Response,*p_Server_Response;

typedef enum{
	ATDATA = 0,
	ATOK,
	ATERR,
	ATFAIL,
	CONNECTOK,
	CONNECTERR,
	CONNECTFAIL,
	ATREADY,
	SENDOK,
	RESPONSESUCCESS,
	SENDFAIL,
	TCPCLOSE,
	WIFIGOTIP,
	WIFIDISCN,
	SMART_SUCCESS,
	ATDFT,
	WIFIRESTORE
}AT_Flag;



typedef enum{
	AT = 0,
	UARTSET,
	TCPON,
	SEND,
	TCPOFF,
	ICCID,
	RST,
	CWMODE,
	SMARTLINK,
	SMARTLINKSTOP,
	RESTORE,
	JAPSTATUS
}AT_Cmd;

//服药情况数据结构体
typedef struct{
	u8		needsend;  //需要发送
	char 	takemedicineTime[20];
	u8		mboxIndex;
	char	alertTime[6];
	u8		bTaken;
}TakeMedicineStu,*p_TakeMedicineStu;


typedef struct{
	char week[7];
	int id; // 服药设定id
	short time;// 服药设定 time
}AppSetWarn,*p_AppSetWarn;


typedef struct{
	char code_token[65];
	char access_token[65];
	char refresh_token[65];
	u32  TokenRefreshTimeValue;

	//设备ID
	uint32_t Device_ID;
	//设备是否已经有绑定手机APP
	u8 bBanding;
	//设备是否授权
	u8 bAuthorize;
	//HTTPS发送锁
	u8 HttpsLocker;

	u8 GetPlan;
	//*********************************************************************************************
	u8 GetAuthorizeStep;  //药盒获取授权令牌步骤，1 发送请求授权；2 重发请求授权，如果服务器授权后，获得code；3 使用2步骤获得的code传送POST请求，获取access_token和refresh_token 4 使用refresh_token获取access_token
	u8 AgreeBandingStep;  //同意绑定手机APP
	u8 GetTakeMedicinePlanStep;  //获取服药管理计划 1 强制更新整个计划 2 只更新修改的计划
	u8 GetUtcTimeStep;			//获取UTC时间
	u8 GetVoiceStep;				// 获取声音
	u8 SendTakeMedicineCaseStep;			//上传服药情况

	//*********************************************************************************************
	//服药情况上传结构体
	TakeMedicineStu takemedicine;
	
	//是否进行授权或者更新授权
	u8 bNeedAuthorize;  //1 完整授权 2更新授权
	//是否进行获取服药计划
	u8 bNeedGetTMPlan;  //1 强制 2 获取更新
	//是否进行获取RTC时间
	u8 bNeedGetUTC;
	//是否需要进行AT按键发送
	u8 bNeedSendATCmd;
	// 是否进行获取声音
	u8 bNeedGetVoice;
	
	// AT key to RESTORE wifi
	u8 nNeedATKeyCmd;    // 

	
	//各个GPRS请求位标志
	/*
		bit0~bit1  	00 无请求 01完整授权 10 更新授权
		bit2~bit3  	00 无请求 01强制获取 10 更新获取
		bit4				 0 无请求  1获取RTC时间
		bit5				 0 无请求  1发送AT按键请求
		bit6				 0 无请求  1发送SIM数据信息
		bit7				 保留
*/
//	u8 GPRSRequestBit;  
	//*********************************************************************************************
	//一周服药计划
	short TakeMedicinePlanArray[28];  //one week take medicine plan array

	short TakeMedicineTimeArray[256]; // all take medicine time array
	int id; // 服药设定id
	u8 isize;
	AppSetWarn AppTakeWarn[36]; // set item
	
	//串口收到的M35模块的AT指令数据
	AT_Frame ATData;
	//串口收到的服务器JSON数据
	Server_Response ServerResponse;
	u8 GetServerResponsesFlag;  //1 开启接收串口来的服务器响应信息，处理完JSON后，切换会接收AT命令解析=0
}ProtocolStu,*p_ProtocolStu;

#endif

//**************************************END OF FILE*****************************************

