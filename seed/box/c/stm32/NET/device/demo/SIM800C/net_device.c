/**
	************************************************************
	************************************************************
	************************************************************
	*	�ļ����� 	net_device.c
	*
	*	���ߣ� 		�ż���
	*
	*	���ڣ� 		2016-11-23
	*
	*	�汾�� 		V1.0
	*
	*	˵���� 		�����豸Ӧ�ò�
	*
	*	�޸ļ�¼��	
	************************************************************
	************************************************************
	************************************************************
**/

#include "stm32f10x.h"	//��Ƭ��ͷ�ļ�

#include "net_device.h"	//�����豸Ӧ�ò�
#include "net_io.h"		//�����豸����IO��

//Ӳ������
#include "delay.h"
#include "led.h"
#include "usart.h"

//C��
#include <string.h>
#include <stdlib.h>
#include <stdio.h>




NET_DEVICE_INFO netDeviceInfo = {"ONENET", "IOT@Chinamobile", 0, 0, 0, 0}; //





void SIM800C_PowerCtl(void)
{
	
	RTOS_TimeDly(200);
	SIM800C_PWRK_OFF;
    RTOS_TimeDly(240);
    SIM800C_PWRK_ON;
    RTOS_TimeDly(140);

}

/*
************************************************************
*	�������ƣ�	NET_DEVICE_IO_Init
*
*	�������ܣ�	��ʼ�������豸IO��
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		��ʼ�������豸�Ŀ������š������շ����ܵ�
************************************************************
*/
void NET_DEVICE_IO_Init(void)
{

	GPIO_InitTypeDef gpioInitStruct;
    
	//ʹ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);
    
	//PowerKey
	gpioInitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	gpioInitStruct.GPIO_Pin =  GPIO_Pin_4;
	gpioInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &gpioInitStruct);
	
	//Status
	gpioInitStruct.GPIO_Pin =  GPIO_Pin_7;
	GPIO_Init(GPIOA, &gpioInitStruct);
	
	SIM800C_PWRK_ON;
	SIM800C_PowerCtl();
	
	NET_IO_Init();											//�����豸����IO���ʼ��

}

/*
************************************************************
*	�������ƣ�	NET_DEVICE_Init
*
*	�������ܣ�	�����豸��ʼ��
*
*	��ڲ�����	��
*
*	���ز�����	���س�ʼ�����
*
*	˵����		0-�ɹ�		1-ʧ��
************************************************************
*/
_Bool NET_DEVICE_Init(void)
{
	
	switch(netDeviceInfo.initStep)
	{
		case 0:
			
			UsartPrintf(USART_DEBUG, "AT+CSMINS?\r\n");
			if(!NET_DEVICE_SendCmd("AT+CSMINS?\r\n", "OK"))											//SIM���Ƿ����
				netDeviceInfo.initStep++;
		
		break;
			
		case 1:
			
			UsartPrintf(USART_DEBUG, "AT+CIPHEAD=1\r\n");
			if(!NET_DEVICE_SendCmd("AT+CIPHEAD=1\r\n", "OK"))										//��ʾ����ͷ
				netDeviceInfo.initStep++;
		
		break;
			
		case 2:
			
			UsartPrintf(USART_DEBUG, "AT+CGCLASS=\"B\"\r\n");
			if(!NET_DEVICE_SendCmd("AT+CGCLASS=\"B\"\r\n", "OK"))									//����GPRS�ƶ�̨���ΪB,֧�ְ����������ݽ���
				netDeviceInfo.initStep++;
		
		break;
			
		case 3:
			
			UsartPrintf(USART_DEBUG, "AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n");
			if(!NET_DEVICE_SendCmd("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n", "OK"))						//����PDP������,��������Э��,��������Ϣ
				netDeviceInfo.initStep++;
		
		break;
			
		case 4:
			
			UsartPrintf(USART_DEBUG, "AT+CGATT=1\r\n");
			if(!NET_DEVICE_SendCmd("AT+CGATT=1\r\n", "OK"))											//����GPRSҵ��
				netDeviceInfo.initStep++;
		
		break;
			
		case 5:
			
			UsartPrintf(USART_DEBUG, "AT+CIPCSGP=1,\"CMNET\"\r\n");
			if(!NET_DEVICE_SendCmd("AT+CIPCSGP=1,\"CMNET\"\r\n", "OK"))								//����ΪGPRS����ģʽ
				netDeviceInfo.initStep++;
		
		break;
			
		case 6:
			
			RTOS_TimeDly(200);
			UsartPrintf(USART_DEBUG, "AT+CIPSTART=\"TCP\",\"183.230.40.39\",876\r\n");
			if(!NET_DEVICE_SendCmd("AT+CIPSTART=\"TCP\",\"183.230.40.39\",876\r\n", "CONNECT"))		//����ƽ̨
				netDeviceInfo.initStep++;
		
		break;
			
		default:
			break;
	}
    
	if(netDeviceInfo.initStep == 7)
		return 0;
	else
		return 1;

}

