#include<msp430f149.h>
#include"delay.h"
//#include"LCD12864.h"

#define SDA_out P2DIR|=BIT0
#define SDA_in  P2DIR&=~BIT0
#define SDA1    P2OUT|=BIT0
#define SDA0    P2OUT&=~BIT0
#define SDA_val (P2IN&BIT0)

#define SCL_out P2DIR|=BIT3
#define SCL1    P2OUT|=BIT3
#define SCL0    P2OUT&=~BIT3



//��ʼ������
void I2C_init()
{
  SCL_out;
  SDA_out;
  
  SCL0;
  delay_us(5);
  SDA1;
  delay_us(5);  
  SCL1;
  delay_us(5);
}
//�����ź�
void I2C_start()
{
  SDA1;
  delay_us(5);
  SCL1;
  delay_us(5);
  SDA0;
  delay_us(5);
}

//ֹͣ�ź�
void I2C_stop()
{
  SDA0;
  delay_us(5);
  SCL1;
  delay_us(5);
  SDA1;
  delay_us(5);
}

//Ӧ���źż��
unsigned char I2C_respon()
{
  unsigned char i = 0;
  SDA_in;
  SCL1;
  delay_us(5);
  while(SDA_val)
  {
    i++;
    if(i>250)
    {
      I2C_stop();
      return 1;
    }
    delay_us(1);
  }
  SCL0;
  SDA_out;
  delay_us(5);
  return 0;
}

//��������Ӧ���ź�
void I2C_ACK()
{
  SCL0;
  delay_us(5);
  SDA0;
  delay_us(5);
  SCL1;
  delay_us(5);
  SCL0;
  delay_us(5);
}

/*******************************************
�������ƣ�I2C_NACK
��    �ܣ����IIC��������Ӧ�����
��    ������
����ֵ  ����
********************************************/
void I2C_NACK()
{
	SDA1;
	delay_us(5);
	SCL1;
	delay_us(5);
	SCL0;
	delay_us(5);
	SDA0;   
	delay_us(5);       
}

//дһ���ֽ�
void I2C_writebyte(unsigned char data)
{
  unsigned char i;
  for(i=0;i<8;i++)
  {
    SCL0;
    delay_us(5);
    if(data&0x80)
      SDA1;
    else
      SDA0;
    data<<=1;
    delay_us(5);
    SCL1;
    delay_us(5);
  }
  SCL0;
  delay_us(5);
  SDA1;
  delay_us(5);
}

//��һ���ֽ�
char I2C_readbyte()
{
  char i,temp;
  SDA_in;
 
  for(i=0;i<8;i++)
  {
    SCL0;
    delay_us(5);    
    SCL1;
    delay_us(5);//��ʱ��Ϊ�ߵ�ƽ�ڼ��ܼ�⵽�ȶ��ĵ�ƽ
    temp = temp<<1;
    if(SDA_val==1)
      temp++;    
    delay_us(5);

  }
  SDA_out;
  delay_us(5);
  return temp;
}