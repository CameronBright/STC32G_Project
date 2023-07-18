#ifndef __Uart_H
#define __Uart_H

#include "main.h"  
void UartInit();		//9600bps@12.000MHz
void Uart_sendbyte(unsigned char byte);  //串口发送
void Uart_String(unsigned char *p);    //串口发送字符串

#endif