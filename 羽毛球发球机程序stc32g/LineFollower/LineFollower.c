#include "LineFollower.h"
uchar read_value;

uchar ReadLine(void) //往左边偏的时候输出1 右边偏输出-1 踩在线上输出0
{	
	uchar read_value = 0;
	char return_value = 0;
	
	if(det_L == 0)
		read_value |= 0x08; //置1
	else 
		read_value &= ~0x08;

	if(det_CL == 0)
		read_value |= 0x04;
	else 
		read_value &= ~0x04;
	
	if(det_CR == 0)
		read_value |= 0x02;
	else 
		read_value &= ~0x02;
	
	if(det_R == 0)
		read_value |= 0x01;
	else 
		read_value &= ~0x01;
	
	switch(read_value)
	{
		case 0x06:            //正常行驶
			return_value = 0;
			break;
		case 0x04:           //往右边偏一点点
			return_value = -1;
			break;
		case 0x02:           //往左边偏一点点
			return_value = 1;
			break;
//		case 0x08:
//			return_value = -2; //往左偏较大角度
//			break;
//		case 0x01:
//			return_value = 2;//往右偏较大角度
//			break;
//		case 0x00:
//			return_value = -3;//没识别到黑线返回 ‘-3‘
//			break;
//		case 0x0F:
//			return_value = 3;//宝马离开地面返回  ‘3‘
//			break;
	}
	
	return return_value;
}