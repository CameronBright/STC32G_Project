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
#include "LineFollower.h"

extern uint s_count;         //�����Ӽ�

extern uint duty;
extern uint duty2;

extern uint delay_cnt; //delay����
uint timer_delay = 0; //1us tick

uchar txbuf[20]; //���ڷ��ͻ���

uchar oled_showtext[20]; //oled��ʾ�ַ���

uint ADCP1;
uint ADCP2;
uint ADCP3;

int line_inaccuracy = 0;

void Disp_refresh(void);

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
	
	PWMA_Config();
	
	ADC_Init();
	
	MOTOR_AIN1 = 0;
	MOTOR_AIN2 = 1;
	
	MOTOR_BIN1 = 0;
	MOTOR_BIN2 = 1;
	
	duty = 600;
	duty2 = 600;
	
	while(1)
	{	
		//rotary_encoder();        //һֱɨ����ת���������� ,��������� �½���		
		//Cancel_determine();      //����ȡ����ȷ����⺯��,��ʱ��ʽ
		//Memu();
			
		ADCP1 = ADC_Readchannel_1();  //P11		
		ADCP2 = ADC_Readchannel_2();  //P00	
		ADCP3 = ADC_Readchannel_3();	//P10
		
		line_inaccuracy = ReadLine();
		
//		if(line_inaccuracy != 0)
//		{
//			if(line_inaccuracy > 0)
//				
//		}
		//sprintf(txbuf,"1:%04d 2:%04d 3:%04d\r\n",ADCP1,ADCP2,ADCP3);
		//Uart_String(txbuf);
		
		Disp_refresh();
		//delay(1000);
		
		Update_duty();	
	}
}
void timer1() interrupt 3       //100us��һ��
{
	if(delay_cnt > 0)
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
	sprintf(oled_showtext,"P11:");
	OLED_16x16(0,0,oled_showtext);
	sprintf(oled_showtext,"P00:");
	OLED_16x16(0,2,oled_showtext);
	sprintf(oled_showtext,"P10:");
	OLED_16x16(0,4,oled_showtext);
	
	OLED_ShowNum(35,0,ADCP1,6);
	OLED_ShowNum(35,2,ADCP2,6);
	OLED_ShowNum(35,4,ADCP3,6);
	
	if(line_inaccuracy < 0)
	{
		line_inaccuracy = line_inaccuracy * -1;
		sprintf(oled_showtext,"- ");
		OLED_16x16(0,6,oled_showtext);
		OLED_ShowNum(15,6,line_inaccuracy,6);
	}
	else 
	{
//		sprintf(oled_showtext," ");
//		OLED_16x16(0,6,oled_showtext);
		OLED_ShowNum(0,6,line_inaccuracy,6);
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

