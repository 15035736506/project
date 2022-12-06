/*************************************
自建一个接收协议,
以收到’@‘字符标志一组消息数据即将到来
以收到’#‘字符标志一组消息数据接收完毕！
************************************/

#include<msp430f149.h>
#include"delay.h"
#include"includes.h"

char ReceiveBuf0[10];//接收缓冲区
char Rx_Offset_Pt = 0;//接收缓冲区消息存储偏移量
char RevSussFlag = 0;//消息接收状态指示
char SendBuf0[100];//发送缓冲区



void InitUSART()
{

    P3SEL |= BIT6 + BIT7; // P3.6,7 = USART0 TXD/RXD
    P3DIR |= BIT6; 
  
    U1CTL |= SWRST;
    ME2 |= URXE1 + UTXE1; // Enable USART0 T/RXD
    
    UCTL1 |= CHAR;   // 8-bit character

    //设置波特率为115200
//    U1TCTL |= SSEL1; //SMCLK  - 8MHZ
//    U1BR0 = 0x45;    //8000000/115200 = 69.44
//    U1BR1 = 0x00;
//    U1MCTL = 0x55;  //0.44*8=3.5=4,把这4位分散开  

    //设置波特率为9600
    U1TCTL |= SSEL1; //SMCLK  - 8MHZ
    U1BR0 = 0x41;    //8000000/9600 = 833.33
    U1BR1 = 0x03;
    U1MCTL = 0x89;  //0.33*8=2.64=3,把这3位分散开 
    
    U1CTL &= ~SWRST;  // Initialize USART state machine
    IE2 |= URXIE1;    //接收中断使能
/*******************************************
    注意： 此句IE1 |= URXIE0一定要放在UCTL0 &= ~SWRST之后，
    否则串口接收中断无效！（具体原因不明）
**********************************/
}
/*******************************************
函数名称：USARTSend1Char
功    能：向PC机发送一个字符
参    数：sendchar--要发送的字符
返回值  ：无
********************************************/
void USARTSend1Char(char sendchar)
{
  while (!(IFG2 & UTXIFG1));    //等待发送寄存器为空         
  TXBUF1 = sendchar; 
   
}
/*******************************************
函数名称：USARTSendStr
功    能：向PC机发送字符串
参    数：ptr--指向发送字符串的指针
返回值  ：无
********************************************/
void USARTSendStr(char *ptr)
{
  while(*ptr != '\0')
  {
    USARTSend1Char(*ptr++);    // 发送数据
  }
  while (!(IFG2 & UTXIFG1));
//      TXBUF0 = '\n';   //发送换行指令
}
/*******************************************
函数名称：Get1Char
功    能：接收一个来自PC机的字符
参    数：无
返回值  ：接收到的字符
********************************************/
//此函数会阻塞程序运行，建议使用中断
unsigned char Get1Char(void)
{
  while (!(IFG2 & URXIFG1));    //等待接收到字符  
//  IFG2 &= ~URXIFG1;
  return  RXBUF1;
}


#pragma vector = UART1RX_VECTOR
__interrupt void usart1_rx(void)
{
  char data=0;
 OS_CPU_SR  cpu_sr;

  OS_ENTER_CRITICAL();  //保存全局中断标志,关总中断/* Tell uC/OS-II that we are starting an ISR*/
  OSIntEnter();  
  data = U1RXBUF;
//  USARTSend1Char(data);
  if(data=='@')
    Rx_Offset_Pt = 0;
  else                        //注意一条消息字符的总量不能超过99
  {
    ReceiveBuf0[Rx_Offset_Pt] = data;
    Rx_Offset_Pt++;
    if(data=='#')
      RevSussFlag = 1;//一组消息接收完成
//    else            //由应用程序清零
//      RevSussFlag = 0;//消息接收状态指示
  }
    
  OS_EXIT_CRITICAL();	  //恢复全局中断标
  OSIntExit();   
}
