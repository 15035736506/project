#include<msp430f149.h>
#include"delay.h"
#include"moniNRF24L01.h"


#define KeyPort         P1IN  //独立键盘接在P10~P13
//#define LED             P3OUT  //LED灯接在P34、P35上


#define keyUP 1
#define keyDOWN 2
#define keyLEFT 3
#define keyRIGHT 4

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
/*
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
            default : break;
            }          
        }      
   }
  else
   {
     key = 0;        
   }
  return key;
}
*/
#define LED1_OFF  P3OUT|=BIT4
#define LED1_ON P3OUT&=~BIT4
#define LED2_OFF  P3OUT|=BIT5
#define LED2_ON P3OUT&=~BIT5

extern void Delay(unsigned int s);

void main()
{
  WDTCTL = WDTPW + WDTHOLD;
  unsigned char tf =0;
//  unsigned char txdata1[20]={0};
//  unsigned char txdata2[20]={0};
  unsigned char TxBuf[3]={0};
  unsigned char RxBuf[3]={0};	  
   
//  txdata1[1] = 1;
//  txdata2[2] = 2;
  TxBuf[1] =1;
  TxBuf[2] =1;  
  Clock_Init();
  P3DIR = BIT4+BIT5;
  P3OUT = BIT4+BIT5;
//  delay_ms(1000);
  Init_NRF24L01();
//    TxBuf[2] =3;

//  nRF24L01_TxPacket(TxBuf);
//  delay_ms(6);
//  P3OUT ^= BIT4;
  P1DIR=0x00;
    while(1)
  {

    if((P1IN&BIT0)==0) 
    {
      LED2_ON;
      TxBuf[2] =1;
      tf = 1 ; 
    }
    if((P1IN&BIT1)==0)
    {
      LED2_ON;
      TxBuf[2] =3;
      tf = 1 ; 
    }
    if (tf==1)
    {
      nRF24L01_TxPacket(TxBuf);//Transmit Tx buffer data
//      delay_us(100);
//      CheckACK();
      TxBuf[1] = 0x00;
      TxBuf[2] = 0x00;
      tf=0;
//      Delay(1000);
      delay_ms(10);
      LED2_OFF;
    }
    SetRX_Mode();
    nRF24L01_RxPacket(RxBuf);
    if(RxBuf[1]|RxBuf[2])
    {					
      if(RxBuf[1]==1)LED1_OFF;
      if(RxBuf[2]==3)LED1_ON;
//      Delay(1000);
      delay_us(80);
    }
    RxBuf[1] = 0x00;
    RxBuf[2] = 0x00;        
  }
//  while(1)
//  {
/*    nRF24L01_TxPacket(txdata1);
    P3OUT ^= BIT4;
    delay_ms(500);
    nRF24L01_TxPacket(txdata2);
    P3OUT ^= BIT4;
//    SetRX_Mode();
    delay_ms(500);
    */
/*    if((P1IN&BIT0)==0) 
    {
//      LED2_ON;
      P3OUT&=~BIT4;
      TxBuf[1] =1;
      tf = 1 ; 
    }
    if((P1IN&BIT1)==0)
    {
      P3OUT&=~BIT4;
      TxBuf[2] =3;
      tf = 1 ; 
    }
    if (tf==1)
    {
      nRF24L01_TxPacket(TxBuf);//Transmit Tx buffer data
      TxBuf[1] = 0x00;
      TxBuf[2] = 0x00;
      tf=0;
      delay_ms(1);
      P3OUT|=BIT4;;
    }
  }*/
}