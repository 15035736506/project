#ifndef __LCD12864_H
#define __LCD12864_H

#include<msp430f149.h>

#define uchar unsigned char
#define uint unsigned int

/********port_conf************/

#define RS_CLR	P5OUT &= ~BIT5        //RS�õ�
#define RS_SET	P5OUT |= BIT5         //RS�ø�

#define RW_CLR	P5OUT &= ~BIT6       //RW�õ�
#define RW_SET	P5OUT |= BIT6         //RW�ø�

#define EN_CLR	P5OUT &= ~BIT7        //E�õ�
#define EN_SET	P5OUT |= BIT7         //E�ø�

#define PSB_CLR	P5OUT &= ~BIT0        //PSB�õͣ����ڷ�ʽ
#define PSB_SET	P5OUT |= BIT0         //PSB�øߣ����ڷ�ʽ

#define RST_CLR	P5OUT &= ~BIT1        //RST�õ�
#define RST_SET	P5OUT |= BIT1         //RST�ø�

#define DataPort      P4OUT                 //P4��Ϊ���ݿ�
//#define DataPortout   P4DIR = 0x00

/*12864Ӧ��ָ��*/
#define CLEAR_SCREEN	0x01		    //����ָ�������ACֵΪ00H
#define AC_INIT		0x02		    //��AC����Ϊ00H�����α��Ƶ�ԭ��λ��
#define CURSE_ADD	0x06		    //�趨�α��Ƶ�����ͼ�������ƶ�����Ĭ���α����ƣ�ͼ�����岻����
#define FUN_MODE	0x30		    //����ģʽ��8λ����ָ�
#define DISPLAY_ON	0x0c		    //��ʾ��,��ʾ�α꣬���α�λ�÷���
#define DISPLAY_OFF	0x08		    //��ʾ��
#define CURSE_DIR	0x14		    //�α������ƶ�:AC=AC+1
#define SET_CG_AC	0x40		    //����AC����ΧΪ��00H~3FH
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