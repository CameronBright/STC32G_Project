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
extern uint s_count;         //�����Ӽ�
void main()
{
	GPIO_init();//GPIO��ʼ��
	OLED_Init();//oled��ʼ��
	OLED_ColorTurn(0);//0������ʾ��1 ��ɫ��ʾ
	OLED_DisplayTurn(0);//0������ʾ 1 ��Ļ��ת��ʾ
	OLED_Clear();//oled����
	Timer1Init();//��ʱ����ʼ��
	UartInit();//���ڳ�ʼ��	
	S1_S0=0;S1_S1=0;//����1 ѡ��P30 P31	
	P54RST=1;//��λ��ʼ��
	
	MOTOR_AIN1 = 1;
	MOTOR_AIN2 = 0;
	
	MOTOR_BIN1 = 1;
	MOTOR_BIN2 = 0;
	
	while(1)
	{	
		//rotary_encoder();        //һֱɨ����ת���������� ,��������� �½���		
		//Cancel_determine();      //����ȡ����ȷ����⺯��,��ʱ��ʽ
		//Memu();
		
//		Uart_sendbyte('1');
//		Uart_String("1323\r\n");
//		IDL = 1;
		
	}
}
void timer1() interrupt 3       //100us��һ��
{
	Pwm_Fun();                 //pwm����
	if(EC11_K==0)
	{
		long_short=1;          //����������Ϊ0   
	}		
	if(long_short==1)         //�����ʱ������
	{
		s_count++;   
	}
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