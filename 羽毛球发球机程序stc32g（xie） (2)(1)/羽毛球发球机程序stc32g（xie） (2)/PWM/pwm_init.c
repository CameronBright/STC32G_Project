#include "pwm_init.h"

int dutyL = 0;
int dutyR = 0;

extern int Left_moto1     = 0;//����PWM
extern int Right_moto2    = 0;//��
extern int Forwar_dmoto1  = 0;//ǰ
extern int Backward_moto2 = 0;//��

unsigned int PWMB_CCR00 = 0;  //P00
unsigned int PWMB_CCR01 = 0;  //P01
unsigned int PWMB_CCR02 = 0;  //P0
unsigned int PWMB_CCR03 = 0;  //P03

unsigned int PWMB_CCA = 0;

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
	
	
	PWMB_CCR5H = (uint)(PWMB_CCR00 >> 8); // ����CC5ռ�ձ�ʱ�� ռ�ձ�2.5%
	PWMB_CCR5L = (uint)(PWMB_CCR00); 
	
	PWMB_CCR6H = (uint)(PWMB_CCR01 >> 8); // ����CC6ռ�ձ�ʱ�� ռ�ձ�2.5%
	PWMB_CCR6L = (uint)(PWMB_CCR01); 

	PWMB_CCR7H = (uint)(PWMB_CCR02 >> 8); // ����CC7ռ�ձ�ʱ�� ռ�ձ�2.5%
	PWMB_CCR7L = (uint)(PWMB_CCR02); 
	
	PWMB_CCR8H = (uint)(PWMB_CCR03 >> 8); // ����CC8ռ�ձ�ʱ�� ռ�ձ�2.5%
	PWMB_CCR8L = (uint)(PWMB_CCR03); 
	

	PWMB_ARRH = (uint)(PWM_PERIOD >> 8); // ����PWMA����20����
	PWMB_ARRL = (uint)(PWM_PERIOD); 

	PWMB_ENO = 0x55; 										// ʹ�� PWM1P 2p �˿����
	PWMB_BKR = 0x80;										// ʹ�������

	PWMB_CR1 = 0x01; 										// ����PWM��ʱ��		
}

/**************************************************************************
�������ܣ�����PWM��ֵ 
��ڲ�������
����  ֵ����
**************************************************************************/
//void Xianfu_Pwm(void)
//{
//	 //===PWM������100 ������95
//    if(Left_moto1<-950 ) Left_moto1=-950 ;
//		if(Left_moto1>950 )  Left_moto1=950 ;
//	  if(Right_moto2<-950 ) Right_moto2=-950 ;
//		if(Right_moto2>950 )  Right_moto2=950 ;
//	  if(Forwar_dmoto1<-950 ) Forwar_dmoto1=-950 ;
//		if(Forwar_dmoto1>950 )  Forwar_dmoto1=950 ;
//	  if(Backward_moto2<-950 ) Backward_moto2=-950 ;
//		if(Backward_moto2>950 )  Backward_moto2=950 ;
//}

//����PWM 0
void Update_Pwm0(unsigned int Duty0)
{
	PWMB_CCR5H = (uint)(Duty0 >> 8); // ����CC5ռ�ձ�ʱ�� ռ�ձ�2.5%
	PWMB_CCR5L = (uint)(Duty0); 
}
//����PWM 1
void Update_Pwm1(unsigned int Duty1)
{
	PWMB_CCR6H = (uint)(Duty1 >> 8); // ����CC6ռ�ձ�ʱ�� ռ�ձ�2.5%
	PWMB_CCR6L = (uint)(Duty1); 	
}
//����PWM 2
void Update_Pwm2(unsigned int Duty2)
{
	PWMB_CCR7H = (uint)(Duty2 >> 8); // ����CC7ռ�ձ�ʱ�� ռ�ձ�2.5%
	PWMB_CCR7L = (uint)(Duty2); 	
}
//����PWM 3
void Update_Pwm3(unsigned int Duty3)
{
	PWMB_CCR8H = (uint)(Duty3 >> 8); // ����CC8ռ�ձ�ʱ�� ռ�ձ�2.5%
	PWMB_CCR8L = (uint)(Duty3); 	
}
//����PWM 4 (���´���ĵ��)
void Update_Pwm4(unsigned int Duty4)
{
	PWMA_CCR3H = (uint)(Duty4 >> 8); // ����CC3ռ�ձ�ʱ�� ռ�ձ�2.5%
	PWMA_CCR3L = (uint)(Duty4); 
}

//void Set_FBPwm(int Forwar_dmoto1,int Backward_moto2)    //����ǰ����ռ�ձ�
//{
//	
//	
//	if(Forwar_dmoto1<0)		
//	CIN2=1,			CIN1=0;  //���С������Ƿ�ת  �ȱ任��ƽ  motol
//	else
//	CIN2=0,			CIN1=1;
//	
//	
//	PWMA_CCR3H = (myabs(Forwar_dmoto1) >> 8);  
//	PWMA_CCR3L = (myabs(Forwar_dmoto1));

//	
//	if(Forwar_dmoto1<0)		
//	DIN1=0,			DIN2=1;
//	
//	else
//	DIN1=1,			DIN2=0;
//	
//	PWMA_CCR4H = (myabs(Backward_moto2)>> 8);  
//	PWMA_CCR4L = (myabs(Backward_moto2));
//	
//	
//}

void Update_PWM(int pwm0,int pwm1,int pwm2,int pwm3,int pwm4)
{
	Update_Pwm0(pwm0);
	Update_Pwm1(pwm1);
	Update_Pwm2(pwm2);
	Update_Pwm3(pwm3);
	
	Update_Pwm4(pwm4);
}
	

