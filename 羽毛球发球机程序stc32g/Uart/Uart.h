#ifndef _Uart_H
#define _Uart_H

#include "main.h"  

//����1
void Uart1_Init(void);		//115200bps@24.000MHz
void Uart1_sendbyte(uchar byte);  //���ڷ��͵����ֽ�
void Uart1_String(uchar *p);    //���ڷ����ַ���

//����2
void Uart2_Init(void);		//115200bps@24.000MHz
void Uart2_sendbyte(uchar byte);  //���ڷ��͵����ֽ�
void Uart2_String(uchar *p);    //���ڷ����ַ���
void UART2_SendString(uchar *DAT, uchar len);

//����3
void Uart3_Init(void);		//115200bps@24.000MHz
void Uart3_sendbyte(uchar byte);  //���ڷ��͵����ֽ�
void Uart3_String(uchar *p);    //���ڷ����ַ���


#endif