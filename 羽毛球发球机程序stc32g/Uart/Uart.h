#ifndef __Uart_H
#define __Uart_H

#include "main.h"  
void UartInit();		//9600bps@12.000MHz
void Uart_sendbyte(unsigned char byte);  //���ڷ���
void Uart_String(unsigned char *p);    //���ڷ����ַ���

#endif