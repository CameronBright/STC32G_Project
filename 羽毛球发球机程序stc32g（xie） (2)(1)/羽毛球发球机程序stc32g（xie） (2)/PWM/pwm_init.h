#ifndef __PWM_INIT_H_
#define __PWM_INIT_H_


#include <STC32G.H>
#include "main.h"

#define PWM_PSC 			(24-1)	//24��Ƶ
#define PWM_PERIOD 		1000		//����1����


void PWM_Config(void);
int myabs(int a);
void Xianfu_Pwm(void);
void Turn_Off(float angle);

void Set_LRPwm(int Left_moto1,int Right_moto2);    //�������ҷ��ռ�ձ�
void Set_FBPwm(int Forwar_dmoto1,int Backward_moto2);    //����ǰ����ռ�ձ�
void Update_duty(uchar sw,int PWMR,int PWML);						//����ռ�ձȺ���	

void Update_Pwm0(unsigned int Duty0);
void Update_Pwm1(unsigned int Duty1);
void Update_Pwm2(unsigned int Duty2);
void Update_Pwm3(unsigned int Duty3);
void Update_Pwm4(unsigned int Duty4);

void Update_PWM(int pwm0,int pwm1,int pwm2,int pwm3,int pwm4);

#endif
