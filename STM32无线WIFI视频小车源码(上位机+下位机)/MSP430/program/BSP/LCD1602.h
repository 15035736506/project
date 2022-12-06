#ifndef __LCD1602_H
#define __LCD1602_H

#include<msp430f149.h>

#define uchar unsigned char
#define uint unsigned int
/********port_conf************/
#define RSset P5OUT|=BIT5
#define RSclr P5OUT&=~BIT5
#define RWset P5OUT|=BIT6
#define RWclr P5OUT&=~BIT6
#define ENset P5OUT|=BIT7
#define ENclr P5OUT&=~BIT7
#define dataport P4OUT

void LCD1602_Port_Init();

void LCD1602_write_strAt(uchar x,uchar y,uchar *s);
void LCD1602_writeAt(uchar x,uchar y);
void LCD1602_write_str(uchar *s);
void LCD1602_write(uchar data);
void LCD_Init();

#endif