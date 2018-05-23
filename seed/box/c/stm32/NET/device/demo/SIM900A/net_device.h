#ifndef _NET_DEVICE_H_
#define _NET_DEVICE_H_







typedef struct
{
	char staName[20];
	char staPass[40];
	
	unsigned short err : 2; //��������
	unsigned short initStep : 4; //��ʼ������
	unsigned short dataType : 4; //�趨���ݷ�������--16��
	unsigned short NET_DEVICE_OK : 1;
	unsigned short reverse : 5;

} NET_DEVICE_INFO;

extern NET_DEVICE_INFO netDeviceInfo;



#define NET_DEVICE_PWRK_ON		GPIO_SetBits(GPIOF, GPIO_Pin_5)
#define NET_DEVICE_PWRK_OFF		GPIO_ResetBits(GPIOF, GPIO_Pin_5)

#define NET_DEVICE_RST_ON		GPIO_ResetBits(GPIOF, GPIO_Pin_6)
#define NET_DEVICE_RST_OFF		GPIO_SetBits(GPIOF, GPIO_Pin_6)

#define NET_DEVICE_TRANS		0 //1-ʹ��͸��ģʽ		0-ʧ��͸��ģʽ


#define DEV_OK		1
#define DEV_ERR		0


void NET_DEVICE_IO_Init(void);

_Bool NET_DEVICE_Init(void);

void NET_DEVICE_Reset(void);

_Bool NET_DEVICE_ReLink(void);

_Bool NET_DEVICE_SendCmd(char *cmd, char *res);

void NET_DEVICE_SendData(unsigned char *data, unsigned short len);

unsigned char *NET_DEVICE_GetIPD(unsigned short timeOut);

void NET_DEVICE_ClrData(void);

unsigned char NET_DEVICE_Check(void);

void NET_DEVICE_ReConfig(unsigned char step);

void NET_DEVICE_Set_DataMode(unsigned char mode);

unsigned char NET_DEVICE_Get_DataMode(void);

#endif
