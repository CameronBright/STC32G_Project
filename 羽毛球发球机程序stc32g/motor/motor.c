#include "motor.h"

void Motor_Init(void) //�����ʼ��
{
	PWMA_Config(); //configure the PWM
	Motor_Forward(); //forward
}

void Motor_Forward(void) //forward motor (�����ת)
{
	MOTOR_AIN1 = 0;
	MOTOR_AIN2 = 1;
	
	MOTOR_BIN1 = 0;
	MOTOR_BIN2 = 1;
}

void Motor_reverse(void) //reverse motor (�����ת)
{
	MOTOR_AIN1 = 1;
	MOTOR_AIN2 = 0;
	
	MOTOR_BIN1 = 1;
	MOTOR_BIN2 = 0;
}

/*
When motor1 >= 0, the motor1 is forware, otherwise the motor1 is reverse
When motor2 >= 0, the motor2 is forware, otherwise the motor2 is reverse
��pwm����0ʱ�������ת С��0ʱ�� �����ת
*/

void Motor_FRcontrol(int motor1, int motor2)
{
	if(motor1 >= 0)
	{
		MOTOR_AIN1 = 0; //Forward motor1(���1��ת)
		MOTOR_AIN2 = 1;
	}
	else 
	{
		MOTOR_AIN1 = 1; //Forward motor1(���1��ת)
		MOTOR_AIN2 = 0;
	}
	
	if(motor2 >= 0)
	{
		MOTOR_BIN1 = 0; //Forward motor2(���2��ת)
		MOTOR_BIN2 = 1;
	}
	else 
	{
		MOTOR_BIN1 = 1; //Forward motor2(���2��ת)
		MOTOR_BIN2 = 0;
	}
}
