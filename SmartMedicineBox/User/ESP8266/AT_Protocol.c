/**
  ******************************************************************************
  * @file    AT_Protocol.c
  * @author  xiaobing19851019@163.com
  * @version V1.0
  * @date    2013.09.01
  * @brief   AT协议解析
  ******************************************************************************
  * @attention
  *
  * 平台		:STM32F103ZET6 
  * 公司    :http://www.jjc12320.com
  * 
  *
  ******************************************************************************
  */ 

#include "AT_Protocol.h"
#include "24cxx.h"
#include "cJSON.h"

// TEST  0 使用正式服务器
//       1 使用测试服务器
#define TEST	1

#if TEST
#define URL_HOST_NAME		"ygj-test.caijiyun.cn" //"http://ygj-test.caijiyun.cn"
#else
#define URL_HOST_NAME		"ygj.caijiyun.cn" //"http://ygj.caijiyun.cn"
#endif

#define URL_SCOPE					"yaojide_device_read+yaojide_device_write"  //scope=yaojide_device_read+yaojide_device_write
#define DEF_BASIC_TOKEN		"MWRiZWE2YWQ2ODMxMTg2MzBkY2EyZjc2MWJkZjEzOGM1MzE2Y2JiZWI1M2U2ZGNlNTc0MzY5NzhhZGFiMWQ4ZTowODNjYzMxMzIyOWE3YWRhNDk3YjUxNTZiN2E0ZmFmYTBlNTEyMTE3MTkzMzA1NjAyY2JlOWJkZWQwMTg3NmU2"
#define URL_TIME_SYNC		"/v1/devices/get_time?device_uid="
#define URL_AUTH_TOKEN		"/oauth/token"
#define URL_UPLOAD_ICCID	"/v1/devices/sim"
#define URL_UPLOAD_BATS		"/v1/devices/electric_count"
#define URL_GET_SETTING		"/v1/devices/get_setting"
#define URL_UPLOAD_RECORD	"/v1/devices/medicine"
#define URL_SYNC_VOLUME		"/v1/devices/volume"
#define DEF_USER_NAME		"device_admin"
#define DEF_PASSWORD		"123456"




u8 WIFI_Ready = 0;  //wifi 准备就绪
ProtocolStu protocolstu;

CustomCmdStu cst;

const char str_application[] = "f820a15c14c950f2a9821f2c93c1ba9a9f58de8010bf25375048eaee8d89659c";  //设备程序代码
const char str_RequestAuthorize[] = "https://caijiyun.cn/oauth/limited_authorize?"; //请求授权的https地址
const char str_HttpsCallback[] = "redirect_uri=https%3a%2f%2fcaijiyun.cn%2fcallback"; //HTTPS的回转地址
const char str_Scope[] = "scope=flb_device_write%20flb_device_read"; //属性，包括读和写
const char str_Base64[] = "Basic ZjgyMGExNWMxNGM5NTBmMmE5ODIxZjJjOTNjMWJhOWE5ZjU4ZGU4MDEwYmYyNTM3NTA0OGVhZWU4ZDg5NjU5YzpjZmI1M2FiMDMxNTJmZjUzZTk5ZWE3M2RjMDgwYWI5ZDg4YWQwMmI0ZTIwYzFjYTBlNzg5YmNjNjI5OGUyOTRh";  //Basic [apikey:apiSecret的Base64编码]





int HttpDataLength = 0;

//系统时间
extern rtc_time systmtime;


//解析AT指令反馈信息
u8 ParseOneFrameATCmd(u8 *pdata)
{
	u8 dat = 0;
	u8 oneframe = 0;
	dat = *pdata;
	
	if((dat == '\n')||(dat == '\r'))
	{
		if(protocolstu.ATData.RxBuf_In > 1)
			oneframe = protocolstu.ATData.RxBuf_In;
		protocolstu.ATData.RxBuf[protocolstu.ATData.RxBuf_In] = 0;
		protocolstu.ATData.RxBuf_In = 0;
	}else
	{
		if(protocolstu.ATData.RxBuf_In < 63)
		{
			protocolstu.ATData.RxBuf[protocolstu.ATData.RxBuf_In++] = dat;
			if(dat == 0x20 && protocolstu.ATData.RxBuf[0] == 0x3e) 
			{
				oneframe = protocolstu.ATData.RxBuf_In;
				protocolstu.ATData.RxBuf[protocolstu.ATData.RxBuf_In] = 0;
				protocolstu.ATData.RxBuf_In = 0;
			}
		}else protocolstu.ATData.RxBuf_In = 0; 
	}
		
	return oneframe;
}

//解析自定义指令反馈信息
u8 ParseOneFrameCustomCmd(u8 *pdata)
{
	u8 dat = 0;
	u8 oneframe = 0;
	dat = *pdata;
	
	if((dat == '$')&&(cst.frmHeader == 0))
	{
		cst.frmHeader = 1;
		cst.frmLen = 0;
		cst.frmData[cst.frmLen++] = dat;
	}else
	{
		if(cst.frmLen < 5)
		{
			cst.frmData[cst.frmLen++] = dat;
			if(dat == '@') 
			{
				oneframe = cst.frmLen;
				cst.frmLen = 0;
				cst.frmHeader = 0;
			}
		}else cst.frmLen = 0; 
	}
		
	return oneframe;
}


/**	
	服务器的响应分两种类型，一种是出错，收到400响应，如下
SEND OK之后收到：
<!DOCTYPE HTML PUBLIC "-//IETF//DTD HTML 2.0//EN">
<html>
<head><title>400 Bad Request</title></head>
<body bgcolor="white">
<h1>400 Bad Request</h1>
<p>Your browser sent a request that this server could not understand. Sorry for the inconvenience.<br/>
Please report this message and include the following information to us.<br/>
Thank you very much!</p>
<table>
<tr>
<td>URL:</td>
<td>http://:50045</td>
</tr>
<tr>
<td>Server:</td>
<td>slba31s0-g1p1-proxy-st3.cloud.st3</td>
</tr>
<tr>
<td>Date:</td>
<td>2015/10/30 15:48:23</td>
</tr>
</table>
</body>
</html>

CLOSED
还有一种是成功的，收到JSON响应：
SEND OK之后收到：
{"message":"New limited authorization OK"}
CLOSED
因此，认为收到<符号，就是出错，收到JSON包才解析

*	@brief 向服务器完成请求后，等待服务器响应
*	@param pdata 串口接收数据指针
* @param 
* @param 
* @return 帧长度
*/
int ParseServerResponses(u8 *pdata)
{
	int oneframe = 0;

	if(((*pdata) == '{') && (protocolstu.ServerResponse.ResponseHead == 0))  //收到 { 和 <
	{
		protocolstu.ServerResponse.ResponseHead = 1;
		protocolstu.ServerResponse.ResponseLen	= 0;
		protocolstu.ServerResponse.ResponseBuf[(protocolstu.ServerResponse.ResponseLen)++] = *pdata;
		eventdelay.EventSet |= 0x08;  //发现JSON数据开头，开启定时器
		eventdelay.EventDelay[3] = 0;
	}else
	{
		if((protocolstu.ServerResponse.ResponseLen < 1023) && (protocolstu.ServerResponse.ResponseHead == 1))
			protocolstu.ServerResponse.ResponseBuf[(protocolstu.ServerResponse.ResponseLen)++] = *pdata;
	}

	return oneframe;
}
	






