#include "pwm.h"
uchar Pwm_count=0;       //pwm��ʱ�Ӽ�
uchar Pwm_value=100;       //�洢PWM��ֵ

void Pwm_Fun()
{
	Pwm_count++;         //PWM����
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