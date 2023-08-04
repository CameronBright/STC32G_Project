#ifndef __KEY_H
#define __KEY_H

#include "main.h"

sbit P42 = P4^2;
sbit P41 = P4^1;
sbit P73 = P7^3;
sbit P70 = P7^0;

uchar Key_Rvalue(void);
void Key_Init(void);

#endif