#include "key.h"

uchar Key_Rvalue(void)
{
	uchar value = 0;
	
	if(P42 == 0) value = 1;
	else if(P41 == 0) value = 2;
	else if(P73 == 0) value = 3;
	else if(P70 == 0) value = 4;
	
	return value;
}

void Key_Init(void)
{
	P42 = 1;
	P41 = 1;
	P73 = 1;
	P70 = 1;
}

