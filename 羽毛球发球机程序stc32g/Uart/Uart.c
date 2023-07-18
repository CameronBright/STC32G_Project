#include "Uart.h"
void UartInit(void)		//9600bps@12.000MHz
{
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x01;		//����1ѡ��ʱ��2Ϊ�����ʷ�����
	AUXR &= 0xFB;		//��ʱ��ʱ��12Tģʽ
	T2L = 0xE6;		//���ö�ʱ��ʼֵ
	T2H = 0xFF;		//���ö�ʱ��ʼֵ
	AUXR |= 0x10;		//��ʱ��2��ʼ��ʱ
	ES=1;
	EA=1;
}

void Uart_sendbyte(uchar byte)  //���ڷ��͵����ֽ�
{
	SBUF=byte;
	while(!TI);
	TI = 0;
}

void Uart_String(uchar *p)    //���ڷ����ַ���
{
	while(*p)
	{
		Uart_sendbyte(*p++);
	}
}

void Uart1_Isr() interrupt 4
{
	if(RI)
	{
		RI=0;	
	}
		
}