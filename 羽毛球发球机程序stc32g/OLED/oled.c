#include "oled.h"

void delay_ms(unsigned int ms)
{                         
	unsigned int a;
	while(ms)
	{
		a=1800;
		while(a--);
		ms--;
	}
	return;
}

//���Ժ���
void OLED_ColorTurn(uchar i)
{
	if(i==0)
	{
		OLED_WR_Byte(0xA6,OLED_CMD);//������ʾ
	}
	if(i==1)
	{
		OLED_WR_Byte(0xA7,OLED_CMD);//��ɫ��ʾ
	}
}

//��Ļ��ת180��
void OLED_DisplayTurn(uchar i)
{
	if(i==0)
	{
		OLED_WR_Byte(0xC8,OLED_CMD);//������ʾ
		OLED_WR_Byte(0xA1,OLED_CMD);
	}
	if(i==1)
	{
		OLED_WR_Byte(0xC0,OLED_CMD);//��ת��ʾ
		OLED_WR_Byte(0xA0,OLED_CMD);
	}
}

//����OLED��ʾ 
void OLED_DisPlay_On(void)
{
	OLED_WR_Byte(0x8D,OLED_CMD);//��ɱ�ʹ��
	OLED_WR_Byte(0x14,OLED_CMD);//������ɱ�
	OLED_WR_Byte(0xAF,OLED_CMD);//������Ļ
}

//�ر�OLED��ʾ 
void OLED_DisPlay_Off(void)
{
	OLED_WR_Byte(0x8D,OLED_CMD);//��ɱ�ʹ��
	OLED_WR_Byte(0x10,OLED_CMD);//�رյ�ɱ�
	OLED_WR_Byte(0xAE,OLED_CMD);//�ر���Ļ
}


//��SSD1306д��һ���ֽڡ�
//mode:����/�����־ 0,��ʾ����;1,��ʾ����;
void OLED_WR_Byte(uchar dat,uchar cmd)
{	
	uchar i;			  
	if(cmd)
	{
	  OLED_DC_Set();
	}
	else 
	{
	  OLED_DC_Clr();	
	}	  
	OLED_CS_Clr();
	for(i=0;i<8;i++)
	{			  
		OLED_SCL_Clr();
		if(dat&0x80)
		{
		   OLED_SDA_Set();
		}
		else 
		{
		   OLED_SDA_Clr();
		}
		OLED_SCL_Set();
		dat<<=1;   
	}				 		  
	OLED_CS_Set();
	OLED_DC_Set();   	  
}

//��������
void OLED_Clear()
{
	uchar i,n;
	for(i=0;i<8;i++)
	{
	 	 OLED_WR_Byte(0xb0+i,OLED_CMD);//����ҳ��ַ
	   OLED_WR_Byte(0x10,OLED_CMD);  //�����е�ַ�ĸ�4λ
	   OLED_WR_Byte(0x02,OLED_CMD);  //�����е�ַ�ĵ�4λ
	   for(n=0;n<128;n++)
		 {
			 OLED_WR_Byte(0x00,OLED_DATA);//�����������
		 }
  }
}
//�������ܣ� �����ͷ
void OLED_Clear_0_2()      //��������(ֻҪ�������ҳ���ù�)
{
	uchar i=0,n=0;
	for(i=0;i<8;i++){
	 	 OLED_WR_Byte(0xb0+i,OLED_CMD);//����ҳ��ַ
	   OLED_WR_Byte(0x10,OLED_CMD);  //�����е�ַ�ĸ�4λ
	   OLED_WR_Byte(0x02,OLED_CMD);  //�����е�ַ�ĵ�4λ
	   for(n=0;n<16;n++){
			 OLED_WR_Byte(0x00,OLED_DATA);//�����������
		 }
  }
}
//������ʼ��ַ
void OLED_address(uchar x,uchar y)
{
	x+=2;
	OLED_WR_Byte(0xb0+y,OLED_CMD);              //����ҳ��ַ
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);  //�����е�ַ�ĸ�4λ
	OLED_WR_Byte((x&0x0f),OLED_CMD);            //�����е�ַ�ĵ�4λ
}

