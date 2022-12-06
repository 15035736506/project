/*************************************
�Խ�һ������Э��,
���յ���@���ַ���־һ����Ϣ���ݼ�������
���յ���#���ַ���־һ����Ϣ���ݽ�����ϣ�
************************************/

#include<msp430f149.h>
#include"delay.h"
#include"includes.h"

char ReceiveBuf0[10];//���ջ�����
char Rx_Offset_Pt = 0;//���ջ�������Ϣ�洢ƫ����
char RevSussFlag = 0;//��Ϣ����״ָ̬ʾ
char SendBuf0[100];//���ͻ�����



void InitUSART()
{

    P3SEL |= BIT6 + BIT7; // P3.6,7 = USART0 TXD/RXD
    P3DIR |= BIT6; 
  
    U1CTL |= SWRST;
    ME2 |= URXE1 + UTXE1; // Enable USART0 T/RXD
    
    UCTL1 |= CHAR;   // 8-bit character

    //���ò�����Ϊ115200
//    U1TCTL |= SSEL1; //SMCLK  - 8MHZ
//    U1BR0 = 0x45;    //8000000/115200 = 69.44
//    U1BR1 = 0x00;
//    U1MCTL = 0x55;  //0.44*8=3.5=4,����4λ��ɢ��  

    //���ò�����Ϊ9600
    U1TCTL |= SSEL1; //SMCLK  - 8MHZ
    U1BR0 = 0x41;    //8000000/9600 = 833.33
    U1BR1 = 0x03;
    U1MCTL = 0x89;  //0.33*8=2.64=3,����3λ��ɢ�� 
    
    U1CTL &= ~SWRST;  // Initialize USART state machine
    IE2 |= URXIE1;    //�����ж�ʹ��
/*******************************************
    ע�⣺ �˾�IE1 |= URXIE0һ��Ҫ����UCTL0 &= ~SWRST֮��
    ���򴮿ڽ����ж���Ч��������ԭ������
**********************************/
}
/*******************************************
�������ƣ�USARTSend1Char
��    �ܣ���PC������һ���ַ�
��    ����sendchar--Ҫ���͵��ַ�
����ֵ  ����
********************************************/
void USARTSend1Char(char sendchar)
{
  while (!(IFG2 & UTXIFG1));    //�ȴ����ͼĴ���Ϊ��         
  TXBUF1 = sendchar; 
   
}
/*******************************************
�������ƣ�USARTSendStr
��    �ܣ���PC�������ַ���
��    ����ptr--ָ�����ַ�����ָ��
����ֵ  ����
********************************************/
void USARTSendStr(char *ptr)
{
  while(*ptr != '\0')
  {
    USARTSend1Char(*ptr++);    // ��������
  }
  while (!(IFG2 & UTXIFG1));
//      TXBUF0 = '\n';   //���ͻ���ָ��
}
/*******************************************
�������ƣ�Get1Char
��    �ܣ�����һ������PC�����ַ�
��    ������
����ֵ  �����յ����ַ�
********************************************/
//�˺����������������У�����ʹ���ж�
unsigned char Get1Char(void)
{
  while (!(IFG2 & URXIFG1));    //�ȴ����յ��ַ�  
//  IFG2 &= ~URXIFG1;
  return  RXBUF1;
}


#pragma vector = UART1RX_VECTOR
__interrupt void usart1_rx(void)
{
  char data=0;
 OS_CPU_SR  cpu_sr;

  OS_ENTER_CRITICAL();  //����ȫ���жϱ�־,�����ж�/* Tell uC/OS-II that we are starting an ISR*/
  OSIntEnter();  
  data = U1RXBUF;
//  USARTSend1Char(data);
  if(data=='@')
    Rx_Offset_Pt = 0;
  else                        //ע��һ����Ϣ�ַ����������ܳ���99
  {
    ReceiveBuf0[Rx_Offset_Pt] = data;
    Rx_Offset_Pt++;
    if(data=='#')
      RevSussFlag = 1;//һ����Ϣ�������
//    else            //��Ӧ�ó�������
//      RevSussFlag = 0;//��Ϣ����״ָ̬ʾ
  }
    
  OS_EXIT_CRITICAL();	  //�ָ�ȫ���жϱ�
  OSIntExit();   
}
