#include "timer.h"   //��ʱ��
#include "oled.h"   //OLED����
#include "GPIO.h"   //GPIO
#include "pwm.h"      //pwm
#include "Encoders.h"  //��ת������
#include "iic.h"
#include "Uart.h"   //����
#include "stdio.h"
#include "Meun.h"
#include "motor.h"
#include "ADC.h"
#include "LineFollower.h" //ѭ��ģ��
#include "stdio.h"
#include "MPU6050.h"
#include "MATH.H"

//-------------------------------- system--------------------------------
extern uint s_count;         //��ʱ������
extern uint delay_cnt;       //delay����
uint timer_delay = 0;        //1us tick

uint disp_delay;             //��ʾ��ˢ����ʱ����
uint motor_delay;						 //�������ˢ����ʱ����

//----------------- motor(���������PID����)--------------------------------
extern int dutyL;            //��ߵ������pwm ����1000
extern int dutyR;            //��

uchar motor_sw = 1;//�������

struct pid_parameter positionPID; //PID����
char line_inaccuracy; //ѭ��ģ��ƫ����
char old_position;    //��һ�ε�����

//------------------MPU6050-----------------------------------------------

int Gyro_x, Gyro_y, Gyro_z;                        //����������
int Gyro_angle_x=0, Gyro_angle_y=0, Gyro_angle_z=0;
int Acc_x, Acc_y, Acc_z;                           //������ٶ�
int Temp;                                        //�¶�

int		xdata g_x=0,g_y=0,g_z=0;					         //�����ǽ�������
float	xdata a_x=0,a_y=0;							           //�ǶȽ�������
float	data  AngleX=0, AngleY=0, AngleZ=0;					 //��Ԫ���������ŷ����
float	xdata Angle_gx=0, Angle_gy=0, Angle_gz=0;		 //�ɽ��ٶȼ���Ľ�����(�Ƕ���)
float	xdata Angle_ax=0, Angle_ay=0, Angle_az=0;		 //�ɼ��ٶȼ���ļ��ٶ�(������)

unsigned  int Angle_of_pitch = 0;
unsigned 	int Roll_Angle = 0;

#define	pi		3.14159265f                           
#define	Kp		0.8f                        
#define	Ki		0.001f                         
#define	halfT	0.004f  

float idata q0=1,q1=0,q2=0,q3=0;   
float idata exInt=0,eyInt=0,ezInt=0;  

void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az);

//-----------------other--------------------------------------------------
uchar txbuf[20]; //���ڷ��ͻ���

uchar oled_showtext[20]; //oled��ʾ�ַ���

void Disp_refresh(void);  //�������ʾ����
void Motor_control(void); //������ƺ���

void main()
{
	GPIO_init();//GPIO��ʼ��
	OLED_Init();//oled��ʼ��
	OLED_ColorTurn(0);//0������ʾ��1 ��ɫ��ʾ
	OLED_DisplayTurn(1);//0������ʾ 1 ��Ļ��ת��ʾ
	OLED_Clear();//oled����
	
	Timer1Init();//��ʱ����ʼ��
	UartInit();//���ڳ�ʼ��	
	S1_S0=0;S1_S1=0;//����1 ѡ��P30 P31	
	P54RST=1;//��λ��ʼ��
	
	
	Motor_Init();
	positionPID.basicSpeed = 400;
	
	while(1)
	{	
		//rotary_encoder();        //һֱɨ����ת���������� ,��������� �½���		
		//Cancel_determine();      //����ȡ����ȷ����⺯��,��ʱ��ʽ
		//Memu();
		
		Motor_control();
		Disp_refresh();	
		
		//sprintf(txbuf,"1:%04d 2:%04d 3:%04d\r\n",ADCP1,ADCP2,ADCP3);
		//Uart_String(txbuf); //����
	}
}
void timer1() interrupt 3       //100us��һ��
{
	if(++disp_delay == 100) disp_delay = 0;
	if(++motor_delay == 10) motor_delay = 0;
	
	if(delay_cnt > 0) //��ʱ����
		delay_cnt--;
	
	if(++timer_delay > 10000)
	//Pwm_Fun();                 //pwm����
	if(EC11_K==0)
	{
		long_short=1;          //����������Ϊ0   
	}		
	if(long_short==1)         //�����ʱ������
	{
		s_count++;   
	}
} 

void Disp_refresh(void)
{
	if(disp_delay) return; //10msˢ��һ����Ļ
	disp_delay = 1;
	
	sprintf(oled_showtext,"%1d   ",line_inaccuracy);
	OLED_16x16(0,0,oled_showtext);
	
	sprintf(oled_showtext,"%3d,%3d",dutyR,dutyL);
	OLED_16x16(0,2,oled_showtext);
	
	sprintf(oled_showtext,"%3d",positionPID.basicSpeed);
	OLED_16x16(0,4,oled_showtext);
//	sprintf(oled_showtext,"P00:");
//	OLED_16x16(0,2,oled_showtext);
//	sprintf(oled_showtext,"P10:");
//	OLED_16x16(0,4,oled_showtext);
//	
//	OLED_ShowNum(35,0,ADCP1,6);
//	OLED_ShowNum(35,2,ADCP2,6);
//	OLED_ShowNum(35,4,ADCP3,6);	
}