AT_Flag GetHeadType(void)
{
	if(strstr((const char*)protocolstu.ATData.RxBuf,"WIFI GOT IP") != NULL)
	{
		return WIFIGOTIP;
	}
	if(strstr((const char*)protocolstu.ATData.RxBuf,"SMART SUCCESS") != NULL)
	{
		return SMART_SUCCESS;
	}
	if(strstr((const char*)protocolstu.ATData.RxBuf,"OK") != NULL)
	{
		if(strstr((const char*)protocolstu.ATData.RxBuf,"SEND") != NULL)
			return SENDOK;
		return ATOK;
	}
	if(strstr((const char*)protocolstu.ATData.RxBuf,"CONNECT") != NULL)
	{
		if(strstr((const char*)protocolstu.ATData.RxBuf,"WIFI CONNECTED") != NULL)	return ATDFT;
		if(strstr((const char*)protocolstu.ATData.RxBuf,"WIFI DISCONNECT") != NULL)	return WIFIDISCN;
		return CONNECTOK;
	}
			
	if(strstr((const char*)protocolstu.ATData.RxBuf,"ERROR") != NULL)
	{
		return ATERR;
	}

	if(strstr((const char*)protocolstu.ATData.RxBuf,"CLOSED") != NULL)
		return TCPCLOSE;
	
		if(strstr((const char*)protocolstu.ATData.RxBuf,"RESTORE") != NULL)
		return WIFIRESTORE;
	
	if(strstr((const char*)protocolstu.ATData.RxBuf,"ALREADY CONNECTED") != NULL)
		return CONNECTOK;
	
	if(strstr((const char*)protocolstu.ATData.RxBuf,"No AP") != NULL)
		return WIFIDISCN;
	
	if(strstr((const char*)protocolstu.ATData.RxBuf,"no ip") != NULL)
	return WIFIDISCN;
	
	if(strstr((const char*)protocolstu.ATData.RxBuf,">") != NULL)	//发送数据符号 “>”
		return ATREADY;
	
	return ATDFT;
}

int PrepareATCmd(AT_Cmd cmd,char *strCmd)
{
	int len = 0;
	switch(cmd)
	{
		case AT:
			len = sprintf(strCmd,"AT\r\n");
			break;
		case UARTSET:
			len = sprintf(strCmd,"AT+UART_DEF=115200,8,1,0,0\r\n");
			break;
		case TCPON:
//			len = sprintf(strCmd,"AT+QIOPEN=\"TCP\",\"120.25.129.136\",80\r\n");
//			len = sprintf(strCmd,"AT+QIOPEN=\"TCP\",\"%s\",80\r\n",HOME_API);
#if CHANGE
			len = sprintf(strCmd,"AT+CIPSTART=\"TCP\",\"%s\",80\r\n",URL_HOST_NAME);

#else		
			len = sprintf(strCmd,"AT+CIPSTART=\"TCP\",\"%s\",80\r\n",HOME_API);
#endif
			break;
		case TCPOFF:
			len = sprintf(strCmd,"AT+CIPCLOSE\r\n");
			break;
		case SEND:
			len = sprintf(strCmd,"AT+CIPSEND=%d\r\n",HttpDataLength);
			break;
		case ICCID:
			len = sprintf(strCmd,"AT+QCCID\r\n");
			break;
		case RST:
			len = sprintf(strCmd,"AT+RST\r\n");
			break;
		case CWMODE:
			len = sprintf(strCmd,"AT+CWMODE_DEF=1\r\n");
			break;
		case SMARTLINK:
			len = sprintf(strCmd,"AT+CWSMARTSTART=1\r\n");
			break;
		case SMARTLINKSTOP:
			len = sprintf(strCmd,"AT+CWSTOPSMART\r\n");
			break;
		case RESTORE:
			len = sprintf(strCmd,"AT+RESTORE\r\n");
			break;
		case JAPSTATUS:
			len = sprintf(strCmd,"AT+CWJAP?\r\n");
			break;
		default: break;
	}
	strCmd[len] = 0;
	return len;
}

/**
*	@brief 关闭TCP连接
*	@param 
* @param 
* @param 
* @return 
**/
void TCP_Close(void)
{
	int len = 0;
	char uartdata[128];
	
	len = PrepareATCmd(TCPOFF,uartdata);
	Delay_ms(20);
	Usart_Send(2,(unsigned char *)uartdata,len);
	Delay_ms(5);
	eventdelay.EventSet |= 0x200;  //开启第十个定时器
	eventdelay.EventDelay[9] = 0;
}

// check wifi status
void WIFI_Status_Check(void)
{
	int len = 0;
	char uartdata[128];
	
	len = PrepareATCmd(JAPSTATUS,uartdata);
	Delay_ms(20);
	Usart_Send(2,(unsigned char *)uartdata,len);
	Delay_ms(5);
//	eventdelay.EventSet |= 0x2000;  //开启第个定时器
//	eventdelay.EventDelay[13] = 0;
	
}



/**
*	@brief 请求申请授权，向后台发送GET请求
*	@param data上传数据字符串指针
* @param type 授权申请的类型，0 发起申请和获取code，1使用code获取access_token和refresh_token, 2 使用refresh_token获取access_token  3 通过登陆的方式直接获取token
* @param 
* @return 字符串长度
**/
int RequestAuthorize(char *data,u8 type)
{
	int len = 0;

	switch(type)
	{
//			printf("requesAuthorize tpye=%d\r\n",type);
#if	CHANGE
		case 3:
//			printf("3 通过登陆的方式直接获取token\r\n ");
			len  = sprintf(data,"POST /oauth/token HTTP/1.1\r\n");
			len += sprintf(data + len,"Host: %s\r\n",URL_HOST_NAME);
			len += sprintf(data + len,"Authorization: %s\r\n",DEF_BASIC_TOKEN);  // DEF_BASIC_TOKEN
			len += sprintf(data + len,"Cache-Control: no-cache\r\n");
			len += sprintf(data + len,"Content-Type: application/x-www-form-urlencoded\r\n");
			len += sprintf(data + len,"Content-length: 101\r\n");  // 104 是后面data len
			len += sprintf(data + len,"\r\ngrant_type=password&login=device_admin&password=123456&scope=yaojide_device_read+yaojide_device_write");
		break;
		case 0:
			// change by zhang
//			len = sprintf(data,"GET %sclient_id=%s&%s&response_type=code&%s&device_name=flb&device_id=%08d\r\n",str_RequestAuthorize,str_application,str_HttpsCallback,str_Scope,protocolstu.Device_ID);
//			printf("0 通过登陆的方式直接获取token\r\n ");
			len  = sprintf(data,"POST /oauth/token HTTP/1.1\r\n");
			len += sprintf(data + len,"Host: %s\r\n",URL_HOST_NAME);
			len += sprintf(data + len,"Authorization: %s\r\n",DEF_BASIC_TOKEN);  // DEF_BASIC_TOKEN
			len += sprintf(data + len,"Cache-Control: no-cache\r\n");
			len += sprintf(data + len,"Content-Type: application/x-www-form-urlencoded\r\n");
			len += sprintf(data + len,"Content-length: 101\r\n");  // 104 是后面data len
			len += sprintf(data + len,"\r\ngrant_type=password&login=device_admin&password=123456&scope=yaojide_device_read+yaojide_device_write");
		break;
		case 1:
			len  = sprintf(data,"POST /oauth/token HTTP/1.1\r\n");
			len += sprintf(data + len,"Host: %s\r\n",URL_HOST_NAME);
			len += sprintf(data + len,"Authorization: %s\r\n",DEF_BASIC_TOKEN);
			len += sprintf(data + len,"Cache-Control: no-cache\r\n");
			len += sprintf(data + len,"Content-Type: application/x-www-form-urlencoded\r\n");
			len += sprintf(data + len,"Content-length: 141\r\n");
			len += sprintf(data + len,"\r\ngrant_type=authorization_code&code=%s&redirect_uri=https://caijiyun.cn/callback",protocolstu.code_token);
		break;
		case 2:
			len  = sprintf(data,"POST /oauth/token HTTP/1.1\r\n");
			len += sprintf(data + len,"Host: %s\r\n",URL_HOST_NAME);
			len += sprintf(data + len,"Authorization: %s\r\n",DEF_BASIC_TOKEN);
			len += sprintf(data + len,"Cache-Control: no-cache\r\n");
			len += sprintf(data + len,"Content-Type: application/x-www-form-urlencoded\r\n");
			len += sprintf(data + len,"Content-length: 103\r\n");
			len += sprintf(data + len,"\r\ngrant_type=refresh_token&refresh_token=%s",protocolstu.refresh_token);
		break;
#else 
		case 0:
			len = sprintf(data,"GET %sclient_id=%s&%s&response_type=code&%s&device_name=flb&device_id=%08d\r\n",str_RequestAuthorize,str_application,str_HttpsCallback,str_Scope,protocolstu.Device_ID);
		break;
		case 1:
			len  = sprintf(data,"POST /oauth/token HTTP/1.1\r\n");
			len += sprintf(data + len,"Host: %s\r\n",HOME_PAGE);
			len += sprintf(data + len,"Authorization: %s\r\n",str_Base64);
			len += sprintf(data + len,"Cache-Control: no-cache\r\n");
			len += sprintf(data + len,"Content-Type: application/x-www-form-urlencoded\r\n");
			len += sprintf(data + len,"Content-length: 141\r\n");
			len += sprintf(data + len,"\r\ngrant_type=authorization_code&code=%s&redirect_uri=https://caijiyun.cn/callback",protocolstu.code_token);
		break;
		case 2:
			len  = sprintf(data,"POST /oauth/token HTTP/1.1\r\n");
			len += sprintf(data + len,"Host: %s\r\n",HOME_PAGE);
			len += sprintf(data + len,"Authorization: %s\r\n",str_Base64);
			len += sprintf(data + len,"Cache-Control: no-cache\r\n");
			len += sprintf(data + len,"Content-Type: application/x-www-form-urlencoded\r\n");
			len += sprintf(data + len,"Content-length: 103\r\n");
			len += sprintf(data + len,"\r\ngrant_type=refresh_token&refresh_token=%s",protocolstu.refresh_token);
		break;

#endif
		default: break;
	}
	return len;
}

