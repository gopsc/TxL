/**
	************************************************************
	************************************************************
	************************************************************
	*	�ļ����� 	main.c
	*
	*	���ߣ� 		�ż���
	*
	*	���ڣ� 		2017-01-011
	*
	*	�汾�� 		V1.0
	*
	*	˵���� 		����onenet���ϴ����ݺ��������
	*
	*	�޸ļ�¼��	
	************************************************************
	************************************************************
	************************************************************
**/

//��Ƭ��ͷ�ļ�
#include "stm32f10x.h"

//����Э���
#include "onenet.h"
#include "fault.h"

//�����豸
#include "net_device.h"

//Ӳ������
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "hwtimer.h"
#include "i2c.h"
#include "sht20.h"
#include "at24c02.h"
#include "selfcheck.h"
#include "info.h"

//C��
#include <string.h>



//������
DATA_STREAM dataStream[] = {
								{"Red_Led", &ledStatus.Led5Sta, TYPE_BOOL, 1},
								{"Green_Led", &ledStatus.Led4Sta, TYPE_BOOL, 1},
								{"Yellow_Led", &ledStatus.Led3Sta, TYPE_BOOL, 1},
								{"Blue_Led", &ledStatus.Led2Sta, TYPE_BOOL, 1},
								{"temperature", &sht20Info.tempreture, TYPE_FLOAT, 1},
								{"humidity", &sht20Info.humidity, TYPE_FLOAT, 1},
								{"errType", &faultTypeReport, TYPE_UCHAR, 1},
							};
unsigned char dataStreamLen = sizeof(dataStream) / sizeof(dataStream[0]);





/*
************************************************************
*	�������ƣ�	Hardware_Init
*
*	�������ܣ�	Ӳ����ʼ��
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		��ʼ����Ƭ�������Լ�����豸
************************************************************
*/
void Hardware_Init(void)
{
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);								//�жϿ�������������

	Delay_Init();																//systick��ʼ��
	
	Led_Init();																	//LED��ʼ��
	
	IIC_Init();																	//���IIC���߳�ʼ��
	
	Usart1_Init(115200); 														//��ʼ������   115200bps
	
	Check_PowerOn(); 															//�ϵ��Լ�

	if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST) == SET) 								//����ǿ��Ź���λ����ʾ
	{
		UsartPrintf(USART_DEBUG, "WARN:	IWDG Reboot\r\n");
		
		RCC_ClearFlag();														//������Ź���λ��־λ
		
		faultTypeReport = faultType = FAULT_REBOOT; 							//���Ϊ��������
	}
	else
	{
		UsartPrintf(USART_DEBUG, "2.SSID: %s,   PSWD: %s\r\n"
								"DEVID: %s,     APIKEY: %s\r\n"
								, netDeviceInfo.staName, netDeviceInfo.staPass
								, oneNetInfo.devID, oneNetInfo.apiKey);
	}
	
	//Iwdg_Init(4, 1250); 														//64��Ƶ��ÿ��625�Σ�����1250�Σ�2s
	
	Timer3_4_Init(TIM3, 49, 35999);												//72MHz��36000��Ƶ-500us��50����ֵ�����ж�����Ϊ500us * 50 = 25ms
	Timer3_4_Init(TIM4, 1999, 35999);											//72MHz��36000��Ƶ-500us��2000����ֵ�����ж�����Ϊ500us * 2000 = 1s
																				//��ʱ�������״̬��־λ
	
	UsartPrintf(USART_DEBUG, "3.Hardware init OK\r\n");							//��ʾ��ʼ�����

}

