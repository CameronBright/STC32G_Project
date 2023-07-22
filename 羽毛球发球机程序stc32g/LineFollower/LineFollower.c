#include "LineFollower.h"

int ReadLine(void)
{
	uint adc1,adc2,adc3;
	int inaccuracy;
	
	adc1 = ADC_Readchannel_1();
	adc2 = ADC_Readchannel_2();
	adc3 = ADC_Readchannel_3();
	
	if(adc1 == 4095 && adc2 == 4095 && adc3 == 4095)
		inaccuracy = 0;
	else 
	{
		if(adc1 < 4095 && adc2 < 4095 && adc3 < 4095)
			inaccuracy = 0001;
		else if(adc1 < 4095)
			inaccuracy = (4095 - adc1) * -1;
		else if(adc3 < 4096)
			inaccuracy = (4096 - adc3);
	}
	
	return inaccuracy;
}