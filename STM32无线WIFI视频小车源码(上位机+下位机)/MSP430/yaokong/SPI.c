#include<msp430f149.h>

#define MOSI1  P3OUT|=BIT1
#define MOSI0  P3OUT&=~BIT1
#define MISO_val P3IN&BIT2

#define SCK1 P3OUT|=BIT3
#define SCK0 P3OUT&=~BIT3

/********************
SPI配置，3线主模式，初始化
*********************/
void SPI_Init3M()
{
  U0CTL = SWRST;//打开SWRST才能设置串口
  U0CTL = CHAR + SYNC + MM;//8位，SPI模式，主机模式
  U0TCTL = STC + SSEL1;//3线模式,子系统时钟
//在异步通信时，必须满足3《= UBR《=0FFFFh
  U0BR0 = 0x02;  //波特率设置
  U0BR1 = 0x00;
  U1MCTL = 0x00;//波特率的小数位调整
  IE1 &=~UTXIE0;
  IE1 &=~URXIE0;
  ME1 = USPIE0;//Enable USART1 SPI mode
  
  U1CTL &= ~SWRST;//clear SWRST,设置完毕！  
  P3DIR &= ~0x0e;//第二功能
}

/**************
SPI读写
**************/
/*
unsigned char SPI_RW(unsigned char W_data)
{
  U0CTL = W_data;
  while((IFG1 & UTXIFG0)==0);//等待发送完毕
  
  while((IFG1 & URXIFG0)==0);//等待接收完毕
  return(U0RXBUF);
}
*/
//模拟SPI读写程序
unsigned char SPI_RW(unsigned char W_data)
{
  unsigned char i;
  for(i = 0;i<8;i++)
  {
    if(W_data & 0x80)
      MOSI1;
    else
      MOSI0;
    W_data = W_data<<1;
    SCK1;
    if(MISO_val)
      W_data|=0x01;
    SCK0;
  }
  return (W_data);
}