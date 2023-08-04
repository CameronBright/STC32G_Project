#include "Meun.h"
#include "oled.h"
#include "Encoders.h" 
#include "Uart.h" 
#define interface 16      //���ӻ���ٽ���
extern struct encoder spin;   //�ṹ������ 
extern struct value adjust;  //�ṹ������
extern uchar part_count;   //����ṹ��part�ĵڼ���
enum more page_table;     //ö�� 
page_table=meun_1_1;      //�ڼ�ҳ


const struct Location_and_Content set[interface]={     //const�ǳ���
	
	{10,16,0,"ת�ٵ���",Meun_2_speed},  //�ص�  ת��  ����
	{11,16,2,"�Ƕȵ���",Meun_2_angle},  //�ص�  �Ƕ�  ����
	{12,16,4,"������",Meun_2_Pitching}, //�ص�  ������ ����
	{13,16,6,"����",Meun_2_control},    //�ص�  ����  ����
	{14,16,0,"����"},  
	{15,16,2,"����"},
	{16,16,4,"����"},
	{17,16,6,"����"}, 			
	
	{20,15,0,"ת��:"},              //�����˵���ʾ����		
	{21,15,2,"����:"},   
	{22,15,0,"������:"},
	{23,15,2,"����:"},
	{24,15,0,"������:"},
	{25,15,2,"����:"},
	{26,15,0,"���Է���:"},
	{27,15,2,"����:"}, 		
};


/******************************************************************************
      ����˵�������˵�
			��ڲ�������
      ����ֵ��  ��
******************************************************************************/


void Memu()//�ܲ˵�
{	
	switch(spin.menu_mode)                 
	{
		case 0:
				Main_Meun();//һ���˵�
		break;
		case 1:
			if(part_count == set[0].part%10)     set[0].Meun();  //�ص�  ת��  ����
			else if(part_count==set[1].part%10)  set[1].Meun();  //�ص�  �Ƕ�  ����
			else if(part_count==set[2].part%10)  set[2].Meun();  //�ص�  ������ ����
			else if(part_count==set[3].part%10)  set[3].Meun();	 //�ص�  ����  ����	
			OLED_16x16(0,spin.Page2,">");		        //��ʾ����	
		break;
	}		
}

/******************************************************************************
      ����˵����һ���˵�
			��ڲ�������
      ����ֵ��  ��
******************************************************************************/


void Main_Meun()                         
{ 
	if(part_count < 4&& page_table == 1)
	{
		page_table=meun_1_1;
		OLED_16x16(set[0].x,set[0].y,set[0].Text);   //����(x,y)����ʾ����
		OLED_16x16(set[1].x,set[1].y,set[1].Text);    
		OLED_16x16(set[2].x,set[2].y,set[2].Text);    
		OLED_16x16(set[3].x,set[3].y,set[3].Text);
	}
	else if(part_count == 4&&page_table == 1)
	{
		OLED_Clear();
		spin.Page=0;
		page_table=meun_1_2;
	}	
	else if(part_count >= 4&& page_table == 2)	
	{
		OLED_16x16(set[4].x,set[4].y,set[4].Text);   
		OLED_16x16(set[5].x,set[5].y,set[5].Text);    
		OLED_16x16(set[6].x,set[6].y,set[6].Text);    
		OLED_16x16(set[7].x,set[7].y,set[7].Text);
	}
	else if(part_count < 5&& page_table == 2)
	{
		OLED_Clear();	
		spin.Page=6;
		page_table=meun_1_1;		
	}
		OLED_16x16(0,spin.Page,">");		        //��ʾ����	
	
		Uart_sendbyte(spin.Page);
}	

/******************************************************************************
      ����˵���������˵�
			��ڲ�������
      ����ֵ��  ��
******************************************************************************/


void Meun_2_speed()   //�����˵�ת��
{
	OLED_16x16(set[8].x,set[8].y,set[8].Text);  //����(x,y)����ʾ�ı�
	OLED_ShowNum(57,0,adjust.speed_value*0.01,4);	//��ʾ��ֵ	
	OLED_16x16(set[9].x,set[9].y,set[9].Text);  //����(x,y)����ʾ�ı�	
	OLED_ShowNum(57,2,adjust.qi_value*0.01,4);	//��ʾ��ֵ
}

void Meun_2_angle()   //�����˵��Ƕ�
{
	OLED_16x16(set[10].x,set[10].y,set[10].Text);
	OLED_ShowNum(75,0,adjust.angle_value*0.01,4);	
	OLED_16x16(set[11].x,set[11].y,set[11].Text);	
	OLED_ShowNum(57,2,adjust.qi_value*0.01,4);	//��ʾ��ֵ
}

void Meun_2_Pitching()   //�����˵�������
{
	OLED_16x16(set[12].x,set[12].y,set[12].Text);
	OLED_ShowNum(75,0,adjust.Pitching_value*0.01,4);	
	OLED_16x16(set[13].x,set[13].y,set[13].Text);	
	OLED_ShowNum(57,2,adjust.qi_value*0.01,4);	//��ʾ��ֵ	
}

void Meun_2_control()   //�����˵�����
{
	OLED_16x16(set[14].x,set[14].y,set[14].Text);
	OLED_ShowNum(88,0,adjust.control_value*0.01,4);	
	OLED_16x16(set[15].x,set[15].y,set[15].Text);   	
	OLED_ShowNum(57,2,adjust.qi_value*0.01,4);	//��ʾ��ֵ	
}