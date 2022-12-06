#include <msp430x14x.h>
#include"moniNRF24L01.h"
#include"delay.h"

#define NRF24L01_CEout P2DIR|=BIT2
#define NRF24L01_CE1   P2OUT|=BIT2
#define NRF24L01_CE0   P2OUT&=~BIT2

#define NRF24L01_CSNout P2DIR|=BIT3
#define NRF24L01_CSN1   P2OUT|=BIT3
#define NRF24L01_CSN0   P2OUT&=~BIT3

#define NRF24L01_SCKout P2DIR|=BIT4
#define NRF24L01_SCK1   P2OUT|=BIT4
#define NRF24L01_SCK0   P2OUT&=~BIT4

#define NRF24L01_MOSIout P2DIR|=BIT5
#define NRF24L01_MOSI1   P2OUT|=BIT5
#define NRF24L01_MOSI0   P2OUT&=~BIT5


#define NRF24L01_MISOin P2DIR&=~BIT6
#define NRF24L01_MISO_val   P2IN&BIT6


#define LED1_ON  P3OUT|=BIT4
#define LED1_OFF P3OUT&=~BIT4
#define LED2_ON  P3OUT|=BIT5
#define LED2_OFF P3OUT&=~BIT5

//*********************************************NRF24L01*************************************
#define TX_ADR_WIDTH    5   	// 5 uints TX address width
#define RX_ADR_WIDTH    5   	// 5 uints RX address width
#define TX_PLOAD_WIDTH  3  	// 20 uints TX payload
#define RX_PLOAD_WIDTH  3  	// 20 uints TX payload
unsigned char TX_ADDRESS[TX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};	//本地地址
unsigned char RX_ADDRESS[RX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};	//接收地址


//***************************************NRF24L01寄存器指令*******************************************************
#define READ_REG        0x00  	// 读寄存器指令
#define WRITE_REG       0x20 	// 写寄存器指令
#define RD_RX_PLOAD     0x61  	// 读取接收数据指令
#define WR_TX_PLOAD     0xA0  	// 写待发数据指令
#define FLUSH_TX        0xE1 	// 冲洗发送 FIFO指令
#define FLUSH_RX        0xE2  	// 冲洗接收 FIFO指令
#define REUSE_TX_PL     0xE3  	// 定义重复装载数据指令
#define NOP             0xFF  	// 保留
//*************************************SPI(nRF24L01)寄存器地址****************************************************
#define CONFIG          0x00  // 配置收发状态，CRC校验模式以及收发状态响应方式
#define EN_AA           0x01  // 自动应答功能设置
#define EN_RXADDR       0x02  // 可用信道设置
#define SETUP_AW        0x03  // 收发地址宽度设置
#define SETUP_RETR      0x04  // 自动重发功能设置
#define RF_CH           0x05  // 工作频率设置
#define RF_SETUP        0x06  // 发射速率、功耗功能设置
#define STATUS          0x07  // 状态寄存器
#define OBSERVE_TX      0x08  // 发送监测功能
#define CD              0x09  // 地址检测           
#define RX_ADDR_P0      0x0A  // 频道0接收数据地址
#define RX_ADDR_P1      0x0B  // 频道1接收数据地址
#define RX_ADDR_P2      0x0C  // 频道2接收数据地址
#define RX_ADDR_P3      0x0D  // 频道3接收数据地址
#define RX_ADDR_P4      0x0E  // 频道4接收数据地址
#define RX_ADDR_P5      0x0F  // 频道5接收数据地址
#define TX_ADDR         0x10  // 发送地址寄存器
#define RX_PW_P0        0x11  // 接收频道0接收数据长度
#define RX_PW_P1        0x12  // 接收频道0接收数据长度
#define RX_PW_P2        0x13  // 接收频道0接收数据长度
#define RX_PW_P3        0x14  // 接收频道0接收数据长度
#define RX_PW_P4        0x15  // 接收频道0接收数据长度
#define RX_PW_P5        0x16  // 接收频道0接收数据长度
#define FIFO_STATUS     0x17  // FIFO栈入栈出状态寄存器设置

