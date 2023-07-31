#include "motor.h"

void Motor_Init(void) //电机初始化
{
	PWMA_Config(); //configure the PWM
	MotorL_Forward();
	MotorR_Forward();
}

void MotorL_Forward(void) //右边电机正转
{
	MOTOR_BIN1 = 0;
	MOTOR_BIN2 = 1;
}

void MotorL_Reverse(void) //右边电机反转
{
	MOTOR_BIN1 = 1;
	MOTOR_BIN2 = 0;
}

void MotorR_Forward(void) //左边电机正转
{
	MOTOR_AIN1 = 0;
	MOTOR_AIN2 = 1;
}

void MotorR_Reverse(void) //左边电机反转
{
	MOTOR_AIN1 = 1;
	MOTOR_AIN2 = 0;
}

/*
When motor1 >= 0, the motor1 is forware, otherwise the motor1 is reverse
When motor2 >= 0, the motor2 is forware, otherwise the motor2 is reverse
当pwm大于0时，电机正转 小于0时， 电机反转
*/

void Motor_FRcontrol(int motor1, int motor2)
{
	if(motor1 >= 0)
	{
		MOTOR_AIN1 = 0; //Forward motor1(电机1正转)
		MOTOR_AIN2 = 1;
	}
	else 
	{
		MOTOR_AIN1 = 1; //Forward motor1(电机1反转)
		MOTOR_AIN2 = 0;
	}
	
	if(motor2 >= 0)
	{
		MOTOR_BIN1 = 0; //Forward motor2(电机2正转)
		MOTOR_BIN2 = 1;
	}
	else 
	{
		MOTOR_BIN1 = 1; //Forward motor2(电机2反转)
		MOTOR_BIN2 = 0;
	}
}
