/*
program versions : 2.3.1

�ܿ��Ƶ������ת��ѭ��Ҳ��������� ���ǹ���ʱ���û�дﵽһ����תһ�߷�ת��Ч��

modification: 2023/8/1 00:54

modifier: Cameron Bright

*/
#include "timer.h"   //��ʱ��
#include "oled.h"   //OLED����
#include "GPIO.h"   //GPIO
#include "pwm.h"      //pwm
#include "Encoders.h"  //��ת������
#include "key.h"  	//����
#include "iic.h"    //i2C
#include "Uart.h"   //����
#include "stdio.h"  //c��׼��
#include "Meun.h"   //��ʾ���˵�
#include "motor.h"  //���
#include "ADC.h"    //ADC
#include "LineFollower.h" //ѭ��ģ��
#include "MPU6050.h"//mpu6050
#include "MATH.H"   //��ѧ����ģ��

//-------------------------------- system--------------------------------
extern uint s_count;         //��ʱ������
extern uint delay_cnt;       //delay����
uint sys_led = 0;            //����״̬��

uint disp_delay;             //��ʾ��ˢ����ʱ����
uint motor_delay;						 //�������ˢ����ʱ����
uint mpu6050_delay;					 //mpu6050��ȡ��ʱ����
uint key_delay;              //������ʱˢ�¼���

//----------------- motor(���������PID����)--------------------------------
extern int dutyL;            //��ߵ������pwm ����1000
extern int dutyR;            //��

uchar motor_sw = 1;//�������

struct pid_parameter positionPID; //PID����
int err_kp, err_ki, err_kd;   //���ֵ
char line_inaccuracy; //ѭ��ģ��ƫ����
char old_line_inaccuracy;//��һ��ѭ��ģ��ı�����
char old_position;    //��һ�ε�����

//------------------MPU6050-----------------------------------------------

int Gyro_x, Gyro_y, Gyro_z;                        //����������
int Gyro_x_, Gyro_y_, Gyro_z_;                        //�����������˲�

int Gyro_angle_x=0, Gyro_angle_y=0, Gyro_angle_z=0;
int Acc_x, Acc_y, Acc_z;                           //������ٶ�
int Temp;                                        //�¶�

int		xdata g_x=0,g_y=0,g_z=0;					         //�����ǽ�������
float	xdata a_x=0,a_y=0;							           //�ǶȽ�������
float	data  AngleX=0, AngleY=0, AngleZ=0;					 //��Ԫ���������ŷ����
float	xdata Angle_gx=0, Angle_gy=0, Angle_gz=0;		 //�ɽ��ٶȼ���Ľ�����(�Ƕ���)
float	xdata Angle_ax=0, Angle_ay=0, Angle_az=0;		 //�ɼ��ٶȼ���ļ��ٶ�(������)

uchar MPU6050_DATA[14];

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

uchar oled_showtext[25]; //oled��ʾ�ַ���

void Disp_refresh(void);  //�������ʾ����
void Motor_control(void); //������ƺ���
void MPU6050_Read(void);  //���������ݲɼ�
void Key_Proc(void);

//*****************����***********//

unsigned char key_old = 0;
unsigned char key_value = 0;
unsigned char key_Down = 0;


void main()
{
	GPIO_init();//GPIO��ʼ��
	OLED_Init();//oled��ʼ��
	OLED_ColorTurn(0);//0������ʾ��1 ��ɫ��ʾ
	OLED_DisplayTurn(0);//0������ʾ 1 ��Ļ��ת��ʾ
	OLED_Clear();//oled����
	
	Timer1Init();//��ʱ����ʼ��
	
	UartInit();//���ڳ�ʼ��	
	S1_S0=0;
	S1_S1=0;//����1 ѡ��P30 P31	
	
	P54RST=1;//��λ��ʼ��
	
	InitMPU6050(); //mpu6050��ʼ��
	
	Motor_Init(); //�����ʼ��
	
	//PID����
	positionPID.basicSpeed = 300;//�����˶��ٶ�
	positionPID.kp = 400;
	positionPID.ki = 0;
	positionPID.kd = 0;
	
	LED = 0;

	while(1)
	{	
		Motor_control(); //������ƺ���
		Disp_refresh();	 //��ʾ��ˢ�º���
		MPU6050_Read();  //���������ݲɼ�
		Key_Proc();
		//sprintf(txbuf,"1:%04d 2:%04d 3:%04d\r\n",ADCP1,ADCP2,ADCP3);
		//Uart_String(txbuf); //����
	}
}
//-----------------�ж�-----------------------------------
void timer1() interrupt 3       //100us�ж�һ��
{
	if(++disp_delay == 100) disp_delay = 0;       //��ʾ��ˢ��ʱ��
	if(++motor_delay == 10) motor_delay = 0;			//�������ˢ��ʱ��
	if(++mpu6050_delay == 50) mpu6050_delay = 0;  //mpu6050ִ��ˢ��ʱ��
	if(++key_delay == 100) key_delay = 0;        //����ɨ��ˢ��ʱ��
	
	if(++sys_led >= 5000)                         
	{
		LED ^= 1;
		sys_led = 0;
	}
	
	if(delay_cnt > 0) //��ʱ����
		delay_cnt--;

} 

//-----------------��������-----------------------------------
void Key_Proc(void)
{
	if(key_delay) return; //100msɨ��һ�ΰ���
	key_delay = 1;
	
	key_value = Key_Rvalue();//��ȡ�������µı��
	key_Down = key_value & (key_old ^ key_value);																		
	key_old = key_value;	
	
	switch(key_Down)
	{
		case 1:
				positionPID.kp++;
		break;
		case 2:
				positionPID.kp--;
		break;
		case 3:
				positionPID.kd++;
		break;
		case 4:
				positionPID.kd--;
		break;
	}
}

