#include "Encoders.h"
#include "oled.h"
bit long_short=0,add_lose=0;  //long_short��ʱ �̰����� add_lose=0�� add_lose=1��
bit arrow_Value=0;         //��ͷ��ֵ   0�Ǽ�ͷ 1��ֵ
bit rising_edge=0;         //�����ؼ��
bit falling_Down=0;       //�½��ؼ�� 
uint s_count=0;         //�����Ӽ�
uchar part_count=0;      //һ����������ڼ�ҳ
uchar part2_count=0;     //������������ڼ�ҳ
uchar Confirm_exit=0;   //ȷ�Ϻ��˳�
struct encoder spin;
struct value adjust;
/******************************************************************************
      ����˵����//�����ؼ��
			��ڲ�������
      ����ֵ��  ��
******************************************************************************/
void up_key_detection() 
{
	if(EC11_A==0&&EC11_B==0)  rising_edge=1;                                   	
	if(rising_edge==1)
	{                                  
		if(EC11_A!=EC11_B) 
			spin.Turn=EC11_A;
		if(EC11_A==1&&EC11_B==1)
		{																	
			rising_edge=0;
			if(spin.Turn==1)      add();       //ҳ��		                             
			else if(spin.Turn==0) lose();      //ҳ��	                                 
		}
	}
}
/******************************************************************************
      ����˵����//��ת��������⺯�� �����غ��½��ؼ�⺯��
			��ڲ�������
      ����ֵ��  ��
******************************************************************************/
void rotary_encoder()  
{
	if(EC11_A==1&&EC11_B==1)	falling_Down=1;//�½��ؼ��                     
	if(falling_Down==1)
	{
		if(EC11_A!=EC11_B)
			spin.Turn=EC11_B;
		if(EC11_A==0&&EC11_B==0)
		{
			falling_Down=0;// falling_Down=0 �ǽ�ֹ������
			if(spin.Turn==1)  add();//spin.Turn=1 ����ת   ҳ��			                                 
			if(spin.Turn==0) lose();//spin.Turn=0 ����ת    ҳ�� 
		}
	}
		up_key_detection(); //���������ؼ�⺯��
}
/******************************************************************************
      ����˵����arrow_Value==0//ҳ��
								arrow_Value==1��ֵ��
			��ڲ�������
      ����ֵ��  ��
******************************************************************************/
void add()
{
	if(arrow_Value==0)
	{
		if(spin.menu_mode==0)//һ���˵�
		{
			if(spin.Page>=0&&spin.Page<=4) spin.Page=spin.Page+2;   
		}
		else if(spin.menu_mode==1)//�����˵�
		{
			if(spin.Page2>=0&&spin.Page2<2) spin.Page2=spin.Page2+2;
		}
		if(spin.menu_mode!=1&&part_count>=0&&part_count<=6)	part_count++;   //һ����������ڼ�ҳ
		if(spin.menu_mode==1&&part2_count>=0&&part2_count<=1)	part2_count++;  //������������ڼ�ҳ
		OLED_Clear_0_2();//��>����			
	}
	else
	{
			add_lose=0;	
			Second_menu_value();//�����˵���ֵ�Ӽ�����         
	}
}
/******************************************************************************    
			 ����˵��:arrow_Value==0//ҳ��  
								arrow_Value==1��ֵ��
			��ڲ�������
      ����ֵ��  ��
******************************************************************************/
void lose()    
{ 
	if(arrow_Value==0)
	{ 	
		if(spin.menu_mode==0)//һ���˵�
		{
			if(spin.Page<=6&&spin.Page>=2) spin.Page=spin.Page-2; 	
		}
		else if(spin.menu_mode==1)//�����˵�
		{
			if(spin.Page2<4&&spin.Page2>=2) spin.Page2=spin.Page2-2;
		}
	  if(spin.menu_mode!=1&&part_count>0&&part_count<=7) part_count--;
		if(spin.menu_mode==1&&part2_count>=0&&part2_count<=1)	part2_count--;  //������������ڼ�ҳ		
		OLED_Clear_0_2();		
	}
	else
	{
			add_lose=1;   
			Second_menu_value();					   
	}
}
/******************************************************************************
      ����˵����//�����˵���ֵ�Ӽ�����
			��ڲ�������
      ����ֵ��  ��
******************************************************************************/
void Second_menu_value()   
{
	if(part_count==0&&part2_count==0&&add_lose==0){adjust.speed_value+=1; }  //Pwm_value+=1;
	else if(part_count==0&&part2_count==0&&add_lose==1) {adjust.speed_value-=1;	}		//Pwm_value-=1;
	else if(part_count==0&part2_count==1&&add_lose==0) adjust.qi_value+=1; 
  else if(part_count==0&&part2_count==1&&add_lose==1) adjust.qi_value-=1;

	else if(part_count==1&&part2_count==0&&add_lose==0)	adjust.angle_value+=1;   //part_count�ж�һ���˵���λ��  part2_count�ж϶����˵���λ��  add_lose�жϼӻ��
	else if(part_count==1&&part2_count==0&&add_lose==1) adjust.angle_value-=1;
	else if(part_count==1&&part2_count==1&&add_lose==0) adjust.qi_value+=1; 
	else if(part_count==1&&part2_count==1&&add_lose==1) adjust.qi_value-=1; 
	
	else if(part_count==2&&part2_count==0&&add_lose==0)	adjust.Pitching_value+=1;
	else if(part_count==2&&part2_count==0&&add_lose==1) adjust.Pitching_value-=1;
	else if(part_count==2&&part2_count==1&&add_lose==0) adjust.qi_value+=1; 
	else if(part_count==2&&part2_count==1&&add_lose==1) adjust.qi_value-=1; 

	else if(part_count==3&&part2_count==0&&add_lose==0)	adjust.control_value+=1;
	else if(part_count==3&&part2_count==0&&add_lose==1) adjust.control_value-=1;
	else if(part_count==3&&part2_count==1&&add_lose==0) adjust.qi_value+=1; 
	else if(part_count==3&&part2_count==1&&add_lose==1) adjust.qi_value-=1;	
}

/******************************************************************************
      ����˵����//����ȡ����ȷ����⺯��  ��ʱ��ʽ
			��ڲ�������
      ����ֵ��  ��
******************************************************************************/
void Cancel_determine()        
{
	extern uint s_count;
	if(long_short==1)
	{
		if(EC11_K==1&&s_count<2000)//ȷ�� 200ms
		{  					
			s_count=0; //��������
			OLED_Clear();
			long_short=0; 
			if(Confirm_exit<2) Confirm_exit++;else Confirm_exit=1;//�����˵�ȷ�ϻ��˳�  
			if(Confirm_exit==2) arrow_Value=1;else arrow_Value=0; //arrow_Value=1����ֵ�Ӽ���arrow_Value=0ҳ�Ӽ�
			spin.menu_mode=1;
		}
		else if(EC11_K==1&&s_count>4000)     //ȡ��  400ms
		{   
			s_count=0;//��������			
			OLED_Clear();
			long_short=0;			
			arrow_Value=0;// 0Ϊ>
			Confirm_exit=0;//ȷ�ϻ��˳�				
			spin.menu_mode=0;//����һ���˵�
			spin.Page2=0;	
			part2_count=0;
		}
		else if(EC11_K==1&&s_count>2000)    //���𳤶̰�  
		{
			s_count=0;
		}
	}
}
