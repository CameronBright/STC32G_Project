#include "pwm_init.h"



int dutyL = 0;
int dutyR = 0;

extern int Left_moto1     = 0;//左风机PWM
extern int Right_moto2    = 0;//右
extern int Forwar_dmoto1  = 0;//前
extern int Backward_moto2 = 0;//后


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
	
	
	PWMA_CCR1H = (uint)(1000 >> 8); // 设置CC1占空比时间 占空比2.5%
	PWMA_CCR1L = (uint)(1000); 
	
	PWMA_CCR2H = (uint)(1000 >> 8); // 设置CC2占空比时间 占空比2.5%
	PWMA_CCR2L = (uint)(1000); 
	
	PWMA_CCR3H = (uint)(1000 >> 8); // 设置CC3占空比时间 占空比2.5%
	PWMA_CCR3L = (uint)(1000); 
	
	PWMA_CCR4H = (uint)(1000 >> 8); // 设置CC4占空比时间 占空比2.5%
	PWMA_CCR4L = (uint)(1000); 
	

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
	
	
	PWMB_CCR5H = (uint)(500 >> 8); // 设置CC1占空比时间 占空比2.5%
	PWMB_CCR5L = (uint)(500); 
	
	PWMB_CCR6H = (uint)(500 >> 8); // 设置CC2占空比时间 占空比2.5%
	PWMB_CCR6L = (uint)(500); 

	PWMB_CCR7H = (uint)(500 >> 8); // 设置CC3占空比时间 占空比2.5%
	PWMB_CCR7L = (uint)(500); 
	
	PWMB_CCR8H = (uint)(500 >> 8); // 设置CC4占空比时间 占空比2.5%
	PWMB_CCR8L = (uint)(500); 
	

	PWMB_ARRH = (uint)(PWM_PERIOD >> 8); // 设置PWMA周期20毫秒
	PWMB_ARRL = (uint)(PWM_PERIOD); 

	PWMB_ENO = 0x55; 										// 使能 PWM1P 2p 端口输出
	PWMB_BKR = 0x80;										// 使能主输出

	PWMB_CR1 = 0x01; 										// 启动PWM定时器		
}


/**************************************************************************
函数功能：绝对值函数
入口参数：int
返回  值：unsigned int
目    的：对PWM进行绝对值处理
**************************************************************************/
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}

/**************************************************************************
函数功能：限制PWM赋值 
入口参数：无
返回  值：无
**************************************************************************/
void Xianfu_Pwm(void)
{
	 //===PWM满幅是100 限制在95
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
函数功能：检测异常关闭电机
入口参数：检测异常参数
返回  值：无
**************************************************************************/
//void Turn_Off(float angle)
//{
//		if(angle<-40||angle>40)	 //电池电压低于11.1V关闭电机
//		{	                                   //===倾角大于40度关闭电机																			 
//				moto1=0;
//				moto2=0;
//		}		
//}

/**************************************************************************
函数功能：赋值给PWM寄存器
入口参数：左轮PWM、右轮PWM
返回  值：无
**************************************************************************/

void Set_LRPwm(int Left_moto1,int Right_moto2)    //更新左右风机占空比
{
	
	
	if(Left_moto1<0)		
	AIN2=1,			AIN1=0;  //如果小于零就是反转  先变换电平  motol
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


void Set_FBPwm(int Forwar_dmoto1,int Backward_moto2)    //更新前后风机占空比
{
	
	
	if(Forwar_dmoto1<0)		
	CIN2=1,			CIN1=0;  //如果小于零就是反转  先变换电平  motol
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

void Update_duty(uchar sw,int PWMR,int PWML)										//更新占空比函数
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
	PWMA_CCR1L = PWML;                //调整占空比
		
	PWMA_CCR2H = (uchar)(PWMR >> 8); 
	PWMA_CCR2L = PWMR;	
}

