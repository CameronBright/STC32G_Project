#ifndef __PWM_INIT_H_
#define __PWM_INIT_H_


#include <STC32G.H>
#include "main.h"

#define PWM_PSC 			(24-1)	//24分频
#define PWM_PERIOD 		1000		//周期1毫秒


void PWM_Config(void);
int myabs(int a);
void Xianfu_Pwm(void);
void Turn_Off(float angle);

void Set_LRPwm(int Left_moto1,int Right_moto2);    //更新左右风机占空比
void Set_FBPwm(int Forwar_dmoto1,int Backward_moto2);    //更新前后风机占空比
void Update_duty(uchar sw,int PWMR,int PWML);						//更新占空比函数	

void Update_Pwm0(unsigned int Duty0);
void Update_Pwm1(unsigned int Duty1);
void Update_Pwm2(unsigned int Duty2);
void Update_Pwm3(unsigned int Duty3);
void Update_Pwm4(unsigned int Duty4);

void Update_PWM(int pwm0,int pwm1,int pwm2,int pwm3,int pwm4);

#endif
