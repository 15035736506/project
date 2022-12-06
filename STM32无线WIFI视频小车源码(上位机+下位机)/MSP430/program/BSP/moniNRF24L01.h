#ifndef _moniNRF24L01_h
#define _moniNRF24L01_h

//typedef unsigned char uchar;
//typedef unsigned char uint;

#define NRF24L01_IRQin P2DIR&=~BIT7
#define NRF24L01_IRQval   P2IN&BIT7

void Init_NRF24L01();
unsigned char SPI_RW(unsigned char dat);
unsigned char SPI_Read(unsigned char reg);
void SetRX_Mode(void);
unsigned char SPI_RW_Reg(unsigned char reg, unsigned char value);
unsigned char SPI_Read_Buf(unsigned char reg, unsigned char *pBuf, unsigned char n);
unsigned char SPI_Write_Buf(unsigned char reg, unsigned char *pBuf, unsigned char n);
unsigned char nRF24L01_RxPacket(unsigned char* rx_buf);
void nRF24L01_TxPacket(unsigned char * tx_buf);
unsigned char CheckACK();

#endif