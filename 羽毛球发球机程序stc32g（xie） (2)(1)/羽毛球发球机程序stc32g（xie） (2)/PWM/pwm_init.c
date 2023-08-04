#include "pwm_init.h"

int dutyL = 0;
int dutyR = 0;

extern int Left_moto1     = 0;//左风机PWM
extern int Right_moto2    = 0;//右
extern int Forwar_dmoto1  = 0;//前
extern int Backward_moto2 = 0;//后

unsigned int PWMB_CCR00 = 0;  //P00
unsigned int PWMB_CCR01 = 0;  //P01
unsigned int PWMB_CCR02 = 0;  //P0
unsigned int PWMB_CCR03 = 0;  //P03

unsigned int PWMB_CCA = 0;

void PWM_Config(void)
{
	PWMA_PS = 0xAA;							//PWMA:P60 P62 P64 P66
	
	PWMA_PSCRH = (uint)(PWM_PSC >> 8);
	PWMA_PSCRL = (uint)(PWM_PSC); 				//23+1=24预分频
	
	
	PWMA_CCER1 = 0x00; // 写 CCMRx 前必须先清零 CCERx 关闭通道
	PWMA_CCER2 = 0x00; 
	PWMA_CCMR1 = 0x70; // 设置 CC1 为 PWM模式2
	PWMA_CCMR2 = 0x70; // 设置 CC2 为 PWM模式2
	PWMA_CCMR3 = 0x70; // 设置 CC3 为 PWM模式2
	PWMA_CCMR4 = 0x70; // 设置 CC4 为 PWM模式2
	PWMA_CCER1 = 0x33; // 使能 CC1 CC2通道
	PWMA_CCER2 = 0x33; // 使能 CC3 CC4通道
	
	
	PWMA_CCR1H = (uint)(PWMB_CCA >> 8); // 设置CC1占空比时间 占空比2.5%
	PWMA_CCR1L = (uint)(PWMB_CCA); 
	
	PWMA_CCR2H = (uint)(PWMB_CCA >> 8); // 设置CC2占空比时间 占空比2.5%
	PWMA_CCR2L = (uint)(PWMB_CCA); 
	
	PWMA_CCR3H = (uint)(PWMB_CCA >> 8); // 设置CC3占空比时间 占空比2.5%
	PWMA_CCR3L = (uint)(PWMB_CCA); 
	
	PWMA_CCR4H = (uint)(PWMB_CCA >> 8); // 设置CC4占空比时间 占空比2.5%
	PWMA_CCR4L = (uint)(PWMB_CCA); 
	

	PWMA_ARRH = (uint)(PWM_PERIOD >> 8); // 设置PWMA周期20毫秒
	PWMA_ARRL = (uint)(PWM_PERIOD); 

	PWMA_ENO = 0x55; 										// 使能 PWM1P 2p 端口输出
	PWMA_BKR = 0x80;										// 使能主输出

	PWMA_CR1 = 0x01; 										// 启动PWM定时器	
	
	
	//PWMB
	PWMB_PS = 0xAA;							      //PWMB:P00 P01 P02 P03
	
	PWMB_PSCRH = (uint)(PWM_PSC >> 8);
	PWMB_PSCRL = (uint)(PWM_PSC); 				//23+1=24预分频
	
	
	PWMB_CCER1 = 0x00; // 写 CCMRx 前必须先清零 CCERx 关闭通道
	PWMB_CCER2 = 0x00; 
	
	PWMB_CCMR1 = 0x70; // 设置 CC1 为 PWM模式2
	PWMB_CCMR2 = 0x70; // 设置 CC2 为 PWM模式2
	PWMB_CCMR3 = 0x70; // 设置 CC3 为 PWM模式2
	PWMB_CCMR4 = 0x70; // 设置 CC4 为 PWM模式2
	PWMB_CCER1 = 0x33; // 使能 CC1 CC2通道
	PWMB_CCER2 = 0x33; // 使能 CC3 CC4通道
	
	
	PWMB_CCR5H = (uint)(PWMB_CCR00 >> 8); // 设置CC5占空比时间 占空比2.5%
	PWMB_CCR5L = (uint)(PWMB_CCR00); 
	
	PWMB_CCR6H = (uint)(PWMB_CCR01 >> 8); // 设置CC6占空比时间 占空比2.5%
	PWMB_CCR6L = (uint)(PWMB_CCR01); 

	PWMB_CCR7H = (uint)(PWMB_CCR02 >> 8); // 设置CC7占空比时间 占空比2.5%
	PWMB_CCR7L = (uint)(PWMB_CCR02); 
	
	PWMB_CCR8H = (uint)(PWMB_CCR03 >> 8); // 设置CC8占空比时间 占空比2.5%
	PWMB_CCR8L = (uint)(PWMB_CCR03); 
	

	PWMB_ARRH = (uint)(PWM_PERIOD >> 8); // 设置PWMA周期20毫秒
	PWMB_ARRL = (uint)(PWM_PERIOD); 

	PWMB_ENO = 0x55; 										// 使能 PWM1P 2p 端口输出
	PWMB_BKR = 0x80;										// 使能主输出

	PWMB_CR1 = 0x01; 										// 启动PWM定时器		
}

/**************************************************************************
函数功能：限制PWM赋值 
入口参数：无
返回  值：无
**************************************************************************/
//void Xianfu_Pwm(void)
//{
//	 //===PWM满幅是100 限制在95
//    if(Left_moto1<-950 ) Left_moto1=-950 ;
//		if(Left_moto1>950 )  Left_moto1=950 ;
//	  if(Right_moto2<-950 ) Right_moto2=-950 ;
//		if(Right_moto2>950 )  Right_moto2=950 ;
//	  if(Forwar_dmoto1<-950 ) Forwar_dmoto1=-950 ;
//		if(Forwar_dmoto1>950 )  Forwar_dmoto1=950 ;
//	  if(Backward_moto2<-950 ) Backward_moto2=-950 ;
//		if(Backward_moto2>950 )  Backward_moto2=950 ;
//}

//更新PWM 0
void Update_Pwm0(unsigned int Duty0)
{
	PWMB_CCR5H = (uint)(Duty0 >> 8); // 设置CC5占空比时间 占空比2.5%
	PWMB_CCR5L = (uint)(Duty0); 
}
//更新PWM 1
void Update_Pwm1(unsigned int Duty1)
{
	PWMB_CCR6H = (uint)(Duty1 >> 8); // 设置CC6占空比时间 占空比2.5%
	PWMB_CCR6L = (uint)(Duty1); 	
}
//更新PWM 2
void Update_Pwm2(unsigned int Duty2)
{
	PWMB_CCR7H = (uint)(Duty2 >> 8); // 设置CC7占空比时间 占空比2.5%
	PWMB_CCR7L = (uint)(Duty2); 	
}
//更新PWM 3
void Update_Pwm3(unsigned int Duty3)
{
	PWMB_CCR8H = (uint)(Duty3 >> 8); // 设置CC8占空比时间 占空比2.5%
	PWMB_CCR8L = (uint)(Duty3); 	
}
//更新PWM 4 (往下吹风的电机)
void Update_Pwm4(unsigned int Duty4)
{
	PWMA_CCR3H = (uint)(Duty4 >> 8); // 设置CC3占空比时间 占空比2.5%
	PWMA_CCR3L = (uint)(Duty4); 
}

//void Set_FBPwm(int Forwar_dmoto1,int Backward_moto2)    //更新前后风机占空比
//{
//	
//	
//	if(Forwar_dmoto1<0)		
//	CIN2=1,			CIN1=0;  //如果小于零就是反转  先变换电平  motol
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
	

