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
函数名：delay
描述  ：此文件内部延时函数
输入  ：x大概的延时时间
输出  ：无
*/
void delay(uint x)
{
  uint i,j;
  for(i=x;i>0;i--)
    for(j=100;j>0;j--);
}

/*
函数名：write_com
描述  ：向LCD1602写一字节的命令
输入  ：com命令
输出  ：无
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
函数名：write_data
描述  ：向LCD1602写一字节的数据
输入  ：data数据
输出  ：无
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
函数名：LCD1602_write_strAt
描述  ：向LCD1602的第y行第x位写入字符串，y=0代表第一行，y=1代表第二行
输入  ：x,y.坐标，字符串
输出  ：无
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
函数名：LCD1602_writeAt
描述  ：向LCD1602的第y行第x位写入字符串，y=0代表第一行，y=1代表第二行
输入  ：x,y.坐标
输出  ：无
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
函数名：LCD1602_write_str
描述  ：向LCD1602写入字符串
输入  ：字符串
输出  ：无
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
函数名：LCD1602_write
描述  ：向LCD1602写入字符
输入  ：字符
输出  ：无
*/
void LCD1602_write(uchar data)
{
  write_data(data);
}

/*
函数名：LCD_Init
描述  ：对LCD1602进行初始化
输入  ：无
输出  ：无
*/
void LCD_Init()
{
  write_com(0x38);
  write_com(0x0c);
  write_com(0x06);
  write_com(0x01);
}