#ifndef __OLED_H
#define __OLED_H

#include "main.h"

sbit OLED_SCL=P1^3;    //47
sbit OLED_SDA=P1^7;   //14
sbit OLED_DC=P1^6; //15
sbit OLED_CS=P1^5;   //16
sbit OLED_READ_FS0=P1^4;//17
sbit OLED_ROM_CS=P4^7;//CS2//13


#define OLED_CMD  0   //Ð´ÃüÁî
#define OLED_DATA 1   //Ð´Êý¾Ý

#define OLED_SCL_Set()       OLED_SCL=1;
#define OLED_SCL_Clr()       OLED_SCL=0;

#define OLED_SDA_Set()       OLED_SDA=1;
#define OLED_SDA_Clr()       OLED_SDA=0;

#define OLED_DC_Set()        OLED_DC=1;
#define OLED_DC_Clr()        OLED_DC=0;

#define OLED_CS_Set()        OLED_CS=1;
#define OLED_CS_Clr()        OLED_CS=0;

#define OLED_ROM_CS_Set()    OLED_ROM_CS=1;
#define OLED_ROM_CS_Clr()    OLED_ROM_CS=0;

void OLED_Init(void);
void delay_ms(unsigned int ms);
void OLED_ColorTurn(uchar i);
void OLED_DisplayTurn(uchar i);
void OLED_WR_Byte(uchar dat,uchar cmd);
void OLED_Clear(void);
void OLED_address(uchar x,uchar y);
void OLED_Display_128x64(uchar *dp);
void OLED_Display_16x16(uchar x,uchar y,uchar *dp);
void OLED_Display_8x16(uchar x,uchar y,uchar *dp);
void OLED_Display_5x7(uchar x,uchar y,uchar *dp);
void Send_Command_to_ROM(uchar dat);
uchar Get_data_from_ROM(void);
void OLED_get_data_from_ROM(uchar addrHigh,uchar addrMid,uchar addrLow,uchar *pbuff,uchar DataLen);
void OLED_16x16(uchar x,uchar y,uchar *text);   //16*16
void OLED_Display_string_5x7(uchar x,uchar y,uchar *text);
void OLED_ShowNum(uchar x,uchar y,float num,uchar len);
void OLED_Clear_0_2();//Çåx=16 y=8
#endif