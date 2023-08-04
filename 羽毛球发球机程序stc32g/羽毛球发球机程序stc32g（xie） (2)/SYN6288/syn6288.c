#include "syn6288.h"


/**************芯片设置命令*********************/
uchar SYN_StopCom[] = {0xFD, 0X00, 0X02, 0X02, 0XFD}; //停止合成
uchar SYN_SuspendCom[] = {0XFD, 0X00, 0X02, 0X03, 0XFC}; //暂停合成
uchar SYN_RecoverCom[] = {0XFD, 0X00, 0X02, 0X04, 0XFB}; //恢复合成
uchar SYN_ChackCom[] = {0XFD, 0X00, 0X02, 0X21, 0XDE}; //状态查询
uchar SYN_PowerDownCom[] = {0XFD, 0X00, 0X02, 0X88, 0X77}; //进入POWER DOWN 状态命令
uchar SYN_Change[] = {0xFD,0X00,0X03,0X31,0X00,0XCF}; //修改波特率9600


//Music:  0:无背景音乐  1~15:选择背景音乐
void SYN_FrameInfo(uchar Music, uchar *HZdata)
{
  /****************需要发送的文本**********************************/
  uchar  Frame_Info[50];
  uchar  HZ_Length;
  uchar  ecc  = 0;  			//定义校验字节
  uint i = 0;
  HZ_Length = strlen((char*)HZdata); 			//需要发送文本的长度

  /*****************帧固定配置信息**************************************/
  Frame_Info[0] = 0xFD ; 			//构造帧头FD
  Frame_Info[1] = 0x00 ; 			//构造数据区长度的高字节
  Frame_Info[2] = HZ_Length + 3; 		//构造数据区长度的低字节
  Frame_Info[3] = 0x01 ; 			//构造命令字：合成播放命令
  Frame_Info[4] = 0x01 | Music << 4 ; //构造命令参数：背景音乐设定

  /*******************校验码计算***************************************/
  for(i = 0; i < 5; i++)   				//依次发送构造好的5个帧头字节
  {
    ecc = ecc ^ (Frame_Info[i]);		//对发送的字节进行异或校验
  }

  for(i = 0; i < HZ_Length; i++)   		//依次发送待合成的文本数据
  {
    ecc = ecc ^ (HZdata[i]); 				//对发送的字节进行异或校验
  }
  /*******************发送帧信息***************************************/
	
  memcpy(&Frame_Info[5], HZdata, HZ_Length);   
  Frame_Info[5 + HZ_Length] = ecc;
  UART2_SendString(Frame_Info, 5 + HZ_Length + 1);
	
}


/***********************************************************
* 名    称： YS_SYN_Set(uchar *Info_data)
* 功    能： 主函数	程序入口
* 入口参数： *Info_data:固定的配置信息变量
* 出口参数：
* 说    明：本函数用于配置，停止合成、暂停合成等设置 ，默认波特率9600bps。
* 调用方法：通过调用已经定义的相关数组进行配置。
**********************************************************/
void YS_SYN_Set(uchar *Info_data)
{
  uchar Com_Len;
  Com_Len = strlen((char*)Info_data);
  UART2_SendString(Info_data, Com_Len);
}

//软件延时1ms
void  delay(uint uldata)
{
  uint j  =  0;
  uint g  =  0;
  for (j = 0; j < uldata; j++)
    for (g = 0; g < 850; g++);
}

//SYN_FrameInfo(2, "[v5][t5]欢迎使用绿深旗舰店SYN6288语音合成模块");

