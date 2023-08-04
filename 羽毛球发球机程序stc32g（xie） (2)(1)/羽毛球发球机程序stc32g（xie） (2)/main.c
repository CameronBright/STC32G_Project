/*
program versions : 3.2

增加了电机控制函数，此版本已能悬停

modification: 2023/8/5 0:18

modifier: Cameron Bright

*/
#include "timer.h"   //定时器
#include "oled.h"   //OLED函数
#include "GPIO.h"   //GPIO
#include "key.h"  	//按键
#include "Uart.h"   //串口
#include "stdio.h"  //c标准库
#include "motor.h"  //电机
#include "LineFollower.h" //循迹模块
#include "MPU6050.h"//mpu6050
#include "MATH.H"   //数学运算模块

//=================================

#include "pwm_init.h"  //pwm
#include "Wave.h"      //超声波

//-------------------------------- system--------------------------------
extern uint s_count;         //定时器计数
extern uint delay_cnt;       //delay计数
uint sys_led = 0;            //运行状态灯

uint disp_delay;             //显示屏刷新延时计数
uint motor_delay;						 //电机函数刷新延时计数
uint mpu6050_delay;					 //mpu6050读取延时计数
uint key_delay;              //案件延时刷新计数

//----------------- motor(电机驱动和PID变量)--------------------------------
//extern int dutyL;            //左边电机驱动pwm 周期1000
//extern int dutyR;            //右

//=======================================================================
//******************************
extern int Left_moto1     ;//左风机PWM
extern int Right_moto2    ;//右
extern int Forwar_dmoto1  ;//前
extern int Backward_moto2 ;//后

//======================================================================
uchar motor_sw = 1;//电机开关

struct pid_parameter positionPID; //PID参数
int err_kp, err_ki, err_kd;   //误差值
char line_inaccuracy; //循迹模块偏移量
char old_line_inaccuracy;//上一次循迹模块的便宜量
char old_position;    //上一次的数据

//------------------MPU6050-----------------------------------------------
//===================获取数据========================================================


extern float Angle_gx=0,Angle_gy=0,Angle_gz=0;		//由角速度计算的角速率(角度制)
extern float Angle_ax=0,Angle_ay=0,Angle_az=0;		//由加速度计算的加速度(弧度制)
float edata  Last_Angle_gx=0;					//外环PI输出量  上一次陀螺仪数据
float edata Last_Angle_gy=0;

int		edata g_x=0,g_y=0,g_z=0;					//陀螺仪矫正参数
float	edata a_x=0,a_y=0,a_z=0;							//角度矫正参数

extern int X_Result=0,Y_Result=0,Z_Result=0;						//加入偏差值 

unsigned char	tp[16];		//读MP6050缓冲
//-----------------other--------------------------------------------------
uchar txbuf[20]; //串口发送缓存

uchar oled_showtext[25]; //oled显示字符串

void Disp_refresh(void);  //数码管显示函数
void Motor_control(void); //电机控制函数
void MPU6050_Read(void);  //陀螺仪数据采集
void Key_Proc(void);

//*****************按键***********//

unsigned char key_old = 0;
unsigned char key_value = 0;
unsigned char key_Down = 0;
unsigned char key_up = 0;

extern unsigned int PWMB_CCR00;
extern unsigned int PWMB_CCR01;
extern unsigned int PWMB_CCR02;
extern unsigned int PWMB_CCR03;

extern unsigned int PWMB_CCA;

//*******超声波*****//
unsigned char Wave = 0;
unsigned char Wave_Buff[20];
extern unsigned long TF_Count;

//循迹启动按键
unsigned char T_S_B = 0;  //启动循迹按键
unsigned char flag = 0;
unsigned char flag_count = 0;
unsigned char Tracking_value = 0;  //循迹值

//*******流程*****//
unsigned char scene = 0;
bit wait_sw;
unsigned int wait_tick = 0;

