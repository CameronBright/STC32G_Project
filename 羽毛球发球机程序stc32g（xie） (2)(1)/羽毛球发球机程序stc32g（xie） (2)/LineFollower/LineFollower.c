#include "LineFollower.h"
#include "Wave.h"

//ѭ���߱�־λ
unsigned char count = 0;  
extern unsigned char Wave_flag;
extern unsigned char T_S_B;
extern unsigned char flag;
extern unsigned char flag_count;
extern unsigned char Tracking_value;

//extern char line_inaccuracy;

extern void Motor_control();


char ReadLine(void) //�����ƫ��ʱ�����1 �ұ�ƫ���-1 �����������0
{	
	uchar read_value = 0;
	char return_value = 0;
	
		if(det_L == 1)
			read_value |= 0x10; //��1
		else 
			read_value &= ~0x10;

		if(det_CL == 1)
			read_value |= 0x08;
		else 
			read_value &= ~0x08;
		
		if(det_C == 1)
			read_value |= 0x04;
		else 
			read_value &= ~0x04;
		
		if(det_CR == 1)
			read_value |= 0x02;
		else 
			read_value &= ~0x02;
		
		if(det_R == 1)
			read_value |= 0x01;
		else 
			read_value &= ~0x01;
		
		
		switch(read_value)
		{
			case 0x04:            //������ʻ
				return_value = 0;
				break;
			case 0x08:           //���ұ�ƫһ���
				return_value = 1;
				break;
			case 0x02:           //�����ƫһ���
				return_value = -1;
				break;
			case 0x0C:					 //���ұ�ƫһ���
				return_value = 1;
				break;
			case 0x06:					 //�����ƫһ���
				return_value = -1;
				break;
			case 0x10:
				return_value = 2; //����ƫ�ϴ�Ƕ�
				break;
			case 0x01:
				return_value = -2;//����ƫ�ϴ�Ƕ�
				break;
			case 0x18:
				return_value = 2; //����ƫ�ϴ�Ƕ�
				break;
			case 0x03:
				return_value = -2;//����ƫ�ϴ�Ƕ�
				break;	
			case 0x1F:
				return_value = -3;//�����뿪���淵��  ��-3��
			break;
			
			default:
				return_value = 3;//ûʶ�𵽺��߷��� ��3��
			break;
		} 
			
	return return_value;
}

////Ѳ�ߴ�����
//void ReadLine_proc(void)
//{
//	if(T_S_B == 1)
//	{
//		T_S_B_1_ReadLine();
//	}
//	if(T_S_B == 2)
//	{
//		T_S_B_2_ReadLine();
//	}
//}

////��������

////����2
//void T_S_B_1_ReadLine()
//{
//	Motor_control();
//	if(line_inaccuracy == -3 ||line_inaccuracy == -2)   //�ж�5·���ں�����
//	{
//		flag = 1;
//	}
//	if(Tracking_value == 4)  //ѭ��1Ȧ ѭ��������־λ+4  
//	{
//		Tracking_value = 0;   //ѭ����1Ȧ ��־λ��0
//		flag = 0;             
//		T_S_B = 0;            //״̬�ָ�
//	}  	
//}



////����3
//void T_S_B_2_ReadLine(void)
//{

//		Motor_control();
//		if(line_inaccuracy == -3 || line_inaccuracy == -2)   //�ж�5·���ں�����
//		{
//			flag = 1;
//			if(Tracking_value == 6)  //ѭ��1Ȧ ��־λ+4
//			{
//				Tracking_value = 0;   //ѭ����1Ȧ ��־λ��0
//				flag = 0;             
//				T_S_B = 0;            //״̬�ָ�
//			}  
//		}	
//		if(Tracking_value == 4)  
//		{
////		  moter_wheel();
//		}
//}

//  //��պ���  ��������PWM
//void moter_wheel(int left_moto,int right_moto)//����ѭ��a��b��ת��  ���ҷ���
//{
//		bit Turn_left_flage = 0;
//	
//		Turn_left_flage = 1;

//		//���¿�����ߵ�����ұߵ��
//		Set_LRPwm(left_moto,right_moto);    //����һ �����ת���ᱻѭ������  ��ת��ɺ���λָ�ѭ������
//	
//		
//		
//		if(line_inaccuracy == 2)//��ߵĺ����������������ѭ��
//		{
//			Turn_left_flage = 0;
////			Motor_control(��//�ָ�ѭ�� ֱ�������¸�AB
//		}
//		
//		
//		
//}




//////���Ӳ���
//////����1

//void obstacle_avoidance()
//{
//	Motor_control();
//	if(Wave_ultrasonic() > 40)
//	{
//		
//	}
//}


//////����2