/**
*	@brief 发起授权请求
*	@param 
* @param 
* @param 
* @return 
**/
void StartRequestAuthorize(void)
{
	int len = 0;
	char uartdata[128];
	
//	printf("StartRequestAuthorize\r\n");
	
#if CHANGE // chang by zhang	
	if(WIFI_Ready == 0)	
	{
		return;
	}
	if(protocolstu.HttpsLocker == 0) protocolstu.HttpsLocker = 1;
	else return;
	
	protocolstu.GetAuthorizeStep = 3;
	len = PrepareATCmd(TCPON,uartdata);
	Delay_ms(20);
	Usart_Send(2,(unsigned char *)uartdata,len); // use usart2  send data to wifi
	Delay_ms(5);
	protocolstu.bNeedAuthorize = 0;
	eventdelay.EventDelay[4] = 0;	

#else	
	if(WIFI_Ready == 0)	
	{
		return;
	}
	if(protocolstu.HttpsLocker == 0) protocolstu.HttpsLocker = 1;
	else return;
	
	protocolstu.GetAuthorizeStep = 1;
	len = PrepareATCmd(TCPON,uartdata);
	Delay_ms(20);
	Usart_Send(1,(unsigned char *)uartdata,len);
	protocolstu.bNeedAuthorize = 0;
	eventdelay.EventDelay[4] = 0;

#endif	

}

/**
*	@brief 获取服务器授权令牌
*	@param responses M35模块响应，根据模块进行响应
* @param 
* @param 
* @return 
**/
void GetServerAuthorizeCode(AT_Flag responses)
{
	int len = 0;
	char uartdata[512];
//	printf("GetServerAuthorizeCode\r\n");
#if	CHANGE
	switch(responses)
	{
		case CONNECTOK:
		{
			
			HttpDataLength = RequestAuthorize(uartdata,3);
			len = PrepareATCmd(SEND,uartdata);
			Usart_Send(2,(unsigned char *)uartdata,len);
			Delay_ms(20);
			len = RequestAuthorize(uartdata,3);
			printf("%s",uartdata);
			Usart_Send(2,(unsigned char *)uartdata,len);
			Delay_ms(5);
		}
		break;
		case SENDOK:
		{
			protocolstu.GetServerResponsesFlag = 1;
		}
		break;
		default: break;
	}

	
#else	
	switch(protocolstu.GetAuthorizeStep)
	{
		case 1:
		{
			switch(responses)
			{
				case CONNECTOK:
				{
					HttpDataLength = RequestAuthorize(uartdata,0);
					len = PrepareATCmd(SEND,uartdata);
					Usart_Send(1,(unsigned char *)uartdata,len);
//				}break;
//				case ATREADY:
//				{
					Delay_ms(20);
					len = RequestAuthorize(uartdata,0);
					printf("%s",uartdata);
					Usart_Send(1,(unsigned char *)uartdata,len);
				}break;
				case SENDOK:
				{
					protocolstu.GetServerResponsesFlag = 1;
				}break;
				default: break;
			}
		}break;
		case 2:
		{
			switch(responses)
			{
				case CONNECTOK:
				{
					HttpDataLength = RequestAuthorize(uartdata,0);
					len = PrepareATCmd(SEND,uartdata);
					Usart_Send(1,(unsigned char *)uartdata,len);
//				}break;
//				case ATREADY:
//				{
					Delay_ms(20);
					len = RequestAuthorize(uartdata,0);
					printf("%s",uartdata);
					Usart_Send(1,(unsigned char *)uartdata,len);
				}break;
				case SENDOK:
				{
					protocolstu.GetServerResponsesFlag = 1;
				}break;
				default: break;
			}
		}break;
		case 3:
		{
			switch(responses)
			{
				case CONNECTOK:
				{
					HttpDataLength = RequestAuthorize(uartdata,1);
					len = PrepareATCmd(SEND,uartdata);
					Usart_Send(1,(unsigned char *)uartdata,len);
//				}break;
//				case ATREADY:
//				{
					Delay_ms(20);
					len = RequestAuthorize(uartdata,1);
					printf("%s",uartdata);
					Usart_Send(1,(unsigned char *)uartdata,len);
				}break;
				case SENDOK:
				{
					protocolstu.ServerResponse.ResponseHead = 0;
					protocolstu.GetServerResponsesFlag = 1;
				}break;
				default: break;
			}
		}break;
		case 4:
		{
			switch(responses)
			{
				case CONNECTOK:
				{
					HttpDataLength = RequestAuthorize(uartdata,2);
					len = PrepareATCmd(SEND,uartdata);
					Usart_Send(1,(unsigned char *)uartdata,len);
//				}break;
//				case ATREADY:
//				{
					Delay_ms(20);
					len = RequestAuthorize(uartdata,2);
					printf("%s",uartdata);
					Usart_Send(1,(unsigned char *)uartdata,len);
				}break;
				case SENDOK:
				{
					protocolstu.GetServerResponsesFlag = 1;
				}break;
				default: break;
			}
		}break;
		default: break;
  }

#endif
	
}


/**
*	@brief 获取服药管理计划数据
*	@param responses M35模块响应，根据模块进行响应
* @param type 0 强制刷新 1 刷新修改的数据
* @param 
* @return 
**/
void GetTakeMedicinePlanCmd(AT_Flag responses,u8 type)
{
	int len = 0;
	char uartdata[512];
	
	switch(responses)
	{
		case CONNECTOK:
		{
//			printf("GetTakeMedicinePlanCmd\r\n");
			HttpDataLength = GetTakeMedicinePlan(uartdata,type);  // 获取服药管理计划
			len = PrepareATCmd(SEND,uartdata);
			Usart_Send(2,(unsigned char *)uartdata,len);
//		}break;
//		case ATREADY:
//		{
			Delay_ms(20);
			len = GetTakeMedicinePlan(uartdata,type);
			printf("%s",uartdata);
			Usart_Send(2,(unsigned char *)uartdata,len);
			Delay_ms(5);
		}break;
		case SENDOK:
		{
			protocolstu.GetServerResponsesFlag = 1;
		}break;
		default: break;
	}
}


/**
*	@brief 发起绑定手机APP
*	@param 
* @param 
* @param 
* @return 
**/
void StartAgreeBanding(void)
{
	int len = 0;
	char uartdata[128];
	
	if(WIFI_Ready == 0)	
	{
		return;
	}
	if(protocolstu.HttpsLocker == 0) protocolstu.HttpsLocker = 1;
	else return;
	

	protocolstu.AgreeBandingStep = 1;
	len = PrepareATCmd(TCPON,uartdata);
	Delay_ms(20);
	Usart_Send(2,(unsigned char *)uartdata,len);
	Delay_ms(5);
	protocolstu.bNeedSendATCmd = 0;
	eventdelay.EventDelay[4] = 0;
}

