#include<msp430f149.h>
#include"NRF24L01.h"
#include"SPI.h"
#include"delay.h"

#define NRF24L01_CSout  P3DIR|=BIT7
#define NRF24L01_CS1 P3OUT|=BIT7
#define NRF24L01_CS0 P3OUT&=~BIT7

#define NRF24L01_CEout  P3DIR|=BIT0
#define NRF24L01_CE1  P3OUT|=BIT0
#define NRF24L01_CE0  P3OUT&=~BIT0

#define NRF24L01_IRQin P1DIR|=BIT7


#define RX_DR 0x40	  //接收数据准备就绪
#define TX_DS 0x20	  //已发送数据
#define MAX_RT 0x10

unsigned char TxAddr[]={0x34,0x43,0x10,0x10,0x01};//发送地址

//NRF24L01端口初始化
void NRF24L01_Init()
{
  NRF24L01_CEout;
  NRF24L01_CSout;
  NRF24L01_IRQin;
  
  NRF24L01_CS1;
  NRF24L01_CE0;
  P3DIR = 0xff;
  P3DIR &= ~BIT2;
    
  P3OUT &= ~BIT3;
  
//  SPI_Init3M();
}

//读寄存器函数
unsigned char NRFReadReg(unsigned char reg)
{
  unsigned char reg_val;
  
  NRF24L01_CS0;//片选
  SPI_RW(reg);//选择要读取的寄存器
  reg_val = SPI_RW(0);//读取数据
  NRF24L01_CS1;//关闭片选
  
  return reg_val;
}

//写寄存器函数
unsigned char NRFWriteReg(unsigned char reg,unsigned char value)
{
  unsigned char status;
    
  NRF24L01_CS0;//片选
  status = SPI_RW(reg);//选择要写入的寄存器
  SPI_RW(value);//写数据
  NRF24L01_CS1;//关闭片选
  
  return status;
}

/*****************SPI读取RXFIFO寄存器的值********************************/
unsigned char NRFReadRxDate(unsigned char RegAddr,unsigned char *RxDate,unsigned char DateLen)
{  //寄存器地址//读取数据存放变量//读取数据长度//用于接收
  unsigned char BackDate,i;
  NRF24L01_CS0;//启动时序
  BackDate = SPI_RW(RegAddr);//写入要读取的寄存器地址
  for(i = 0;i<DateLen;i++) //读取数据
  {
    RxDate[i] = SPI_RW(0);
  } 
  NRF24L01_CS1;
  return(BackDate); 
}

/*****************SPI写入TXFIFO寄存器的值**********************************/
unsigned char NRFWriteTxDate(unsigned char RegAddr,unsigned char *TxDate,unsigned char DateLen)
{ //寄存器地址//写入数据存放变量//读取数据长度//用于发送
  unsigned char BackDate,i;
  NRF24L01_CS0;
  BackDate = SPI_RW(RegAddr);//写入要写入寄存器的地址
  for(i=0;i<DateLen;i++)//写入数据
  {
    SPI_RW(*TxDate++);
  }   
  NRF24L01_CS1;
  return(BackDate);
}

/*****************NRF设置为发送模式并发送数据******************************/
void NRFSetTxMode(unsigned char *TxDate)
{//发送模式
  NRF24L01_CE0; 
  NRFWriteTxDate(W_REGISTER+TX_ADDR,TxAddr,TX_ADDR_WITDH);//写寄存器指令+接收地址使能指令+接收地址+地址宽度
  NRFWriteTxDate(W_REGISTER+RX_ADDR_P0,TxAddr,TX_ADDR_WITDH);//为了应答接收设备，接收通道0地址和发送地址相同

  NRFWriteTxDate(W_TX_PAYLOAD,TxDate,TX_DATA_WITDH);//写入数据 
  
  /******下面有关寄存器配置**************/
  NRFWriteReg(W_REGISTER+EN_AA,0x01);       // 使能接收通道0自动应答
  NRFWriteReg(W_REGISTER+EN_RXADDR,0x01);   // 使能接收通道0
  NRFWriteReg(W_REGISTER+SETUP_RETR,0x0a);  // 自动重发延时等待250us+86us，自动重发10次
  NRFWriteReg(W_REGISTER+RF_CH,0x40);         // 选择射频通道0x40
  NRFWriteReg(W_REGISTER+RF_SETUP,0x07);    // 数据传输率1Mbps，发射功率0dBm，低噪声放大器增益
  NRFWriteReg(W_REGISTER+CONFIG,0x0e);      // CRC使能，16位CRC校验，上电  
  NRF24L01_CE1;
  delay_us(11);//保持10us秒以上
}

/*****************NRF设置为接收模式并接收数据******************************/
//主要接收模式
void NRFSetRXMode()
{
  NRF24L01_CE0;  
  NRFWriteTxDate(W_REGISTER+RX_ADDR_P0,TxAddr,TX_ADDR_WITDH);  // 接收设备接收通道0使用和发送设备相同的发送地址
  NRFWriteReg(W_REGISTER+EN_AA,0x01);               // 使能接收通道0自动应答
  NRFWriteReg(W_REGISTER+EN_RXADDR,0x01);           // 使能接收通道0
  NRFWriteReg(W_REGISTER+RF_CH,0x40);                 // 选择射频通道0x40
  NRFWriteReg(W_REGISTER+RX_PW_P0,TX_DATA_WITDH);  // 接收通道0选择和发送通道相同有效数据宽度
  NRFWriteReg(W_REGISTER+RF_SETUP,0x07);            // 数据传输率1Mbps，发射功率0dBm，低噪声放大器增益*/
  NRFWriteReg(W_REGISTER+CONFIG,0x0f);              // CRC使能，16位CRC校验，上电，接收模式
  NRF24L01_CE1;
  delay_us(130);//130us后NRF24L01开始检测空中信息     
}

/****************************检测应答信号******************************/
unsigned char CheckACK()
{  //用于发射
  unsigned char sta;
  sta = NRFReadReg(R_REGISTER+STATUS);                    // 返回状态寄存器
  if(sta&0x20) //TX_DS,发送完毕中断
  {
    NRFWriteReg(W_REGISTER+STATUS,0xff);  // 清除TX_DS或MAX_RT中断标志

    NRF24L01_CS0;
    SPI_RW(FLUSH_TX);//用于清空FIFO ！！关键！！不然会出现意想不到的后果！！！大家记住！！  
    NRF24L01_CS1; 
    return(0);
  }
  else if(sta&0x10)//MAX_RT
    return 2;
  else
    return(1);
}

/******************判断是否接收收到数据，接到就从RX取出*********************/
//用于接收模式
unsigned char NRFRevDate(unsigned char *RevDate)
{
  unsigned char RevFlags=0,sta;
  sta = NRFReadReg(R_REGISTER+STATUS);//发送数据后读取状态寄存器
  if(sta&0x40)//RX_RD 判断是否接收到数据
  {
    NRF24L01_CE0; 			//SPI使能
    NRFReadRxDate(R_RX_PAYLOAD,RevDate,RX_DATA_WITDH);// 从RXFIFO读取数据
    
    NRF24L01_CS0;
    SPI_RW(FLUSH_RX);//用于清空FIFO ！！关键！！不然会出现意想不到的后果！！！大家记住！！ 
    NRF24L01_CS1;
    
    RevFlags = 1;	   //读取数据完成标志    
  }
  NRFWriteReg(W_REGISTER+STATUS,0xff); //接收到数据后RX_DR,TX_DS,MAX_PT都置高为1，通过写1来清楚中断标
//  NRF24L01_CE1;

  return(RevFlags);
}

