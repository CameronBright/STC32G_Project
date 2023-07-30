#include "key.h"

uchar Key_Rvalue(void)
{
	uchar value = 0;
	
	if(P24 == 0) value = 1;
	else if(P44 == 0) value = 2;
	else if(P42 == 0) value = 3;
	else if(P73 == 0) value = 4;
	
	return value;
}

void Key_Init(void)
{
	P24 = 1;
	P44 = 1;
	P42 = 1;
	P73 = 1;
}