/*
************************************************************
*	�������ƣ�	main
*
*	�������ܣ�	
*
*	��ڲ�����	��
*
*	���ز�����	0
*
*	˵����		
************************************************************
*/
int main(void)
{
	
	unsigned char *dataPtr;
	unsigned int runTime = 0;
	_Bool sendFlag = 0;

	Hardware_Init();									//Ӳ����ʼ��
	
	NET_DEVICE_IO_Init();								//�����豸IO��ʼ��
	NET_DEVICE_Reset();									//�����豸��λ
	NET_DEVICE_Set_DataMode(DEVICE_CMD_MODE);			//����Ϊ�����շ�ģʽ(����ESP8266Ҫ����AT�ķ��ػ���ƽ̨�·����ݵķ���)
	
	while(1)
	{
		
		if(oneNetInfo.netWork == 1)
		{
/******************************************************************************
			����������
******************************************************************************/
			if(timInfo.timer3Out - runTime >= 1000)									//25sһ��(25ms�ж�)
			{
				runTime = timInfo.timer3Out;
				
				if(sendFlag)
				{
					TIM_Cmd(OS_TIMER, DISABLE);
					OneNet_Status();												//״̬���
					TIM_Cmd(OS_TIMER, ENABLE);
				}
				else
				{
					TIM_Cmd(OS_TIMER, DISABLE);
					OneNet_SendData(HTTP_TYPE3, dataStreamLen);		//���ݷ���
					TIM_Cmd(OS_TIMER, ENABLE);
				}
		
				sendFlag = !sendFlag;
			}
			
/******************************************************************************
			���ͽ������
******************************************************************************/
			if(oneNetInfo.netWork && NET_DEVICE_Get_DataMode() == DEVICE_DATA_MODE)	//�������� �� ���������ģʽʱ
			{
				dataPtr = NET_DEVICE_GetIPD(0);										//���ȴ�����ȡƽ̨�·�������
				if(dataPtr != NULL)													//�������ָ�벻Ϊ�գ�������յ�������
				{
					OneNet_Event(dataPtr);											//���д���
				}
			}
			
/******************************************************************************
			������
******************************************************************************/
			if(checkInfo.SHT20_OK == DEV_OK) 										//ֻ���豸����ʱ���Ż��ȡֵ����ʾ
			{
				SHT20_GetValue();													//�ɼ�����������
			}
			
/******************************************************************************
			������
******************************************************************************/
			if(faultType != FAULT_NONE)												//��������־������
			{
				UsartPrintf(USART_DEBUG, "WARN:	Fault Process\r\n");
				Fault_Process();													//�����������
			}
			
/******************************************************************************
			���ݷ���
******************************************************************************/
			if(oneNetInfo.sendData)
			{
				oneNetInfo.sendData = OneNet_SendData(HTTP_TYPE3, dataStreamLen);
			}
		}
		else
		{
/******************************************************************************
			��ʼ�������豸������ƽ̨
******************************************************************************/
			if(!oneNetInfo.netWork && (checkInfo.NET_DEVICE_OK == DEV_OK))			//��û������ �� ����ģ���⵽ʱ
			{
				NET_DEVICE_Set_DataMode(DEVICE_CMD_MODE);							//����Ϊ�����շ�ģʽ
				
				if(!NET_DEVICE_Init())												//��ʼ�������豸������������
				{
					oneNetInfo.netWork = 1;
					runTime = timInfo.timer3Out;									//����ʱ��
					NET_DEVICE_Set_DataMode(DEVICE_DATA_MODE);
					UsartPrintf(USART_DEBUG, "Tips:	NetWork OK\r\n");
				}
			}
			
/******************************************************************************
			�����豸���
******************************************************************************/
			if(checkInfo.NET_DEVICE_OK == DEV_ERR) 									//�������豸δ�����
			{
				NET_DEVICE_Set_DataMode(DEVICE_CMD_MODE);							//����Ϊ�����շ�ģʽ
				
				if(timerCount >= NET_TIME) 											//����������ӳ�ʱ
				{
					UsartPrintf(USART_DEBUG, "Tips:		Timer Check Err\r\n");
					
					NET_DEVICE_Reset();												//��λ�����豸
					timerCount = 0;													//�������ӳ�ʱ����
					faultType = FAULT_NONE;											//��������־
				}
				
				if(!NET_DEVICE_SendCmd("AT\r\n", "OK"))								//�����豸���
				{
					UsartPrintf(USART_DEBUG, "NET Device :Ok\r\n");
					checkInfo.NET_DEVICE_OK = DEV_OK;								//��⵽�����豸�����
					NET_DEVICE_Set_DataMode(DEVICE_DATA_MODE);						//����Ϊ�����շ�ģʽ
				}
				else
					UsartPrintf(USART_DEBUG, "NET Device :Error\r\n");
			}
		}
	}

}
