#ifndef __PWM_H__
#define __PWM_H__
//ͷ�ļ�
#include "stc32g.h"
#include "main.h"
//-----
#define PWM_PSC 			(120-1)	//120��Ƶ
#define PWM_PERIOD 		 1000 //����10����
//----����������-----
/*
1.PWMA��ʼ�����ú���
2.����ռ�ձȺ���
*/
void PWMA_Config(void);				//PWMA��ʼ�����ú���
void Update_duty();						//����ռ�ձȺ���

/******************/

#endif