#include "Wave.h"

unsigned long TF_Count = 0;

unsigned char Wave_flag = 0;

unsigned int Wave_ultrasonic()
{
	unsigned char i = 0;
	unsigned long Wave_Distance = 0;

	Tx=1;	
	Delay10us();
	Delay10us();	
	Tx=0;

	
	while(!Rx);
	TR0=0;	
	TH0=0;
	TL0=0;
	TR0=1;
	
	while(Rx);
	
	TR0 = 0;
	
	Wave_Distance = (unsigned long)(TF_Count * 65534 + (( TH0 << 8) | TL0))* 0.00075;   //24分频 1MHZ
	
	TH0=0;
	TL0=0;
	TF_Count = 0;
	if(Wave_Distance <= 300)	
		return Wave_Distance;
	else
		return 255;
}



void Delay10us()		//@24.000MHz
{
	unsigned char data i;

	_nop_();
	_nop_();
	i = 63;
	while (--i);
}




void Timer0Init(void)		//1毫秒@24.000MHz
{
	AUXR |= 0x80;			//定时器时钟1T模式
	TMOD &= 0xF0;			//设置定时器模式
	TL0 = 0;		//设置定时初始值
	TH0 = 0;		//设置定时初始值
	TF0 = 0;		//清除TF0标志
	TR0 = 0;		//定时器0停止计时
	ET0=1;      //关闭中断
}



//超声波测距函数

void Wave_proc(void)
{
	if(Wave_ultrasonic() <= 40)
	{
		Wave_flag = 1;
	}
}
	

