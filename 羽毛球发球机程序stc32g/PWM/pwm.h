#ifndef __PWM_H
#define __PWM_H
#include "main.h"
void Pwm_Fun();
sbit PWM_PIN=P6^3; //PWM输出引脚 
sbit PWM_PIN1=P6^2; //PWM1输出引脚
extern uchar Pwm_count;       //pwm计时加加
extern uchar Pwm_value;       //存储PWM的值
#endif