/**
*	@brief 同意绑定手机APP
*	@param responses M35模块响应，根据模块进行响应
* @param 
* @param 
* @return 
**/
void GetAgreeBandingCmd(AT_Flag responses)
{
	int len = 0;
	char uartdata[512];
	
	switch(responses)
	{
		case CONNECTOK:
		{
			HttpDataLength = BindingConfirm(uartdata);
			len = PrepareATCmd(SEND,uartdata);
			Usart_Send(2,(unsigned char *)uartdata,len);
//		}break;
//		case ATREADY:
//		{
			Delay_ms(20);
			len = BindingConfirm(uartdata);
			printf("%s",uartdata);

			Usart_Send(2,(unsigned char *)uartdata,len);
			Delay_ms(5);			
		}break;
		case SENDOK:
		{
			protocolstu.GetServerResponsesFlag = 1;
		}break;
		default: break;
	}
}

// 发起获取服务器声音
void StartGetVoice(void)
{
	int len = 0;
	char uartdata[128];

	if(WIFI_Ready == 0)
	{
		return;
	}
	
	if(protocolstu.HttpsLocker == 0) protocolstu.HttpsLocker = 1;
	else return;
		
	protocolstu.GetVoiceStep = 1;
	
	len = PrepareATCmd(TCPON,uartdata);
	Delay_ms(20);
	Usart_Send(2,(unsigned char *)uartdata,len);	
	protocolstu.bNeedGetVoice = 2;
	eventdelay.EventDelay[4] = 0;	
}
// get server voice
void GetRegulateVoiceCmd(AT_Flag responses)
{
	int len = 0;
	char uartdata[512];
//	printf("GetRegulateTimeCmd\r\n");
	switch(responses)
	{
		case CONNECTOK:
		{
//			printf("add by zhang at get service voice\r\n");
			HttpDataLength = GetVoiceValue(uartdata);
			len = PrepareATCmd(SEND,uartdata);
			Usart_Send(2,(unsigned char *)uartdata,len);
			
			myDelay_ms(20);
			len = GetVoiceValue(uartdata);
			printf("%s",uartdata);
			
			Usart_Send(2,(unsigned char *)uartdata,len);
			Delay_ms(5);
		}break;
		case SENDOK:
		{
			protocolstu.GetServerResponsesFlag = 1;
		}break;
		default: break;
	}
}

// parse voice json data
void ParseGetServerVoiceJson(char *pMsg)
{
	cJSON *pJson ,*pSub,*pSubsub;
	extern VoiceQueue	VQ;

	pJson = cJSON_Parse(pMsg);
	
	if(NULL == pJson) 
	{
		printf("J0E Voice	\r\n");
		return;
	}	
	
	pSub = cJSON_GetObjectItem(pJson, "volume");
	if(NULL == pSub) 
	{
		printf("J1E Voice	\r\n");
		cJSON_Delete(pJson);
		return;
	}	
	
	VQ.VoiceLevel = pSub->valueint;
//	printf("\r\n pSub->valueint = %d	 VQ.VoiceLevel = %d	\r\n",pSub->valueint,VQ.VoiceLevel);	
	
	protocolstu.GetVoiceStep = 0;
	protocolstu.bNeedGetVoice = 0;
	cJSON_Delete(pJson);
}




/**
*	@brief 发起获取服务器UTC时间
*	@param 
* @param 
* @param 
* @return 
**/
void StartGetUTCTime(void)
{
	int len = 0;
	char uartdata[128];
//	printf("StartGetUTCTime\r\n");
	if(WIFI_Ready == 0)
	{
		return;
	}
	
	if(protocolstu.HttpsLocker == 0) protocolstu.HttpsLocker = 1;
	else return;
	
	
	protocolstu.GetUtcTimeStep = 1;
	len = PrepareATCmd(TCPON,uartdata);
	Delay_ms(20);
	Usart_Send(2,(unsigned char *)uartdata,len);
	Delay_ms(5);
	protocolstu.bNeedGetUTC = 0;
	eventdelay.EventDelay[4] = 0;
}



/**
*	@brief 获取服务器UTC时间
*	@param responses M35模块响应，根据模块进行响应
* @param 
* @param 
* @return 
**/
void GetRegulateTimeCmd(AT_Flag responses)
{
	int len = 0;
	char uartdata[512];
//	printf("GetRegulateTimeCmd\r\n");
	switch(responses)
	{
		case CONNECTOK:
		{
			HttpDataLength = GetRtcValue(uartdata);
			len = PrepareATCmd(SEND,uartdata);
			Usart_Send(2,(unsigned char *)uartdata,len);
//		}break;
//		case ATREADY:
//		{
			Delay_ms(20);
			len = GetRtcValue(uartdata);
			printf("%s",uartdata);

//			printf("chang by zhang at get service time\r\n");
			Usart_Send(2,(unsigned char *)uartdata,len);
			Delay_ms(5);
		}break;
		case SENDOK:
		{
			protocolstu.GetServerResponsesFlag = 1;
		}break;
		default: break;
	}
}

/**
*	@brief 解析获取RTC时间
*	@param pMsg CJSON数据字符串指针
* @param 
* @param 
* @return 
**/
void ParseGetServerRtcTimeJson(char *pMsg)
{
	cJSON *pJson ,*pSub,*pSubsub;
//	printf("ParseGetServerRtcTimeJson\r\n");
	pJson = cJSON_Parse(pMsg);
	//	printf("eventdelay.EventDelay[3] = %d\r\n",eventdelay.EventDelay[3]);
	if(NULL == pJson) 
	{
//		printf("%s\r\n",pMsg);
		printf("J0E\r\n");
		return;
	}
#if	CHANGE
	pSub = cJSON_GetObjectItem(pJson, "current_time");
#else
	pSub = cJSON_GetObjectItem(pJson, "response");
#endif
	if(NULL == pSub) 
	{
		printf("J1E\r\n");
		cJSON_Delete(pJson);
		return;
	}

	pSubsub = cJSON_GetObjectItem(pSub,"message");
	
	if(NULL == pSubsub)
	{
		//成功则进入第二步骤
		protocolstu.GetUtcTimeStep = 2;
		protocolstu.bNeedAuthorize = 1;
		Time_RegulateByInternet(pSub->valueint);  // set RTC time at current_time
//		printf("changes to rtc %d\r\n",pSub->valueint);
//		printf(" UNIX时间戳 = %d 当前时间为: %d年(%s年) %d月 %d日 (星期%s)  %0.2d:%0.2d:%0.2d\r\n",TimeVar,
//	                    systime->tm_year, zodiac_sign[(tm->tm_year-3)%12], tm->tm_mon, tm->tm_mday, 
//	                    WEEK_STR[tm->tm_wday], tm->tm_hour, 
//	                    tm->tm_min, tm->tm_sec);
	}else
	{
		protocolstu.GetUtcTimeStep = 0;
		protocolstu.bNeedAuthorize = 2;
		protocolstu.bNeedGetUTC = 1;
		printf("%s\r\n",pSubsub->valuestring);
		printf("below change by zhang\r\n");
	}
//	printf("GetUtcTimeStep = %d\r\n",GetUtcTimeStep);
	cJSON_Delete(pJson);
}


/**
*	@brief 使用refresh token 刷新令牌
*	@param 
* @param 
* @param 
* @return 
**/
void RefreshAccessToken(void)
{
	int len = 0;
	char uartdata[256];
//	printf("RefreshAccessToken\r\n");
	if(WIFI_Ready == 0)
	{
		return;
	}
	if(protocolstu.HttpsLocker == 0) protocolstu.HttpsLocker = 1;
	else return;
	
	protocolstu.GetAuthorizeStep = 4;

	len = PrepareATCmd(TCPON,uartdata);
	Delay_ms(20);
	Usart_Send(2,(unsigned char *)uartdata,len);
	Delay_ms(5);
	protocolstu.bNeedAuthorize = 0;
	eventdelay.EventDelay[4] = 0;
}

