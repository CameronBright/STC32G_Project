#ifndef __MOTOR_H
#define __MOTOR_H

#include "main.h"
#include "pwm.h"

sbit MOTOR_AIN1=P0^1; //AIN1 
sbit MOTOR_AIN2=P0^2; //AIN2

sbit MOTOR_BIN1=P0^3; //BIN1
sbit MOTOR_BIN2=P0^4; //BIN2

void Motor_Init(void);

void MotorR_Forward(void); //��ߵ����ת
void MotorR_Reverse(void); //��ߵ����ת
void MotorL_Forward(void); //�ұߵ����ת
void MotorL_Reverse(void); //�ұߵ����ת
	
void Motor_FRcontrol(int motor1, int motor2); //Forward / reverse control(����ת����) 
	
struct pid_parameter     //PID����
{
	float kp;
	float ki;
	float kd;
	int basicSpeed; //�����ٶ�
};

#endif