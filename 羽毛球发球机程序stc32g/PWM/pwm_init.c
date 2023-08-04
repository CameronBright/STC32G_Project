#include "pwm_init.h"



int dutyL = 0;
int dutyR = 0;

extern int Left_moto1     = 0;//����PWM
extern int Right_moto2    = 0;//��
extern int Forwar_dmoto1  = 0;//ǰ
extern int Backward_moto2 = 0;//��

unsigned int PWMB_CCR5 = 300;
unsigned int PWMB_CCR6 = 300;
unsigned int PWMB_CCR7 = 300;
unsigned int PWMB_CCR8 = 300;

unsigned int PWMB_CCA = 1000;
void PWM_Config(void)
{
	PWMA_PS = 0xAA;							//PWMA:P60 P62 P64 P66
	
	PWMA_PSCRH = (uint)(PWM_PSC >> 8);
	PWMA_PSCRL = (uint)(PWM_PSC); 				//23+1=24Ԥ��Ƶ
	
	
	PWMA_CCER1 = 0x00; // д CCMRx ǰ���������� CCERx �ر�ͨ��
	PWMA_CCER2 = 0x00; 
	PWMA_CCMR1 = 0x70; // ���� CC1 Ϊ PWMģʽ2
	PWMA_CCMR2 = 0x70; // ���� CC2 Ϊ PWMģʽ2
	PWMA_CCMR3 = 0x70; // ���� CC3 Ϊ PWMģʽ2
	PWMA_CCMR4 = 0x70; // ���� CC4 Ϊ PWMģʽ2
	PWMA_CCER1 = 0x33; // ʹ�� CC1 CC2ͨ��
	PWMA_CCER2 = 0x33; // ʹ�� CC3 CC4ͨ��
	
	
	PWMA_CCR1H = (uint)(PWMB_CCA >> 8); // ����CC1ռ�ձ�ʱ�� ռ�ձ�2.5%
	PWMA_CCR1L = (uint)(PWMB_CCA); 
	
	PWMA_CCR2H = (uint)(PWMB_CCA >> 8); // ����CC2ռ�ձ�ʱ�� ռ�ձ�2.5%
	PWMA_CCR2L = (uint)(PWMB_CCA); 
	
	PWMA_CCR3H = (uint)(PWMB_CCA >> 8); // ����CC3ռ�ձ�ʱ�� ռ�ձ�2.5%
	PWMA_CCR3L = (uint)(PWMB_CCA); 
	
	PWMA_CCR4H = (uint)(PWMB_CCA >> 8); // ����CC4ռ�ձ�ʱ�� ռ�ձ�2.5%
	PWMA_CCR4L = (uint)(PWMB_CCA); 
	

	PWMA_ARRH = (uint)(PWM_PERIOD >> 8); // ����PWMA����20����
	PWMA_ARRL = (uint)(PWM_PERIOD); 

	PWMA_ENO = 0x55; 										// ʹ�� PWM1P 2p �˿����
	PWMA_BKR = 0x80;										// ʹ�������

	PWMA_CR1 = 0x01; 										// ����PWM��ʱ��	
	
	
	//PWMB
	PWMB_PS = 0xAA;							      //PWMB:P00 P01 P02 P03
	
	PWMB_PSCRH = (uint)(PWM_PSC >> 8);
	PWMB_PSCRL = (uint)(PWM_PSC); 				//23+1=24Ԥ��Ƶ
	
	
	PWMB_CCER1 = 0x00; // д CCMRx ǰ���������� CCERx �ر�ͨ��
	PWMB_CCER2 = 0x00; 
	
	PWMB_CCMR1 = 0x70; // ���� CC1 Ϊ PWMģʽ2
	PWMB_CCMR2 = 0x70; // ���� CC2 Ϊ PWMģʽ2
	PWMB_CCMR3 = 0x70; // ���� CC3 Ϊ PWMģʽ2
	PWMB_CCMR4 = 0x70; // ���� CC4 Ϊ PWMģʽ2
	PWMB_CCER1 = 0x33; // ʹ�� CC1 CC2ͨ��
	PWMB_CCER2 = 0x33; // ʹ�� CC3 CC4ͨ��
	
	
	PWMB_CCR5H = (uint)(PWMB_CCR5 >> 8); // ����CC5ռ�ձ�ʱ�� ռ�ձ�2.5%
	PWMB_CCR5L = (uint)(PWMB_CCR5); 
	
	PWMB_CCR6H = (uint)(PWMB_CCR6 >> 8); // ����CC6ռ�ձ�ʱ�� ռ�ձ�2.5%
	PWMB_CCR6L = (uint)(PWMB_CCR6); 

	PWMB_CCR7H = (uint)(PWMB_CCR7 >> 8); // ����CC7ռ�ձ�ʱ�� ռ�ձ�2.5%
	PWMB_CCR7L = (uint)(PWMB_CCR7); 
	
	PWMB_CCR8H = (uint)(PWMB_CCR8 >> 8); // ����CC8ռ�ձ�ʱ�� ռ�ձ�2.5%
	PWMB_CCR8L = (uint)(PWMB_CCR8); 
	

	PWMB_ARRH = (uint)(PWM_PERIOD >> 8); // ����PWMA����20����
	PWMB_ARRL = (uint)(PWM_PERIOD); 

	PWMB_ENO = 0x55; 										// ʹ�� PWM1P 2p �˿����
	PWMB_BKR = 0x80;										// ʹ�������

	PWMB_CR1 = 0x01; 										// ����PWM��ʱ��		
}


