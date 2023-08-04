/*
program versions : 2.3.1

���洬��ʼ�汾

modification: 2023/8/4 19:20

modifier: Cameron Bright

*/
#include "timer.h"   //��ʱ��
#include "oled.h"   //OLED����
#include "GPIO.h"   //GPIO
#include "key.h"  	//����
#include "Uart.h"   //����
#include "stdio.h"  //c��׼��
#include "motor.h"  //���
#include "LineFollower.h" //ѭ��ģ��
#include "MPU6050.h"//mpu6050
#include "MATH.H"   //��ѧ����ģ��

//=================================

#include "pwm_init.h"  //pwm
#include "Wave.h"      //������

//-------------------------------- system--------------------------------
extern uint s_count;         //��ʱ������
extern uint delay_cnt;       //delay����
uint sys_led = 0;            //����״̬��

uint disp_delay;             //��ʾ��ˢ����ʱ����
uint motor_delay;						 //�������ˢ����ʱ����
uint mpu6050_delay;					 //mpu6050��ȡ��ʱ����
uint key_delay;              //������ʱˢ�¼���

//----------------- motor(���������PID����)--------------------------------
//extern int dutyL;            //��ߵ������pwm ����1000
//extern int dutyR;            //��

//=======================================================================
//******************************
extern int Left_moto1     ;//����PWM
extern int Right_moto2    ;//��
extern int Forwar_dmoto1  ;//ǰ
extern int Backward_moto2 ;//��


//======================================================================
uchar motor_sw = 1;//�������

struct pid_parameter positionPID; //PID����
int err_kp, err_ki, err_kd;   //���ֵ
char line_inaccuracy; //ѭ��ģ��ƫ����
char old_line_inaccuracy;//��һ��ѭ��ģ��ı�����
char old_position;    //��һ�ε�����

//------------------MPU6050-----------------------------------------------
//===================��ȡ����========================================================


extern float Angle_gx=0,Angle_gy=0,Angle_gz=0;		//�ɽ��ٶȼ���Ľ�����(�Ƕ���)
extern float Angle_ax=0,Angle_ay=0,Angle_az=0;		//�ɼ��ٶȼ���ļ��ٶ�(������)
float edata  Last_Angle_gx=0;					//�⻷PI�����  ��һ������������
float edata Last_Angle_gy=0;

int		edata g_x=0,g_y=0,g_z=0;					//�����ǽ�������
float	edata a_x=0,a_y=0,a_z=0;							//�ǶȽ�������

extern int X_Result=0,Y_Result=0,Z_Result=0;						//����ƫ��ֵ 

unsigned char	tp[16];		//��MP6050����
//-----------------other--------------------------------------------------
uchar txbuf[20]; //���ڷ��ͻ���

uchar oled_showtext[25]; //oled��ʾ�ַ���

void Disp_refresh(void);  //�������ʾ����
void Motor_control(void); //������ƺ���
void MPU6050_Read(void);  //���������ݲɼ�
void Key_Proc(void);

//*****************����***********//

unsigned char key_old = 0;
unsigned char key_value = 0;
unsigned char key_Down = 0;
unsigned char key_up = 0;

extern unsigned int PWMB_CCR5;
extern unsigned int PWMB_CCR6;
extern unsigned int PWMB_CCR7;
extern unsigned int PWMB_CCR8;

//*******������*****//
unsigned char Wave = 0;
unsigned char Wave_Buff[20];
extern unsigned long TF_Count;

//ѭ����������
unsigned char T_S_B = 0;  //����ѭ������
unsigned char flag = 0;
unsigned char flag_count = 0;
unsigned char Tracking_value = 0;  //ѭ��ֵ
extern int dutyL; //����
extern int dutyR; //����

