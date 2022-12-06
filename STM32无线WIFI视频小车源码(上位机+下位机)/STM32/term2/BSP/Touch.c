/******************** (C) COPYRIGHT 2012 WildFire Team **************************
 * 文件名  ：Touch.c
 * 描述    ：LCD触摸应用函数库         
 * 实验平台：野火STM32开发板
 * 库版本  ：ST3.5.0
 *
 * 作者    ：wildfire team 
 * 论坛    ：www.ourdev.cn/bbs/bbs_list.jsp?bbs_id=1008
 * 淘宝    ：http://firestm32.taobao.com
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

/* 差值门限 */
#define THRESHOLD 2 
  


/******************************************************
* 函数名：Touch_spi_init
* 描述  ：2046 Touch SPI 初始化
* 输入  : 无
* 输出  ：无
* 举例  ：无
* 注意  ：无
*********************************************************/    
static void Touch_spi_init(void) 
{ 
    SPI_InitTypeDef  SPI_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
    
    /* DISABLE SPI1 */ 
    SPI_Cmd(SPI1, DISABLE); 
    
    /* SPI1 配置 --*/ 
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
* 函数名：Touch_init
* 描述  ：2046 Touch 端口初始化
* 输入  : 无
* 输出  ：无
* 举例  ：无
* 注意  ：无
*********************************************************/    
void Touch_init(void) 
{ 
//    EXTI_InitTypeDef EXTI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);
    
    /* 配置SPI1管脚: SCK--PA5, MISO--PA6 and MOSI--PA7 ---------------------------------*/ 
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
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD ; //下拉输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
//    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
/*
    TOUCH_NVIC_Configuration();
    EXTI_InitStructure.EXTI_Line = EXTI_Line6;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	//下降沿中断
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource6);
*/    
    CS_2046(1); 
    Touch_spi_init(); 
} 





/******************************************************
* 函数名：TOUCH_NVIC_Configuration
* 描述  ：2046 Touch INT 嵌套中断配置
* 输入  : 无
* 输出  ：无
* 举例  ：无
* 注意  ：留意一下优先级
*********************************************************/
    
void TOUCH_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;


    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
 

    /*使能EXTI9_5 中断 */		      
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn  ; //需要配置的中断向量   
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//配置相应中断向量抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //配置相应中断向量响应优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	//使能或关闭相应的中断响应
    NVIC_Init(&NVIC_InitStructure);
    
}





/******************************************************
* 函数名：DelayUS
* 描述  ：粗略延时cnt 微妙
* 输入  : cnt
* 输出  ：无
* 举例  ：无
* 注意  ：该函数创建的目的是局部使用,从而不必特意去使用定时器来延时
*********************************************************/    
static void DelayUS(vu32 cnt)
{
    uint16_t i;
    for(i = 0;i<cnt;i++)
    {
        uint8_t us = 12; /* 设置值为12，大约延1微秒 */    
        while (us--)     /* 延1微秒	*/
        {
            ;   
        }
    }
}

/******************************************************
* 函数名：WR_CMD
* 描述  ：向 Touch 2046写数据
* 输入  : cmd 		--需要写入的数据
* 输出  ：无
* 举例  ：无
* 注意  ：内部使用
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
* 函数名：Read_touch_ad
* 描述  ：通过SPI方式读取 2046所获得的触摸AD转换值
* 输入  : 无
* 输出  ：无
* 举例  ：无
* 注意  ：内部使用
*********************************************************/    
int Read_touch_ad(void)  
{ 
    unsigned short buf,temp; 
    /*  等待数据寄存器空 */  
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET); 

    /* 通过SPI1发送数据 */  
    SPI_I2S_SendData(SPI1,0x0000); 

    /* 等待接收到一个字节的数据 */ 
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET); 

    /* 读取SPI1接收的数据 */		 
    temp=SPI_I2S_ReceiveData(SPI1); 

    buf=temp<<8; 
    DelayUS(1);

		/*  等待数据寄存器空 */  
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET); 

    /* 通过SPI1发送数据 */ 
    SPI_I2S_SendData(SPI1,0x0000); 

    /* 等待接收到一个字节的数据 */ 
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET); 
 
    /* 读取SPI1接收的数据 */ 
    temp=SPI_I2S_ReceiveData(SPI1); 
 
    buf |= temp; 	 //共读出了16位
    buf>>=3; 		 //输出的应该是一个12位的ADC数据
    buf&=0xfff; 
    return buf; 

} 

/******************************************************
* 函数名：Read_X
* 描述  ：通过SPI方式读取 2046所获得的触摸 通道X+ AD转换值
* 输入  : 无
* 输出  ：X+通道AD转换值
* 举例  ：无
* 注意  ：无
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
* 函数名：Read_Y
* 描述  ：通过SPI方式读取 2046所获得的触摸 通道Y+ AD转换值
* 输入  : 无
* 输出  ：Y+通道AD转换值
* 举例  ：无
* 注意  ：无
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

