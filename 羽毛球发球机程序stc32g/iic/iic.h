/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.STCMCU.com --------------------------------------------*/
/* --- Web: www.STCMCUDATA.com  ---------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* 如果要在程序中使用此代码,请在程序中注明使用了STC的资料及程序            */
/*---------------------------------------------------------------------*/

#ifndef	__IIC_H
#define	__IIC_H

#include "main.h"
void IIC_Delay(unsigned char i);
void IIC_Start(void);
void IIC_Stop(void);
void IIC_SendAck(bit ackbit);
bit IIC_WaitAck(void);
void IIC_SendByte(unsigned char byt);
unsigned char IIC_RecByte(void);
uchar Hall_element(uchar readaddr);
#endif