/**
*	@brief 解析请求授权时候收到的JSON数据
*	@param pMsg CJSON数据字符串指针
* @param 
* @param 
* @return 
**/
void ParseGetServerAuthorizeJson(char *pMsg)
{
	cJSON *pJson ,*pSub;
	int len = 0;
	char uartdata[32];
	 pJson = cJSON_Parse(pMsg);

//	printf("ParseGetServerAuthorizeJson GetAuthorizeStep = %d\r\n",protocolstu.GetAuthorizeStep);
	if(NULL == pJson) 
	{
		printf("%s\r\n",pMsg);
		printf("J0E\r\n");
		return;
	}
//	printf("\r\n start to parse authorize json data\r\n");
	switch(protocolstu.GetAuthorizeStep)
	{
		case 1:
		{
			pSub = cJSON_GetObjectItem(pJson, "message");
			if(NULL == pSub) 
			{
				printf("J1E\r\n");
				cJSON_Delete(pJson);
				return;
			}
			
			if(strstr((const char *)pSub->valuestring,"New limited authorization failure") != NULL)	protocolstu.GetAuthorizeStep = 0;  //错误返回
			else	protocolstu.GetAuthorizeStep = 2;  //成功则进入第二步骤

//			printf("GetAuthorizeStep = %d\r\n",GetAuthorizeStep);
		}break;
		case 2:
		{
			pSub = cJSON_GetObjectItem(pJson, "code");
			if(NULL == pSub) 
			{
				printf("J2E\r\n");
				cJSON_Delete(pJson);
				return;
			}
			memcpy((char*)protocolstu.code_token,pSub->valuestring,65*sizeof(char));
			//成功则进入第三步骤
			protocolstu.GetAuthorizeStep = 3;
		}break;
		case 3:
		{
			pSub = cJSON_GetObjectItem(pJson, "access_token");
			if(NULL == pSub) 
			{
				printf("J31E\r\n");
				cJSON_Delete(pJson);
				return;
			}
			memcpy((char*)protocolstu.access_token,pSub->valuestring,65*sizeof(char));
			
			pSub = cJSON_GetObjectItem(pJson, "refresh_token");
			if(NULL == pSub)
			{
				printf("J32E\r\n");
				cJSON_Delete(pJson);
				return;
			}
			memcpy((char*)protocolstu.refresh_token,pSub->valuestring,65*sizeof(char));
			
			pSub = cJSON_GetObjectItem(pJson, "created_at");
			if(NULL == pSub)
			{
				printf("J32E\r\n");
				cJSON_Delete(pJson);
				return;
			}
			protocolstu.TokenRefreshTimeValue = pSub->valueint;
			
			//成功获取令牌，退出授权请求
			protocolstu.GetAuthorizeStep = 0;
			protocolstu.bAuthorize = 1;
			//save code
			Delay_ms(2);
#if 1 // change by zhang
			SaveTokenCode();
#endif
		}break;
		case 4:
		{
			pSub = cJSON_GetObjectItem(pJson, "access_token");
			if(NULL == pSub)
			{
				printf("J41E\r\n");
				cJSON_Delete(pJson);
				return;
			}
			
			memcpy((char*)protocolstu.access_token,pSub->valuestring,65*sizeof(char));
			pSub = cJSON_GetObjectItem(pJson, "refresh_token");
			if(NULL == pSub)
			{
				printf("J42E\r\n");
				cJSON_Delete(pJson);
				return;
			}
			memcpy((char*)protocolstu.refresh_token,pSub->valuestring,65*sizeof(char));
			
			pSub = cJSON_GetObjectItem(pJson, "created_at");
			if(NULL == pSub)
			{
				printf("J32E\r\n");
				cJSON_Delete(pJson);
				return;
			}
			protocolstu.TokenRefreshTimeValue = pSub->valueint;
			
			//成功获取令牌，退出授权请求
			protocolstu.GetAuthorizeStep = 0;
			len = PrepareATCmd(TCPOFF,uartdata);
			Delay_ms(20);
			Usart_Send(2,(unsigned char *)uartdata,len);
			protocolstu.bAuthorize = 1;
			//save code
			Delay_ms(100);
			SaveTokenCode();
		}break;
		default: break;
	}
	cJSON_Delete(pJson);
}


/**
*	@brief 同意绑定APP
*	@param data上传数据字符串指针
* @param 
* @param 
* @return 字符串长度
**/
int BindingConfirm(char *data)
{
	int len = 0;
#if	0 //CHANGE
	
#else
	len  = sprintf(data,"POST /api/1/medicine_box/devices/at_send HTTP/1.1\r\n");
	len += sprintf(data + len,"Host: %s\r\n",HOME_PAGE);
	len += sprintf(data + len,"Authorization: Bearer %s\r\n",protocolstu.access_token);
	len += sprintf(data + len,"Cache-Control: no-cache\r\n");
	len += sprintf(data + len,"Content-Type: application/x-www-form-urlencoded\r\n");
	len += sprintf(data + len,"Content-length: 19\r\n");  //固定长度

	len += sprintf(data + len,"\r\ndevice_uid=%08d",protocolstu.Device_ID);

#endif	
	return len;
}



int GetVoiceValue(char *data)
{
	int len = 0;
	int id_len = 0;

//	printf("\r\n start to get Voice value\r\n");
	len = sprintf(data,"POST /v1/devices/volume HTTP/1.1\r\n");
	len += sprintf(data + len,"Host: %s\r\n",URL_HOST_NAME);
	len += sprintf(data + len,"Authorization: Bearer %s\r\n",protocolstu.access_token);
	len += sprintf(data + len,"Cache-Control: no-cache\r\n");
	len += sprintf(data + len,"Content-Type: application/x-www-form-urlencoded\r\n");
	
	len += sprintf(data + len,"Content-length: 19\r\n");  //固定长度
	len += sprintf(data + len,"\r\ndevice_uid=%08d",protocolstu.Device_ID);	
	
	return len;
}

/**
*	@brief 获取RTC时间
*	@param data上传数据字符串指针
* @param 
* @param 
* @return 字符串长度
**/
int GetRtcValue(char *data)
{
	int len = 0;
#if	CHANGE
//	printf("get service rtc\r\n");
	len = sprintf(data,"GET http://ygj-test.caijiyun.cn/v1/devices/get_time?device_uid=%08d\r\n",protocolstu.Device_ID);

#else
	len = sprintf(data,"POST /api/1/medicine_box/medicine_boxs/get_time HTTP/1.1\r\n");
	len += sprintf(data + len,"Host: %s\r\n",HOME_PAGE);
	len += sprintf(data + len,"Authorization: Bearer %s\r\n",protocolstu.access_token);
	len += sprintf(data + len,"Cache-Control: no-cache\r\n");
	len += sprintf(data + len,"Content-Type: application/x-www-form-urlencoded\r\n");
	len += sprintf(data + len,"Content-length: 19\r\n");  //固定长度
	
	len += sprintf(data + len,"\r\ndevice_uid=%08d",protocolstu.Device_ID);


#endif	
	return len;
}


/**
*	@brief 上传吃药情况数据
*	@param data上传数据字符串指针
* @param 
* @param 
* @return 字符串长度
**/
int SendTakeMedicineSituation(char *data)
{
	int len = 0;
	int id_len = 0;
#if	CHANGE
//	printf("\r\n start to send take medicine record\r\n");
	len = sprintf(data,"POST /v1/devices/medicine HTTP/1.1\r\n");
	len += sprintf(data + len,"Host: %s\r\n",URL_HOST_NAME);
	len += sprintf(data + len,"Authorization: Bearer %s\r\n",protocolstu.access_token);
	len += sprintf(data + len,"Cache-Control: no-cache\r\n");
	len += sprintf(data + len,"Content-Type: application/x-www-form-urlencoded\r\n");
	
//	if(protocolstu.takemedicine.bTaken)
	{
		id_len = sprintf(data + len,"Content-length: %d\r\n",protocolstu.id) - sprintf(data + len,"Content-length: \r\n");
		len += sprintf(data + len,"Content-length: %d\r\n",(id_len + 34));  //固定长度	25+8+1+2
		len += sprintf(data + len,"\r\ndevice_uid=%08d&eat_type=%d&id=%d\r\n",protocolstu.Device_ID,protocolstu.takemedicine.bTaken,protocolstu.id);//此处需要修改
	}

	
#else
	// https://api.caijiyun.cn/api/1/medicine_box/medicine_boxs  HOME_PAGE
	len = sprintf(data,"POST /api/1/medicine_box/medicine_boxs HTTP/1.1\r\n");
	len += sprintf(data + len,"Host: %s\r\n",HOME_PAGE);  // #define HOME_PAGE "caijiyun.cn"  #define HOME_API 	"api.caijiyun.cn"
	len += sprintf(data + len,"Authorization: Bearer %s\r\n",protocolstu.access_token);
	len += sprintf(data + len,"Cache-Control: no-cache\r\n");
	len += sprintf(data + len,"Content-Type: application/x-www-form-urlencoded\r\n");
	
	if(protocolstu.takemedicine.bTaken)
	{
		if(protocolstu.takemedicine.mboxIndex < 10)	len += sprintf(data + len,"Content-length: 80\r\n");  //固定长度
		else len += sprintf(data + len,"Content-length: 81\r\n");  //固定长度
		len += sprintf(data + len,"\r\ndevice_uid=%08d&date=%s&index=%d&alert_time=%s&taken=true",protocolstu.Device_ID,protocolstu.takemedicine.takemedicineTime,protocolstu.takemedicine.mboxIndex,protocolstu.takemedicine.alertTime);
	}
	else
	{
		if(protocolstu.takemedicine.mboxIndex < 10)	len += sprintf(data + len,"Content-length: 81\r\n");  //固定长度
		else len += sprintf(data + len,"Content-length: 82\r\n");  //固定长度
		len += sprintf(data + len,"\r\ndevice_uid=%08d&date=%s&index=%d&alert_time=%s&taken=false",protocolstu.Device_ID,protocolstu.takemedicine.takemedicineTime,protocolstu.takemedicine.mboxIndex,protocolstu.takemedicine.alertTime);
	}
#endif
	return len;
}

