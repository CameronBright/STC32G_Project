#include "ADC.h"

void ADC_Init(void)   
{
	ADCTIM = 0x3f; //设置ADC内部时序
	ADCCFG = 0x23; //数据右对齐  设置ADC时钟为系统时钟12/2/3
	ADC_CONTR = 0xf0;          //使能第一个通道P1.0		
	ADC_POWER = 1; //使能ADC模块
}

void ADC_Read(uint *channel1, uint *channel2, uint *channel3)  //ADC读
{                           //
	uint value = 0;  //ADC数值保存变量
	uchar Switching_channel = 0;

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
		{
			ADC_CONTR = ADC_CONTR + 0x01;          //使能第一个通道P1.1	
			
			*channel1 = value;
		}
		break;
		case 2:
		{
			ADC_CONTR = ADC_CONTR & 0xf0 | 0x08;    //使能第一个通道P0.0
			*channel2 = value;
		}	
		break;
		case 3:
		{
			ADC_CONTR = ADC_CONTR & 0xf0;   //重新回到通道P1.0
			*channel3 = value;
			Switching_channel = 0;
		}
		break;
	}
}

uint ADC_Readchannel_1(void)
{
	uint value = 0;  //ADC数值保存变量
	
	ADC_CONTR = ADC_CONTR + 0x01;          //使能第一个通道P1.1	
	

	ADC_START = 1;         //启动AD转化
	_nop_();              //空指令
	_nop_();	           //空指令
	
	while(!ADC_FLAG);   //等待ADC转换结束
	ADC_FLAG = 0;
	
	value = ((ADC_RES << 8) + ADC_RESL); //计算ADC的数值
	return value;
}

uint ADC_Readchannel_2(void)
{
	uint value = 0;  //ADC数值保存变量
	
	ADC_CONTR = ADC_CONTR & 0xf0 | 0x08;    //使能第一个通道P0.0

	ADC_START = 1;         //启动AD转化
	_nop_();              //空指令
	_nop_();	           //空指令
	
	while(!ADC_FLAG);   //等待ADC转换结束
	ADC_FLAG = 0;
	
	value = ((ADC_RES << 8) + ADC_RESL); //计算ADC的数值
	return value;
}

uint ADC_Readchannel_3(void)
{
	uint value = 0;  //ADC数值保存变量
	
	ADC_CONTR = ADC_CONTR & 0xf0;   //重新回到通道P1.0

	ADC_START = 1;         //启动AD转化
	_nop_();              //空指令
	_nop_();	           //空指令
	
	while(!ADC_FLAG);   //等待ADC转换结束
	ADC_FLAG = 0;
	
	value = ((ADC_RES << 8) + ADC_RESL); //计算ADC的数值
	return value;
}