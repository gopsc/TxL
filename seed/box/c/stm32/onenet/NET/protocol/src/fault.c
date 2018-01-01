#include "fault.h"

#include "usart.h"
#include "onenet.h"
#include "delay.h"
#include "selfcheck.h"

#include "net_device.h"







unsigned char faultType = FAULT_NONE; //��ʼΪ�޴�������
unsigned char faultTypeReport = FAULT_NONE; //������������õ�




void Fault_Process(void)
{

	NET_DEVICE_Set_DataMode(DEVICE_CMD_MODE);
	
	switch(faultType)
	{
		case FAULT_NONE:
			
			UsartPrintf(USART_DEBUG, "WARN:	FAULT_NONE\r\n");
		
		break;
		
		case FAULT_REBOOT: //���������Ĵ���
			
			UsartPrintf(USART_DEBUG, "WARN:	FAULT_REBOOT\r\n");
		
			faultType = FAULT_NONE;
			
			//�����������豸�ᱻ��λ�����������³�ʼ��
		
		break;
		
		case FAULT_EDP: //Э�����Ĵ���
			
			UsartPrintf(USART_DEBUG, "WARN:	FAULT_EDP\r\n");
		
			faultType = FAULT_NONE;
			
			NET_DEVICE_ReLink();
			oneNetInfo.netWork = 0; //������������ƽ̨
		
		break;
		
		case FAULT_NODEVICE: //�豸����Ĵ���
			
			UsartPrintf(USART_DEBUG, "WARN:	FAULT_NODEVICE\r\n");
		
			faultType = FAULT_NONE;

			checkInfo.NET_DEVICE_OK = DEV_ERR;
			NET_DEVICE_Reset();
			NET_DEVICE_ReConfig(0);
			oneNetInfo.netWork = 0;
		
		break;
		
		default:
			break;
	}
	
	NET_DEVICE_Set_DataMode(DEVICE_DATA_MODE);

}
