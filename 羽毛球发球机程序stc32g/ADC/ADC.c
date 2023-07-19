#include "ADC.h"

uint8_t Switching_channel = 0;

void ADC_Init(void)   
{
	ADCTIM = 0x3f; //����ADC�ڲ�ʱ��
	ADCCFG = 0x2f; //�����Ҷ���  ����ADCʱ��Ϊϵͳʱ��12/2/16
	ADC_CONTR = 0xf0;          //ʹ�ܵ�һ��ͨ��P1.0		
	ADC_POWER = 1; //ʹ��ADCģ��
}

unsigned int ADC_Read(void)  //ADC��
{                           //
	unsigned int value = 0;  //ADC��ֵ�������

	ADC_START = 1;         //����ADת��
	_nop_();              //��ָ��
	_nop_();	           //��ָ��
	
	while(!ADC_FLAG);   //�ȴ�ADCת������
	ADC_FLAG = 0;
	
	value = ((ADC_RES << 8) + ADC_RESL); //����ADC����ֵ
	
	Switching_channel++;
	switch(Switching_channel)
	{
		case 1:
			ADC_CONTR = ADC_CONTR + 0x01;          //ʹ�ܵ�һ��ͨ��P1.1	
		break;
		case 2:
			ADC_CONTR = ADC_CONTR & 0xf0 | 0x08;    //ʹ�ܵ�һ��ͨ��P0.0	
		break;
		case 3:
			ADC_CONTR = ADC_CONTR & 0xf0;   //���»ص�ͨ��P1.0
			Switching_channel = 0;
		break;
	}
	return value;
}


