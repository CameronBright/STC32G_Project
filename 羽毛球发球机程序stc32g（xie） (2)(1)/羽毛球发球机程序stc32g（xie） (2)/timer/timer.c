#include "timer.h"

void Timer1_Init(void)		//100微秒@24.000MHz
{
	AUXR |= 0x40;			//定时器时钟1T模式
	TMOD &= 0x0F;			//设置定时器模式
	TL1 = 0xA0;				//设置定时初始值
	TH1 = 0xF6;				//设置定时初始值
	TF1 = 0;				//清除TF1标志
	TR1 = 1;				//定时器1开始计时
	ET1 = 1;				//使能定时器1中断
	EA = 1;
}

//*********定时器3*********//
void Timer3_Init(void)		//1毫秒@24.000MHz
{
	T4T3M |= 0x02;			//定时器时钟1T模式
	T3L = 0x40;				//设置定时初始值
	T3H = 0xA2;				//设置定时初始值
	T4T3M |= 0x08;			//定时器3开始计时
	IE2 |= 0x20;			//使能定时器3中断	
}

uint delay_cnt = 0;

void sys_delay(uint delay) //定时器延时 12mhz
{
	delay_cnt = delay;
	while(delay_cnt > 0);
}
