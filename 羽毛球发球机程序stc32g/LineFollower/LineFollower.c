#include "LineFollower.h"
uchar read_value;

uchar ReadLine(void) //�����ƫ��ʱ�����1 �ұ�ƫ���-1 �����������0
{	
	uchar read_value = 0;
	char return_value = 0;
	
	if(det_L == 1)
		read_value |= 0x10; //��1
	else 
		read_value &= ~0x10;

	if(det_CL == 1)
		read_value |= 0x08;
	else 
		read_value &= ~0x08;
	
	if(det_C == 1)
		read_value |= 0x04;
	else 
		read_value &= ~0x04;
	
	if(det_CR == 1)
		read_value |= 0x02;
	else 
		read_value &= ~0x02;
	
	if(det_R == 1)
		read_value |= 0x01;
	else 
		read_value &= ~0x01;
	
	switch(read_value)
	{
		case 0x04:            //������ʻ
			return_value = 0;
			break;
		case 0x08:           //���ұ�ƫһ���
			return_value = 1;
			break;
		case 0x02:           //�����ƫһ���
			return_value = -1;
			break;
		case 0x0C:					 //���ұ�ƫһ���
			return_value = 1;
			break;
		case 0x06:					 //�����ƫһ���
			return_value = -1;
			break;
		case 0x10:
			return_value = 2; //����ƫ�ϴ�Ƕ�
			break;
		case 0x01:
			return_value = -2;//����ƫ�ϴ�Ƕ�
			break;
		case 0x18:
			return_value = 2; //����ƫ�ϴ�Ƕ�
			break;
		case 0x03:
			return_value = -2;//����ƫ�ϴ�Ƕ�
			break;	
		case 0x1F:
			return_value = -3;//�����뿪���淵��  ��-3��
			break;
		default:
			return_value = 3;//ûʶ�𵽺��߷��� ��3��
			break;
	} 
	
	return return_value;
}