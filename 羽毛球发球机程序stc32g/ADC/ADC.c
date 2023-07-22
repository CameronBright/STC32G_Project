#include "ADC.h"

void ADC_Init(void)   
{
	ADCTIM = 0x3f; //����ADC�ڲ�ʱ��
	ADCCFG = 0x23; //�����Ҷ���  ����ADCʱ��Ϊϵͳʱ��12/2/3
	ADC_CONTR = 0xf0;          //ʹ�ܵ�һ��ͨ��P1.0		
	ADC_POWER = 1; //ʹ��ADCģ��
}

void ADC_Read(uint *channel1, uint *channel2, uint *channel3)  //ADC��
{                           //
	uint value = 0;  //ADC��ֵ�������
	uchar Switching_channel = 0;

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
		{
			ADC_CONTR = ADC_CONTR + 0x01;          //ʹ�ܵ�һ��ͨ��P1.1	
			
			*channel1 = value;
		}
		break;
		case 2:
		{
			ADC_CONTR = ADC_CONTR & 0xf0 | 0x08;    //ʹ�ܵ�һ��ͨ��P0.0
			*channel2 = value;
		}	
		break;
		case 3:
		{
			ADC_CONTR = ADC_CONTR & 0xf0;   //���»ص�ͨ��P1.0
			*channel3 = value;
			Switching_channel = 0;
		}
		break;
	}
}

uint ADC_Readchannel_1(void)
{
	uint value = 0;  //ADC��ֵ�������
	
	ADC_CONTR = ADC_CONTR + 0x01;          //ʹ�ܵ�һ��ͨ��P1.1	
	

	ADC_START = 1;         //����ADת��
	_nop_();              //��ָ��
	_nop_();	           //��ָ��
	
	while(!ADC_FLAG);   //�ȴ�ADCת������
	ADC_FLAG = 0;
	
	value = ((ADC_RES << 8) + ADC_RESL); //����ADC����ֵ
	return value;
}

uint ADC_Readchannel_2(void)
{
	uint value = 0;  //ADC��ֵ�������
	
	ADC_CONTR = ADC_CONTR & 0xf0 | 0x08;    //ʹ�ܵ�һ��ͨ��P0.0

	ADC_START = 1;         //����ADת��
	_nop_();              //��ָ��
	_nop_();	           //��ָ��
	
	while(!ADC_FLAG);   //�ȴ�ADCת������
	ADC_FLAG = 0;
	
	value = ((ADC_RES << 8) + ADC_RESL); //����ADC����ֵ
	return value;
}

uint ADC_Readchannel_3(void)
{
	uint value = 0;  //ADC��ֵ�������
	
	ADC_CONTR = ADC_CONTR & 0xf0;   //���»ص�ͨ��P1.0

	ADC_START = 1;         //����ADת��
	_nop_();              //��ָ��
	_nop_();	           //��ָ��
	
	while(!ADC_FLAG);   //�ȴ�ADCת������
	ADC_FLAG = 0;
	
	value = ((ADC_RES << 8) + ADC_RESL); //����ADC����ֵ
	return value;
}