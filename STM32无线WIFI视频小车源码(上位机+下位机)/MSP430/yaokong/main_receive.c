#include<msp430f149.h>
#include"delay.h"
#include"moniNRF24L01.h"

unsigned char RX_buf[20];

void Clock_Init()
{
  unsigned char i;
  WDTCTL = WDTPW + WDTHOLD;       //关闭看门狗
  BCSCTL1&=~XT2OFF;                 //打开XT振荡器
  BCSCTL2|=SELM1+SELS;              //MCLK为8MHZ，SMCLK为8MHZ
  do{
    IFG1&=~OFIFG;                   //清除震荡标志
    for(i=0;i<100;i++)
       _NOP();                      //延时等待
  }
  while((IFG1&OFIFG)!=0);           //如果标志为1，则继续循环等待
  IFG1&=~OFIFG; 
}

void conf_NRF24L01_IRQ()
{
  P1DIR&=~BIT3;//输入
  P1SEL|=BIT3;//引脚功能
  P1IE|=BIT3;//允许中断
  P1IES|=BIT3;//下降沿触发
//  P2IFG=0x80;
}

void main()
{
  unsigned char RxBuf[20]={0};	  
  Clock_Init();
//  conf_NRF24L01_IRQ();
  
  _EINT();
  P3DIR |= BIT4 + BIT5;
  Init_NRF24L01();
  SetRX_Mode();
  P3OUT |= BIT4 + BIT5;
  conf_NRF24L01_IRQ();
  
  while(1)
  {
/*    if(NRF24L01_IRQval==0)
//    if(nRF24L01_RxPacket(RX_buf)==1)
    {
      nRF24L01_RxPacket(&RX_buf[0]);
      if(RX_buf[0]==1)
        P3OUT |= BIT4;
      else
        P3OUT &= ~BIT4;
    }*/
//    SetRX_Mode();
//    while(nRF24L01_RxPacket(RX_buf)!=1);P3OUT ^= BIT5;
//    if(RX_buf[1]|RX_buf[2])
//    {					
//      if(RX_buf[1]==1)P3OUT |= BIT4;
//      if(RX_buf[2]==2)P3OUT &= ~BIT4;
//      delay_ms(100);
//    }
//    RX_buf[1] = 0x00;
//    RX_buf[2] = 0x00;
//    SetRX_Mode();
    nRF24L01_RxPacket(RxBuf);
    if(RxBuf[1]|RxBuf[2])
    {					
      if(RxBuf[1]==1)P3OUT|=BIT4;
      if(RxBuf[2]==3)P3OUT&=~BIT4;
      delay_us(80);
    }
    RxBuf[1] = 0x00;
    RxBuf[2] = 0x00;
  
  }
}


#pragma vector = PORT1_VECTOR
__interrupt void P1_IRQ()
{
/*  switch(P1IFG)
  {
  case 0x01:
    break;
  case 0x04:
    break;
  case 0x80:      */
//    nRF24L01_RxPacket(RX_buf);
    P3OUT ^= BIT5;
//    break;
//  default :
//    break;
//  }
  P1IFG = 0x00;
}