#ifndef _ADC_H
#define _ADC_H

#include "main.h"
#define uint8_t unsigned char 
#define uint16_t unsigned int
	

void ADC_Init(void);
void ADC_Read(uint *channel1, uint *channel2, uint *channel3);  //ADC¶Á

uint ADC_Readchannel_1(void);
uint ADC_Readchannel_2(void);
uint ADC_Readchannel_3(void);	


#endif