//��ʾ128x64����ͼ��
void OLED_Display_128x64(uchar *dp)
{
	uchar i=0,j=0;
	for(i=0;i<8;i++)
	{
		OLED_address(0,i);
		for(j=0;j<128;j++)
		{
			OLED_WR_Byte(*dp,OLED_DATA); //д���ݵ�OLED,ÿд��һ��8λ�����ݺ��е�ַ�Զ���1
			dp++;
    }
  }
}

//��ʾ16x16����ͼ�񡢺��֡���Ƨ�ֻ�16x16���������ͼ��
void OLED_Display_16x16(uchar x,uchar y,uchar *dp)
{
	uchar i,j;
	for(j=0;j<2;j++)
	{
		OLED_address(x,y);
		for(i=0;i<16;i++)
		{
			OLED_WR_Byte(*dp,OLED_DATA);  //д���ݵ�OLED,ÿд��һ��8λ�����ݺ��е�ַ�Զ���1
			dp++;
    }
		y++;
  }
}

//��ʾ8x16����ͼ��ASCII, ��8x16����������ַ�������ͼ��
void OLED_Display_8x16(uchar x,uchar y,uchar *dp)
{
	uchar i,j;
	for(j=0;j<2;j++)
	{
		OLED_address(x,y);
		for(i=0;i<8;i++)
		{
			OLED_WR_Byte(*dp,OLED_DATA);   //д���ݵ�LCD,ÿд��һ��8λ�����ݺ��е�ַ�Զ���1
			dp++;
    }
		y++;
  }
}

//��ʾ5*7����ͼ��ASCII, ��5x7����������ַ�������ͼ��
void OLED_Display_5x7(uchar x,uchar y,uchar *dp)
{
	uchar i;
  OLED_address(x,y);
	for(i=0;i<6;i++)
	{
		OLED_WR_Byte(*dp,OLED_DATA);//OLED_DATA д����
		dp++;
  }
}

//��ָ�����Ѷ�ֿ�IC
void Send_Command_to_ROM(uchar dat)
{
	uchar i;
	for(i=0;i<8;i++)
	{
		OLED_SCL_Clr();
		if(dat&0x80)
		{
			OLED_SDA_Set();
    }
		else
		{
			OLED_SDA_Clr();
    }
		dat<<=1;
		OLED_SCL_Set();
  }
}

//�Ӿ���Ѷ�ֿ�IC��ȡ���ֻ��ַ����ݣ�1���ֽڣ�
uchar Get_data_from_ROM()
{
	uchar i=0,read=0;
	for(i=0;i<8;i++)
	{
		OLED_SCL_Clr();
		read<<=1;
		if(OLED_READ_FS0)
		{
			read++;
    }
		OLED_SCL_Set();
  }
	return read;
}


//����ص�ַ��addrHigh����ַ���ֽ�,addrMid����ַ���ֽ�,addrLow����ַ���ֽڣ�����������DataLen���ֽڵ����ݵ� pbuff�ĵ�ַ
//������ȡ
void OLED_get_data_from_ROM(uchar addrHigh,uchar addrMid,uchar addrLow,uchar *pbuff,uchar DataLen)
{
	uchar i;
	OLED_ROM_CS_Clr();
	Send_Command_to_ROM(0x03);
	Send_Command_to_ROM(addrHigh);
	Send_Command_to_ROM(addrMid);
	Send_Command_to_ROM(addrLow);
	for(i=0;i<DataLen;i++)
	{
		*(pbuff+i)=Get_data_from_ROM();
  }
	OLED_ROM_CS_Set();
}


