#ifndef _WAVE_H
#define _WAVE_H

#include "main.h"



sbit Tx=P1^0;
sbit Rx=P1^1;

void Timer0Init(void);		//1����@24.000MHz
unsigned int Wave_ultrasonic();
void Wave_proc(void);//������������
void Delay10us();

#endif
