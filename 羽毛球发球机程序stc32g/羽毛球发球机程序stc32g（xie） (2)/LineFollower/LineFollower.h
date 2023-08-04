#ifndef __LINEFOLLOWER_H
#define __LINEFOLLOWER_H

#include "main.h"
#include "ADC.h"

sbit det_R=P5^2;  //     右边
sbit det_CR=P5^3;  //  右边中间
sbit det_C = P0^7; //    中间
sbit det_CL=P0^5;  //  左边中间
sbit det_L=P0^6;  //   左边


void ReadLine_proc(void);
void T_S_B_1_ReadLine(void);
void T_S_B_2_ReadLine(void);


#endif