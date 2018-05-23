/**
	************************************************************
	************************************************************
	************************************************************
	*	�ļ����� 	onenet.c
	*
	*	���ߣ� 		�ż���
	*
	*	���ڣ� 		2016-12-2
	*
	*	�汾�� 		V1.0
	*
	*	˵���� 		��onenetƽ̨�����ݽ�����Э���
	*
	*	�޸ļ�¼��	
	************************************************************
	************************************************************
	************************************************************
**/

//��Ƭ��ͷ�ļ�
#include "stm32f10x.h"

//�����豸
#include "net_device.h"

//Э���ļ�
#include "onenet.h"

//Ӳ������
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "sht20.h"
#include "fault.h"
#include "hwtimer.h"
#include "selfcheck.h"

//ͼƬ�����ļ�
#include "image_2k.h"

//C��
#include <string.h>
#include <stdlib.h>
#include <stdio.h>




ONETNET_INFO oneNetInfo = {"4093253", "f=LzrTF6B77mlSm=vo=kmNp43oI=", 0, 0, 0, 0};
extern DATA_STREAM dataStream[];


/*
************************************************************
*	�������ƣ�	OneNet_Load_DataStream
*
*	�������ܣ�	��������װ
*
*	��ڲ�����	type���������ݵĸ�ʽ
*				send_buf�����ͻ���ָ��
*				len�������������ĸ���
*
*	���ز�����	��
*
*	˵����		��װ��������ʽ
************************************************************
*/
void OneNet_Load_DataStream(unsigned char type, char *send_buf, unsigned char len)
{
	
	unsigned char count = 0;
	char stream_buf[50];
	char *ptr = send_buf;

	switch(type)
	{
		case HTTP_TYPE1:
			
			UsartPrintf(USART_DEBUG, "Tips:	OneNet_SendData-HTTP_TYPE1\r\n");
		
			strcpy(send_buf, "{\"datastreams\":[");
			for(; count < len; count++)
			{
				if(dataStream[count].flag) //���ʹ�ܷ��ͱ�־λ
				{
					memset(stream_buf, 0, sizeof(stream_buf));
					switch((unsigned char)dataStream[count].dataType)
					{
						case TYPE_BOOL:
							sprintf(stream_buf, "{\"id\":\"%s\",\"datapoints\":[{\"value\":%d}]},", dataStream[count].name, *(_Bool *)dataStream[count].data);
						break;
						
						case TYPE_CHAR:
							sprintf(stream_buf, "{\"id\":\"%s\",\"datapoints\":[{\"value\":%d}]},", dataStream[count].name, *(char *)dataStream[count].data);
						break;
						
						case TYPE_UCHAR:
							sprintf(stream_buf, "{\"id\":\"%s\",\"datapoints\":[{\"value\":%d}]},", dataStream[count].name, *(unsigned char *)dataStream[count].data);
						break;
						
						case TYPE_SHORT:
							sprintf(stream_buf, "{\"id\":\"%s\",\"datapoints\":[{\"value\":%d}]},", dataStream[count].name, *(short *)dataStream[count].data);
						break;
						
						case TYPE_USHORT:
							sprintf(stream_buf, "{\"id\":\"%s\",\"datapoints\":[{\"value\":%d}]},", dataStream[count].name, *(unsigned short *)dataStream[count].data);
						break;
						
						case TYPE_INT:
							sprintf(stream_buf, "{\"id\":\"%s\",\"datapoints\":[{\"value\":%d}]},", dataStream[count].name, *(int *)dataStream[count].data);
						break;
						
						case TYPE_UINT:
							sprintf(stream_buf, "{\"id\":\"%s\",\"datapoints\":[{\"value\":%d}]},", dataStream[count].name, *(unsigned int *)dataStream[count].data);
						break;
						
						case TYPE_LONG:
							sprintf(stream_buf, "{\"id\":\"%s\",\"datapoints\":[{\"value\":%ld}]},", dataStream[count].name, *(long *)dataStream[count].data);
						break;
						
						case TYPE_ULONG:
							sprintf(stream_buf, "{\"id\":\"%s\",\"datapoints\":[{\"value\":%ld}]},", dataStream[count].name, *(unsigned long *)dataStream[count].data);
						break;
							
						case TYPE_FLOAT:
							sprintf(stream_buf, "{\"id\":\"%s\",\"datapoints\":[{\"value\":%f}]},", dataStream[count].name, *(float *)dataStream[count].data);
						break;
						
						case TYPE_DOUBLE:
							sprintf(stream_buf, "{\"id\":\"%s\",\"datapoints\":[{\"value\":%f}]},", dataStream[count].name, *(float *)dataStream[count].data);
						break;
						
						case TYPE_GPS:
							sprintf(stream_buf, "{\"id\":\"%s\",\"datapoints\":[{\"value\":{\"lon\":1.1,\"lat\":1.1}}]},", dataStream[count].name);
						break;
					}
					
					strcat(send_buf, stream_buf);
				}
			}
			
			while(*ptr != '\0')					//�ҵ�������
				ptr++;
			*(--ptr) = '\0';					//������','�滻Ϊ������
			
			strcat(send_buf, "]}");
		
		break;
			
		case HTTP_TYPE3:
			
			UsartPrintf(USART_DEBUG, "Tips:	OneNet_SendData-HTTP_TYPE3\r\n");
		
			strcpy(send_buf, "{");
			for(; count < len; count++)
			{
				if(dataStream[count].flag) //���ʹ�ܷ��ͱ�־λ
				{
					memset(stream_buf, 0, sizeof(stream_buf));
					switch((unsigned char)dataStream[count].dataType)
					{
						case TYPE_BOOL:
							sprintf(stream_buf, "\"%s\":%d,", dataStream[count].name, *(_Bool *)dataStream[count].data);
						break;
							
						case TYPE_CHAR:
							sprintf(stream_buf, "\"%s\":%d,", dataStream[count].name, *(char *)dataStream[count].data);
						break;
							
						case TYPE_UCHAR:
							sprintf(stream_buf, "\"%s\":%d,", dataStream[count].name, *(unsigned char *)dataStream[count].data);
						break;
							
						case TYPE_SHORT:
							sprintf(stream_buf, "\"%s\":%d,", dataStream[count].name, *(short *)dataStream[count].data);
						break;
							
						case TYPE_USHORT:
							sprintf(stream_buf, "\"%s\":%d,", dataStream[count].name, *(unsigned short *)dataStream[count].data);
						break;
							
						case TYPE_INT:
							sprintf(stream_buf, "\"%s\":%d,", dataStream[count].name, *(int *)dataStream[count].data);
						break;
							
						case TYPE_UINT:
							sprintf(stream_buf, "\"%s\":%d,", dataStream[count].name, *(unsigned int *)dataStream[count].data);
						break;
							
						case TYPE_LONG:
							sprintf(stream_buf, "\"%s\":%ld,", dataStream[count].name, *(long *)dataStream[count].data);
						break;
							
						case TYPE_ULONG:
							sprintf(stream_buf, "\"%s\":%ld,", dataStream[count].name, *(unsigned long *)dataStream[count].data);
						break;
							
						case TYPE_FLOAT:
							sprintf(stream_buf, "\"%s\":%f,", dataStream[count].name, *(float *)dataStream[count].data);
						break;
							
						case TYPE_DOUBLE:
							sprintf(stream_buf, "\"%s\":%f,", dataStream[count].name, *(float *)dataStream[count].data);
						break;
					}
					
					strcat(send_buf, stream_buf);
				}
			}
			
			while(*ptr != '\0')					//�ҵ�������
				ptr++;
			*(--ptr) = '\0';					//������','�滻Ϊ������
			
			strcat(send_buf, "}");
		
		break;
		
		case HTTP_TYPE4:
			
			UsartPrintf(USART_DEBUG, "Tips:	OneNet_SendData-HTTP_TYPE4\r\n");
		
			strcpy(send_buf, "{");
			for(; count < len; count++)
			{
				if(dataStream[count].flag) //���ʹ�ܷ��ͱ�־λ
				{
					memset(stream_buf, 0, sizeof(stream_buf));
					switch((unsigned char)dataStream[count].dataType)
					{
						case TYPE_BOOL:
							sprintf(stream_buf, "\"%s\":{\"2016-08-10T12:31:17\":%d},", dataStream[count].name, *(_Bool *)dataStream[count].data);
						break;
						
						case TYPE_CHAR:
							sprintf(stream_buf, "\"%s\":{\"2016-08-10T12:31:17\":%d},", dataStream[count].name, *(char *)dataStream[count].data);
						break;
						
						case TYPE_UCHAR:
							sprintf(stream_buf, "\"%s\":{\"2016-08-10T12:31:17\":%d},", dataStream[count].name, *(unsigned char *)dataStream[count].data);
						break;
						
						case TYPE_SHORT:
							sprintf(stream_buf, "\"%s\":{\"2016-08-10T12:31:17\":%d},", dataStream[count].name, *(short *)dataStream[count].data);
						break;
						
						case TYPE_USHORT:
							sprintf(stream_buf, "\"%s\":{\"2016-08-10T12:31:17\":%d},", dataStream[count].name, *(unsigned short *)dataStream[count].data);
						break;
						
						case TYPE_INT:
							sprintf(stream_buf, "\"%s\":{\"2016-08-10T12:31:17\":%d},", dataStream[count].name, *(int *)dataStream[count].data);
						break;
						
						case TYPE_UINT:
							sprintf(stream_buf, "\"%s\":{\"2016-08-10T12:31:17\":%d},", dataStream[count].name, *(unsigned int *)dataStream[count].data);
						break;
						
						case TYPE_LONG:
							sprintf(stream_buf, "\"%s\":{\"2016-08-10T12:31:17\":%ld},", dataStream[count].name, *(long *)dataStream[count].data);
						break;
						
						case TYPE_ULONG:
							sprintf(stream_buf, "\"%s\":{\"2016-08-10T12:31:17\":%ld},", dataStream[count].name, *(unsigned long *)dataStream[count].data);
						break;
						
						case TYPE_FLOAT:
							sprintf(stream_buf, "\"%s\":{\"2016-08-10T12:31:17\":%f},", dataStream[count].name, *(float *)dataStream[count].data);
						break;
						
						case TYPE_DOUBLE:
							sprintf(stream_buf, "\"%s\":{\"2016-08-10T12:31:17\":%f}", dataStream[count].name, *(float *)dataStream[count].data);
						break;
					}
					
					strcat(send_buf, stream_buf);
				}
			}
			
			while(*ptr != '\0')					//�ҵ�������
				ptr++;
			*(--ptr) = '\0';					//������','�滻Ϊ������
			
			strcat(send_buf, "}");
		
		break;
		
		case HTTP_TYPE5:
			
			UsartPrintf(USART_DEBUG, "Tips:	OneNet_SendData-HTTP_TYPE5\r\n");
		
			strcpy(send_buf, ",;");
			for(; count < len; count++)
			{
				if(dataStream[count].flag) //���ʹ�ܷ��ͱ�־λ
				{
					memset(stream_buf, 0, sizeof(stream_buf));
					switch((unsigned char)dataStream[count].dataType)
					{
						case TYPE_BOOL:
							sprintf(stream_buf, "%s,%d;", dataStream[count].name, *(_Bool *)dataStream[count].data);
						break;
						
						case TYPE_CHAR:
							sprintf(stream_buf, "%s,%d;", dataStream[count].name, *(char *)dataStream[count].data);
						break;
						
						case TYPE_UCHAR:
							sprintf(stream_buf, "%s,%d;", dataStream[count].name, *(unsigned char *)dataStream[count].data);
						break;
						
						case TYPE_SHORT:
							sprintf(stream_buf, "%s,%d;", dataStream[count].name, *(short *)dataStream[count].data);
						break;
						
						case TYPE_USHORT:
							sprintf(stream_buf, "%s,%d;", dataStream[count].name, *(unsigned short *)dataStream[count].data);
						break;
						
						case TYPE_INT:
							sprintf(stream_buf, "%s,%d;", dataStream[count].name, *(int *)dataStream[count].data);
						break;
						
						case TYPE_UINT:
							sprintf(stream_buf, "%s,%d;", dataStream[count].name, *(unsigned int *)dataStream[count].data);
						break;
						
						case TYPE_LONG:
							sprintf(stream_buf, "%s,%ld;", dataStream[count].name, *(long *)dataStream[count].data);
						break;
						
						case TYPE_ULONG:
							sprintf(stream_buf, "%s,%ld;", dataStream[count].name, *(unsigned long *)dataStream[count].data);
						break;
						
						case TYPE_FLOAT:
							sprintf(stream_buf, "%s,%f;", dataStream[count].name, *(float *)dataStream[count].data);
						break;
						
						case TYPE_DOUBLE:
							sprintf(stream_buf, "%s,%f;", dataStream[count].name, *(float *)dataStream[count].data);
						break;
					}
				}
				
				strcat(send_buf, stream_buf);
			}
		
		break;
	}

}

