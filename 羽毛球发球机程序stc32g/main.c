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
uint mpu6050_delay;					 //mpu6050读取延时计数

//----------------- motor(电机驱动和PID变量)--------------------------------
extern int dutyL;            //左边电机驱动pwm 周期1000
extern int dutyR;            //右

uchar motor_sw = 1;//电机开关

struct pid_parameter positionPID; //PID参数
char line_inaccuracy; //循迹模块偏移量
char old_position;    //上一次的数据

//------------------MPU6050-----------------------------------------------

int Gyro_x, Gyro_y, Gyro_z;                        //三轴陀螺仪
int Gyro_x_, Gyro_y_, Gyro_z_;                        //三轴陀螺仪滤波

int Gyro_angle_x=0, Gyro_angle_y=0, Gyro_angle_z=0;
int Acc_x, Acc_y, Acc_z;                           //三轴加速度
int Temp;                                        //温度

int		xdata g_x=0,g_y=0,g_z=0;					         //陀螺仪矫正参数
float	xdata a_x=0,a_y=0;							           //角度矫正参数
float	data  AngleX=0, AngleY=0, AngleZ=0;					 //四元数解算出的欧拉角
float	xdata Angle_gx=0, Angle_gy=0, Angle_gz=0;		 //由角速度计算的角速率(角度制)
float	xdata Angle_ax=0, Angle_ay=0, Angle_az=0;		 //由加速度计算的加速度(弧度制)

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
uchar txbuf[20]; //串口发送缓存

uchar oled_showtext[25]; //oled显示字符串

void Disp_refresh(void);  //数码管显示函数
void Motor_control(void); //电机控制函数
void MPU6050_Read(void);  //陀螺仪数据采集

void main()
{
	GPIO_init();//GPIO初始化
	OLED_Init();//oled初始化
	OLED_ColorTurn(0);//0正常显示，1 反色显示
	OLED_DisplayTurn(1);//0正常显示 1 屏幕翻转显示
	OLED_Clear();//oled清屏
	
	Timer1Init();//定时器初始化
	
	UartInit();//串口初始化	
	S1_S0=0;
	S1_S1=0;//串口1 选择P30 P31	
	
	P54RST=1;//复位初始化
	
	Motor_Init(); //电机初始化
	positionPID.basicSpeed = 400;//基础运动速度
	
	InitMPU6050(); //mpu6050初始化
	
	while(1)
	{	
		Motor_control(); //电机控制函数
		Disp_refresh();	 //显示屏刷新函数
		MPU6050_Read();  //陀螺仪数据采集
		
		//sprintf(txbuf,"1:%04d 2:%04d 3:%04d\r\n",ADCP1,ADCP2,ADCP3);
		//Uart_String(txbuf); //串口
	}
}
void timer1() interrupt 3       //100us中断一次
{
	if(++disp_delay == 100) disp_delay = 0;
	if(++motor_delay == 10) motor_delay = 0;
	if(++mpu6050_delay == 50) mpu6050_delay = 0;
	
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

	sprintf(oled_showtext,"X:%3d Y:%3d",Gyro_x,Gyro_y);
	OLED_Display_string_5x7(0,6,oled_showtext);
	
	sprintf(oled_showtext,"Z:%3d",Gyro_z);
	OLED_Display_string_5x7(0,7,oled_showtext);
	
	//sprintf(txbuf,"X:%d Y:%d Z:%d\r\n",Gyro_x,Gyro_y,Gyro_z);
	//Uart_String(txbuf); //串口
	
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

void MPU6050_Read(void)
{
	if(mpu6050_delay) return; //10ms刷新一次屏幕
	mpu6050_delay = 1;
	
	Read_MPU6050(MPU6050_DATA);
	Acc_x = MPU6050_DATA[0]<<8|MPU6050_DATA[1]; //加速度
	Acc_y = MPU6050_DATA[2]<<8|MPU6050_DATA[3];
	Acc_z = MPU6050_DATA[4]<<8|MPU6050_DATA[5];
	
	Temp  = MPU6050_DATA[6]<<8|MPU6050_DATA[7]; //temperature
	
	Gyro_x = MPU6050_DATA[8]<<8|MPU6050_DATA[9]; //陀螺仪
	Gyro_y = MPU6050_DATA[10]<<8|MPU6050_DATA[11]; 
	Gyro_z = MPU6050_DATA[12]<<8|MPU6050_DATA[13]; 
	
	if((Gyro_x == -1)||(Gyro_y == -1)||(Gyro_z == -1)||(Gyro_x == 0)||(Gyro_y == 0)||(Gyro_z == 0)) //高通滤波
	{
		Gyro_x = Gyro_x_;
		Gyro_y = Gyro_y_;
		Gyro_z = Gyro_z_;
	}
	else 
	{
		Gyro_x_ = Gyro_x;
		Gyro_y_ = Gyro_y;
		Gyro_z_ = Gyro_z;
	}
	
	//串口查看波形
	Uart_sendbyte(0x03);
	Uart_sendbyte(~0x03);	
	
	Uart_sendbyte((int)(Gyro_x));
	Uart_sendbyte((int)(Gyro_x)>>8);
	
	Uart_sendbyte((int)(Gyro_y));
	Uart_sendbyte((int)(Gyro_y)>>8);
	
	Uart_sendbyte((int)(Gyro_z));
	Uart_sendbyte((int)(Gyro_z)>>8);		
	
	Uart_sendbyte(~0x03);					
	Uart_sendbyte(0x03);
	
	Angle_ax = Acc_x/8192.0; //偏移角
	Angle_ay = Acc_y/8192.0; 
	Angle_az = Acc_z/8192.0; 
	
	Angle_gx = Gyro_x/65.5; //
	Angle_gy = Gyro_y/65.5;
	Angle_gz = Gyro_z/65.5;
	
	IMUupdate(Angle_gx*0.0174533f,Angle_gy*0.0174533f,Angle_gz*0.0174533f,Angle_ax,Angle_ay,Angle_az);
	
	Angle_of_pitch = AngleX;
	Roll_Angle = AngleY;
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

