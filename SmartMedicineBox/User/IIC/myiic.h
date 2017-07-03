#ifndef __MYIIC_H
#define __MYIIC_H
#include "main.h"
//////////////////////////////////////////////////////////////////////////////////	 
				  
//////////////////////////////////////////////////////////////////////////////////

//IO��������
 
#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;}
#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<<28;}

//IO��������	 
#define IIC_SCL(a)    if (a)	\
											GPIO_SetBits(GPIOB,GPIO_Pin_6);\
											else		\
											GPIO_ResetBits(GPIOB,GPIO_Pin_6)//SCL//PBout(6)
#define IIC_SDA(a)     if (a)	\
											GPIO_SetBits(GPIOB,GPIO_Pin_7);\
											else		\
											GPIO_ResetBits(GPIOB,GPIO_Pin_7)//PBout(7) //SDA	 
#define READ_SDA    	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)	//PBin(7)  //����SDA 

//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);

										


#endif
















