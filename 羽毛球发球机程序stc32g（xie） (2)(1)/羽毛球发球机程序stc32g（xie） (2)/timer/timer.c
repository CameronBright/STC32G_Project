#include "timer.h"

void Timer1_Init(void)		//100΢��@24.000MHz
{
	AUXR |= 0x40;			//��ʱ��ʱ��1Tģʽ
	TMOD &= 0x0F;			//���ö�ʱ��ģʽ
	TL1 = 0xA0;				//���ö�ʱ��ʼֵ
	TH1 = 0xF6;				//���ö�ʱ��ʼֵ
	TF1 = 0;				//���TF1��־
	TR1 = 1;				//��ʱ��1��ʼ��ʱ
	ET1 = 1;				//ʹ�ܶ�ʱ��1�ж�
	EA = 1;
}

//*********��ʱ��3*********//
void Timer3_Init(void)		//1����@24.000MHz
{
	T4T3M |= 0x02;			//��ʱ��ʱ��1Tģʽ
	T3L = 0x40;				//���ö�ʱ��ʼֵ
	T3H = 0xA2;				//���ö�ʱ��ʼֵ
	T4T3M |= 0x08;			//��ʱ��3��ʼ��ʱ
	IE2 |= 0x20;			//ʹ�ܶ�ʱ��3�ж�	
}

uint delay_cnt = 0;

void sys_delay(uint delay) //��ʱ����ʱ 12mhz
{
	delay_cnt = delay;
	while(delay_cnt > 0);
}
