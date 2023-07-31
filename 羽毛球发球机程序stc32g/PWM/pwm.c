#include "pwm.h"


//变量声明区

int dutyL = 0;
int dutyR = 0;

void PWMA_Config(void)
{
	PWMA_PS = 0x0a;											//PWM1P引脚切换到P6.0 P6.2
	
	PWMA_PSCRH = (uint)(PWM_PSC >> 8);
	PWMA_PSCRL = (uint)(PWM_PSC); 				//119+1=120预分频
	
	PWMA_CCER1 = 0x00; 									// 写 CCMRx 前必须先清零 CCERx 关闭通道
	PWMA_CCMR1 = 0x60; 									// 设置 CC1 为 PWMA 输出模式，PWM模式1
	PWMA_CCMR2 = 0x60; 									// 设置 CC2 为 PWMA 输出模式，PWM模式1
	PWMA_CCER1 = 0x11; 									// 使能 CC1 CC2通道
	
	PWMA_CCR1H = (uint)(PWM_PERIOD >> 8);// 设置初始占空比100
	PWMA_CCR1L = (uint)(PWM_PERIOD); 
	
	PWMA_CCR2H = (uint)(PWM_PERIOD >> 8);// 设置初始占空比100
	PWMA_CCR2L = (uint)(PWM_PERIOD); 
	
	PWMA_ARRH = (uint)(PWM_PERIOD >> 8); // 设置PWM周期
	PWMA_ARRL = (uint)(PWM_PERIOD); 
	
	PWMA_ENO = 0x05; 										// 使能 PWM1P 2p 端口输出
	PWMA_BKR = 0x80;										// 使能主输出
	
	PWMA_CR1 = 0x01; 										// 启动PWM定时器
}

/*
when sw = 1,duty = duty; 
when sw = 0, motor can't work
*/

void Update_duty(uchar sw,int PWMR,int PWML)										//更新占空比函数
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
	PWMA_CCR1L = PWML;                //调整占空比
		
	PWMA_CCR2H = (uchar)(PWMR >> 8); 
	PWMA_CCR2L = PWMR;	
//	}
}

void Update2_duty(uchar sw)										//更新占空比函数
{
//	if(!sw)
//	{
//		dutyL = 0;
//		dutyR = 0;
//	}
	
	PWMA_CCR1H	= (uchar)(dutyL >> 8);  
	PWMA_CCR1L = dutyL;                //调整占空比
		
	PWMA_CCR2H = (uchar)(dutyR >> 8); 
	PWMA_CCR2L = dutyR;	

}
