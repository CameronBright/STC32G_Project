#include "pwm.h"
uchar Pwm_count=0;       //pwm计时加加
uchar Pwm_value=100;       //存储PWM的值

void Pwm_Fun()
{
	Pwm_count++;         //PWM调节
	if(Pwm_count>=200){
		Pwm_count=0;
	}
	if(Pwm_value<=Pwm_count){
		PWM_PIN=0;
		PWM_PIN1=0;
	}
	else{
		PWM_PIN=1;
		PWM_PIN1=1;
	}
}