/*
************************************************************
*	�������ƣ�	NET_DEVICE_Reset
*
*	�������ܣ�	�����豸��λ
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void NET_DEVICE_Reset(void)
{
	


}

/*
************************************************************
*	�������ƣ�	NET_DEVICE_ReLink
*
*	�������ܣ�	����ƽ̨
*
*	��ڲ�����	��
*
*	���ز�����	�������ӽ��
*
*	˵����		0-�ɹ�		1-ʧ��
************************************************************
*/
_Bool NET_DEVICE_ReLink(void)
{
	
	_Bool status = 0;
	
	NET_DEVICE_SendCmd("AT+CIPCLOSE=1\r\n", "CLOSE OK");										//�ر�����
	RTOS_TimeDly(100);
	
	UsartPrintf(USART_DEBUG, "AT+CIPSTART=\"TCP\",\"183.230.40.39\",876\r\n");
	status = NET_DEVICE_SendCmd("AT+CIPSTART=\"TCP\",\"183.230.40.39\",876\r\n", "CONNECT");	//����ƽ̨
	
	return status;

}

/*
************************************************************
*	�������ƣ�	NET_DEVICE_SendCmd
*
*	�������ܣ�	�������豸����һ��������ȴ���ȷ����Ӧ
*
*	��ڲ�����	cmd����Ҫ���͵�����		res����Ҫ��������Ӧ
*
*	���ز�����	�������ӽ��
*
*	˵����		0-�ɹ�		1-ʧ��
************************************************************
*/
_Bool NET_DEVICE_SendCmd(char *cmd, char *res)
{
	
	unsigned char timeOut = 200;
	
	NET_IO_Send((unsigned char *)cmd, strlen((const char *)cmd));	//д��������豸
	
	while(timeOut--)												//�ȴ�
	{
		if(NET_IO_WaitRecive() == REV_OK)							//����յ�����
		{
			if(strstr((const char *)netIOInfo.buf, res) != NULL)	//����������ؼ���
			{
				NET_IO_ClearRecive();								//��ջ���
				
				return 0;
			}
		}
		
		RTOS_TimeDly(2);											//����ȴ�
	}
	
	return 1;

}

