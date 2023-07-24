#include "timer.h"   //定时器
#include "oled.h"   //OLED函数
#include "GPIO.h"   //GPIO
#include "pwm.h"      //pwm
#include "Encoders.h"  //旋转编码器
#include "iic.h"
#include "Uart.h"   //串口
#include "stdio.h"
#include "Meun.h"
#include "motor.h"
#include "ADC.h"
#include "LineFollower.h" //循迹模块
#include "stdio.h"
#include "MPU6050.h"
#include "MATH.H"

//-------------------------------- system--------------------------------
extern uint s_count;         //定时器计数
extern uint delay_cnt;       //delay计数
uint timer_delay = 0;        //1us tick

uint disp_delay;             //显示屏刷新延时计数
uint motor_delay;						 //电机函数刷新延时计数

//----------------- motor(电机驱动和PID变量)--------------------------------
extern int dutyL;            //左边电机驱动pwm 周期1000
extern int dutyR;            //右

uchar motor_sw = 1;//电机开关

struct pid_parameter positionPID; //PID参数
char line_inaccuracy; //循迹模块偏移量
char old_position;    //上一次的数据

//------------------MPU6050-----------------------------------------------

int Gyro_x, Gyro_y, Gyro_z;                        //三轴陀螺仪
int Gyro_angle_x=0, Gyro_angle_y=0, Gyro_angle_z=0;
int Acc_x, Acc_y, Acc_z;                           //三轴加速度
int Temp;                                        //温度

int		xdata g_x=0,g_y=0,g_z=0;					         //陀螺仪矫正参数
float	xdata a_x=0,a_y=0;							           //角度矫正参数
float	data  AngleX=0, AngleY=0, AngleZ=0;					 //四元数解算出的欧拉角
float	xdata Angle_gx=0, Angle_gy=0, Angle_gz=0;		 //由角速度计算的角速率(角度制)
float	xdata Angle_ax=0, Angle_ay=0, Angle_az=0;		 //由加速度计算的加速度(弧度制)

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
uchar txbuf[20]; //串口发送缓存

uchar oled_showtext[20]; //oled显示字符串

void Disp_refresh(void);  //数码管显示函数
void Motor_control(void); //电机控制函数

void main()
{
	GPIO_init();//GPIO初始化
	OLED_Init();//oled初始化
	OLED_ColorTurn(0);//0正常显示，1 反色显示
	OLED_DisplayTurn(1);//0正常显示 1 屏幕翻转显示
	OLED_Clear();//oled清屏
	
	Timer1Init();//定时器初始化
	UartInit();//串口初始化	
	S1_S0=0;S1_S1=0;//串口1 选择P30 P31	
	P54RST=1;//复位初始化
	
	
	Motor_Init();
	positionPID.basicSpeed = 400;
	
	while(1)
	{	
		//rotary_encoder();        //一直扫描旋转编码器函数 ,检测上升沿 下降沿		
		//Cancel_determine();      //按键取消和确定检测函数,计时方式
		//Memu();
		
		Motor_control();
		Disp_refresh();	
		
		//sprintf(txbuf,"1:%04d 2:%04d 3:%04d\r\n",ADCP1,ADCP2,ADCP3);
		//Uart_String(txbuf); //串口
	}
}
void timer1() interrupt 3       //100us加一次
{
	if(++disp_delay == 100) disp_delay = 0;
	if(++motor_delay == 10) motor_delay = 0;
	
	if(delay_cnt > 0) //延时函数
		delay_cnt--;
	
	if(++timer_delay > 10000)
	//Pwm_Fun();                 //pwm函数
	if(EC11_K==0)
	{
		long_short=1;          //编码器按下为0   
	}		
	if(long_short==1)         //进入计时的条件
	{
		s_count++;   
	}
} 

void Disp_refresh(void)
{
	if(disp_delay) return; //10ms刷新一次屏幕
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
	
	line_inaccuracy = ReadLine();//读取循线状态 1、-1、0
	
	if(line_inaccuracy > 1 || line_inaccuracy < -1)
	{
		if(line_inaccuracy == -2)//传感器远离地面时
			motor_sw = 0;
		else if(line_inaccuracy == 2) //所有传感器都在地面但没识别到线时
			line_inaccuracy = old_position;
	}
	else 
	{
		motor_sw = 1;//正常
		old_position = line_inaccuracy;//记录上一次的位置
	}
		
	dutyR = positionPID.basicSpeed + line_inaccuracy*600; //右偏左偏
	dutyL = positionPID.basicSpeed - line_inaccuracy*600;
	
	Motor_FRcontrol(dutyR,dutyL);//pwm值小于0就反转，大于0正转
	
	Update_duty(motor_sw);//更新PWM输出
}

void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az)
{
	float data norm;
	float idata vx, vy, vz;
	float idata ex, ey, ez;

	norm = sqrt(ax*ax + ay*ay + az*az);	//把加速度计的三维向量转成单维向量   
	ax = ax / norm;
	ay = ay / norm;
	az = az / norm;

		//	下面是把四元数换算成《方向余弦矩阵》中的第三列的三个元素。 
		//	根据余弦矩阵和欧拉角的定义，地理坐标系的重力向量，转到机体坐标系，正好是这三个元素
		//	所以这里的vx vy vz，其实就是当前的欧拉角（即四元数）的机体坐标参照系上，换算出来的
		//	重力单位向量。
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

	AngleX = asin(2*(q0*q2 - q1*q3 )) * 57.2957795f; // 俯仰   换算成度
	AngleY = asin(2*(q0*q1 + q2*q3 )) * 57.2957795f; // 横滚
	AngleZ = atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.2957795f;
}

//	IT0=0;  //中断0    IT0=0;上升沿和下降沿触发   IT0=1 下降沿触发
//	EX0=1;	//使能中断0
//	IT1=0;  //中断0    IT0=0;上升沿和下降沿触发   IT0=1 下降沿触发
//	EX1=1;	//使能中断0

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