/**
*	@brief 准备上传服药记录数据结构体
*	@param 
* @param 
* @param 
* @return 
**/
void PrepareTakeMedicineStu(u8 index,short alerttime,u8 btaken)
{
//	char curTime[20] = "";
	
	sprintf(protocolstu.takemedicine.takemedicineTime,"%04d-%02d-%02d %02d:%02d:%02d",systmtime.tm_year,systmtime.tm_mon,systmtime.tm_mday,systmtime.tm_hour,systmtime.tm_min,systmtime.tm_sec);
	protocolstu.takemedicine.bTaken = btaken;
	protocolstu.takemedicine.mboxIndex = index;
	sprintf(protocolstu.takemedicine.alertTime,"%02d:%02d",alerttime/60,alerttime%60);
}

/**
*	@brief 发起上传服药记录
*	@param 
* @param 
* @param 
* @return 
**/
void StartSendTakeMedicineSitu(void)
{
	int len = 0;
	char uartdata[128];
	
	if(WIFI_Ready == 0)
	{
		return;
	}
	if(protocolstu.HttpsLocker == 0) protocolstu.HttpsLocker = 1;
	else return;
	
	
	protocolstu.SendTakeMedicineCaseStep = 1;
	len = PrepareATCmd(TCPON,uartdata);
	Delay_ms(20);
	Usart_Send(2,(unsigned char *)uartdata,len);
	Delay_ms(5);
	protocolstu.takemedicine.needsend = 0;
	eventdelay.EventDelay[4] = 0;
}


/**
*	@brief 上传服药情况
*	@param responses M35模块响应，根据模块进行响应
* @param 
* @param 
* @return 
**/
void SendTakeMedicineSituationCmd(AT_Flag responses)
{
	int len = 0;
	char uartdata[512];
	
	switch(responses)
	{
		case CONNECTOK:
		{
			HttpDataLength = SendTakeMedicineSituation(uartdata);
			len = PrepareATCmd(SEND,uartdata);
			Usart_Send(2,(unsigned char *)uartdata,len);
//		}break;
//		case ATREADY:
//		{
			Delay_ms(20);
			len = SendTakeMedicineSituation(uartdata);
			printf("%s",uartdata);
			Usart_Send(2,(unsigned char *)uartdata,len);
			Delay_ms(5);
		}break;
		case SENDOK:
		{
			protocolstu.GetServerResponsesFlag = 1;
		}break;
		default: break;
	}
}

/**
*	@brief 发起获取服药管理计划
*	@param 
* @param type 0 强制获取整个计划 1 获取修改的数据
* @param 
* @return 字符串长度
**/
void StartGetTakeMedicinePlan(u8 type)
{
	int len = 0;
	char uartdata[128];
	


	if(WIFI_Ready == 0)
	{
		return;
	}
	if(protocolstu.HttpsLocker == 0) protocolstu.HttpsLocker = 1;
	else return;
	
	
	protocolstu.GetTakeMedicinePlanStep = 1 + type;
	len = PrepareATCmd(TCPON,uartdata);
	Delay_ms(20);
	Usart_Send(2,(unsigned char *)uartdata,len);
	Delay_ms(5);
	protocolstu.bNeedGetTMPlan = 0;
	eventdelay.EventDelay[4] = 0;
}

/**
*	@brief 获取服药管理计划
*	@param data上传数据字符串指针
* @param type 0 强制获取整个计划 1 获取修改的数据
* @param 
* @return 字符串长度
**/
int GetTakeMedicinePlan(char *data,u8 type)
{
	int len = 0;
//	printf("start to get get_setting\r\n");
#if	CHANGE
	
	len = sprintf(data,"POST /v1/devices/get_setting HTTP/1.1\r\n");
	len += sprintf(data + len,"Host: %s\r\n",URL_HOST_NAME);
	len += sprintf(data + len,"Authorization: Bearer %s\r\n",protocolstu.access_token);
	len += sprintf(data + len,"Cache-Control: no-cache\r\n");
	len += sprintf(data + len,"Content-Type: application/x-www-form-urlencoded\r\n");
	len += sprintf(data + len,"Content-length: 19\r\n");  //固定长度
	len += sprintf(data + len,"\r\ndevice_uid=%02d",protocolstu.Device_ID);
	
#else
	len = sprintf(data,"POST /api/5/medicine_box/medicine_boxs/get_setting HTTP/1.1\r\n");
	len += sprintf(data + len,"Host: %s\r\n",HOME_PAGE);
	len += sprintf(data + len,"Authorization: Bearer %s\r\n",protocolstu.access_token);
	len += sprintf(data + len,"Cache-Control: no-cache\r\n");
	len += sprintf(data + len,"Content-Type: application/x-www-form-urlencoded\r\n");
	if(type == 1)
	{
		len += sprintf(data + len,"Content-length: 19\r\n");  //固定长度
	
		len += sprintf(data + len,"\r\ndevice_uid=%02d",protocolstu.Device_ID);
	}else
	{
		len += sprintf(data + len,"Content-length: 30\r\n");  //固定长度
	
		len += sprintf(data + len,"\r\ndevice_uid=%08d&reset=true",protocolstu.Device_ID);
	}
#endif
	return len;
}


