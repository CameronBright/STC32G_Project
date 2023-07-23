#ifndef __MOTOR_H
#define __MOTOR_H

#include "main.h"
#include "pwm.h"

sbit MOTOR_AIN1=P0^2; //AIN1 
sbit MOTOR_AIN2=P0^1; //AIN2

sbit MOTOR_BIN1=P0^3; //BIN1
sbit MOTOR_BIN2=P0^4; //BIN2

void Motor_Init(void);
void Motor_Forward(void); //forward motor (�����ת)
void Motor_reverse(void); //reverse motor (�����ת)
void Motor_FRcontrol(int motor1, int motor2); //Forward / reverse control(����ת����) 
	
struct pid_parameter     //PID����
{
	char kp;
	char ki;
	char kd;
	int basicSpeed; //�����ٶ�
};

#endif