#include "ADC.h"

uint8_t Switching_channel = 0;

void ADC_Init(void)   
{
	ADCTIM = 0x3f; //设置ADC内部时序
	ADCCFG = 0x2f; //数据右对齐  设置ADC时钟为系统时钟12/2/16
	ADC_CONTR = 0xf0;          //使能第一个通道P1.0		
	ADC_POWER = 1; //使能ADC模块
}

unsigned int ADC_Read(void)  //ADC读
{                           //
	unsigned int value = 0;  //ADC数值保存变量

	ADC_START = 1;         //启动AD转化
	_nop_();              //空指令
	_nop_();	           //空指令
	
	while(!ADC_FLAG);   //等待ADC转换结束
	ADC_FLAG = 0;
	
	value = ((ADC_RES << 8) + ADC_RESL); //计算ADC的数值
	
	Switching_channel++;
	switch(Switching_channel)
	{
		case 1:
			ADC_CONTR = ADC_CONTR + 0x01;          //使能第一个通道P1.1	
		break;
		case 2:
			ADC_CONTR = ADC_CONTR & 0xf0 | 0x08;    //使能第一个通道P0.0	
		break;
		case 3:
			ADC_CONTR = ADC_CONTR & 0xf0;   //重新回到通道P1.0
			Switching_channel = 0;
		break;
	}
	return value;
}


