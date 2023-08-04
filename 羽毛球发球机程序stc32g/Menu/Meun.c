#include "Meun.h"
#include "oled.h"
#include "Encoders.h" 
#include "Uart.h" 
#define interface 16      //增加或减少界面
extern struct encoder spin;   //结构体声明 
extern struct value adjust;  //结构体声明
extern uchar part_count;   //区别结构体part的第几个
enum more page_table;     //枚举 
page_table=meun_1_1;      //第几页


const struct Location_and_Content set[interface]={     //const是常量
	
	{10,16,0,"转速调节",Meun_2_speed},  //回调  转速  函数
	{11,16,2,"角度调节",Meun_2_angle},  //回调  角度  函数
	{12,16,4,"俯仰角",Meun_2_Pitching}, //回调  俯仰角 函数
	{13,16,6,"控制",Meun_2_control},    //回调  控制  函数
	{14,16,0,"其他"},  
	{15,16,2,"其他"},
	{16,16,4,"其他"},
	{17,16,6,"其他"}, 			
	
	{20,15,0,"转速:"},              //二级菜单显示内容		
	{21,15,2,"功能:"},   
	{22,15,0,"调左右:"},
	{23,15,2,"功能:"},
	{24,15,0,"俯仰角:"},
	{25,15,2,"功能:"},
	{26,15,0,"测试发球:"},
	{27,15,2,"功能:"}, 		
};


/******************************************************************************
      函数说明：主菜单
			入口参数：无
      返回值：  无
******************************************************************************/


void Memu()//总菜单
{	
	switch(spin.menu_mode)                 
	{
		case 0:
				Main_Meun();//一级菜单
		break;
		case 1:
			if(part_count == set[0].part%10)     set[0].Meun();  //回调  转速  函数
			else if(part_count==set[1].part%10)  set[1].Meun();  //回调  角度  函数
			else if(part_count==set[2].part%10)  set[2].Meun();  //回调  俯仰角 函数
			else if(part_count==set[3].part%10)  set[3].Meun();	 //回调  控制  函数	
			OLED_16x16(0,spin.Page2,">");		        //显示函数	
		break;
	}		
}

/******************************************************************************
      函数说明：一级菜单
			入口参数：无
      返回值：  无
******************************************************************************/


void Main_Meun()                         
{ 
	if(part_count < 4&& page_table == 1)
	{
		page_table=meun_1_1;
		OLED_16x16(set[0].x,set[0].y,set[0].Text);   //坐标(x,y)及显示内容
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
		OLED_16x16(0,spin.Page,">");		        //显示函数	
	
		Uart_sendbyte(spin.Page);
}	

/******************************************************************************
      函数说明：二级菜单
			入口参数：无
      返回值：  无
******************************************************************************/


void Meun_2_speed()   //二级菜单转速
{
	OLED_16x16(set[8].x,set[8].y,set[8].Text);  //坐标(x,y)及显示文本
	OLED_ShowNum(57,0,adjust.speed_value*0.01,4);	//显示数值	
	OLED_16x16(set[9].x,set[9].y,set[9].Text);  //坐标(x,y)及显示文本	
	OLED_ShowNum(57,2,adjust.qi_value*0.01,4);	//显示数值
}

void Meun_2_angle()   //二级菜单角度
{
	OLED_16x16(set[10].x,set[10].y,set[10].Text);
	OLED_ShowNum(75,0,adjust.angle_value*0.01,4);	
	OLED_16x16(set[11].x,set[11].y,set[11].Text);	
	OLED_ShowNum(57,2,adjust.qi_value*0.01,4);	//显示数值
}

void Meun_2_Pitching()   //二级菜单俯仰角
{
	OLED_16x16(set[12].x,set[12].y,set[12].Text);
	OLED_ShowNum(75,0,adjust.Pitching_value*0.01,4);	
	OLED_16x16(set[13].x,set[13].y,set[13].Text);	
	OLED_ShowNum(57,2,adjust.qi_value*0.01,4);	//显示数值	
}

void Meun_2_control()   //二级菜单控制
{
	OLED_16x16(set[14].x,set[14].y,set[14].Text);
	OLED_ShowNum(88,0,adjust.control_value*0.01,4);	
	OLED_16x16(set[15].x,set[15].y,set[15].Text);   	
	OLED_ShowNum(57,2,adjust.qi_value*0.01,4);	//显示数值	
}