/*
************************************************************
*	�������ƣ�	OneNet_SendData
*
*	�������ܣ�	�ϴ����ݵ�ƽ̨
*
*	��ڲ�����	type���������ݵĸ�ʽ
*
*	���ز�����	��
*
*	˵����		������Ҫ����Ϊ�Լ���Ҫ�ϴ������ݣ����ո�ʽ��д
************************************************************
*/
_Bool OneNet_SendData(HTTP_TYPE  type, unsigned char len)
{
	
	char sendBuf[640];
	char streams[600];
	unsigned short strLen = 0;
	
	if(!oneNetInfo.netWork || NET_DEVICE_Get_DataMode() != DEVICE_DATA_MODE)
		return 1;
	
	NET_DEVICE_Set_DataMode(DEVICE_CMD_MODE);
	
	memset(sendBuf, 0, sizeof(sendBuf));
	memset(streams, 0, sizeof(streams));
	
	OneNet_Load_DataStream(type, streams, len);
	
	strLen = strlen(streams);
	
	//�������
	sprintf(sendBuf, "POST /devices/%s/datapoints?type=%d HTTP/1.1\r\napi-key:%s\r\nHost:api.heclouds.com\r\n"
					"Content-Length:%d\r\n\r\n",
	
					oneNetInfo.devID, type, oneNetInfo.apiKey, strLen);
	
	strcat(sendBuf, streams);
	
	NET_DEVICE_SendData((unsigned char *)sendBuf, strlen(sendBuf));
	
	faultTypeReport = FAULT_NONE; //����֮��������
	
	NET_DEVICE_Set_DataMode(DEVICE_DATA_MODE);
	
	return 0;
	
}

