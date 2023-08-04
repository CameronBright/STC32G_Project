#ifndef _Uart_H
#define _Uart_H

#include "main.h"  

//串口1
void Uart1_Init(void);		//115200bps@24.000MHz
void Uart1_sendbyte(uchar byte);  //串口发送单个字节
void Uart1_String(uchar *p);    //串口发送字符串

//串口2
void Uart2_Init(void);		//115200bps@24.000MHz
void Uart2_sendbyte(uchar byte);  //串口发送单个字节
void Uart2_String(uchar *p);    //串口发送字符串
void UART2_SendString(uchar *DAT, uchar len);

//串口3
void Uart3_Init(void);		//115200bps@24.000MHz
void Uart3_sendbyte(uchar byte);  //串口发送单个字节
void Uart3_String(uchar *p);    //串口发送字符串


#endif