ulong fontaddr=0;
void OLED_16x16(uchar x,uchar y,uchar *text)   //16*16
{
	uchar i=0;
	uchar addrHigh,addrMid,addrLow;
	uchar fontbuf[32];
	while(text[i]>0x00)
	{
		if((text[i]>=0xb0)&&(text[i]<=0xf7)&&(text[i+1]>=0xa1))
		{
			//������壨GB2312�������ھ���Ѷ�ֿ�IC�еĵ�ַ�����¹�ʽ�����㣺
			//Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*32+ BaseAdd;BaseAdd=0
			//���ڵ���8λ��Ƭ���г˷�������⣬���Է�����ȡ��ַ
			fontaddr=(text[i]-0xb0)*94;
			fontaddr+=(text[i+1]-0xa1)+846;
			fontaddr=fontaddr*32;
			
			addrHigh=(fontaddr&0xff0000)>>16;   //��ַ�ĸ�8λ,��24λ
			addrMid=(fontaddr&0xff00)>>8;       //��ַ����8λ,��24λ
			addrLow=(fontaddr&0xff);            //��ַ�ĵ�8λ,��24λ
			
			OLED_get_data_from_ROM(addrHigh,addrMid,addrLow,fontbuf,32);
			//ȡ32���ֽڵ����ݣ��浽"fontbuf[32]"
			OLED_Display_16x16(x,y,fontbuf);
			//��ʾ���ֵ�LCD�ϣ�yΪҳ��ַ��xΪ�е�ַ��fontbuf[]Ϊ����
			x+=16;
			i+=2;
    }
		else if((text[i]>=0xa1)&&(text[i]<=0xa3)&&(text[i+1]>=0xa1))
		{
			
			fontaddr=(text[i]-0xa1)*94;
			fontaddr+=(text[i+1]-0xa1);
			fontaddr=fontaddr*32;
			
			addrHigh=(fontaddr&0xff0000)>>16;
			addrMid=(fontaddr&0xff00)>>8;
			addrLow=(fontaddr&0xff);
			
			OLED_get_data_from_ROM(addrHigh,addrMid,addrLow,fontbuf,32);
			OLED_Display_16x16(x,y,fontbuf);
			x+=16;
			i+=2;
    }
		else if((text[i]>=0x20)&&(text[i]<=0x7e))
		{
			unsigned char fontbuf[16];
			fontaddr=(text[i]-0x20);
			fontaddr=(unsigned long)(fontaddr*16);
			fontaddr=(unsigned long)(fontaddr+0x3cf80);
			
			addrHigh=(fontaddr&0xff0000)>>16;
			addrMid=(fontaddr&0xff00)>>8;
			addrLow=fontaddr&0xff;
			
			OLED_get_data_from_ROM(addrHigh,addrMid,addrLow,fontbuf,16);
			OLED_Display_8x16(x,y,fontbuf);
			x+=8;
			i+=1;
    }
		else 
			i++;
  }
}

void OLED_Display_string_5x7(uchar x,uchar y,uchar *text)   //�ַ�����ʾ
{
	uchar i=0;
	uchar addrHigh,addrMid,addrLow;
	while(text[i]>0x00)
	{
		if((text[i]>=0x20)&&(text[i]<=0x7e))
		{
			uchar fontbuf[8];
			fontaddr=(text[i]-0x20);
			fontaddr=(unsigned long)(fontaddr*8);
			fontaddr=(unsigned long)(fontaddr+0x3bfc0);
			
			addrHigh=(fontaddr&0xff0000)>>16;
			addrMid=(fontaddr&0xff00)>>8;
			addrLow=fontaddr&0xff;
			
			OLED_get_data_from_ROM(addrHigh,addrMid,addrLow,fontbuf,8);
			OLED_Display_5x7(x,y,fontbuf);
			x+=6;
			i+=1;
    }
		else 
			i++;
  }
}

