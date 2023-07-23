#ifndef __MOTOR_H
#define __MOTOR_H

#include "main.h"
#include "pwm.h"

sbit MOTOR_AIN1=P0^2; //AIN1 
sbit MOTOR_AIN2=P0^1; //AIN2

sbit MOTOR_BIN1=P0^3; //BIN1
sbit MOTOR_BIN2=P0^4; //BIN2

void Motor_Init(void);
void Motor_Forward(void); //forward motor (电机正转)
void Motor_reverse(void); //reverse motor (电机反转)
void Motor_FRcontrol(int motor1, int motor2); //Forward / reverse control(正反转控制) 
	
struct pid_parameter     //PID参数
{
	char kp;
	char ki;
	char kd;
	int basicSpeed; //基础速度
};

#endif