void main()
{
	GPIO_init();//GPIO��ʼ��
	OLED_Init();//oled��ʼ��
	OLED_ColorTurn(0);//0������ʾ��1 ��ɫ��ʾ
	OLED_DisplayTurn(0);//0������ʾ 1 ��Ļ��ת��ʾ
	OLED_Clear();//oled����
	Timer0Init();//��ʱ�� 0 ��ʼ��
	Timer1_Init(); //��ʱ�� 1 ��ʼ��
	Timer3_Init();//��ʱ�� 3 ��ʼ��
	Uart1_Init();// ����1��ʼ��
	S1_S0=0; S1_S1=0;//����1 ѡ��P30 P31	
	P54RST=1;//��λ��ʼ��
	InitMPU6050(); //mpu6050��ʼ��
	Motor_Init(); //�����ʼ��
	//PID����
	positionPID.basicSpeed = 600;//�����˶��ٶ�
	positionPID.kp = 400;
	positionPID.ki = 0;
	positionPID.kd = 20;
	
	while(1)
	{
		Disp_refresh();
		MPU6050_Read();
		Motor_control();
		Key_Proc();
	}
}

//��ʱ��0�жϺ���

void Timer0() interrupt 1
{
	TF_Count += 1;
}


//-----------------��ʱ��1-----------------------------------
void timer1() interrupt 3       //100us�ж�һ��
{
	if(++disp_delay == 100) disp_delay = 0;       //��ʾ��ˢ��ʱ��
	if(++motor_delay == 10) motor_delay = 0;			//�������ˢ��ʱ��
	if(++mpu6050_delay == 50) mpu6050_delay = 0;  //mpu6050ִ��ˢ��ʱ��
	if(++key_delay == 10) key_delay = 0;        //����ɨ��ˢ��ʱ��
	
	if(++sys_led >= 5000)                         
	{
		sys_led = 0;
	}
} 


void Timer3_Isr(void) interrupt 19   //�ȴ����� 1ms
{
	if(flag == 1)         
	{
		flag_count++;
		if(flag_count > 100)
		{
			Tracking_value ++;   //ѭ������
			flag_count = 0;
			flag = 0;
		}
	}
}



//-----------------��������-----------------------------------

void Key_Proc(void)
{
//	if(key_delay) return; //100msɨ��һ�ΰ���
//		key_delay = 1;
	
	key_value = Key_Rvalue();//��ȡ�������µı��
	key_Down = key_value & (key_old ^ key_value);	
	key_up = ~key_value & (key_old ^ key_value);		
	key_old = key_value;	
	
	switch(key_Down)
	{
		case 1:
		PWMB_CCR5 += 50;
		Update_5(PWMB_CCR5);
		if(PWMB_CCR5 > 1000)
			PWMB_CCR5 = 300;
		break;
		case 2:
		PWMB_CCR6 += 50;	
		Update_6(PWMB_CCR6);
		if(PWMB_CCR6 > 1000)
			PWMB_CCR6 = 300;
		break;
		case 3:
		PWMB_CCR7 += 50;
		Update_7(PWMB_CCR7);
		if(PWMB_CCR7 > 1000)
			PWMB_CCR7 = 300;
		break;
		case 4:
		PWMB_CCR8 += 50;
		Update_8(PWMB_CCR8);
		if(PWMB_CCR8 > 1000)
			PWMB_CCR8 = 300;
		break;
	}
}

//-----------------��ʾ������-----------------------------------
void Disp_refresh(void)
{
//	if(disp_delay) return; //10msˢ��һ����Ļ
//	disp_delay = 1;
	
//		sprintf(oled_showtext,"line:%2d  ",line_inaccuracy);
//	OLED_Display_string_5x7(0,0,oled_showtext);
//	
////		sprintf(oled_showtext,"L:%3d,R%3d ",dutyL,dutyR);
////	OLED_Display_string_5x7(0,2,oled_showtext);
//	
//		sprintf(oled_showtext,"basic:%3d ",positionPID.basicSpeed);
//	OLED_Display_string_5x7(0,4,oled_showtext);
//	
//		sprintf(oled_showtext,"Z:%5d  ",(int)Z_Result);
//	OLED_Display_string_5x7(0,6,oled_showtext);
//	
//	
//		sprintf(oled_showtext,"p:%4d,d:%4d",(int)positionPID.kp,(int)positionPID.kd);
//	OLED_Display_string_5x7(64,0,oled_showtext);
//	
//		sprintf(oled_showtext,"R: %5d ",(int)X_Result);
//	OLED_Display_string_5x7(64,2,oled_showtext);
//	
//		sprintf(oled_showtext,"Y: %5d ",(int)Y_Result);
//	OLED_Display_string_5x7(64,4,oled_showtext);
//	
//		sprintf(oled_showtext,"Y: %5d ",(int)flag_count);
//	OLED_Display_string_5x7(64,7,oled_showtext);

		sprintf(oled_showtext,"P42 5: %5d ",PWMB_CCR5);
		OLED_Display_string_5x7(0,0,oled_showtext);
		sprintf(oled_showtext,"P41 6: %5d ",PWMB_CCR6);
		OLED_Display_string_5x7(0,2,oled_showtext);
		sprintf(oled_showtext,"P73 7: %5d ",PWMB_CCR7);
		OLED_Display_string_5x7(0,4,oled_showtext);
		sprintf(oled_showtext,"P70 8: %5d ",PWMB_CCR8);
		OLED_Display_string_5x7(0,6,oled_showtext);
}


