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


#define RX_DR 0x40	  //��������׼������
#define TX_DS 0x20	  //�ѷ�������
#define MAX_RT 0x10

unsigned char TxAddr[]={0x34,0x43,0x10,0x10,0x01};//���͵�ַ

//NRF24L01�˿ڳ�ʼ��
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

//���Ĵ�������
unsigned char NRFReadReg(unsigned char reg)
{
  unsigned char reg_val;
  
  NRF24L01_CS0;//Ƭѡ
  SPI_RW(reg);//ѡ��Ҫ��ȡ�ļĴ���
  reg_val = SPI_RW(0);//��ȡ����
  NRF24L01_CS1;//�ر�Ƭѡ
  
  return reg_val;
}

//д�Ĵ�������
unsigned char NRFWriteReg(unsigned char reg,unsigned char value)
{
  unsigned char status;
    
  NRF24L01_CS0;//Ƭѡ
  status = SPI_RW(reg);//ѡ��Ҫд��ļĴ���
  SPI_RW(value);//д����
  NRF24L01_CS1;//�ر�Ƭѡ
  
  return status;
}

/*****************SPI��ȡRXFIFO�Ĵ�����ֵ********************************/
unsigned char NRFReadRxDate(unsigned char RegAddr,unsigned char *RxDate,unsigned char DateLen)
{  //�Ĵ�����ַ//��ȡ���ݴ�ű���//��ȡ���ݳ���//���ڽ���
  unsigned char BackDate,i;
  NRF24L01_CS0;//����ʱ��
  BackDate = SPI_RW(RegAddr);//д��Ҫ��ȡ�ļĴ�����ַ
  for(i = 0;i<DateLen;i++) //��ȡ����
  {
    RxDate[i] = SPI_RW(0);
  } 
  NRF24L01_CS1;
  return(BackDate); 
}

/*****************SPIд��TXFIFO�Ĵ�����ֵ**********************************/
unsigned char NRFWriteTxDate(unsigned char RegAddr,unsigned char *TxDate,unsigned char DateLen)
{ //�Ĵ�����ַ//д�����ݴ�ű���//��ȡ���ݳ���//���ڷ���
  unsigned char BackDate,i;
  NRF24L01_CS0;
  BackDate = SPI_RW(RegAddr);//д��Ҫд��Ĵ����ĵ�ַ
  for(i=0;i<DateLen;i++)//д������
  {
    SPI_RW(*TxDate++);
  }   
  NRF24L01_CS1;
  return(BackDate);
}

/*****************NRF����Ϊ����ģʽ����������******************************/
void NRFSetTxMode(unsigned char *TxDate)
{//����ģʽ
  NRF24L01_CE0; 
  NRFWriteTxDate(W_REGISTER+TX_ADDR,TxAddr,TX_ADDR_WITDH);//д�Ĵ���ָ��+���յ�ַʹ��ָ��+���յ�ַ+��ַ���
  NRFWriteTxDate(W_REGISTER+RX_ADDR_P0,TxAddr,TX_ADDR_WITDH);//Ϊ��Ӧ������豸������ͨ��0��ַ�ͷ��͵�ַ��ͬ

  NRFWriteTxDate(W_TX_PAYLOAD,TxDate,TX_DATA_WITDH);//д������ 
  
  /******�����йؼĴ�������**************/
  NRFWriteReg(W_REGISTER+EN_AA,0x01);       // ʹ�ܽ���ͨ��0�Զ�Ӧ��
  NRFWriteReg(W_REGISTER+EN_RXADDR,0x01);   // ʹ�ܽ���ͨ��0
  NRFWriteReg(W_REGISTER+SETUP_RETR,0x0a);  // �Զ��ط���ʱ�ȴ�250us+86us���Զ��ط�10��
  NRFWriteReg(W_REGISTER+RF_CH,0x40);         // ѡ����Ƶͨ��0x40
  NRFWriteReg(W_REGISTER+RF_SETUP,0x07);    // ���ݴ�����1Mbps�����书��0dBm���������Ŵ�������
  NRFWriteReg(W_REGISTER+CONFIG,0x0e);      // CRCʹ�ܣ�16λCRCУ�飬�ϵ�  
  NRF24L01_CE1;
  delay_us(11);//����10us������
}

/*****************NRF����Ϊ����ģʽ����������******************************/
//��Ҫ����ģʽ
void NRFSetRXMode()
{
  NRF24L01_CE0;  
  NRFWriteTxDate(W_REGISTER+RX_ADDR_P0,TxAddr,TX_ADDR_WITDH);  // �����豸����ͨ��0ʹ�úͷ����豸��ͬ�ķ��͵�ַ
  NRFWriteReg(W_REGISTER+EN_AA,0x01);               // ʹ�ܽ���ͨ��0�Զ�Ӧ��
  NRFWriteReg(W_REGISTER+EN_RXADDR,0x01);           // ʹ�ܽ���ͨ��0
  NRFWriteReg(W_REGISTER+RF_CH,0x40);                 // ѡ����Ƶͨ��0x40
  NRFWriteReg(W_REGISTER+RX_PW_P0,TX_DATA_WITDH);  // ����ͨ��0ѡ��ͷ���ͨ����ͬ��Ч���ݿ��
  NRFWriteReg(W_REGISTER+RF_SETUP,0x07);            // ���ݴ�����1Mbps�����书��0dBm���������Ŵ�������*/
  NRFWriteReg(W_REGISTER+CONFIG,0x0f);              // CRCʹ�ܣ�16λCRCУ�飬�ϵ磬����ģʽ
  NRF24L01_CE1;
  delay_us(130);//130us��NRF24L01��ʼ��������Ϣ     
}

/****************************���Ӧ���ź�******************************/
unsigned char CheckACK()
{  //���ڷ���
  unsigned char sta;
  sta = NRFReadReg(R_REGISTER+STATUS);                    // ����״̬�Ĵ���
  if(sta&0x20) //TX_DS,��������ж�
  {
    NRFWriteReg(W_REGISTER+STATUS,0xff);  // ���TX_DS��MAX_RT�жϱ�־

    NRF24L01_CS0;
    SPI_RW(FLUSH_TX);//�������FIFO �����ؼ�������Ȼ��������벻���ĺ����������Ҽ�ס����  
    NRF24L01_CS1; 
    return(0);
  }
  else if(sta&0x10)//MAX_RT
    return 2;
  else
    return(1);
}

/******************�ж��Ƿ�����յ����ݣ��ӵ��ʹ�RXȡ��*********************/
//���ڽ���ģʽ
unsigned char NRFRevDate(unsigned char *RevDate)
{
  unsigned char RevFlags=0,sta;
  sta = NRFReadReg(R_REGISTER+STATUS);//�������ݺ��ȡ״̬�Ĵ���
  if(sta&0x40)//RX_RD �ж��Ƿ���յ�����
  {
    NRF24L01_CE0; 			//SPIʹ��
    NRFReadRxDate(R_RX_PAYLOAD,RevDate,RX_DATA_WITDH);// ��RXFIFO��ȡ����
    
    NRF24L01_CS0;
    SPI_RW(FLUSH_RX);//�������FIFO �����ؼ�������Ȼ��������벻���ĺ����������Ҽ�ס���� 
    NRF24L01_CS1;
    
    RevFlags = 1;	   //��ȡ������ɱ�־    
  }
  NRFWriteReg(W_REGISTER+STATUS,0xff); //���յ����ݺ�RX_DR,TX_DS,MAX_PT���ø�Ϊ1��ͨ��д1������жϱ�
//  NRF24L01_CE1;

  return(RevFlags);
}

