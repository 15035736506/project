/******************** (C) COPYRIGHT 2012 WildFire Team **************************
 * �ļ���  ��Touch.c
 * ����    ��LCD����Ӧ�ú�����         
 * ʵ��ƽ̨��Ұ��STM32������
 * ��汾  ��ST3.5.0
 *
 * ����    ��wildfire team 
 * ��̳    ��www.ourdev.cn/bbs/bbs_list.jsp?bbs_id=1008
 * �Ա�    ��http://firestm32.taobao.com
**********************************************************************************/
#include "Touch.h"
//#include "systick.h"
//#include "lcd.h"
#include "stm32f10x_spi.h"
#include "misc.h"
#include "stm32f10x_exti.h"
#include "lcd_botton.h"

//extern volatile unsigned char touch_flag;

//Parameter touch_para ;
//Coordinate  display ;

/* ��ֵ���� */
#define THRESHOLD 2 
  


/******************************************************
* ��������Touch_spi_init
* ����  ��2046 Touch SPI ��ʼ��
* ����  : ��
* ���  ����
* ����  ����
* ע��  ����
*********************************************************/    
static void Touch_spi_init(void) 
{ 
    SPI_InitTypeDef  SPI_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
    
    /* DISABLE SPI1 */ 
    SPI_Cmd(SPI1, DISABLE); 
    
    /* SPI1 ���� --*/ 
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; 
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master; 
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; 
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; 
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; 
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; 
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256; 
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; 
    SPI_InitStructure.SPI_CRCPolynomial = 7; 
    SPI_Init(SPI1, &SPI_InitStructure); 
    
		/* Enable SPI1 */ 
    SPI_Cmd(SPI1, ENABLE); 
} 

/******************************************************
* ��������Touch_init
* ����  ��2046 Touch �˿ڳ�ʼ��
* ����  : ��
* ���  ����
* ����  ����
* ע��  ����
*********************************************************/    
void Touch_init(void) 
{ 
//    EXTI_InitTypeDef EXTI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);
    
    /* ����SPI1�ܽ�: SCK--PA5, MISO--PA6 and MOSI--PA7 ---------------------------------*/ 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5  | GPIO_Pin_6 | GPIO_Pin_7; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;       
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
    /* 2046CS --PB7 */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    /* 2046 INT_IRQ --PB6*/
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD ; //��������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
//    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
/*
    TOUCH_NVIC_Configuration();
    EXTI_InitStructure.EXTI_Line = EXTI_Line6;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	//�½����ж�
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource6);
*/    
    CS_2046(1); 
    Touch_spi_init(); 
} 





/******************************************************
* ��������TOUCH_NVIC_Configuration
* ����  ��2046 Touch INT Ƕ���ж�����
* ����  : ��
* ���  ����
* ����  ����
* ע��  ������һ�����ȼ�
*********************************************************/
    
void TOUCH_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;


    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
 

    /*ʹ��EXTI9_5 �ж� */		      
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn  ; //��Ҫ���õ��ж�����   
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//������Ӧ�ж�������ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //������Ӧ�ж�������Ӧ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	//ʹ�ܻ�ر���Ӧ���ж���Ӧ
    NVIC_Init(&NVIC_InitStructure);
    
}





/******************************************************
* ��������DelayUS
* ����  ��������ʱcnt ΢��
* ����  : cnt
* ���  ����
* ����  ����
* ע��  ���ú���������Ŀ���Ǿֲ�ʹ��,�Ӷ���������ȥʹ�ö�ʱ������ʱ
*********************************************************/    
static void DelayUS(vu32 cnt)
{
    uint16_t i;
    for(i = 0;i<cnt;i++)
    {
        uint8_t us = 12; /* ����ֵΪ12����Լ��1΢�� */    
        while (us--)     /* ��1΢��	*/
        {
            ;   
        }
    }
}

/******************************************************
* ��������WR_CMD
* ����  ���� Touch 2046д����
* ����  : cmd 		--��Ҫд�������
* ���  ����
* ����  ����
* ע��  ���ڲ�ʹ��
*********************************************************/    
void WR_CMD (uint8_t cmd)  
{ 
    /* Wait for SPI1 Tx buffer empty */ 
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET); 
    /* Send SPI1 data */ 
    SPI_I2S_SendData(SPI1,cmd); 
    /* Wait for SPI1 data reception */ 
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET); 
    /* Read SPI1 received data */ 
    SPI_I2S_ReceiveData(SPI1); 
} 

/******************************************************
* ��������Read_touch_ad
* ����  ��ͨ��SPI��ʽ��ȡ 2046����õĴ���ADת��ֵ
* ����  : ��
* ���  ����
* ����  ����
* ע��  ���ڲ�ʹ��
*********************************************************/    
int Read_touch_ad(void)  
{ 
    unsigned short buf,temp; 
    /*  �ȴ����ݼĴ����� */  
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET); 

    /* ͨ��SPI1�������� */  
    SPI_I2S_SendData(SPI1,0x0000); 

    /* �ȴ����յ�һ���ֽڵ����� */ 
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET); 

    /* ��ȡSPI1���յ����� */		 
    temp=SPI_I2S_ReceiveData(SPI1); 

    buf=temp<<8; 
    DelayUS(1);

		/*  �ȴ����ݼĴ����� */  
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET); 

    /* ͨ��SPI1�������� */ 
    SPI_I2S_SendData(SPI1,0x0000); 

    /* �ȴ����յ�һ���ֽڵ����� */ 
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET); 
 
    /* ��ȡSPI1���յ����� */ 
    temp=SPI_I2S_ReceiveData(SPI1); 
 
    buf |= temp; 	 //��������16λ
    buf>>=3; 		 //�����Ӧ����һ��12λ��ADC����
    buf&=0xfff; 
    return buf; 

} 

/******************************************************
* ��������Read_X
* ����  ��ͨ��SPI��ʽ��ȡ 2046����õĴ��� ͨ��X+ ADת��ֵ
* ����  : ��
* ���  ��X+ͨ��ADת��ֵ
* ����  ����
* ע��  ����
*********************************************************/    
int Read_X(void)  
{  
    int i; 
    CS_2046(0); 
    DelayUS(1); 
    WR_CMD(CHX); 
    DelayUS(1); 
    i=Read_touch_ad(); 
    CS_2046(1); 
    return i;    
} 

/******************************************************
* ��������Read_Y
* ����  ��ͨ��SPI��ʽ��ȡ 2046����õĴ��� ͨ��Y+ ADת��ֵ
* ����  : ��
* ���  ��Y+ͨ��ADת��ֵ
* ����  ����
* ע��  ����
*********************************************************/    
int Read_Y(void)  
{  
    int i; 
    CS_2046(0); 
    DelayUS(1); 
    WR_CMD(CHY); 
    DelayUS(1); 
    i=Read_touch_ad(); 
    CS_2046(1); 
    return i;     
} 

/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/