/*
************************************************************
*	�������ƣ�	OneNet_Status
*
*	�������ܣ�	����״̬���
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void OneNet_Status(void)
{
	
	unsigned char errType = 0;
	
	if(!oneNetInfo.netWork || NET_DEVICE_Get_DataMode() != DEVICE_DATA_MODE)	//�������Ϊ���� �� ��Ϊ�����շ�ģʽ
		return;
	
	NET_DEVICE_Set_DataMode(DEVICE_CMD_MODE);									//����Ϊ�����շ�ģʽ
	
	errType = NET_DEVICE_Check();
	
	if(errType == CHECK_CLOSED || errType == CHECK_GOT_IP)
	{
		faultTypeReport = faultType = FAULT_EDP;								//���ΪЭ�����
		
		oneNetInfo.errCount++;
	}
	else if(errType == CHECK_NO_DEVICE)
	{
		faultTypeReport = faultType = FAULT_NODEVICE;							//���Ϊ�豸����
		
		oneNetInfo.errCount++;
	}
	else
	{
		faultTypeReport = faultType = FAULT_NONE;								//�޴���
	}
	
	NET_DEVICE_ClrData();														//�������
	
	NET_DEVICE_Set_DataMode(DEVICE_DATA_MODE);									//����Ϊ�����շ�ģʽ
	
	

}

/*
************************************************************
*	�������ƣ�	OneNet_Event
*
*	�������ܣ�	ƽ̨�������ݼ��
*
*	��ڲ�����	dataPtr��ƽ̨���ص�����
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void OneNet_Event(unsigned char *dataPtr)
{

	//������������Ƿ��ͳɹ�
	if(strstr((char *)dataPtr, "succ"))
	{
		UsartPrintf(USART_DEBUG, "Tips:		Send OK\r\n");
		oneNetInfo.errCount = 0;
	}
	else
	{
		UsartPrintf(USART_DEBUG, "Tips:		Send Err\r\n");
		oneNetInfo.errCount++;
	}

}
