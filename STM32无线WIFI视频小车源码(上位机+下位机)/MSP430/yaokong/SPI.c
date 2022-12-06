#include<msp430f149.h>

#define MOSI1  P3OUT|=BIT1
#define MOSI0  P3OUT&=~BIT1
#define MISO_val P3IN&BIT2

#define SCK1 P3OUT|=BIT3
#define SCK0 P3OUT&=~BIT3

/********************
SPI���ã�3����ģʽ����ʼ��
*********************/
void SPI_Init3M()
{
  U0CTL = SWRST;//��SWRST�������ô���
  U0CTL = CHAR + SYNC + MM;//8λ��SPIģʽ������ģʽ
  U0TCTL = STC + SSEL1;//3��ģʽ,��ϵͳʱ��
//���첽ͨ��ʱ����������3��= UBR��=0FFFFh
  U0BR0 = 0x02;  //����������
  U0BR1 = 0x00;
  U1MCTL = 0x00;//�����ʵ�С��λ����
  IE1 &=~UTXIE0;
  IE1 &=~URXIE0;
  ME1 = USPIE0;//Enable USART1 SPI mode
  
  U1CTL &= ~SWRST;//clear SWRST,������ϣ�  
  P3DIR &= ~0x0e;//�ڶ�����
}

/**************
SPI��д
**************/
/*
unsigned char SPI_RW(unsigned char W_data)
{
  U0CTL = W_data;
  while((IFG1 & UTXIFG0)==0);//�ȴ��������
  
  while((IFG1 & URXIFG0)==0);//�ȴ��������
  return(U0RXBUF);
}
*/
//ģ��SPI��д����
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