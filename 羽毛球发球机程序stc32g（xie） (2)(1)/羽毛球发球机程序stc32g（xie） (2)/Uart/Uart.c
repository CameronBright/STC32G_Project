#include "Uart.h"


void Uart1_Init(void)		//115200bps@24.000MHz
{
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x01;		//����1ѡ��ʱ��2Ϊ�����ʷ�����
	AUXR |= 0x04;		//��ʱ��ʱ��1Tģʽ
	T2L = 0x8F;			//���ö�ʱ��ʼֵ
	T2H = 0xFD;			//���ö�ʱ��ʼֵ
	AUXR |= 0x10;		//��ʱ��2��ʼ��ʱ
	ES = 1;
	EA = 1;
}

void Uart1_sendbyte(uchar byte)  //���ڷ��͵����ֽ�
{
	SBUF=byte;
	while(!TI);
	TI = 0;
}

void Uart1_String(uchar *p)    //���ڷ����ַ���
{
	while(*p)
	{
		Uart1_sendbyte(*p++);
	}
}

void Uart1_Isr() interrupt 4
{
	if(RI)
	{
		RI=0;	
	}
		
}


//*********����2**********//
void Uart2_Init(void)		//9600bps@24.000MHz
{
	S2CON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x04;		//��ʱ��ʱ��1Tģʽ
	T2L = 0x8F;			//���ö�ʱ��ʼֵ
	T2H = 0xFD;			//���ö�ʱ��ʼֵ
	AUXR |= 0x10;		//��ʱ��2��ʼ��ʱ
	ES2 = 1;
}

void Uart2_sendbyte(uchar byte)  //���ڷ��͵����ֽ�
{
	S2BUF=byte;
	while(!S2TI);
	S2TI = 0;
}

void Uart2_String(uchar *p)    //���ڷ����ַ���
{
	while(*p)
	{
		Uart2_sendbyte(*p++);
	}
}

//����2�����ַ�������
void UART2_SendString(uchar *DAT, uchar len)
{
  uchar i;
  for(i = 0; i < len; i++)
  {
    Uart2_sendbyte(*DAT++);
  }
}


void Uart2_Isr() interrupt 8
{
	if(S2RI)
	{
		S2RI=0;	
	}
}

//**********����3**********//
void Uart3_Init(void)		//115200bps@24.000MHz
{
	S3CON = 0x10;		//8λ����,�ɱ䲨����
	S3CON &= 0xBF;		//����3ѡ��ʱ��2Ϊ�����ʷ�����
	AUXR |= 0x04;		//��ʱ��ʱ��1Tģʽ
	T2L = 0xCC;			//���ö�ʱ��ʼֵ
	T2H = 0xFF;			//���ö�ʱ��ʼֵ
	AUXR |= 0x10;		//��ʱ��2��ʼ��ʱ
	ES3 = 1;
}

void Uart3_sendbyte(uchar byte)  //���ڷ��͵����ֽ�
{
	S2BUF=byte;
	while(!S2TI);
	S2TI = 0;
}

void Uart3_String(uchar *p)    //���ڷ����ַ���
{
	while(*p)
	{
		Uart3_sendbyte(*p++);
	}
}

