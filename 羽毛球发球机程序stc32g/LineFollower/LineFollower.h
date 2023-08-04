#ifndef __LINEFOLLOWER_H
#define __LINEFOLLOWER_H

#include "main.h"
#include "ADC.h"
#include "pwm_init.h"

sbit det_R=P2^5;  //     右边
sbit det_CR=P2^6;  //  右边中间
sbit det_C = P2^7; //    中间
sbit det_CL=P7^5;  //  左边中间
sbit det_L=P7^6;  //   左边


void ReadLine_proc(void);
void T_S_B_1_ReadLine(void);
void T_S_B_2_ReadLine(void);
void moter_wheel(int left_moto,int right_moto);//用与循迹a、b点转弯  左右风速
void obstacle_avoidance();
#endif