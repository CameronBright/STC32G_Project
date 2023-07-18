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
extern uint s_count;         //计数加加
void main()
{
	GPIO_init();//GPIO初始化
	OLED_Init();//oled初始化
	OLED_ColorTurn(0);//0正常显示，1 反色显示
	OLED_DisplayTurn(0);//0正常显示 1 屏幕翻转显示
	OLED_Clear();//oled清屏
	Timer1Init();//定时器初始化
	UartInit();//串口初始化	
	S1_S0=0;S1_S1=0;//串口1 选择P30 P31	
	P54RST=1;//复位初始化
	
	MOTOR_AIN1 = 1;
	MOTOR_AIN2 = 0;
	
	MOTOR_BIN1 = 1;
	MOTOR_BIN2 = 0;
	
	while(1)
	{	
		//rotary_encoder();        //一直扫描旋转编码器函数 ,检测上升沿 下降沿		
		//Cancel_determine();      //按键取消和确定检测函数,计时方式
		//Memu();
		
//		Uart_sendbyte('1');
//		Uart_String("1323\r\n");
//		IDL = 1;
		
	}
}
void timer1() interrupt 3       //100us加一次
{
	Pwm_Fun();                 //pwm函数
	if(EC11_K==0)
	{
		long_short=1;          //编码器按下为0   
	}		
	if(long_short==1)         //进入计时的条件
	{
		s_count++;   
	}
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