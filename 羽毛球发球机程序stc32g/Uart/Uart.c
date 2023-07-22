#include "Uart.h"
void UartInit(void)		//9600bps@24.000MHz
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

void Uart_sendbyte(uchar byte)  //串口发送单个字节
{
	SBUF=byte;
	while(!TI);
	TI = 0;
}

void Uart_String(uchar *p)    //串口发送字符串
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