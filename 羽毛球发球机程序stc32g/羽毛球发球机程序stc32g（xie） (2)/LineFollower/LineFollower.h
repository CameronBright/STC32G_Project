#ifndef __LINEFOLLOWER_H
#define __LINEFOLLOWER_H

#include "main.h"
#include "ADC.h"

sbit det_R=P5^2;  //     �ұ�
sbit det_CR=P5^3;  //  �ұ��м�
sbit det_C = P0^7; //    �м�
sbit det_CL=P0^5;  //  ����м�
sbit det_L=P0^6;  //   ���


void ReadLine_proc(void);
void T_S_B_1_ReadLine(void);
void T_S_B_2_ReadLine(void);


#endif