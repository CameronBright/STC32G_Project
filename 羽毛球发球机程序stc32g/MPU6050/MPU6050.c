#include "main.h"
#include "MPU6050.h"

sbit    SCL = P3^6;		//IICʱ�����Ŷ���
sbit    SDA = P3^7;		//IIC�������Ŷ���

void  InitMPU6050(void);	//��ʼ��MPU6050
void  Delay2us(void);
void  I2C_Start(void);
void  I2C_Stop(void);

bit   I2C_RecvACK(void);

void  I2C_SendByte(uchar dat);
uchar	  I2C_RecvByte(void);

void  I2C_ReadPage(void);
void  I2C_WritePage(void);
uchar	  Single_ReadI2C(uchar REG_Address);				//��ȡI2C����
void  Single_WriteI2C(uchar REG_Address,uchar REG_data);	//��I2Cд������


//I^Cʱ������ʱ���ã�����μ���оƬ�������ֲ�  6050�Ƽ���С1.3us ���ǻ�����⣬������ʱʵ��1.9us����
void Delay2us(void)
{
	uchar i;
	i = 7;		// @24MHZ, 6 + 33 = 39T, 1.625us
	while (--i);
}

//**************************************
//I2C��ʼ�ź�
//**************************************
void I2C_Start(void)
{
	Delay2us();                 //��ʱ
	Delay2us();                 //��ʱ
	SDA = 1;                    //����������
	SCL = 1;                    //����ʱ����
	Delay2us();                 //��ʱ
	Delay2us();                 //��ʱ
	SDA = 0;                    //�����½���
	Delay2us();                 //��ʱ
	SCL = 0;                    //����ʱ����
	Delay2us();                 //��ʱ
}

//**************************************
//I2Cֹͣ�ź�
//**************************************
void I2C_Stop(void)
{
    Delay2us();                 //��ʱ
    SDA = 0;                    //����������
    SCL = 1;                    //����ʱ����
	Delay2us();                 //��ʱ
    SDA = 1;                    //����������
	Delay2us();                 //��ʱ
}

//**************************************
//I2C����Ӧ���ź�
//**************************************
bit I2C_RecvACK(void)
{
	SDA = 1;
	Delay2us();                 //��ʱ
    SCL = 1;                    //����ʱ����
    Delay2us();                 //��ʱ
    CY = SDA;                   //��Ӧ���ź�
    SCL = 0;                    //����ʱ����
    Delay2us();                 //��ʱ
    return CY;
}

//**************************************
//��I2C���߷���һ���ֽ�����
//**************************************
void I2C_SendByte(uchar dat)
{
    uchar i;
    for (i=0; i<8; i++)         //8λ������
    {
        dat <<= 1;              //�Ƴ����ݵ����λ
        SDA = CY;               //�����ݿ�
        SCL = 1;                //����ʱ����
        Delay2us();             //��ʱ
        SCL = 0;                //����ʱ����
        Delay2us();             //��ʱ
    }
    I2C_RecvACK();
}

//**************************************
//��I2C���߽���һ���ֽ�����
//**************************************
uchar I2C_RecvByte(void)
{
    uchar i;
    uchar dat = 0;
    SDA = 1;                    //ʹ���ڲ�����,׼����ȡ����,
    for (i=0; i<8; i++)         //8λ������
    {
        dat <<= 1;
        SCL = 1;                //����ʱ����
        Delay2us();             //��ʱ
        dat |= SDA;             //������
        SCL = 0;                //����ʱ����
        Delay2us();             //��ʱ
    }
    return dat;
}

//**************************************
//��I2C�豸д��һ���ֽ�����
//**************************************
void Single_WriteI2C(uchar REG_Address,uchar REG_data)
{
    I2C_Start();                  //��ʼ�ź�
    I2C_SendByte(SlaveAddress);   //�����豸��ַ+д�ź�
    I2C_SendByte(REG_Address);    //�ڲ��Ĵ�����ַ��
    I2C_SendByte(REG_data);       //�ڲ��Ĵ������ݣ�
    I2C_Stop();                   //����ֹͣ�ź�
}
//**************************************
//��I2C�豸��ȡһ���ֽ�����
//**************************************
/*
uchar Single_ReadI2C(uchar REG_Address)
{
	uchar REG_data;
	I2C_Start();                   //��ʼ�ź�
	I2C_SendByte(SlaveAddress);    //�����豸��ַ+д�ź�
	I2C_SendByte(REG_Address);     //���ʹ洢��Ԫ��ַ����0��ʼ
	I2C_Start();                   //��ʼ�ź�
	I2C_SendByte(SlaveAddress+1);  //�����豸��ַ+���ź�
	REG_data=I2C_RecvByte();       //�����Ĵ�������
	
	SDA = 1;                    //дӦ���ź�
	SCL = 1;                    //����ʱ����
	Delay2us();                 //��ʱ
	SCL = 0;                    //����ʱ����
	Delay2us();                 //��ʱ
	
	I2C_Stop();                    //ֹͣ�ź�
	return REG_data;
}
*/

//**************************************
//��ʼ��MPU6050
//**************************************
void InitMPU6050(void)
{
	//P2n_open_drain(0x30);	//P2.4 -- SDA,   P2.5 -- SCL
	Delay2us();
	Delay2us();
	Delay2us();
	Delay2us();

	Single_WriteI2C(PWR_MGMT_1, 0x00);	//�������״̬
	Single_WriteI2C(SMPLRT_DIV, 0x07);  //������125hz
	Single_WriteI2C(CONFIG, 0x04);      //21HZ�˲� ��ʱA8.5ms G8.3ms  �˴�ȡֵӦ�൱ע�⣬��ʱ��ϵͳ�������Ϊ��
	Single_WriteI2C(GYRO_CONFIG, 0x08); //������500��/S 65.5LSB/g
	Single_WriteI2C(ACCEL_CONFIG, 0x08);//���ٶ�+-4g  8192LSB/g
}

//**************************************
//�ϳ�����
//**************************************
/*
int GetData(uchar REG_Address)
{
	char H,L;
	H = Single_ReadI2C(REG_Address);
	L = Single_ReadI2C(REG_Address+1);
//	return (H<<8)+L;   //�ϳ�����
	return (((uint)H << 8) + L);   //�ϳ�����
}
*/

void Read_MPU6050(uchar *buf)
{
	uchar	i;
	
	I2C_Start();                  //��ʼ�ź�
	I2C_SendByte(SlaveAddress);   //�����豸��ַ+д�ź�
	I2C_SendByte(ACCEL_XOUT_H);    //�ڲ��Ĵ�����ַ��
	I2C_Start();                   //��ʼ�ź�
	I2C_SendByte(SlaveAddress+1);  //�����豸��ַ+���ź�
	for(i=0; i<13; i++)
	{
		buf[i] = I2C_RecvByte();       //�����Ĵ�������
		SDA = 0;                    //дӦ���ź�
		SCL = 1;                    //����ʱ����
		Delay2us();                 //��ʱ
		SCL = 0;                    //����ʱ����
		Delay2us();                 //��ʱ
	}
		buf[i] = I2C_RecvByte();	//���һ���ֽ�
		SDA = 1;                    //д��Ӧ���ź�
		SCL = 1;                    //����ʱ����
		Delay2us();                 //��ʱ
		SCL = 0;                    //����ʱ����
		Delay2us();                 //��ʱ
	I2C_Stop();                    //ֹͣ�ź�
}
