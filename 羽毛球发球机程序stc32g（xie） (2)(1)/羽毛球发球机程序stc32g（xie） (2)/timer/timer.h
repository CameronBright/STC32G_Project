#ifndef __TIMER_H
#define __TIMER_H

#include "main.h"


void Timer1_Init(void);		//100΢��@24.000MHz
void Timer3_Init(void);		//1����@24.000MHz

void sys_delay(uint delay);


#endif