#inlcude "control.h"



//-----------------������ƺ���-------------------------------------
void Motor_control(void)
{
	if(motor_delay) return; //��ʱ
	motor_delay = 1;
	
	line_inaccuracy = ReadLine();//��ȡѭ��״̬ 1��-1��0
	
	if(line_inaccuracy > 2 || line_inaccuracy < -2)
	{
		if(line_inaccuracy == -3)//������Զ�����ʱ
			motor_sw = 0;
		else if(line_inaccuracy == 3) //���д��������ڵ��浫ûʶ����ʱ
			line_inaccuracy = old_position;
	}
	else 
	{
		motor_sw = 1;//�����������
		old_position = line_inaccuracy;//��¼��һ�ε�λ��
	}
	
	err_kp = positionPID.kp * line_inaccuracy;     //ѭ��ģ������
	err_ki = positionPID.ki * line_inaccuracy;     //����
	err_kd = positionPID.kd * old_line_inaccuracy; //΢��
	
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

		Set_LRPwm(Left_moto1,Right_moto2);//��������PWM��ǰ��PWM 
		Set_FBPwm(Forwar_dmoto1,Forwar_dmoto1);;//����PWM���
	old_line_inaccuracy = line_inaccuracy;
}
