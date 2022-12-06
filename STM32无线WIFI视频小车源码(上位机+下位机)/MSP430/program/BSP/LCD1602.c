#include"LCD1602.h"
#include"delay.h"

void LCD1602_Port_Init()
{
  P4SEL=0x00;
  P4DIR=0xff;
  P5SEL&=~(BIT5+BIT6+BIT7);
  P5DIR=BIT5+BIT6+BIT7;
}
/*
��������delay
����  �����ļ��ڲ���ʱ����
����  ��x��ŵ���ʱʱ��
���  ����
*/
void delay(uint x)
{
  uint i,j;
  for(i=x;i>0;i--)
    for(j=100;j>0;j--);
}

/*
��������write_com
����  ����LCD1602дһ�ֽڵ�����
����  ��com����
���  ����
*/
void write_com(uchar com)
{
  RSclr;
  RWclr;
  ENclr;  
  dataport=com;
  ENset;
  delay_ms(5);
  ENclr;
}

/*
��������write_data
����  ����LCD1602дһ�ֽڵ�����
����  ��data����
���  ����
*/

void write_data(uchar data)
{
  RSset;
  RWclr;
  ENclr;  
  dataport=data;
  ENset;
  delay_ms(5);
  ENclr;
}

/*
��������LCD1602_write_strAt
����  ����LCD1602�ĵ�y�е�xλд���ַ�����y=0�����һ�У�y=1����ڶ���
����  ��x,y.���꣬�ַ���
���  ����
*/
void LCD1602_write_strAt(uchar x,uchar y,uchar *s)
{
  switch(y)
  {
  case 0:
    write_com(0x80+x);
    break;
  case 1:
    write_com(0x80+0x40+x);
    break;  
  }
  while(*s)
  {
    write_data(*s);
    s++;
    delay(100);    
  }
}

/*
��������LCD1602_writeAt
����  ����LCD1602�ĵ�y�е�xλд���ַ�����y=0�����һ�У�y=1����ڶ���
����  ��x,y.����
���  ����
*/
void LCD1602_writeAt(uchar x,uchar y)
{
  switch(y)
  {
  case 0:
    write_com(0x80+x);
    break;
  case 1:
    write_com(0x80+0x40+x);
    break;  
  }
}

/*
��������LCD1602_write_str
����  ����LCD1602д���ַ���
����  ���ַ���
���  ����
*/
void LCD1602_write_str(uchar *s)
{
  while(*s)
  {
    write_data(*s);
    s++;
    delay(50);    
  }
}

/*
��������LCD1602_write
����  ����LCD1602д���ַ�
����  ���ַ�
���  ����
*/
void LCD1602_write(uchar data)
{
  write_data(data);
}

/*
��������LCD_Init
����  ����LCD1602���г�ʼ��
����  ����
���  ����
*/
void LCD_Init()
{
  write_com(0x38);
  write_com(0x0c);
  write_com(0x06);
  write_com(0x01);
}