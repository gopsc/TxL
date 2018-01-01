#include "net_io.h"

#include "delay.h"

#include <string.h>




NET_IO_INFO netIOInfo;




void NET_IO_Init(void)
{

	GPIO_InitTypeDef gpioInitStructure;
	USART_InitTypeDef usartInitStructure;
	NVIC_InitTypeDef nvicInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	gpioInitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	gpioInitStructure.GPIO_Mode = GPIO_Mode_AF;
	gpioInitStructure.GPIO_OType = GPIO_OType_PP;
	gpioInitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	gpioInitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	
	GPIO_Init(GPIOB, &gpioInitStructure);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
	
	usartInitStructure.USART_BaudRate = 115200;
	usartInitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������������
	usartInitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; //���ͺͽ���
	usartInitStructure.USART_Parity = USART_Parity_No; //����żУ��
	usartInitStructure.USART_StopBits = USART_StopBits_1; //1λֹͣλ
	usartInitStructure.USART_WordLength = USART_WordLength_8b; //8λ���ݳ���
	
	USART_Init(USART3, &usartInitStructure);
	
	USART_Cmd(USART3, ENABLE); //ʹ�ܴ���
	USART_ClearFlag(USART3, USART_FLAG_TC);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); //���������ж�
	
	nvicInitStructure.NVIC_IRQChannel = USART3_IRQn;
	nvicInitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	nvicInitStructure.NVIC_IRQChannelSubPriority = 0;
	nvicInitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&nvicInitStructure);
	
	NET_IO_ClearRecive();

}

void NET_IO_Send(unsigned char *str, unsigned short len)
{

	unsigned short count = 0;
	
	for(; count < len; count++)
	{
		USART_SendData(NET_IO, *str++);
		while(USART_GetFlagStatus(NET_IO, USART_FLAG_TC) == RESET);
	}

}

//����0����ok		����1����δ���
_Bool NET_IO_WaitRecive(void)
{

	if(netIOInfo.dataLen == 0) //������ռ���Ϊ0 ��˵��û�д��ڽ��������У�����ֱ����������������
		return REV_WAIT;
		
	if(netIOInfo.dataLen == netIOInfo.dataLenPre) //�����һ�ε�ֵ�������ͬ����˵���������
	{
		netIOInfo.dataLen = 0; //��0���ռ���
			
		return REV_OK; //����
	}
		
	netIOInfo.dataLenPre = netIOInfo.dataLen; //��Ϊ��ͬ
	
	return REV_WAIT;

}

void NET_IO_ClearRecive(void)
{

	netIOInfo.dataLen = 0;
	
	memset(netIOInfo.buf, 0, sizeof(netIOInfo.buf));

}

void USART3_IRQHandler(void)
{
	
	OSIntEnter();

	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //�����ж�
	{
		if(netIOInfo.dataLen >= sizeof(netIOInfo.buf))	netIOInfo.dataLen = 0; //��ֹ���ڱ�ˢ��
		netIOInfo.buf[netIOInfo.dataLen++] = USART3->DR;
		
		USART_ClearFlag(USART3, USART_FLAG_RXNE);
	}
	
	OSIntExit();

}