//��ʾ2������
//x,y :�������
//num1��Ҫ��ʾ��С��
//len :���ֵ�λ��
void OLED_ShowNum(uchar x,uchar y,float num1,uchar len)
{
	uchar i=0;
	ulong t=0,num=0;
	x=x+len*8+8;//Ҫ��ʾ��С�����λ�ĺ�����
	num=num1*100;//��С��������λ��ת��Ϊ����
	OLED_16x16(x-24,y,".");//��ʾС����
	for(i=0;i<len;i++)
	{
		t=num%10;//ȡ��λ������ֵ
		num=num/10;//����������һλ
		x-=8;
		if(i==2){x-=8;}//����ʾ��������С��֮�󣬿ճ�С�����λ��
		switch(t)
		{
			case 0 :OLED_16x16(x,y,"0");break;
			case 1 :OLED_16x16(x,y,"1");break;
			case 2 :OLED_16x16(x,y,"2");break;
			case 3 :OLED_16x16(x,y,"3");break;
			case 4 :OLED_16x16(x,y,"4");break;
			case 5 :OLED_16x16(x,y,"5");break;
			case 6 :OLED_16x16(x,y,"6");break;
			case 7 :OLED_16x16(x,y,"7");break;
			case 8 :OLED_16x16(x,y,"8");break;
			case 9 :OLED_16x16(x,y,"9");break;
		}
	}
}

//OLED�ĳ�ʼ��
void OLED_Init(void)
{
	
	OLED_CS_Set();
	OLED_ROM_CS_Set();
	delay_ms(200);
	OLED_WR_Byte(0xAE,OLED_CMD); /*display off*/ 
	OLED_WR_Byte(0x02,OLED_CMD); /*set lower column address*/ 
	OLED_WR_Byte(0x10,OLED_CMD); /*set higher column address*/ 
	OLED_WR_Byte(0x40,OLED_CMD); /*set display start line*/ 
	OLED_WR_Byte(0xB0,OLED_CMD); /*set page address*/
	OLED_WR_Byte(0x81,OLED_CMD); /*contract control*/ 
	OLED_WR_Byte(0xcf,OLED_CMD); /*128*/ 
	OLED_WR_Byte(0xA1,OLED_CMD); /*set segment remap*/ 
	OLED_WR_Byte(0xA6,OLED_CMD); /*normal / reverse*/ 
	OLED_WR_Byte(0xA8,OLED_CMD); /*multiplex ratio*/ 
	OLED_WR_Byte(0x3F,OLED_CMD); /*duty = 1/64*/ 
	OLED_WR_Byte(0xad,OLED_CMD); /*set charge pump enable*/ 
	OLED_WR_Byte(0x8b,OLED_CMD); /* 0x8B �ڹ� VCC */ 
	OLED_WR_Byte(0x33,OLED_CMD); /*0X30---0X33 set VPP 9V */ 
	OLED_WR_Byte(0xC8,OLED_CMD); /*Com scan direction*/ 
	OLED_WR_Byte(0xD3,OLED_CMD); /*set display offset*/ 
	OLED_WR_Byte(0x00,OLED_CMD); /* 0x20 */ 
	OLED_WR_Byte(0xD5,OLED_CMD); /*set osc division*/ 
	OLED_WR_Byte(0x80,OLED_CMD); 
	OLED_WR_Byte(0xD9,OLED_CMD); /*set pre-charge period*/ 
	OLED_WR_Byte(0x1f,OLED_CMD); /*0x22*/ 
	OLED_WR_Byte(0xDA,OLED_CMD); /*set COM pins*/ 
	OLED_WR_Byte(0x12,OLED_CMD); 
	OLED_WR_Byte(0xdb,OLED_CMD); /*set vcomh*/ 
	OLED_WR_Byte(0x40,OLED_CMD);
	OLED_Clear();
	OLED_WR_Byte(0xAF,OLED_CMD); /*display ON*/
}


