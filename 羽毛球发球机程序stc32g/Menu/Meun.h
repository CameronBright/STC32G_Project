#ifndef _MEUN_H
#define _MEUN_H

#include "main.h"
#include "oled.h"

void Menu2();      //�����˵���ʾ����
void Memu();       //�ܲ˵�
void Main_Meun();  //һ���˵�
void Meun_2_speed();   //�����˵�  ת��  ����
void Meun_2_angle();   //�����˵�  �Ƕ�  ����
void Meun_2_Pitching();//�����˵�  ������ ����
void Meun_2_control(); //�����˵�  ����  ����
void Meun_2_qita();    //�����˵�

struct Location_and_Content     //���꼰����
{
	uchar part;  
	uchar x;
	uchar y;
	uchar *Text;
	void (*Meun)();
};

enum more   //ö��
{
		meun_1_1=1,
		meun_1_2=2,
		meun_2_1=3,
};

#endif