//-----------------��ʾ������-----------------------------------
void Disp_refresh(void)
{
	if(disp_delay) return; //10msˢ��һ����Ļ
	disp_delay = 1;
	
	sprintf(oled_showtext,"line:%2d  ",line_inaccuracy);
	OLED_16x16(0,0,oled_showtext);
	
	sprintf(oled_showtext,"L:%3d,R%3d ",dutyL,dutyR);
	OLED_16x16(0,2,oled_showtext);
	
	sprintf(oled_showtext,"basic:%3d ",positionPID.basicSpeed);
	OLED_16x16(0,4,oled_showtext);

//	sprintf(oled_showtext,"X:%3d Y:%3d     ",Angle_ax,Angle_ax);
//	OLED_Display_string_5x7(0,6,oled_showtext);
	
	sprintf(oled_showtext,"Z:%02d  ",(int)AngleZ);
	OLED_Display_string_5x7(0,7,oled_showtext);
	
	sprintf(oled_showtext,"p:%4d,d:%4d",(int)positionPID.kp,(int)positionPID.kd);
	OLED_Display_string_5x7(50,7,oled_showtext);
	
	//sprintf(txbuf,"X:%d Y:%d Z:%d\r\n",Gyro_x,Gyro_y,Gyro_z);
	//Uart_String(txbuf); //����
	
//	OLED_ShowNum(35,4,ADCP3,6);
}


//-------------�����ǿ��ƺ���----------------------------------------
void MPU6050_Read(void)
{
	if(mpu6050_delay) return; 
	mpu6050_delay = 1;
	
	Read_MPU6050(MPU6050_DATA);
	Acc_x = MPU6050_DATA[0]<<8|MPU6050_DATA[1]; //���ٶ�
	Acc_y = MPU6050_DATA[2]<<8|MPU6050_DATA[3];
	Acc_z = MPU6050_DATA[4]<<8|MPU6050_DATA[5];
	
	Temp  = MPU6050_DATA[6]<<8|MPU6050_DATA[7]; //temperature
	
	Gyro_x = MPU6050_DATA[8]<<8|MPU6050_DATA[9]; //���ٶ�
	Gyro_y = MPU6050_DATA[10]<<8|MPU6050_DATA[11]; 
	Gyro_z = MPU6050_DATA[12]<<8|MPU6050_DATA[13]; 
	
//	if((Gyro_x == -1)||(Gyro_y == -1)||(Gyro_z == -1)||(Gyro_x == 0)||(Gyro_y == 0)||(Gyro_z == 0)) //��ͨ�˲�
//	{
//		Gyro_x = Gyro_x_;
//		Gyro_y = Gyro_y_;
//		Gyro_z = Gyro_z_;
//	}
//	else 
//	{
//		Gyro_x_ = Gyro_x;
//		Gyro_y_ = Gyro_y;
//		Gyro_z_ = Gyro_z;
//	}
	
	Angle_ax = Acc_x/8192.0; //ƫ�ƽ�
	Angle_ay = Acc_y/8192.0; 
	Angle_az = Acc_z/8192.0; 
	
	Angle_gx = Gyro_x/65.5; //
	Angle_gy = Gyro_y/65.5;
	Angle_gz = Gyro_z/65.5;
	
	IMUupdate(Angle_gx*0.0174533f,Angle_gy*0.0174533f,Angle_gz*0.0174533f,Angle_ax,Angle_ay,Angle_az);
	
	Angle_of_pitch = AngleX;
	Roll_Angle = AngleY;
	
	//==========���ڲ鿴����=============
//	Uart_sendbyte(0x03);
//	Uart_sendbyte(~0x03);	
//	
//	Uart_sendbyte((int)(Gyro_x));
//	Uart_sendbyte((int)(Gyro_x)>>8);
//														
//	Uart_sendbyte((int)(Gyro_y));
//	Uart_sendbyte((int)(Gyro_y)>>8);
//														
//	Uart_sendbyte((int)(Gyro_z));
//	Uart_sendbyte((int)(Gyro_z)>>8);
//	
////	Uart_sendbyte((int)(Angle_gx));
////	Uart_sendbyte((int)(Angle_gx)>>8);
////														
////	Uart_sendbyte((int)(Angle_gy));
////	Uart_sendbyte((int)(Angle_gy)>>8);
////														
////	Uart_sendbyte((int)(Angle_gz));
////	Uart_sendbyte((int)(Angle_gz)>>8);

////	Uart_sendbyte((int)(AngleX));
////	Uart_sendbyte((int)(AngleX)>>8);
////														
////	Uart_sendbyte((int)(AngleY));
////	Uart_sendbyte((int)(AngleY)>>8);
////														
////	Uart_sendbyte((int)(AngleZ));
////	Uart_sendbyte((int)(AngleZ)>>8);	
//	
//	Uart_sendbyte(~0x03);					
//	Uart_sendbyte(0x03);
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
	AngleZ = atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.2957795f; //�˲����ֵ
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
		dutyR = positionPID.basicSpeed + err_kp + err_kd; 
		dutyL = positionPID.basicSpeed - err_kp - err_kd;
	}
	else 
	{
		dutyR = positionPID.basicSpeed;
	} dutyL = positionPID.basicSpeed;
	

	
	Update_duty(motor_sw,dutyR,dutyL);//����PWM���
	
	
	
	old_line_inaccuracy = line_inaccuracy;
	
	
}
