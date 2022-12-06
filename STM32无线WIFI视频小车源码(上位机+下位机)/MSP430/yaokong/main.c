#include<msp430f149.h>
#include"delay.h"
#include"moniNRF24L01.h"


#define KeyPort         P1IN  //�������̽���P10~P13
//#define LED             P3OUT  //LED�ƽ���P34��P35��


#define keyUP 1
#define keyDOWN 2
#define keyLEFT 3
#define keyRIGHT 4

unsigned char key;

void Clock_Init()
{
  unsigned char i;
  WDTCTL = WDTPW + WDTHOLD;       //�رտ��Ź�
  BCSCTL1&=~XT2OFF;                 //��XT����
  BCSCTL2|=SELM1+SELS;              //MCLKΪ8MHZ��SMCLKΪ8MHZ
  do{
    IFG1&=~OFIFG;                   //����𵴱�־
    for(i=0;i<100;i++)
       _NOP();                      //��ʱ�ȴ�
  }
  while((IFG1&OFIFG)!=0);           //�����־Ϊ1�������ѭ���ȴ�
  IFG1&=~OFIFG; 
}

//**********************************************************************
//	����ɨ���ӳ��򣬲������ɨ��ķ�ʽ
//**********************************************************************
/*
unsigned char Key_Scan(void) 
{
  unsigned char key_check;
  unsigned char key_checkin;
  
  key_checkin = KeyPort;          	//��ȡIO��״̬���ж��Ƿ��м�����
  key_checkin &= 0x0F;          		//��ȡIO��״̬���ж��Ƿ��м�����
  if(key_checkin!=0x0F)            	//IO��ֵ�����仯���ʾ�м�����
    {
      delay_ms(20);                  	//������������ʱ20MS
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