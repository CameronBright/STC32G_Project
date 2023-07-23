#include "LineFollower.h"
uchar read_value;

uchar ReadLine(void) //往左边偏的时候输出1 右边偏输出-1 踩在线上输出0
{	
	uchar read_value = 0;
	char return_value = 0;
	
	if(det_R == 1)
		read_value |= 0x04; //置1
	else 
		read_value &= ~0x04;

	if(det_C == 1)
		read_value |= 0x02;
	else 
		read_value &= ~0x02;
	
	if(det_L == 1)
		read_value |= 0x01;
	else 
		read_value &= ~0x01;
	
	switch(read_value)
	{
		case 0x02:
			return_value = 0;
			break;
		case 0x01:
			return_value = -1;
			break;
		case 0x04:
			return_value = 1;
			break;
		case 0x00:
			return_value = 2; //没识别到黑线时
			break;
		case 0x07:
			return_value = -2;//error:离开地面时
			break;
	}
	
	return return_value;
}