/**  respones  {"setting_data":[{"id":1, "time":"07:30", "eat_scope":"0,1,2,3"}]}
*	@brief 解析获取服药管理计划
*	@param pMsg CJSON数据字符串指针
* @param 
* @param 
* @return 
**/
void ParseGetTakeMedicinePlanJson(char *pMsg)
{
	cJSON *pJson ,*pSub,*pSubSub1,*pSubSub2,*pSubSub3,*root;
	int iSize = 0,iCnt = 0;
	char *pTimeStr,*pScopeStr;
//	char week[7];  //0--3 早中午晚 0 -- 6  日 -- 六
//	int weekday = -1;
	u8 hours = 0,minutes = 0;
	int needTime = -1,limitime = -1;
	int i = 0,j = 0,w = 0,len = 0;
	
	
	
	protocolstu.GetTakeMedicinePlanStep = 0;
	pJson = cJSON_Parse(pMsg);
	if(NULL == pJson) 
	{
		printf("%s\r\n",pMsg);
		printf("J0E\r\n");
		return;
	}
//	printf("\r\n	start to parse setting_data	\r\n");
#if	CHANGE
	
	root = cJSON_GetObjectItem(pJson, "setting_data");
	iSize = cJSON_GetArraySize(root);
	protocolstu.isize = iSize;
	printf("\r\n	iSize = %d	\r\n",iSize);
	for(iCnt = 0; iCnt < iSize; iCnt++)
	{
		limitime = -1;
		needTime = -1;
		protocolstu.AppTakeWarn[iCnt].id = 0;
		protocolstu.AppTakeWarn[iCnt].time = 0;
		for(i = 0; i < 7; i++)
		{
			protocolstu.AppTakeWarn[iCnt].week[i] = 0;
		}
		// clean only one
		if(iCnt == 0)
		{
			len = 0;
			len = sizeof(protocolstu.TakeMedicineTimeArray)/sizeof(short);
//			printf("array = %d\r\n",len);
			for(i = 0; i < len; i++)
			{
				protocolstu.TakeMedicineTimeArray[i] = -1;
			}
			len = 0;
			len = sizeof(protocolstu.AppTakeWarn)/sizeof(AppSetWarn);
//			printf("item = %d\r\n",len);
			for(i = 0; i < len;i++)
			{
				protocolstu.AppTakeWarn[i].id = 0;
				protocolstu.AppTakeWarn[i].time = 0;
				for(w = 0; w < 7; w++)
				{
					protocolstu.AppTakeWarn[i].week[w] = 0;
				}
			}	
		
		}
		
		pSub = cJSON_GetArrayItem(root, iCnt);
		if(NULL == pSub)
		{
		printf("invalid arrayitem is %d\r\n",iCnt);
		continue;
		}
		// get id
		pSubSub1 = cJSON_GetObjectItem(pSub,"id");
		if(NULL == pSubSub1)
		{
			printf("invalid arrayitem id");
		}
		protocolstu.id = pSubSub1->valueint;
		protocolstu.AppTakeWarn[iCnt].id = pSubSub1->valueint;
//		printf("setting_data id = %d\r\n",protocolstu.AppTakeWarn[iCnt].id);
		
		pSubSub2 = cJSON_GetObjectItem(pSub, "eat_scope");
		if(NULL == pSubSub2) 
		{
		printf("invalid at in array eat_scope\r\n");
		continue;
		}
//		printf("setting_data eat_scope = %s\r\n",pSubSub2->valuestring);
		pScopeStr = pSubSub2->valuestring;
//		printf("pScopeStr = %s\r\n",pScopeStr);
//		ParseTakeMedicinePlanScopeString(pScopeStr,week);
		ParseTakeMedicinePlanScopeString(pScopeStr,protocolstu.AppTakeWarn[iCnt].week);
		
		pSubSub3 = cJSON_GetObjectItem(pSub,"time");
		if(NULL == pSubSub3)
		{
			printf("invalid at in array tiem\r\n");
			continue;
		}
//		printf("setting_data time = %s\r\n",pSubSub3->valuestring);
		pTimeStr = pSubSub3->valuestring;
		needTime = ParseTimeString(pTimeStr,&hours,&minutes);
		if(-1 == needTime)
		{
			printf("parse time fail\r\n");
		}
		protocolstu.AppTakeWarn[iCnt].time = needTime;
		
	}


	// 
	// show all plan
	for(i = 0; i < protocolstu.isize; i++)
	{
		printf("isize = %d id = %d time = %d",i,protocolstu.AppTakeWarn[i].id,protocolstu.AppTakeWarn[i].time);
		for(j = 0; j < 7; j++)
		{
			printf(" week[%d] = %d",j,protocolstu.AppTakeWarn[i].week[j]);
		}
		printf("\r\n");
	}
	
	


#else
	root = cJSON_GetObjectItem(pJson, "response");
	
	iSize = cJSON_GetArraySize(root);
	printf("iSize = %d\r\n",iSize);
	printf("1 chage by zhang at parse medicineplanjson\r\n");
	
	for(iCnt = 0; iCnt < iSize; iCnt++)
	{
		pSub = cJSON_GetArrayItem(root, iCnt);
		if(NULL == pSub)
		{
		printf("invalid arrayitem is %d\r\n",iCnt);
		continue;
		}
		//	pTemp = pSub->child;
//		pSubSub = cJSON_GetObjectItem(pSub, "index");
//		if(NULL == pSubSub) 
//		{
//		printf("invalid index in array %d\r\n",iCnt);	
//		continue;
//		}
//		iValue = pSubSub->valueint;
		pSubSub = cJSON_GetObjectItem(pSub, "t");
		if(NULL == pSubSub) 
		{
		printf("invalid at in array %d\r\n",iCnt);
		continue;
		}
		pTimeStr = pSubSub->valuestring;
		ParseTakeMedicinePlanString(pTimeStr,iCnt);
//		printf("alert_time[%2d] : [%s]\r\n", iValue, pTimeStr);
	}   
#endif
	protocolstu.GetPlan = 1;
	
	cJSON_Delete(pJson);
}


/**
*	@brief 解析服药计划时间字符串
*	@param str字符串指针
* @param index 药盒编号
* @param 
* @return 字符串长度
**/
void ParseTakeMedicinePlanString(const char *str,u8 index)
{
	static char temp[6] = "";
	u8 i = 0;
	static u8 hours = 0,minutes = 0;

	// 上午 00:00 - 10:59(0-659) 中午 11:00 - 14:00(660-840) 下午 14:01 - 20:00(841-1200) 晚上 20:01 - 23:59(1201-1439)
#if	CHANGE
	while(*str != 0)
	{

			protocolstu.TakeMedicinePlanArray[index] = ParseTimeString(str,&(hours),&(minutes));
//			str++;
	}	
	
#else	
	while(*str != 0)
	{
		if(*str != ',')	temp[i++] = *str;
		else
		{
			temp[i] = 0;
			i = 0;
			//
			protocolstu.TakeMedicinePlanArray[index] = ParseTimeString(temp,&(hours),&(minutes));
			
		}
		str++;
	}
	temp[i] = 0;
	i = 0;

	protocolstu.TakeMedicinePlanArray[index] = ParseTimeString(temp,&(hours),&(minutes));
#endif
	// changes by zhang
//	printf("%d,%d\r\n",index,protocolstu.TakeMedicinePlanArray[index]);
}

/**
*	@brief 解析服药计划时间范围字符串
*	@param str字符串指针
* @param week 星期几
* @param 
* @return 字符串长度
**/
void ParseTakeMedicinePlanScopeString(const char *str,char *week)
{
	char temp[7] = "";
	u8 i = 0;
	u8 index = 0;
	while(*str != 0)
	{
		if(*str == ',') 
		{
			
			i++;
		}
		else
		{
			temp[i] = *str;
			index = atoi(&temp[i]);
			if(index <7)
				week[index] = 1;   // 星期几 有服药计划
		}
		str++;
	}
		
}




/**
*	@brief 解析时间字符串
*	@param str字符串指针
* @param 
* @param 
* @return 当天所在的分钟数
**/
short ParseTimeString(const char *str,u8 *hours,u8 *minutes)
{
	char temp[3] = "";
	u8 i = 0;
	
	while(*str != 0)
	{
		if(*str != ':')
			temp[i++] = *str;
		else
		{
			temp[i] = 0;
			if(i == 2)  *hours = (temp[0] - 0x30)*10 + (temp[1] - 0x30);
			else				*hours = (temp[0] - 0x30);
			i = 0;
		}
		str++;
	}
	if(i == 2)  *minutes = (temp[0] - 0x30)*10 + (temp[1] - 0x30);
//	else				*minutes = (temp[0] - 0x30);
	if(i == 0) return -1;
	return (*hours*60 + *minutes);
}

