#include "syn6288.h"


/**************оƬ��������*********************/
uchar SYN_StopCom[] = {0xFD, 0X00, 0X02, 0X02, 0XFD}; //ֹͣ�ϳ�
uchar SYN_SuspendCom[] = {0XFD, 0X00, 0X02, 0X03, 0XFC}; //��ͣ�ϳ�
uchar SYN_RecoverCom[] = {0XFD, 0X00, 0X02, 0X04, 0XFB}; //�ָ��ϳ�
uchar SYN_ChackCom[] = {0XFD, 0X00, 0X02, 0X21, 0XDE}; //״̬��ѯ
uchar SYN_PowerDownCom[] = {0XFD, 0X00, 0X02, 0X88, 0X77}; //����POWER DOWN ״̬����
uchar SYN_Change[] = {0xFD,0X00,0X03,0X31,0X00,0XCF}; //�޸Ĳ�����9600


//Music:  0:�ޱ�������  1~15:ѡ�񱳾�����
void SYN_FrameInfo(uchar Music, uchar *HZdata)
{
  /****************��Ҫ���͵��ı�**********************************/
  uchar  Frame_Info[50];
  uchar  HZ_Length;
  uchar  ecc  = 0;  			//����У���ֽ�
  uint i = 0;
  HZ_Length = strlen((char*)HZdata); 			//��Ҫ�����ı��ĳ���

  /*****************֡�̶�������Ϣ**************************************/
  Frame_Info[0] = 0xFD ; 			//����֡ͷFD
  Frame_Info[1] = 0x00 ; 			//�������������ȵĸ��ֽ�
  Frame_Info[2] = HZ_Length + 3; 		//�������������ȵĵ��ֽ�
  Frame_Info[3] = 0x01 ; 			//���������֣��ϳɲ�������
  Frame_Info[4] = 0x01 | Music << 4 ; //����������������������趨

  /*******************У�������***************************************/
  for(i = 0; i < 5; i++)   				//���η��͹���õ�5��֡ͷ�ֽ�
  {
    ecc = ecc ^ (Frame_Info[i]);		//�Է��͵��ֽڽ������У��
  }

  for(i = 0; i < HZ_Length; i++)   		//���η��ʹ��ϳɵ��ı�����
  {
    ecc = ecc ^ (HZdata[i]); 				//�Է��͵��ֽڽ������У��
  }
  /*******************����֡��Ϣ***************************************/
	
  memcpy(&Frame_Info[5], HZdata, HZ_Length);   
  Frame_Info[5 + HZ_Length] = ecc;
  UART2_SendString(Frame_Info, 5 + HZ_Length + 1);
	
}


/***********************************************************
* ��    �ƣ� YS_SYN_Set(uchar *Info_data)
* ��    �ܣ� ������	�������
* ��ڲ����� *Info_data:�̶���������Ϣ����
* ���ڲ�����
* ˵    �����������������ã�ֹͣ�ϳɡ���ͣ�ϳɵ����� ��Ĭ�ϲ�����9600bps��
* ���÷�����ͨ�������Ѿ�������������������á�
**********************************************************/
void YS_SYN_Set(uchar *Info_data)
{
  uchar Com_Len;
  Com_Len = strlen((char*)Info_data);
  UART2_SendString(Info_data, Com_Len);
}

//�����ʱ1ms
void  delay(uint uldata)
{
  uint j  =  0;
  uint g  =  0;
  for (j = 0; j < uldata; j++)
    for (g = 0; g < 850; g++);
}

//SYN_FrameInfo(2, "[v5][t5]��ӭʹ�������콢��SYN6288�����ϳ�ģ��");