void main()
{
	GPIO_init();//GPIO初始化
	OLED_Init();//oled初始化
	OLED_ColorTurn(0);//0正常显示，1 反色显示
	OLED_DisplayTurn(0);//0正常显示 1 屏幕翻转显示
	OLED_Clear();//oled清屏
	
	Timer0Init();//定时器 0 初始化
	Timer1_Init(); //定时器 1 初始化
	Timer3_Init();//定时器 3 初始化
	
	Uart1_Init();// 串口1初始化
	S1_S0=0; S1_S1=0;//串口1 选择P30 P31	
	
	P54RST=1;//复位初始化
	
	InitMPU6050(); //mpu6050初始化
	
	Motor_Init(); //电机初始化
	
	//PID参数
	positionPID.basicSpeed = 0;        //基础运动速度
	positionPID.kp = 600;
	positionPID.ki = 0;
	positionPID.kd = 50;
	
	PWMB_CCA = 1000; //浮空
	
	scene = 1;
		
	while(1)
	{
		Disp_refresh(); //数码管刷新函数
		MPU6050_Read(); //MPU6050控制函数
		Motor_control(); //电机控制函数
		Key_Proc();   //按键处理函数
		//Process(); //流程控制函数
	}
}

//定时器0中断函数
void Timer0() interrupt 1
{
	TF_Count += 1;
}

//-----------------定时器1中断-----------------------------------
void timer1() interrupt 3       //100us中断一次
{
	if(++disp_delay == 100) disp_delay = 0;       //显示屏刷新时间
	if(++motor_delay == 10) motor_delay = 0;			//电机控制刷新时间
	if(++mpu6050_delay == 50) mpu6050_delay = 0;  //mpu6050执行刷新时间
	if(++key_delay == 10) key_delay = 0;        //按键扫描刷新时间
	
	if(++sys_led >= 5000)                         
	{
		LED ^= 1;
		sys_led = 0;
	}
	
	if(delay_cnt > 0) //延时函数
		delay_cnt--;
	
} 

void Timer3_Isr(void) interrupt 19   //等待测试 1ms
{
	if(flag == 1)         
	{
		flag_count++;
		if(flag_count > 100)
		{
			Tracking_value ++;   //循迹次数
			flag_count = 0;
			flag = 0;
		}
	}
}


//-----------------按键处理-----------------------------------

void Key_Proc(void)
{
	if(key_delay) return; //100ms扫描一次按键
		key_delay = 1;
	
	key_value = Key_Rvalue();//读取按键按下的编号
	key_Down = key_value & (key_old ^ key_value);	
	key_up = ~key_value & (key_old ^ key_value);		
	key_old = key_value;	
	
	switch(key_Down)
	{
		case 1:
		PWMB_CCR00 += 50;
		if(PWMB_CCR00 > 1000)
			PWMB_CCR00 = 300;
		break;
		case 2:
		PWMB_CCR01 += 50;	
		if(PWMB_CCR01 > 1000)
			PWMB_CCR01 = 300;
		break;
		case 3:
		PWMB_CCR02 += 50;
		if(PWMB_CCR02 > 1000)
			PWMB_CCR02 = 300;
		break;
		case 4:
		PWMB_CCR03 += 50;
		if(PWMB_CCR03 > 1000)
			PWMB_CCR03 = 300;
		break;
	}
}

