#ifndef _MEUN_H
#define _MEUN_H

#include "main.h"
#include "oled.h"

void Menu2();      //二级菜单显示内容
void Memu();       //总菜单
void Main_Meun();  //一级菜单
void Meun_2_speed();   //二级菜单  转速  函数
void Meun_2_angle();   //二级菜单  角度  函数
void Meun_2_Pitching();//二级菜单  俯仰角 函数
void Meun_2_control(); //二级菜单  控制  函数
void Meun_2_qita();    //二级菜单

struct Location_and_Content     //坐标及内容
{
	uchar part;  
	uchar x;
	uchar y;
	uchar *Text;
	void (*Meun)();
};

enum more   //枚举
{
		meun_1_1=1,
		meun_1_2=2,
		meun_2_1=3,
};

#endif