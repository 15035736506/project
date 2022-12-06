#ifndef __OV7725_H
#define __OV7725_H 

	   
#include "stm32f10x.h"
 
#pragma  diag_suppress 870	                             /*��ֹ���"invalid multibyte character sequence"����*/

#define FIFO_CS_H()     GPIOD->BSRR =GPIO_Pin_6	  
#define FIFO_CS_L()     GPIOD->BRR  =GPIO_Pin_6	  /*����ʹFIFO���ʹ��*/

#define FIFO_WRST_H()   GPIOB->BSRR =GPIO_Pin_5	  /*��������FIFOд(����from����ͷ)ָ���˶� */
#define FIFO_WRST_L()   GPIOB->BRR  =GPIO_Pin_5	  /*����ʹFIFOд(����from����ͷ)ָ�븴λ*/

#define FIFO_RRST_H()   GPIOE->BSRR =GPIO_Pin_0	  /*��������FIFO��(���ݴ�FIFO���)ָ���˶� */
#define FIFO_RRST_L()   GPIOE->BRR  =GPIO_Pin_0	  /*����ʹFIFO��(���ݴ�FIFO���)ָ�븴λ */

#define FIFO_RCLK_H()   GPIOE->BSRR =GPIO_Pin_2	  
#define FIFO_RCLK_L()   GPIOE->BRR  =GPIO_Pin_2	  /*FIFO�������ʱ��*/

#define FIFO_WE_H()     GPIOD->BSRR =GPIO_Pin_3	  /*����ʹFIFOд����*/
#define FIFO_WE_L()     GPIOD->BRR  =GPIO_Pin_3	  

#define OV7725_ID       0x21

extern u8 Ov7725_vsync;

ErrorStatus Ov7725_Init(void);
void VSYNC_Init(void);
void FIFO_GPIO_Configuration(void);
int  OV7725_ReadReg(u8 LCD_Reg,u16 LCD_RegValue);
int  OV7725_WriteReg(u8 LCD_Reg,u16 LCD_RegValue);

#endif