//-----------------显示屏函数-----------------------------------
void Disp_refresh(void)
{
	if(disp_delay) return; //10ms刷新一次屏幕
	disp_delay = 1;
	
	sprintf(oled_showtext,"Line:%d",line_inaccuracy); //循迹函数返回值
	OLED_Display_string_5x7(0,0,oled_showtext);
	
	sprintf(oled_showtext,"0:%03d 1:%03d ",PWMB_CCR00,PWMB_CCR01);
	OLED_Display_string_5x7(0,2,oled_showtext);
	
	sprintf(oled_showtext,"2:%03d 3:%03d ",PWMB_CCR02,PWMB_CCR03);
	OLED_Display_string_5x7(0,3,oled_showtext);

	sprintf(oled_showtext,"speed:%d",PWMB_CCA);
	OLED_Display_string_5x7(0,4,oled_showtext);
	
	
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

//		sprintf(oled_showtext,"P42 5: %5d ",PWMB_CCR00);
//		OLED_Display_string_5x7(0,0,oled_showtext);
//		sprintf(oled_showtext,"P41 6: %5d ",PWMB_CCR01);
//		OLED_Display_string_5x7(0,2,oled_showtext);
//		sprintf(oled_showtext,"P73 7: %5d ",PWMB_CCR02);
//		OLED_Display_string_5x7(0,4,oled_showtext);
//		sprintf(oled_showtext,"P70 8: %5d ",PWMB_CCR03);
//		OLED_Display_string_5x7(0,6,oled_showtext);
}

//-------------陀螺仪控制函数----------------------------------------
//相关变量

//------------
void MPU6050_Read(void)
{
	if(mpu6050_delay) return; 
	mpu6050_delay = 1;
	
		//********************************************************************************************
		Read_MPU6050(tp);	// 720us @24MHz

		Angle_ax = ((float)(((int *)&tp)[0])) / 8192.0;	//加速度处理	结果单位是 +- g
		Angle_ay = ((float)(((int *)&tp)[1])) / 8192.0;	//转换关系	8192 LSB/g, 1g对应读数8192
		Angle_az = ((float)(((int *)&tp)[2])) / 8192.0;	//加速度量程 +-4g/S
		Last_Angle_gx = Angle_gx;		//储存上一次角速度数据
		Last_Angle_gy = Angle_gy;
		Angle_gx = ((float)(((int *)&tp)[4] - g_x)) / 65.5;	//陀螺仪处理	结果单位是 +-度
		Angle_gy = ((float)(((int *)&tp)[5] - g_y)) / 65.5;	//陀螺仪量程 +-500度/S, 1度/秒 对应读数 65.536
		Angle_gz = ((float)(((int *)&tp)[6] - g_z)) / 65.5;	//转换关系65.5 LSB/度


		//**********************************X轴指向************************************************
		X_Result  = Angle_ax * 100 ;		//处理过的数值减去偏差值
		//**************Y轴指向**************************************************
		Y_Result  = Angle_ay * 100;		//处理过的数值减去偏差值
		//**************Z轴指向*****************************	
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

//-----------------电机控制函数-------------------------------------
void Motor_control(void)
{
	if(motor_delay) return; //延时
	motor_delay = 1;
	
	if(scene == 1)
	{
		positionPID.basicSpeed = 0;
		sys_delay(60000);
		positionPID.basicSpeed = 600;
		scene = 0;
	}
		
	line_inaccuracy = ReadLine();//读取循线状态 2、1、0、-1、-2
	
	if(line_inaccuracy > 2 || line_inaccuracy < -2)
	{
		if(line_inaccuracy == 3)
			line_inaccuracy = old_position;
	}
	else 
	{
		old_position = line_inaccuracy;//记录上一次的位置
	}
	
	if(!line_inaccuracy)
	{
		PWMB_CCR02 = positionPID.basicSpeed;
		PWMB_CCR00 = PWMB_CCR01 = 0;
	}
	else if(line_inaccuracy > 0)
	{
		PWMB_CCR00 = (positionPID.kp * line_inaccuracy) + (positionPID.kd * old_line_inaccuracy);
		PWMB_CCR01 = 0;
		
	}
	else if(line_inaccuracy < 0)
	{
		PWMB_CCR01 = (positionPID.kp * -line_inaccuracy) + (positionPID.kd * -old_line_inaccuracy)+200;
		PWMB_CCR00 = 0;
	}
	
	Update_PWM(PWMB_CCR00,PWMB_CCR01,PWMB_CCR02,PWMB_CCR03,PWMB_CCA);
	old_line_inaccuracy = line_inaccuracy;
}

