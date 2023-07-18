#ifndef __ENCODERS_H
#define __ENCODERS_H
#include "oled.h"
#include "pwm.h"
#include "main.h"  
sbit EC11_K=P3^4;  //����ȷ��
sbit EC11_A=P3^2;  //�ж�0
sbit EC11_B=P3^3;  //�ж�1
sbit LED=P6^4;     //LED��
extern uint s_count; //�����Ӽ�
extern bit long_short;//�жϳ������Ƕ̰�  ����Ϊ1
extern uint s_count; //�����Ӽ�
void rotary_encoder();//��ת��������⺯�� 3 1 0 2
void up_key_detection();//������������庯��
void Second_menu_value();//�����˵���ֵ�Ӽ����� 
void Cancel_determine(); //����ȡ����ȷ����⺯��  ��ʱ��ʽ
void add();      //ҳ��
void lose();     //ҳ��
void OLED_Clear_0_2();      //��������

struct encoder{        //��ת�������ṹ��
	uchar Turn;  			   //1Ϊ������ת+ 0Ϊ����ѡת-
	uchar Page;     	   //һ ���˵�ҳ
	uchar menu_mode;     //spin.menu_mode==1������������˵� 
	uchar Page2;         //�� ���˵�ҳ
};            

struct value{            //���ڱ����ṹ��
	uchar speed_value;      //����ת��ֵ
	uchar angle_value;     //�Ƕȵ���ֵ   
	uchar Pitching_value;  //������ֵ
	uchar control_value;   //���Ƶ�ֵ
	uchar qi_value;
};  
#endif