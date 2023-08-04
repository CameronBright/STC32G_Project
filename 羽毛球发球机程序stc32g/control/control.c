#inlcude "control.h"



//-----------------电机控制函数-------------------------------------
void Motor_control(void)
{
	if(motor_delay) return; //延时
	motor_delay = 1;
	
	line_inaccuracy = ReadLine();//读取循线状态 1、-1、0
	
	if(line_inaccuracy > 2 || line_inaccuracy < -2)
	{
		if(line_inaccuracy == -3)//传感器远离地面时
			motor_sw = 0;
		else if(line_inaccuracy == 3) //所有传感器都在地面但没识别到线时
			line_inaccuracy = old_position;
	}
	else 
	{
		motor_sw = 1;//电机正常工作
		old_position = line_inaccuracy;//记录上一次的位置
	}
	
	err_kp = positionPID.kp * line_inaccuracy;     //循迹模块数据
	err_ki = positionPID.ki * line_inaccuracy;     //积分
	err_kd = positionPID.kd * old_line_inaccuracy; //微分
	
	if(line_inaccuracy)
	{
		Right_moto2 = positionPID.basicSpeed + err_kp + err_kd; 
		Left_moto1 = positionPID.basicSpeed - err_kp - err_kd;
		

		
	}
	else 
	{
		Set_FBPwm = positionPID.basicSpeed;
		Set_LRPwm = positionPID.basicSpeed;
	}

		Set_LRPwm(Left_moto1,Right_moto2);//设置左右PWM和前后PWM 
		Set_FBPwm(Forwar_dmoto1,Forwar_dmoto1);;//更新PWM输出
	old_line_inaccuracy = line_inaccuracy;
}
