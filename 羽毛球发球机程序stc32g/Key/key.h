#ifndef __KEY_H
#define __KEY_H

#include "main.h"

sbit P24 = P2^4;
sbit P44 = P4^4;
sbit P42 = P4^2;
sbit P73 = P7^3;

uchar Key_Rvalue(void);
void Key_Init(void);

#endif