#ifndef __LINEFOLLOWER_H
#define __LINEFOLLOWER_H

#include "main.h"
#include "ADC.h"

sbit det_R=P5^2;  //����ȷ��
sbit det_C=P5^3;  //�ж�0
sbit det_L=P0^5;  //�ж�1

uchar ReadLine(void);

#endif