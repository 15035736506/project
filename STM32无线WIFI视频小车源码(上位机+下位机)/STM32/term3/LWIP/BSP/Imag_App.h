#ifndef	_IMAG_APP_H_
#define	_IMAG_APP_H_

#include "stm32f10x.h"
#include "OV7725.h"

typedef struct
{
	unsigned short x;	//显示起始点坐标
	unsigned short y;
	unsigned short Data[320];
}Data;

#define READ_FIFO_PIXEL(RGB565)   	do{RGB565=0; FIFO_RCLK_L();RGB565 = (GPIOB->IDR) & 0xff00;FIFO_RCLK_H();FIFO_RCLK_L();RGB565 |= (GPIOB->IDR >>8) & 0x00ff;FIFO_RCLK_H();}while(0)
#define FIFO_PREPARE                do{FIFO_RRST_L();FIFO_RCLK_L();FIFO_RCLK_H();FIFO_RRST_H();FIFO_RCLK_L();FIFO_RCLK_H();}while(0)


void Get_imag_and_discor(void);	 	


#endif
