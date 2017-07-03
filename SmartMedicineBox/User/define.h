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
////����IC���ŵ�ַ
//#define BOX_TAKED						1
//#define LOW_POWER						7
//#define CONNECT_SUCCESS			3
//#define CONNECT_WAIT				5
//#define	MEDICINE_CHECK			9  //�ĳ���ȡҩ����
//#define	REMIND_MUSIC				11
//#else
//#define BOX_TAKED						2
//#define LOW_POWER						8
//#define CONNECT_SUCCESS			4
//#define CONNECT_WAIT				6
//#define	MEDICINE_CHECK			10  //�ĳ���ȡҩ����
//#define	REMIND_MUSIC				11
//#endif


//#define CHINESE 

//#ifdef CHINESE
////����IC���ŵ�ַ
////#define BOX_TAKED						1
////#define LOW_POWER						7
////#define CONNECT_SUCCESS			3
////#define CONNECT_WAIT				5
////#define	MEDICINE_CHECK			9  //�ĳ���ȡҩ����
////#define	REMIND_MUSIC				11
////������оƬ�ĵ�ַ
//#define BOX_TAKED						5
//#define LOW_POWER						11
//#define CONNECT_SUCCESS			0
//#define CONNECT_WAIT				0
//#define	MEDICINE_CHECK			0  //�ĳ���ȡҩ����
//#define	REMIND_MUSIC				14  
//#else
//#define BOX_TAKED						2
//#define LOW_POWER						8
//#define CONNECT_SUCCESS			4
//#define CONNECT_WAIT				6
//#define	MEDICINE_CHECK			10  //�ĳ���ȡҩ����
//#define	REMIND_MUSIC				11
//#endif

//#define chinese

#ifdef chinese							// ����
#define BOX_TAKED						1
#define LOW_POWER						7
#define CONNECT_SUCCESS			3
#define CONNECT_WAIT				5
#define	MEDICINE_CHECK			9  //�ĳ���ȡҩ����
#define	REMIND_MUSIC				11

#else												// english
#define BOX_TAKED						2
#define LOW_POWER						8
#define CONNECT_SUCCESS			4
#define CONNECT_WAIT				6
#define	MEDICINE_CHECK			10  //�ĳ���ȡҩ����
#define	REMIND_MUSIC				11

#endif


#define CLI()      	__set_PRIMASK(1)				/* �ر����ж� */  
#define SEI() 			__set_PRIMASK(0)				/* �������ж� */ 

//�����¼���ʱ�ṹ�壬֧�����32�������¼���ʱ
typedef struct{
	u32	EventSet;
	u32	EventDelay[32];  //��λms
}EventDelay,*p_EventDelay;


typedef struct{
	u8 frmHeader;
	u8 frmData[8];
	u8 frmLen;
}CustomCmdStu,*p_CustomCmdStu;

typedef struct{
	u8 VoiceArray[20];
	int VoiceOrder;   //���ܴ���19�����20����������
	u8 VoiceLevel;
}VoiceQueue,*p_VoiceQueue;



typedef struct{
	GPIO_TypeDef* GPIOx;
	uint16_t			GPIO_Pin;
}GPIOInfo;


//RTCʱ�ӽṹ��
typedef struct {
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
	int tm_wday;
}rtc_time;

//atָ������ݽṹ��
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

//��ҩ������ݽṹ��
typedef struct{
	u8		needsend;  //��Ҫ����
	char 	takemedicineTime[20];
	u8		mboxIndex;
	char	alertTime[6];
	u8		bTaken;
}TakeMedicineStu,*p_TakeMedicineStu;


typedef struct{
	char week[7];
	int id; // ��ҩ�趨id
	short time;// ��ҩ�趨 time
}AppSetWarn,*p_AppSetWarn;


typedef struct{
	char code_token[65];
	char access_token[65];
	char refresh_token[65];
	u32  TokenRefreshTimeValue;

	//�豸ID
	uint32_t Device_ID;
	//�豸�Ƿ��Ѿ��а��ֻ�APP
	u8 bBanding;
	//�豸�Ƿ���Ȩ
	u8 bAuthorize;
	//HTTPS������
	u8 HttpsLocker;

	u8 GetPlan;
	//*********************************************************************************************
	u8 GetAuthorizeStep;  //ҩ�л�ȡ��Ȩ���Ʋ��裬1 ����������Ȩ��2 �ط�������Ȩ�������������Ȩ�󣬻��code��3 ʹ��2�����õ�code����POST���󣬻�ȡaccess_token��refresh_token 4 ʹ��refresh_token��ȡaccess_token
	u8 AgreeBandingStep;  //ͬ����ֻ�APP
	u8 GetTakeMedicinePlanStep;  //��ȡ��ҩ����ƻ� 1 ǿ�Ƹ��������ƻ� 2 ֻ�����޸ĵļƻ�
	u8 GetUtcTimeStep;			//��ȡUTCʱ��
	u8 GetVoiceStep;				// ��ȡ����
	u8 SendTakeMedicineCaseStep;			//�ϴ���ҩ���

	//*********************************************************************************************
	//��ҩ����ϴ��ṹ��
	TakeMedicineStu takemedicine;
	
	//�Ƿ������Ȩ���߸�����Ȩ
	u8 bNeedAuthorize;  //1 ������Ȩ 2������Ȩ
	//�Ƿ���л�ȡ��ҩ�ƻ�
	u8 bNeedGetTMPlan;  //1 ǿ�� 2 ��ȡ����
	//�Ƿ���л�ȡRTCʱ��
	u8 bNeedGetUTC;
	//�Ƿ���Ҫ����AT��������
	u8 bNeedSendATCmd;
	// �Ƿ���л�ȡ����
	u8 bNeedGetVoice;
	
	// AT key to RESTORE wifi
	u8 nNeedATKeyCmd;    // 

	
	//����GPRS����λ��־
	/*
		bit0~bit1  	00 ������ 01������Ȩ 10 ������Ȩ
		bit2~bit3  	00 ������ 01ǿ�ƻ�ȡ 10 ���»�ȡ
		bit4				 0 ������  1��ȡRTCʱ��
		bit5				 0 ������  1����AT��������
		bit6				 0 ������  1����SIM������Ϣ
		bit7				 ����
*/
//	u8 GPRSRequestBit;  
	//*********************************************************************************************
	//һ�ܷ�ҩ�ƻ�
	short TakeMedicinePlanArray[28];  //one week take medicine plan array

	short TakeMedicineTimeArray[256]; // all take medicine time array
	int id; // ��ҩ�趨id
	u8 isize;
	AppSetWarn AppTakeWarn[36]; // set item
	
	//�����յ���M35ģ���ATָ������
	AT_Frame ATData;
	//�����յ��ķ�����JSON����
	Server_Response ServerResponse;
	u8 GetServerResponsesFlag;  //1 �������մ������ķ�������Ӧ��Ϣ��������JSON���л������AT�������=0
}ProtocolStu,*p_ProtocolStu;

#endif

//**************************************END OF FILE*****************************************