/*
************************************************************
*	�������ƣ�	NET_DEVICE_SendData
*
*	�������ܣ�	ʹ�����豸�������ݵ�ƽ̨
*
*	��ڲ�����	data����Ҫ���͵�����
*				len�����ݳ���
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void NET_DEVICE_SendData(unsigned char *data, unsigned short len)
{

	char cmdBuf[30];

	RTOS_TimeDly(10);								//�ȴ�һ��
	
	NET_IO_ClearRecive();							//��ս��ջ���
	sprintf(cmdBuf, "AT+CIPSEND=%d\r\n", len);		//��������
	if(!NET_DEVICE_SendCmd(cmdBuf, ">"))			//�յ���>��ʱ���Է�������
	{
		NET_IO_Send(data, len);  					//�����豸������������
	}

}

/*
************************************************************
*	�������ƣ�	NET_DEVICE_GetIPD
*
*	�������ܣ�	��ȡƽ̨���ص�����
*
*	��ڲ�����	�ȴ���ʱ��(����10ms)
*
*	���ز�����	ƽ̨���ص�ԭʼ����
*
*	˵����		��ͬ�����豸���صĸ�ʽ��ͬ����Ҫȥ����
				��ESP8266�ķ��ظ�ʽΪ	"+IPD,x:yyy"	x�������ݳ��ȣ�yyy����������
************************************************************
*/
unsigned char *NET_DEVICE_GetIPD(unsigned short timeOut)
{

	unsigned char byte = 0, count = 0;
	char sByte[5];
	char *ptrIPD;
	
	do
	{
		if(NET_IO_WaitRecive() == REV_OK)								//����������
		{
			ptrIPD = strstr((char *)netIOInfo.buf, "IPD,");				//������IPD��ͷ
			if(ptrIPD == NULL)											//���û�ҵ���������IPDͷ���ӳ٣�������Ҫ�ȴ�һ�ᣬ�����ᳬ���趨��ʱ��
			{
				//UsartPrintf(USART_DEBUG, "\"IPD\" not found\r\n");
			}
			else
			{
				ptrIPD = strstr(ptrIPD, ",");ptrIPD++;					//�ҵ�','  ��Ȼ��ƫ�Ƶ���һ���ַ����������ݳ��ȵĵ�һ������
				
				while(*ptrIPD != ':')									//��','��':' ֮��Ķ������ݳ��ȵ�����
				{
					sByte[count++] = *ptrIPD++;
				}
				byte = (unsigned char)atoi(sByte);						//���ַ�תΪ��ֵ��ʽ
				
				ptrIPD++;												//��ʱptrIPDָ�뻹ָ��':'��������Ҫƫ�Ƶ��¸�����
				for(count = 0; count < byte; count++)					//��������
				{
					netIOInfo.buf[count] = *ptrIPD++;
				}
				memset(netIOInfo.buf + byte,
						0, sizeof(netIOInfo.buf) - byte);				//����ߵ��������
				return netIOInfo.buf;									//������ɣ���������ָ��
			}
		}
		
		RTOS_TimeDly(2);												//��ʱ�ȴ�
	} while(timeOut--);
	
	return NULL;														//��ʱ��δ�ҵ������ؿ�ָ��

}

/*
************************************************************
*	�������ƣ�	NET_DEVICE_ClrData
*
*	�������ܣ�	��������豸���ݽ��ջ���
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void NET_DEVICE_ClrData(void)
{

	NET_IO_ClearRecive();	//��ջ���

}

/*
************************************************************
*	�������ƣ�	NET_DEVICE_Check
*
*	�������ܣ�	��������豸����״̬
*
*	��ڲ�����	��
*
*	���ز�����	����״̬
*
*	˵����		
************************************************************
*/
unsigned char NET_DEVICE_Check(void)
{
	
	unsigned char status = 0;
	
	
	return status;

}

/*
************************************************************
*	�������ƣ�	NET_DEVICE_ReConfig
*
*	�������ܣ�	�豸�����豸��ʼ���Ĳ���
*
*	��ڲ�����	����ֵ
*
*	���ز�����	��
*
*	˵����		�ú������õĲ����������豸��ʼ������õ�
************************************************************
*/
void NET_DEVICE_ReConfig(unsigned char step)
{

	netDeviceInfo.initStep = step;

}

/*
************************************************************
*	�������ƣ�	NET_DEVICE_Set_DataMode
*
*	�������ܣ�	�����豸�����շ�ģʽ
*
*	��ڲ�����	ģʽ
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void NET_DEVICE_Set_DataMode(unsigned char mode)
{

	netDeviceInfo.dataType = mode;

}

/*
************************************************************
*	�������ƣ�	NET_DEVICE_Get_DataMode
*
*	�������ܣ�	��ȡ�豸�����շ�ģʽ
*
*	��ڲ�����	��
*
*	���ز�����	ģʽ
*
*	˵����		
************************************************************
*/
unsigned char NET_DEVICE_Get_DataMode(void)
{

	return netDeviceInfo.dataType;

}