//����PWMB 5
void Update_5(unsigned int Duty1)
{
	PWMB_CCR5H = (uint)(Duty1 >> 8); // ����CC5ռ�ձ�ʱ�� ռ�ձ�2.5%
	PWMB_CCR5L = (uint)(Duty1); 
}
//����PWMB 6
void Update_6(unsigned int Duty2)
{
	PWMB_CCR6H = (uint)(Duty2 >> 8); // ����CC6ռ�ձ�ʱ�� ռ�ձ�2.5%
	PWMB_CCR6L = (uint)(Duty2); 	
}
//����PWMB 7
void Update_7(unsigned int Duty3)
{
	PWMB_CCR7H = (uint)(Duty3 >> 8); // ����CC7ռ�ձ�ʱ�� ռ�ձ�2.5%
	PWMB_CCR7L = (uint)(Duty3); 	
}
//����PWMB 8
void Update_8(unsigned int Duty4)
{
	PWMB_CCR8H = (uint)(Duty4 >> 8); // ����CC8ռ�ձ�ʱ�� ռ�ձ�2.5%
	PWMB_CCR8L = (uint)(Duty4); 	
}

/**************************************************************************
�������ܣ�����ֵ����
��ڲ�����int
����  ֵ��unsigned int
Ŀ    �ģ���PWM���о���ֵ����
**************************************************************************/
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}

/**************************************************************************
�������ܣ�����PWM��ֵ 
��ڲ�������
����  ֵ����
**************************************************************************/
void Xianfu_Pwm(void)
{
	 //===PWM������100 ������95
    if(Left_moto1<-950 ) Left_moto1=-950 ;
		if(Left_moto1>950 )  Left_moto1=950 ;
	  if(Right_moto2<-950 ) Right_moto2=-950 ;
		if(Right_moto2>950 )  Right_moto2=950 ;
	  if(Forwar_dmoto1<-950 ) Forwar_dmoto1=-950 ;
		if(Forwar_dmoto1>950 )  Forwar_dmoto1=950 ;
	  if(Backward_moto2<-950 ) Backward_moto2=-950 ;
		if(Backward_moto2>950 )  Backward_moto2=950 ;
}

/**************************************************************************
�������ܣ�����쳣�رյ��
��ڲ���������쳣����
����  ֵ����
**************************************************************************/
//void Turn_Off(float angle)
//{
//		if(angle<-40||angle>40)	 //��ص�ѹ����11.1V�رյ��
//		{	                                   //===��Ǵ���40�ȹرյ��																			 
//				moto1=0;
//				moto2=0;
//		}		
//}

/**************************************************************************
�������ܣ���ֵ��PWM�Ĵ���
��ڲ���������PWM������PWM
����  ֵ����
**************************************************************************/

void Set_LRPwm(int Left_moto1,int Right_moto2)    //�������ҷ��ռ�ձ�
{
	
	
	if(Left_moto1<0)		
	AIN2=1,			AIN1=0;  //���С������Ƿ�ת  �ȱ任��ƽ  motol
	else
	AIN2=0,			AIN1=1;
	
	
	PWMA_CCR1H = (myabs(Left_moto1) >> 8);  
	PWMA_CCR1L = (myabs(Left_moto1));

	
	if(Right_moto2<0)		
	BIN1=0,			BIN2=1;
	
	else
	BIN1=1,			BIN2=0;
	
	PWMA_CCR2H = (myabs(Right_moto2)>> 8);  
	PWMA_CCR2L = (myabs(Right_moto2));
	
	
}


void Set_FBPwm(int Forwar_dmoto1,int Backward_moto2)    //����ǰ����ռ�ձ�
{
	
	
	if(Forwar_dmoto1<0)		
	CIN2=1,			CIN1=0;  //���С������Ƿ�ת  �ȱ任��ƽ  motol
	else
	CIN2=0,			CIN1=1;
	
	
	PWMA_CCR3H = (myabs(Forwar_dmoto1) >> 8);  
	PWMA_CCR3L = (myabs(Forwar_dmoto1));

	
	if(Forwar_dmoto1<0)		
	DIN1=0,			DIN2=1;
	
	else
	DIN1=1,			DIN2=0;
	
	PWMA_CCR4H = (myabs(Backward_moto2)>> 8);  
	PWMA_CCR4L = (myabs(Backward_moto2));
	
	
}

void Update_duty(uchar sw,int PWMR,int PWML)										//����ռ�ձȺ���
{
	
	if(!sw)
	{
		PWML = 0;
		PWMR = 0;
	}
	
	if(PWMR < 0)
	{
		PWMR = -PWMR;
		MotorR_Reverse();
	}
	else 
		MotorR_Forward();
		
	if(PWML < 0)
	{
		PWML = -PWML;
		MotorL_Reverse();
	}
	else 
		MotorL_Forward();
		
	
	PWMA_CCR1H	= (uchar)(PWML >> 8);  
	PWMA_CCR1L = PWML;                //����ռ�ձ�
		
	PWMA_CCR2H = (uchar)(PWMR >> 8); 
	PWMA_CCR2L = PWMR;	
}

