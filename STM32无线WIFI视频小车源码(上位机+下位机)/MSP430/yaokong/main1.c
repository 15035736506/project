#include<msp430f149.h>
#include"delay.h"
#include"moniNRF24L01.h"


#define KeyPort         P1IN  //独立键盘接在P10~P13
//#define LED             P3OUT  //LED灯接在P34、P35上


#define keyUP 1
#define keyDOWN 2
#define keyLEFT 3
#define keyRIGHT 4
#define STOP 0

unsigned char key;

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

//**********************************************************************
//	键盘扫描子程序，采用逐键扫描的方式
//**********************************************************************

unsigned char Key_Scan(void) 
{
  unsigned char key_check;
  unsigned char key_checkin;
  
  key_checkin = KeyPort;          	//读取IO口状态，判断是否有键按下
  key_checkin &= 0x0F;          		//读取IO口状态，判断是否有键按下
  if(key_checkin!=0x0F)            	//IO口值发生变化则表示有键按下
    {
      delay_ms(20);                  	//键盘消抖，延时20MS
      key_checkin=KeyPort;
      if(key_checkin!=0x0F)
        {  
          key_check = KeyPort;
          switch (key_check & 0x0F)
            {
              case 0x0E:key = keyUP;break;
              case 0x0D:key = keyDOWN;break;
              case 0x0B:key = keyLEFT;break;
              case 0x07:key = keyRIGHT;break;
            default :
              key = STOP;
              break;
            }          
        }      
   }
  else
   {
     key = STOP;        
   }
  return key;
}

void main()
{                       
  unsigned char TxBuf[3]={150,150,0};//左右轮速度，方向
  Clock_Init();
  P3DIR = BIT4+BIT5;
  P3OUT = BIT4+BIT5;
  P1DIR |= 0x00;  

  Init_NRF24L01();
  while(1)
  {
    Key_Scan();
    
    if(TxBuf[2] != key)
//    if(key!=0)
    {
      TxBuf[2] = key;
      nRF24L01_TxPacket(TxBuf);
//      while(CheckACK());
      delay_ms(100);
      
      P3OUT ^= ~BIT4;
    }
  }
}