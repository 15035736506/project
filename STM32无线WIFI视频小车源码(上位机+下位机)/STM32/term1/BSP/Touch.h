#ifndef _TOUCHPANEL_H_
#define _TOUCHPANEL_H_

#include "stm32f10x.h"

/* ADͨ��ѡ�������ֺ͹����Ĵ��� */
#define	CHX 	0xd0 	/* ͨ��Y+��ѡ������� */	
#define	CHY 	0x90		/* ͨ��X+��ѡ������� */



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

