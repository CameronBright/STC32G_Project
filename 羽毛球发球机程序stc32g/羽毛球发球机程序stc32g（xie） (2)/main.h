#ifndef __MAIN_H
#define __MAIN_H

#include "STC32G.H"
#define uchar unsigned char    
#define uint unsigned int
#define uF float
#define ulong unsigned long
typedef unsigned char   u8;     //  8 bits



//全局变量
//陀螺仪
extern float Angle_gx,Angle_gy,Angle_gz;		//由角速度计算的角速率(角度制)
extern float Angle_ax,Angle_ay,Angle_az;		//由加速度计算的加速度(弧度制)

extern int X_Result,Y_Result,Z_Result;						//加入遥控器控制量后的角度 
	
#endif
