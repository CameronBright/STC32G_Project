#ifndef __LINEFOLLOWER_H
#define __LINEFOLLOWER_H

#include "main.h"
#include "ADC.h"

sbit det_R=P5^2;  //按下确定
sbit det_C=P5^3;  //中断0
sbit det_L=P0^5;  //中断1

uchar ReadLine(void);

#endif