//-------------�����ǿ��ƺ���----------------------------------------
//��ر���

//------------
void MPU6050_Read(void)
{
	if(mpu6050_delay) return; 
	mpu6050_delay = 1;
	
		//********************************************************************************************
		Read_MPU6050(tp);	// 720us @24MHz

		Angle_ax = ((float)(((int *)&tp)[0])) / 8192.0;	//���ٶȴ���	�����λ�� +- g
		Angle_ay = ((float)(((int *)&tp)[1])) / 8192.0;	//ת����ϵ	8192 LSB/g, 1g��Ӧ����8192
		Angle_az = ((float)(((int *)&tp)[2])) / 8192.0;	//���ٶ����� +-4g/S
		Last_Angle_gx = Angle_gx;		//������һ�ν��ٶ�����
		Last_Angle_gy = Angle_gy;
		Angle_gx = ((float)(((int *)&tp)[4] - g_x)) / 65.5;	//�����Ǵ���	�����λ�� +-��
		Angle_gy = ((float)(((int *)&tp)[5] - g_y)) / 65.5;	//���������� +-500��/S, 1��/�� ��Ӧ���� 65.536
		Angle_gz = ((float)(((int *)&tp)[6] - g_z)) / 65.5;	//ת����ϵ65.5 LSB/��


		//**********************************X��ָ��************************************************
		X_Result  = Angle_ax * 100 ;		//���������ֵ��ȥƫ��ֵ
		//**************Y��ָ��**************************************************
		Y_Result  = Angle_ay * 100;		//���������ֵ��ȥƫ��ֵ
		//**************Z��ָ��*****************************	
		Z_Result = Angle_az * 100;


		//==============================================================
				
//		Uart1_sendbyte(0x03);
//		Uart1_sendbyte(~0x03);
//		
//		Uart1_sendbyte((int)(X_Result*100));
//		Uart1_sendbyte((int)(X_Result*100)>>8);
//		Uart1_sendbyte((int)(Y_Result*100));
//		Uart1_sendbyte((int)(Y_Result*100)>>8);
//		Uart1_sendbyte((int)(Z_Result*100));
//		Uart1_sendbyte((int)(Z_Result*100)>>8);
//		
//		Uart1_sendbyte(~0x03);
//		Uart1_sendbyte(0x03);
			 
		//========================= 
}





//-----------------������ƺ���-------------------------------------
void Motor_control(void)
{
	if(motor_delay) return; //��ʱ
	motor_delay = 1;
	
	line_inaccuracy = ReadLine();//��ȡѭ��״̬ 1��-1��0
	
	if(line_inaccuracy > 2 || line_inaccuracy < -2)
	{
		if(line_inaccuracy == -3)//������Զ�����ʱ
			motor_sw = 1;           //�����ת
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
		dutyR = positionPID.basicSpeed + err_kp + err_kd; 
		dutyL = positionPID.basicSpeed - err_kp - err_kd;
	}
	else 
	{
		dutyR = positionPID.basicSpeed;
		dutyL = positionPID.basicSpeed;
	}

	Update_duty(motor_sw,dutyR,dutyL);//����PWM���
	old_line_inaccuracy = line_inaccuracy;
}

