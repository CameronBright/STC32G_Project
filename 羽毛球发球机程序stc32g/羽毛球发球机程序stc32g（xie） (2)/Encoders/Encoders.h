#ifndef __ENCODERS_H
#define __ENCODERS_H
#include "oled.h"
#include "pwm.h"
#include "main.h"  
sbit EC11_K=P3^4;  //按下确定
sbit EC11_A=P3^2;  //中断0
sbit EC11_B=P3^3;  //中断1
sbit LED=P6^4;     //LED灯
extern uint s_count; //计数加加
extern bit long_short;//判断长按还是短按  长按为1
extern uint s_count; //计数加加
void rotary_encoder();//旋转编码器检测函数 3 1 0 2
void up_key_detection();//检测上升沿脉冲函数
void Second_menu_value();//二级菜单数值加减函数 
void Cancel_determine(); //按键取消和确定检测函数  计时方式
void add();      //页加
void lose();     //页减
void OLED_Clear_0_2();      //清屏函数

struct encoder{        //旋转编码器结构体
	uchar Turn;  			   //1为向左旋转+ 0为向右选转-
	uchar Page;     	   //一 级菜单页
	uchar menu_mode;     //spin.menu_mode==1清屏进入二级菜单 
	uchar Page2;         //二 级菜单页
};            

struct value{            //调节变量结构体
	uchar speed_value;      //调节转速值
	uchar angle_value;     //角度调节值   
	uchar Pitching_value;  //俯仰角值
	uchar control_value;   //控制的值
	uchar qi_value;
};  
#endif