#ifndef __PWM_H
#define __PWM_H
#include "main.h"
void Pwm_Fun();
sbit PWM_PIN=P6^3; //PWM������� 
sbit PWM_PIN1=P6^2; //PWM1�������
extern uchar Pwm_count;       //pwm��ʱ�Ӽ�
extern uchar Pwm_value;       //�洢PWM��ֵ
#endif


