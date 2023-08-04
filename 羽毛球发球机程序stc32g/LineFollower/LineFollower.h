#ifndef __LINEFOLLOWER_H
#define __LINEFOLLOWER_H

#include "main.h"
#include "ADC.h"
#include "pwm_init.h"

sbit det_R=P2^5;  //     �ұ�
sbit det_CR=P2^6;  //  �ұ��м�
sbit det_C = P2^7; //    �м�
sbit det_CL=P7^5;  //  ����м�
sbit det_L=P7^6;  //   ���


void ReadLine_proc(void);
void T_S_B_1_ReadLine(void);
void T_S_B_2_ReadLine(void);
void moter_wheel(int left_moto,int right_moto);//����ѭ��a��b��ת��  ���ҷ���
void obstacle_avoidance();
#endif