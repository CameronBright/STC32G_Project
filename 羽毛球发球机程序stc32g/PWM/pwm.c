#include "pwm.h"


//����������

int dutyL = 0;
int dutyR = 0;

void PWMA_Config(void)
{
	PWMA_PS = 0x0a;											//PWM1P�����л���P6.0 P6.2
	
	PWMA_PSCRH = (uint)(PWM_PSC >> 8);
	PWMA_PSCRL = (uint)(PWM_PSC); 				//119+1=120Ԥ��Ƶ
	
	PWMA_CCER1 = 0x00; 									// д CCMRx ǰ���������� CCERx �ر�ͨ��
	PWMA_CCMR1 = 0x60; 									// ���� CC1 Ϊ PWMA ���ģʽ��PWMģʽ1
	PWMA_CCMR2 = 0x60; 									// ���� CC2 Ϊ PWMA ���ģʽ��PWMģʽ1
	PWMA_CCER1 = 0x11; 									// ʹ�� CC1 CC2ͨ��
	
	PWMA_CCR1H = (uint)(PWM_PERIOD >> 8);// ���ó�ʼռ�ձ�100
	PWMA_CCR1L = (uint)(PWM_PERIOD); 
	
	PWMA_CCR2H = (uint)(PWM_PERIOD >> 8);// ���ó�ʼռ�ձ�100
	PWMA_CCR2L = (uint)(PWM_PERIOD); 
	
	PWMA_ARRH = (uint)(PWM_PERIOD >> 8); // ����PWM����
	PWMA_ARRL = (uint)(PWM_PERIOD); 
	
	PWMA_ENO = 0x05; 										// ʹ�� PWM1P 2p �˿����
	PWMA_BKR = 0x80;										// ʹ�������
	
	PWMA_CR1 = 0x01; 										// ����PWM��ʱ��
}

/*
when sw = 1,duty = duty; 
when sw = 0, motor can't work
*/

void Update_duty(uchar sw,int PWMR,int PWML)										//����ռ�ձȺ���
{
//	if(!sw)
//	{
//		PWML = 0;
//		PWMR = 0;
//	}
	
	if(PWMR < 0)
		PWMR = PWMR * -1;
	
	if(PWML < 0)
		PWML = PWML * -1;
	
	PWMA_CCR1H	= (uchar)(PWML >> 8);  
	PWMA_CCR1L = PWML;                //����ռ�ձ�
		
	PWMA_CCR2H = (uchar)(PWMR >> 8); 
	PWMA_CCR2L = PWMR;	
//	}
}

void Update2_duty(uchar sw)										//����ռ�ձȺ���
{
//	if(!sw)
//	{
//		dutyL = 0;
//		dutyR = 0;
//	}
	
	PWMA_CCR1H	= (uchar)(dutyL >> 8);  
	PWMA_CCR1L = dutyL;                //����ռ�ձ�
		
	PWMA_CCR2H = (uchar)(dutyR >> 8); 
	PWMA_CCR2L = dutyR;	

}
