#include "Uart.h"


void Uart1_Init(void)		//115200bps@24.000MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
	AUXR |= 0x04;		//定时器时钟1T模式
	T2L = 0x8F;			//设置定时初始值
	T2H = 0xFD;			//设置定时初始值
	AUXR |= 0x10;		//定时器2开始计时
	ES = 1;
	EA = 1;
}

void Uart1_sendbyte(uchar byte)  //串口发送单个字节
{
	SBUF=byte;
	while(!TI);
	TI = 0;
}

void Uart1_String(uchar *p)    //串口发送字符串
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


//*********串口2**********//
void Uart2_Init(void)		//9600bps@24.000MHz
{
	S2CON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x04;		//定时器时钟1T模式
	T2L = 0x8F;			//设置定时初始值
	T2H = 0xFD;			//设置定时初始值
	AUXR |= 0x10;		//定时器2开始计时
	ES2 = 1;
}

void Uart2_sendbyte(uchar byte)  //串口发送单个字节
{
	S2BUF=byte;
	while(!S2TI);
	S2TI = 0;
}

void Uart2_String(uchar *p)    //串口发送字符串
{
	while(*p)
	{
		Uart2_sendbyte(*p++);
	}
}

//串口2发送字符串数据
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

//**********串口3**********//
void Uart3_Init(void)		//115200bps@24.000MHz
{
	S3CON = 0x10;		//8位数据,可变波特率
	S3CON &= 0xBF;		//串口3选择定时器2为波特率发生器
	AUXR |= 0x04;		//定时器时钟1T模式
	T2L = 0xCC;			//设置定时初始值
	T2H = 0xFF;			//设置定时初始值
	AUXR |= 0x10;		//定时器2开始计时
	ES3 = 1;
}

void Uart3_sendbyte(uchar byte)  //串口发送单个字节
{
	S2BUF=byte;
	while(!S2TI);
	S2TI = 0;
}

void Uart3_String(uchar *p)    //串口发送字符串
{
	while(*p)
	{
		Uart3_sendbyte(*p++);
	}
}