//*****************************************长延时*****************************************
void Delay(unsigned int s)
{
  unsigned int i;
  for(i=0; i<s; i++);
  for(i=0; i<s; i++);
}
//******************************************************************************************
#define RX_DR 0x40	  //接收数据准备就绪
#define TX_DS 0x20	  //已发送数据
#define MAX_RT 0x10
/******************************************************************************************/
//延时函数
/******************************************************************************************/
void InerDelay_us(unsigned char n)
{
  for(;n>0;n--)_NOP();
}
/****************************************************************************************************
NRF24L01 IO口设置函数
*****************************************************************************************************/
void NRF_IO_Init(void)
{
  NRF24L01_CEout;
  NRF24L01_CSNout;
  NRF24L01_SCKout;
  NRF24L01_MOSIout;
  NRF24L01_IRQin;
  NRF24L01_MISOin;  
  
}
/****************************************************************************************************/
//函数：uint SPI_RW(uint uchar)
//功能：NRF24L01的SPI写时序
/****************************************************************************************************/
unsigned char SPI_RW(unsigned char dat)
{
  unsigned char i;
  for(i=0;i<8;i++) // output 8-bit
  {
    if(dat & 0x80)NRF24L01_MOSI1;         // output 'uchar', MSB to MOSI
    else NRF24L01_MOSI0;
    delay_us(2);
    dat = dat << 1;           // shift next bit into MSB..
    NRF24L01_SCK1;                      // Set SCK high..
    if(NRF24L01_MISO_val)dat|=0x01;     // capture current MISO bit
    delay_us(2);
    NRF24L01_SCK0;            		  // ..then set SCK low again
    delay_us(2);
  }
  return(dat);           		  // return read uchar
}
/****************************************************************************************************/
//函数：uchar SPI_Read(uchar reg)
//功能：NRF24L01的SPI时序
/****************************************************************************************************/
unsigned char SPI_Read(unsigned char reg)
{
  unsigned char reg_val;
  
  NRF24L01_CSN0;                // CSN low, initialize SPI communication...
  SPI_RW(reg);            // Select register to read from..
  reg_val = SPI_RW(0);    // ..then read registervalue
  NRF24L01_CSN1;                // CSN high, terminate SPI communication
  
  return(reg_val);        // return register value
}
/****************************************************************************************************/
//功能：NRF24L01读写寄存器函数
/****************************************************************************************************/
unsigned char SPI_RW_Reg(unsigned char reg, unsigned char value)
{
  unsigned char status;
  
  NRF24L01_CSN0;                   // CSN low, init SPI transaction
  status = SPI_RW(reg);      // select register
  SPI_RW(value);             // ..and write value to it..
  NRF24L01_CSN1;                   // CSN high again
  
  return(status);            // return nRF24L01 status uchar
}
/****************************************************************************************************/
//函数：uint SPI_Read_Buf(uchar reg, uchar *pBuf, uchar uchars)
//功能: 用于读数据，reg：为寄存器地址，pBuf：为待读出数据地址，uchars：读出数据的个数
/****************************************************************************************************/
unsigned char SPI_Read_Buf(unsigned char reg, unsigned char *pBuf, unsigned char n)
{
  unsigned char status,i;
  
  NRF24L01_CSN0;                    		// Set CSN low, init SPI tranaction
  status = SPI_RW(reg);       		// Select register to write to and read status uchar
  for(i=0;i<n;i++)pBuf[i] = SPI_RW(0);
  NRF24L01_CSN1;                           
  return(status);                    // return nRF24L01 status uchar
}
/*********************************************************************************************************/
//函数：uint SPI_Write_Buf(uchar reg, uchar *pBuf, uchar uchars)
//功能: 用于写数据：为寄存器地址，pBuf：为待写入数据地址，uchars：写入数据的个数
/*********************************************************************************************************/
unsigned char SPI_Write_Buf(unsigned char reg, unsigned char *pBuf, unsigned char n)
{
  unsigned char status,i;
  
  NRF24L01_CSN0;            //SPI使能       
  status = SPI_RW(reg);   
  for(i=0;i<n;i++)SPI_RW(*pBuf++);
  NRF24L01_CSN1;           //关闭SPI
  return(status);  // 
}
/****************************************************************************************************/
//函数：void SetRX_Mode(void)
//功能：数据接收配置 
/****************************************************************************************************/
void SetRX_Mode(void)
{
  NRF24L01_CE0;
  SPI_RW_Reg(WRITE_REG + CONFIG, 0x0f);   		// IRQ收发完成中断响应，16位CRC	，主接收
  NRF24L01_CE1; 
//  InerDelay_us(130);
  delay_us(130);//130us后NRF24L01开始检测空中信息 
}
/******************************************************************************************************/
//函数：unsigned char nRF24L01_RxPacket(unsigned char* rx_buf)
//功能：数据读取后放如rx_buf接收缓冲区中
/******************************************************************************************************/
unsigned char nRF24L01_RxPacket(unsigned char *rx_buf)
{
    unsigned char revale=0;
    unsigned char sta;
    sta=SPI_Read(STATUS);	// 读取状态寄存其来判断数据接收状况
    if(RX_DR&sta)				// 判断是否接收到数据
    {
      NRF24L01_CE0; 			//SPI使能
      SPI_Read_Buf(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH);// read receive payload from RX_FIFO buffer
      revale =1;			//读取数据完成标志
    }
    SPI_RW_Reg(WRITE_REG+STATUS,sta);   //接收到数据后RX_DR,TX_DS,MAX_PT都置高为1，通过写1来清楚中断标志
    NRF24L01_CE1;
    return revale;
}
/***********************************************************************************************************/
//函数：void nRF24L01_TxPacket(unsigned char * tx_buf)
//功能：发送 tx_buf中数据
/**********************************************************************************************************/
void nRF24L01_TxPacket(unsigned char *tx_buf)
{
  NRF24L01_CE0;			//StandBy I模式	
  SPI_Write_Buf(WRITE_REG + RX_ADDR_P0,TX_ADDRESS, TX_ADR_WIDTH); // 装载接收端地址
  SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH); 		// 装载数据	
  SPI_RW_Reg(WRITE_REG + CONFIG, 0x0e);   		 // IRQ收发完成中断响应，16位CRC，主发送
  NRF24L01_CE1;		 //置高CE，激发数据发送
