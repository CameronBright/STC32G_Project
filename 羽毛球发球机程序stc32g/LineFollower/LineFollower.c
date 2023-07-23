#include "LineFollower.h"
uchar read_value;

uchar ReadLine(void) //�����ƫ��ʱ�����1 �ұ�ƫ���-1 �����������0
{	
	uchar read_value = 0;
	char return_value = 0;
	
	if(det_R == 1)
		read_value |= 0x04; //��1
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
			return_value = 2; //ûʶ�𵽺���ʱ
			break;
		case 0x07:
			return_value = -2;//error:�뿪����ʱ
			break;
	}
	
	return return_value;
}