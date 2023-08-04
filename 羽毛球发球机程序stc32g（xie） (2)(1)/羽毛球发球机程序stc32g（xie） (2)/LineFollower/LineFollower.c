#include "LineFollower.h"
#include "Wave.h"

//循迹线标志位
unsigned char count = 0;  
extern unsigned char Wave_flag;
extern unsigned char T_S_B;
extern unsigned char flag;
extern unsigned char flag_count;
extern unsigned char Tracking_value;

//extern char line_inaccuracy;

extern void Motor_control();


char ReadLine(void) //往左边偏的时候输出1 右边偏输出-1 踩在线上输出0
{	
	uchar read_value = 0;
	char return_value = 0;
	
		if(det_L == 1)
			read_value |= 0x10; //置1
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
			case 0x04:            //正常行驶
				return_value = 0;
				break;
			case 0x08:           //往右边偏一点点
				return_value = 1;
				break;
			case 0x02:           //往左边偏一点点
				return_value = -1;
				break;
			case 0x0C:					 //往右边偏一点点
				return_value = 1;
				break;
			case 0x06:					 //往左边偏一点点
				return_value = -1;
				break;
			case 0x10:
				return_value = 2; //往右偏较大角度
				break;
			case 0x01:
				return_value = -2;//往坐偏较大角度
				break;
			case 0x18:
				return_value = 2; //往右偏较大角度
				break;
			case 0x03:
				return_value = -2;//往坐偏较大角度
				break;	
			case 0x1F:
				return_value = -3;//宝马离开地面返回  ‘-3‘
			break;
			
			default:
				return_value = 3;//没识别到黑线返回 ‘3‘
			break;
		} 
			
	return return_value;
}

////巡线处理函数
//void ReadLine_proc(void)
//{
//	if(T_S_B == 1)
//	{
//		T_S_B_1_ReadLine();
//	}
//	if(T_S_B == 2)
//	{
//		T_S_B_2_ReadLine();
//	}
//}

////基础部分

////功能2
//void T_S_B_1_ReadLine()
//{
//	Motor_control();
//	if(line_inaccuracy == -3 ||line_inaccuracy == -2)   //判断5路都在黑线上
//	{
//		flag = 1;
//	}
//	if(Tracking_value == 4)  //循迹1圈 循迹次数标志位+4  
//	{
//		Tracking_value = 0;   //循迹完1圈 标志位置0
//		flag = 0;             
//		T_S_B = 0;            //状态恢复
//	}  	
//}



////功能3
//void T_S_B_2_ReadLine(void)
//{

//		Motor_control();
//		if(line_inaccuracy == -3 || line_inaccuracy == -2)   //判断5路都在黑线上
//		{
//			flag = 1;
//			if(Tracking_value == 6)  //循迹1圈 标志位+4
//			{
//				Tracking_value = 0;   //循迹完1圈 标志位置0
//				flag = 0;             
//				T_S_B = 0;            //状态恢复
//			}  
//		}	
//		if(Tracking_value == 4)  
//		{
////		  moter_wheel();
//		}
//}

//  //左拐函数  输入左右PWM
//void moter_wheel(int left_moto,int right_moto)//用与循迹a、b点转弯  左右风速
//{
//		bit Turn_left_flage = 0;
//	
//		Turn_left_flage = 1;

//		//重新开启左边电机和右边电机
//		Set_LRPwm(left_moto,right_moto);    //问题一 如何左转不会被循迹控制  左转完成后如何恢复循迹控制
//	
//		
//		
//		if(line_inaccuracy == 2)//左边的红外碰到碰到最近的循迹
//		{
//			Turn_left_flage = 0;
////			Motor_control(）//恢复循迹 直到碰到下个AB
//		}
//		
//		
//		
//}




//////发挥部分
//////功能1

//void obstacle_avoidance()
//{
//	Motor_control();
//	if(Wave_ultrasonic() > 40)
//	{
//		
//	}
//}


//////功能2




