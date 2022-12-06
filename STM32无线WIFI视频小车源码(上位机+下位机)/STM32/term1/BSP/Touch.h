#ifndef _TOUCHPANEL_H_
#define _TOUCHPANEL_H_

#include "stm32f10x.h"

/* AD通道选择命令字和工作寄存器 */
#define	CHX 	0xd0 	/* 通道Y+的选择控制字 */	
#define	CHY 	0x90		/* 通道X+的选择控制字 */



#define CS_2046(a)	if (a)	\
					GPIO_SetBits(GPIOB,GPIO_Pin_7);\
					else		\
					GPIO_ResetBits(GPIOB,GPIO_Pin_7)



#define INT_IN_2046   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)


int Read_X(void);
int Read_Y(void);

			
void Touch_init(void);

void TOUCH_NVIC_Configuration(void);	



#endif