/**
*	@brief 保存设备TOKEN
*	@param 
* @param 
* @param 
* @return 
**/
void SaveTokenCode(void)
{
	u16 i = 0;
	u8 I2c_Buf_Write[192] = {0};
	u8 I2c_Buf_Read[192] = {0};
	
	printf("写入TOKEN数据\r\n");
  
	for(i = 0; i < 64; i++)
	{
		I2c_Buf_Write[i] = protocolstu.access_token[i];
		I2c_Buf_Write[64+i] = protocolstu.refresh_token[i];
	}
	
	I2c_Buf_Write[128] = (u8)protocolstu.Device_ID;
	I2c_Buf_Write[129] = (u8)(protocolstu.Device_ID>>8);
	I2c_Buf_Write[130] = (u8)(protocolstu.Device_ID>>16);
	I2c_Buf_Write[131] = (u8)(protocolstu.Device_ID>>24);
	I2c_Buf_Write[132] = protocolstu.bBanding;
	I2c_Buf_Write[133] = protocolstu.bAuthorize;
	
#if NEW_EEPROM
	protocolstu.bBanding = 1;
	I2c_Buf_Write[132] = protocolstu.bBanding;
#endif
	
	//写入token刷新时间
	I2c_Buf_Write[134] = (u8)protocolstu.TokenRefreshTimeValue;
	I2c_Buf_Write[135] = (u8)(protocolstu.TokenRefreshTimeValue>>8);
	I2c_Buf_Write[136] = (u8)(protocolstu.TokenRefreshTimeValue>>16);
	I2c_Buf_Write[137] = (u8)(protocolstu.TokenRefreshTimeValue>>24);
	
	//写入录音标志
	I2c_Buf_Write[138] = 0;//bCustomRecord;
  //将access_token 和 refresh_token保存到EEPROM中
	Delay_ms(300);
//	I2C_EE_BufferWrite(I2c_Buf_Write, 0, 192); // change by zhang
	AT24CXX_Write(0,I2c_Buf_Write,192);
	Delay_ms(300);

  printf("\n\r写成功\r\n");

  //将EEPROM读出数据顺序保持到I2c_Buf_Read中，作为校验
	Delay_ms(200);
//	I2C_EE_BufferRead(I2c_Buf_Read, 0, 192); // change by zhang
	AT24CXX_Read(0,I2c_Buf_Read,192);
	Delay_ms(200);

  //将I2c_Buf_Read中的数据通过串口打印
	for (i=0; i<64; i++)
	{	
		if(I2c_Buf_Read[64+i] != protocolstu.refresh_token[i] || I2c_Buf_Read[i] != protocolstu.access_token[i])
		{
			printf("错误:I2C EEPROM写入与读出的数据不一致，开始重写\r\n");
			return;
		}
	}
	
	if(I2c_Buf_Read[128] != (u8)protocolstu.Device_ID) return;
	if(I2c_Buf_Read[129] != (u8)(protocolstu.Device_ID>>8)) return;
	if(I2c_Buf_Read[130] != (u8)(protocolstu.Device_ID>>16)) return;
	if(I2c_Buf_Read[131] != (u8)(protocolstu.Device_ID>>24)) return;
	if(I2c_Buf_Read[132] != protocolstu.bBanding) return;
	if(I2c_Buf_Read[133] != protocolstu.bAuthorize) return;
	
	//写入token刷新时间
	if(I2c_Buf_Read[134] != (u8)protocolstu.TokenRefreshTimeValue) return;
	if(I2c_Buf_Read[135] != (u8)(protocolstu.TokenRefreshTimeValue>>8)) return;
	if(I2c_Buf_Read[136] != (u8)(protocolstu.TokenRefreshTimeValue>>16)) return;
	if(I2c_Buf_Read[137] != (u8)(protocolstu.TokenRefreshTimeValue>>24)) return;
	
	//写入录音标志
	if(I2c_Buf_Read[138] != bCustomRecord) return;
	
  printf("I2C(AT24C02)数据保存成功\r\n");
}
/**
*	@brief 读取设备token
*	@param 
* @param 
* @param 
* @return 
**/
void ReadTokenCode(void)
{
	u16 i = 0;
	u8 I2c_Buf_Read[192] = {0};

	Delay_ms(180);
	printf("开始读取token数据\r\n");
//	I2C_EE_BufferRead(I2c_Buf_Read, 0, 192); 
	AT24CXX_Read(0,I2c_Buf_Read,192);
	Delay_ms(180);
	for(i = 0; i < 64;i++)
	{
		protocolstu.access_token[i] = I2c_Buf_Read[i];
		protocolstu.refresh_token[i] = I2c_Buf_Read[64+i];
	}
	protocolstu.access_token[64] = 0;
	protocolstu.refresh_token[64] = 0;
	
	protocolstu.Device_ID = I2c_Buf_Read[131]<<24 | I2c_Buf_Read[130]<<16 | I2c_Buf_Read[129] << 8 | I2c_Buf_Read[128];
	protocolstu.bBanding = I2c_Buf_Read[132];
	protocolstu.bAuthorize = I2c_Buf_Read[133];
	printf("Device_ID = %d,bBanding = %d,bAuthorize = %d\r\n",protocolstu.Device_ID,protocolstu.bBanding,protocolstu.bAuthorize);
	
	protocolstu.TokenRefreshTimeValue = I2c_Buf_Read[137]<<24 | I2c_Buf_Read[136]<<16 | I2c_Buf_Read[135] << 8 | I2c_Buf_Read[134];
	printf("TokenRefreshTimeValue = %d\r\n",protocolstu.TokenRefreshTimeValue);
	
	bCustomRecord = I2c_Buf_Read[138];
	printf("bCustomRecord = %d\r\n",bCustomRecord);
	
	printf("读取数据完成\r\n");
}

/**
*	@brief 保存设备服药计划
*	@param 
* @param 
* @param 
* @return 
**/
void SaveTMPlan(void)
{
	u8 I2c_Buf_Write[64] = {0};
	int i = 0;
	printf("写入Plan数据\r\n");
  
	for(i = 0; i < 28;i++)
	{
		I2c_Buf_Write[i] = (u8)(protocolstu.TakeMedicinePlanArray[i]);
		I2c_Buf_Write[2*i+1] = (u8)(protocolstu.TakeMedicinePlanArray[i] >> 8);
	}

	Delay_ms(50);
//	I2C_EE_BufferWrite(I2c_Buf_Write, 192, 64); // chang by zhang
	AT24CXX_Write(192,I2c_Buf_Write,64);
	Delay_ms(200);
  printf("\n\r写成功\r\n");
}
/**
*	@brief 读取设备服药计划
*	@param 
* @param 
* @param 
* @return 
**/
void ReadTMPlan(void)
{
	u8 I2c_Buf_Read[6] = {0};
	int i = 0;
	Delay_ms(50);
//	printf("开始读取数据\r\n");
//	I2C_EE_BufferRead(I2c_Buf_Read, 192, 64); 
	AT24CXX_Read(192,I2c_Buf_Read,64);
	Delay_ms(200);
	for(i = 0; i < 28;i++)
	{
		protocolstu.TakeMedicinePlanArray[i] = (short)((u16)(I2c_Buf_Read[2*i+1])<<8 |I2c_Buf_Read[i]);
	}
	Delay_ms(50);
}

#if 0

/**
*	@brief 保存设备ID,是否绑定，是否授权等参数
*	@param 
* @param 
* @param 
* @return 
**/
void SaveDeviceParam(void)
{
	u8 I2c_Buf_Read[64] = {0};
	printf("写入设备ID数据\r\n");
  
	I2c_Buf_Read[0] = (u8)Device_ID;
	I2c_Buf_Read[1] = (u8)(Device_ID>>8);
	I2c_Buf_Read[2] = (u8)(Device_ID>>16);
	I2c_Buf_Read[3] = (u8)(Device_ID>>24);
	I2c_Buf_Read[4] = bBanding;
	I2c_Buf_Read[5] = bAuthorize;
  //将access_token 和 refresh_token保存到EEPROM中
	Delay_ms(10);
//	I2C_EE_BufferWrite(I2c_Buf_Read, 0, 64); // chang by zhang
	AT24CXX_Write(0,I2c_Buf_Read,64);
	Delay_ms(10);
  printf("\n\r写成功\r\n");
}
/**
*	@brief 读取设备设备ID,是否绑定，是否授权等参数
*	@param 
* @param 
* @param 
* @return 
**/
void ReadDeviceParam(void)
{
	u8 I2c_Buf_Read[6] = {0};
	Delay_ms(100);
//	printf("开始读取数据\r\n");
//	I2C_EE_BufferRead(I2c_Buf_Read, 128, 6); 
	AT24CXX_Read(128,I2c_Buf_Read,6);
	Delay_ms(10);
	Device_ID = I2c_Buf_Read[3]<<24 | I2c_Buf_Read[2]<<16 | I2c_Buf_Read[1] << 8 | I2c_Buf_Read[0];
	
	bBanding = I2c_Buf_Read[4];
	bAuthorize = I2c_Buf_Read[5];

	printf("Device_ID = %d,bBanding = %d,bAuthorize = %d\r\n",Device_ID,bBanding,bAuthorize);
}
#endif

//************************END OF FILE******************************//
