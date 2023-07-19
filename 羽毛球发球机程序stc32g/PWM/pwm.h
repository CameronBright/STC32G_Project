#ifndef __PWM_H__
#define __PWM_H__
//头文件
#include "stc32g.h"
#include "main.h"
//-----
#define PWM_PSC 			(120-1)	//120分频
#define PWM_PERIOD 		 1000 //周期10毫秒
//----函数声明区-----
/*
1.PWMA初始化设置函数
2.更新占空比函数
*/
void PWMA_Config(void);				//PWMA初始化设置函数
void Update_duty();						//更新占空比函数

/******************/

#endif