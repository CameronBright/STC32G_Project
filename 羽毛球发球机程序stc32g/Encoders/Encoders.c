#include "Encoders.h"
#include "oled.h"
bit long_short=0,add_lose=0;  //long_short计时 短按进入 add_lose=0加 add_lose=1减
bit arrow_Value=0;         //箭头或值   0是箭头 1是值
bit rising_edge=0;         //上升沿检测
bit falling_Down=0;       //下降沿检测 
uint s_count=0;         //计数加加
uchar part_count=0;      //一级界面区别第几页
uchar part2_count=0;     //二级界面区别第几页
uchar Confirm_exit=0;   //确认和退出
struct encoder spin;
struct value adjust;
/******************************************************************************
      函数说明：//上升沿检测
			入口参数：无
      返回值：  无
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
			if(spin.Turn==1)      add();       //页加		                             
			else if(spin.Turn==0) lose();      //页减	                                 
		}
	}
}
/******************************************************************************
      函数说明：//旋转编码器检测函数 上升沿和下降沿检测函数
			入口参数：无
      返回值：  无
******************************************************************************/
void rotary_encoder()  
{
	if(EC11_A==1&&EC11_B==1)	falling_Down=1;//下降沿检测                     
	if(falling_Down==1)
	{
		if(EC11_A!=EC11_B)
			spin.Turn=EC11_B;
		if(EC11_A==0&&EC11_B==0)
		{
			falling_Down=0;// falling_Down=0 是禁止进入检测
			if(spin.Turn==1)  add();//spin.Turn=1 是右转   页加			                                 
			if(spin.Turn==0) lose();//spin.Turn=0 是左转    页减 
		}
	}
		up_key_detection(); //按键上升沿检测函数
}
/******************************************************************************
      函数说明：arrow_Value==0//页加
								arrow_Value==1数值加
			入口参数：无
      返回值：  无
******************************************************************************/
void add()
{
	if(arrow_Value==0)
	{
		if(spin.menu_mode==0)//一级菜单
		{
			if(spin.Page>=0&&spin.Page<=4) spin.Page=spin.Page+2;   
		}
		else if(spin.menu_mode==1)//二级菜单
		{
			if(spin.Page2>=0&&spin.Page2<2) spin.Page2=spin.Page2+2;
		}
		if(spin.menu_mode!=1&&part_count>=0&&part_count<=6)	part_count++;   //一级界面区别第几页
		if(spin.menu_mode==1&&part2_count>=0&&part2_count<=1)	part2_count++;  //二级界面区别第几页
		OLED_Clear_0_2();//清>函数			
	}
	else
	{
			add_lose=0;	
			Second_menu_value();//二级菜单数值加减函数         
	}
}
/******************************************************************************    
			 函数说明:arrow_Value==0//页减  
								arrow_Value==1数值减
			入口参数：无
      返回值：  无
******************************************************************************/
void lose()    
{ 
	if(arrow_Value==0)
	{ 	
		if(spin.menu_mode==0)//一级菜单
		{
			if(spin.Page<=6&&spin.Page>=2) spin.Page=spin.Page-2; 	
		}
		else if(spin.menu_mode==1)//二级菜单
		{
			if(spin.Page2<4&&spin.Page2>=2) spin.Page2=spin.Page2-2;
		}
	  if(spin.menu_mode!=1&&part_count>0&&part_count<=7) part_count--;
		if(spin.menu_mode==1&&part2_count>=0&&part2_count<=1)	part2_count--;  //二级界面区别第几页		
		OLED_Clear_0_2();		
	}
	else
	{
			add_lose=1;   
			Second_menu_value();					   
	}
}
/******************************************************************************
      函数说明：//二级菜单数值加减函数
			入口参数：无
      返回值：  无
******************************************************************************/
void Second_menu_value()   
{
	if(part_count==0&&part2_count==0&&add_lose==0){adjust.speed_value+=1; }  //Pwm_value+=1;
	else if(part_count==0&&part2_count==0&&add_lose==1) {adjust.speed_value-=1;	}		//Pwm_value-=1;
	else if(part_count==0&part2_count==1&&add_lose==0) adjust.qi_value+=1; 
  else if(part_count==0&&part2_count==1&&add_lose==1) adjust.qi_value-=1;

	else if(part_count==1&&part2_count==0&&add_lose==0)	adjust.angle_value+=1;   //part_count判断一级菜单的位置  part2_count判断二级菜单的位置  add_lose判断加或减
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
      函数说明：//按键取消和确定检测函数  计时方式
			入口参数：无
      返回值：  无
******************************************************************************/
void Cancel_determine()        
{
	extern uint s_count;
	if(long_short==1)
	{
		if(EC11_K==1&&s_count<2000)//确定 200ms
		{  					
			s_count=0; //计数清零
			OLED_Clear();
			long_short=0; 
			if(Confirm_exit<2) Confirm_exit++;else Confirm_exit=1;//二级菜单确认或退出  
			if(Confirm_exit==2) arrow_Value=1;else arrow_Value=0; //arrow_Value=1进入值加减，arrow_Value=0页加减
			spin.menu_mode=1;
		}
		else if(EC11_K==1&&s_count>4000)     //取消  400ms
		{   
			s_count=0;//计数清零			
			OLED_Clear();
			long_short=0;			
			arrow_Value=0;// 0为>
			Confirm_exit=0;//确认或退出				
			spin.menu_mode=0;//返回一级菜单
			spin.Page2=0;	
			part2_count=0;
		}
		else if(EC11_K==1&&s_count>2000)    //区别长短按  
		{
			s_count=0;
		}
	}
}
