#include<msp430f149.h>
#include"delay.h"
#include"moniNRF24L01.h"


#define KeyPort         P1IN  //�������̽���P10~P13
//#define LED             P3OUT  //LED�ƽ���P34��P35��


#define keyUP 1
#define keyDOWN 2
#define keyLEFT 3
#define keyRIGHT 4
#define STOP 0

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
  unsigned char TxBuf[3]={150,150,0};//�������ٶȣ�����
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