//  InerDelay_us(10);
  delay_us(11);//保持10us秒以上
}

/****************************检测应答信号******************************/
unsigned char CheckACK()
{  //用于发射
  unsigned char sta;
  sta=SPI_Read(STATUS);
//  sta = NRFReadReg(R_REGISTER+STATUS);                    // 返回状态寄存器
  if(sta&TX_DS) //TX_DS,发送完毕中断
  {
    SPI_RW_Reg(WRITE_REG+STATUS,sta);  // 清除TX_DS或MAX_RT中断标志

    NRF24L01_CSN0;
    SPI_RW(FLUSH_TX);//用于清空FIFO ！！关键！！不然会出现意想不到的后果！！！大家记住！！  
    NRF24L01_CSN1; 
    return(0);
  }
//  else if(sta&0x10)//MAX_RT
//    return 2;
  else
    return(1);
}


//****************************************************************************************/
//NRF24L01初始化
/***************************************************************************************/
void Init_NRF24L01()
{
  NRF_IO_Init();
  InerDelay_us(100);
  NRF24L01_CE0;    // chip enable
  NRF24L01_CSN1;   // Spidisable 
  NRF24L01_SCK0;   // 
  SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);    // 写本地地址	
  SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); // 写接收端地址
  SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);      //  频道0自动	ACK应答允许	
  SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);  //  允许接收地址只有频道0，如果需要多频道可以参考Page21  
  SPI_RW_Reg(WRITE_REG + RF_CH, 0);        //   设置信道工作为2.4GHZ，收发必须一致
  SPI_RW_Reg(WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); //设置接收数据长度，
  SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07);   		//设置发射速率为1MHZ，发射功率为最大值0dB
}

//************************************主函数************************************************************
/*
void main(void)
{
  WDTCTL=WDTPW+WDTHOLD;//关闭看门狗
  unsigned char tf =0,i;
  unsigned char TxBuf[20]={0};	 // 
  unsigned char RxBuf[20]={0};	
  P3DIR |= BIT4+BIT5;
  P3OUT |=BIT4 + BIT5;
  NRF_IO_Init();

  Init_NRF24L01();
  LED1_OFF;
//  LED2_OFF;  TxBuf[1] =1;
  TxBuf[2] =3;

  nRF24L01_TxPacket(TxBuf);	// Transmit Tx buffer data
  Delay(6000);
  while(1)
  {
    if((P1IN&BIT0)==0) 
    {
      LED2_ON;
      TxBuf[1] =1;
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
      TxBuf[1] = 0x00;
      TxBuf[2] = 0x00;
      tf=0;
      Delay(1000);
      LED2_OFF;
    }
    SetRX_Mode();
    nRF24L01_RxPacket(RxBuf);
    if(RxBuf[1]|RxBuf[2])
    {					
      if(RxBuf[1]==1)LED1_OFF;
      if(RxBuf[2]==3)LED1_ON;
      Delay(1000);
    }
    RxBuf[1] = 0x00;
    RxBuf[2] = 0x00;
  }
  
}
*/