#ifndef __LINEFOLLOWER_H
#define __LINEFOLLOWER_H

#include "main.h"
#include "ADC.h"

sbit det_R=P5^2;  //
sbit det_CR=P5^3;  //
sbit det_CL=P0^5;  //
sbit det_L=P0^6;  //


uchar ReadLine(void);

#endif