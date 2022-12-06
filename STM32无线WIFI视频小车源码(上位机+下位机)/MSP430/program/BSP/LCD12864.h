#ifndef __LCD12864_H
#define __LCD12864_H

#include<msp430f149.h>

#define uchar unsigned char
#define uint unsigned int

/********port_conf************/

#define RS_CLR	P5OUT &= ~BIT5        //RS置低
#define RS_SET	P5OUT |= BIT5         //RS置高

#define RW_CLR	P5OUT &= ~BIT6       //RW置低
#define RW_SET	P5OUT |= BIT6         //RW置高

#define EN_CLR	P5OUT &= ~BIT7        //E置低
#define EN_SET	P5OUT |= BIT7         //E置高

#define PSB_CLR	P5OUT &= ~BIT0        //PSB置低，串口方式
#define PSB_SET	P5OUT |= BIT0         //PSB置高，并口方式

#define RST_CLR	P5OUT &= ~BIT1        //RST置低
#define RST_SET	P5OUT |= BIT1         //RST置高

#define DataPort      P4OUT                 //P4口为数据口
//#define DataPortout   P4DIR = 0x00

/*12864应用指令*/
#define CLEAR_SCREEN	0x01		    //清屏指令：清屏且AC值为00H
#define AC_INIT		0x02		    //将AC设置为00H。且游标移到原点位置
#define CURSE_ADD	0x06		    //设定游标移到方向及图像整体移动方向（默认游标右移，图像整体不动）
#define FUN_MODE	0x30		    //工作模式：8位基本指令集
#define DISPLAY_ON	0x0c		    //显示开,显示游标，且游标位置反白
#define DISPLAY_OFF	0x08		    //显示关
#define CURSE_DIR	0x14		    //游标向右移动:AC=AC+1
#define SET_CG_AC	0x40		    //设置AC，范围为：00H~3FH
#define SET_DD_AC	0x80


void LCD_write_data(unsigned char data);
void LCD12864_init();
void LCD_clear();
void LCD12864_writeAt(uchar x,uchar y);
void LCD12864_write_str(uchar *s);
void LCD12864_write_strAt(uchar x,uchar y,uchar *s);
//void DisplayCgrom(uchar addr,uchar *hz);
void zhuanhuan_wendu(uchar x,uchar y,unsigned int wendu);
void zhuanhuan_4(uchar x,uchar y,unsigned int shuju);

void Clear_GDRAM(void);
uchar LCD_read_data();
void GUI_dot(uchar x,uchar y,uchar cl);
void huaxian(int x1,int y1,int x2,int y2);
void gui_hline(uchar x0, uchar x1, uchar y);
void gui_rline(uchar x, uchar y0, uchar y1);
void gui_rectangle(uchar x0 , uchar y0 , uchar x1 , uchar y1);
void gui_rectangle_fill(uchar x0 , uchar y0 , uchar x1 , uchar y1);
void Line(	uchar X0,
			uchar Y0,
			uchar X1,
			uchar Y1);

#endif