void Motor_control(void)
{
	if(motor_delay) return;
	motor_delay = 1;
	
	line_inaccuracy = ReadLine();//��ȡѭ��״̬ 1��-1��0
	
	if(line_inaccuracy > 1 || line_inaccuracy < -1)
	{
		if(line_inaccuracy == -2)//������Զ�����ʱ
			motor_sw = 0;
		else if(line_inaccuracy == 2) //���д��������ڵ��浫ûʶ����ʱ
			line_inaccuracy = old_position;
	}
	else 
	{
		motor_sw = 1;//����
		old_position = line_inaccuracy;//��¼��һ�ε�λ��
	}
		
	dutyR = positionPID.basicSpeed + line_inaccuracy*600; //��ƫ��ƫ
	dutyL = positionPID.basicSpeed - line_inaccuracy*600;
	
	Motor_FRcontrol(dutyR,dutyL);//pwmֵС��0�ͷ�ת������0��ת
	
	Update_duty(motor_sw);//����PWM���
}

void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az)
{
	float data norm;
	float idata vx, vy, vz;
	float idata ex, ey, ez;

	norm = sqrt(ax*ax + ay*ay + az*az);	//�Ѽ��ٶȼƵ���ά����ת�ɵ�ά����   
	ax = ax / norm;
	ay = ay / norm;
	az = az / norm;

		//	�����ǰ���Ԫ������ɡ��������Ҿ����еĵ����е�����Ԫ�ء� 
		//	�������Ҿ����ŷ���ǵĶ��壬��������ϵ������������ת����������ϵ��������������Ԫ��
		//	���������vx vy vz����ʵ���ǵ�ǰ��ŷ���ǣ�����Ԫ�����Ļ����������ϵ�ϣ����������
		//	������λ������
	vx = 2*(q1*q3 - q0*q2);
	vy = 2*(q0*q1 + q2*q3);
	vz = q0*q0 - q1*q1 - q2*q2 + q3*q3 ;

	ex = (ay*vz - az*vy) ;
	ey = (az*vx - ax*vz) ;
	ez = (ax*vy - ay*vx) ;

	exInt = exInt + ex * Ki;
	eyInt = eyInt + ey * Ki;
	ezInt = ezInt + ez * Ki;

	gx = gx + Kp*ex + exInt;
	gy = gy + Kp*ey + eyInt;
	gz = gz + Kp*ez + ezInt;

	q0 = q0 + (-q1*gx - q2*gy - q3*gz) * halfT;
	q1 = q1 + ( q0*gx + q2*gz - q3*gy) * halfT;
	q2 = q2 + ( q0*gy - q1*gz + q3*gx) * halfT;
	q3 = q3 + ( q0*gz + q1*gy - q2*gx) * halfT;

	norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
	q0 = q0 / norm;
	q1 = q1 / norm;
	q2 = q2 / norm;
	q3 = q3 / norm;

	AngleX = asin(2*(q0*q2 - q1*q3 )) * 57.2957795f; // ����   ����ɶ�
	AngleY = asin(2*(q0*q1 + q2*q3 )) * 57.2957795f; // ���
	AngleZ = atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.2957795f;
}

//	IT0=0;  //�ж�0    IT0=0;�����غ��½��ش���   IT0=1 �½��ش���
//	EX0=1;	//ʹ���ж�0
//	IT1=0;  //�ж�0    IT0=0;�����غ��½��ش���   IT0=1 �½��ش���
//	EX1=1;	//ʹ���ж�0

//add_loseit EC11_OLD_A;
//add_loseit EC11_UP_A;
//add_loseit EC11_DOWN_A
//add_loseit EC11_OLD_add_lose;
//void init0_Isr() interrupt 0
//{
//		IE1=0;
//		EC11_UP_A=~EC11_A&(EC11_OLD_A^EC11_A);
//		EC11_DOWN_A=EC11_A&(EC11_OLD_A^EC11_A)
//		EC11_OLD_A=EC11_A;
//		if(EC11_UP_A)
//		{
//			if(EC11_add_lose)
//			{
//				flag=1;
//					t+=0.01;
//			
//			}else{
//				t-=0.01;
//			}
//		}
//		if(EC11_DOWN_A)
//		{
//			if(EC11_add_lose==0)
//			{
//				flag=1;
//					t-=0.01;
//			
//			}else{
//				flag=1;
//					t-=0.01;
//			}
//		}
//}
//void init1_Isr() interrupt 2
//{
//	IE0=0;
//}

