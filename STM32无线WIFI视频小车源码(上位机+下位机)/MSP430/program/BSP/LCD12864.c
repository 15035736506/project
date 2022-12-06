#include "LCD12864.h"
#include "delay.h"
#include<math.h>

//*************************************************************************
//			初始化IO口子程序
//*************************************************************************
void Port_init()
{

	P4SEL = 0x00;
        P4DIR = 0xFF;
        P5SEL &= ~(BIT0 + BIT1 + BIT5 + BIT6 + BIT7);
        P5DIR|= BIT0 + BIT1 + BIT5 + BIT6 + BIT7;
        RST_CLR;
        delay_ms(1);
        PSB_SET;		  //液晶并口方式
	RST_SET;		  //复位脚RST置高
        EN_CLR;
}

//***********************************************************************
//	显示屏命令写入函数
//***********************************************************************


void LCD_write_com(unsigned char com) 
{	
	RS_CLR;
	RW_CLR;
	DataPort = com;
	EN_SET;        
//	delay_ms(1);
        delay_us(20);         
	EN_CLR;
}

//***********************************************************************
//	显示屏数据写入函数
//***********************************************************************

void LCD_write_data(unsigned char data) 
{
	RS_SET;
	RW_CLR;
	DataPort = data;
	EN_SET;        
//	delay_ms(1);
        delay_us(20);         
	EN_CLR;
}

//***********************************************************************
//	显示屏清空显示
//***********************************************************************

void LCD_clear() 
{
	LCD_write_com(0x01);
	delay_ms(2);
//        delay_us(50); 
}

/*
函数名：LCD12864_writeAt
描述  ：向LCD12864的第y行第x位写入字符串，y=0代表第一行，y=1代表第二行
输入  ：x,y.坐标
输出  ：无
*/
void LCD12864_writeAt(uchar x,uchar y)
{
  switch(y)
  {
  case 0:
    LCD_write_com(0x80+x);
    break;
  case 1:
    LCD_write_com(0x90+x);
    break;  
  case 2:
    LCD_write_com(0x88+x);
    break;
  case 3:
    LCD_write_com(0x98+x);
    break; 
  default:
    break;    
  }
    delay_us(5);   
}

/*
函数名：LCD12864_write_str
描述  ：向LCD12864写入字符串
输入  ：字符串
输出  ：无
*/
void LCD12864_write_str(uchar *s)
{
  while(*s)
  {
    LCD_write_data(*s);
    s++;
    delay_us(5);     
  }
}

/*
函数名：LCD12864_write_strAt
描述  ：向LCD12864的第y行第x位写入字符串，y=0代表第一行，y=1代表第二行
输入  ：x,y.坐标，字符串
输出  ：无
*/
void LCD12864_write_strAt(uchar x,uchar y,uchar *s)
{
  switch(y)
  {
  case 0:
    LCD_write_com(0x80+x);
    break;
  case 1:
    LCD_write_com(0x90+x);
    break;  
  case 2:
    LCD_write_com(0x88+x);
    break;
  case 3:
    LCD_write_com(0x98+x);
    break;
  default:
    break;
  }
    delay_us(5);   
  while(*s)
  {
    LCD_write_data(*s);
    s++;
//    delay_ms(1); 
    delay_us(5);
  }
}

//***********************************************************************
//函数名称：DisplayCgrom(uchar hz)显示CGROM里的汉字
//***********************************************************************
/*void DisplayCgrom(uchar addr,uchar *hz)
{
	LCD_write_com(addr);
        delay_ms(5);
	while(*hz != '\0')
	{
		LCD_write_data(*hz);
		hz++;
                delay_ms(5);
	}

}*/

void LCD12864_init() 
{
  Port_init();
  delay_ms(2);
  LCD_write_com(FUN_MODE);	//工作模式：8位基本指令集
  delay_ms(2);
  LCD_write_com(AC_INIT);	//将AC设置为00H。且游标移到原点位置		//显示模式设置
  delay_ms(2);
  LCD_write_com(DISPLAY_ON);	//显示开
  delay_ms(2);
  LCD_write_com(CLEAR_SCREEN);	//清屏
  delay_ms(2);
  LCD_write_com(CURSE_ADD);//设定游标移到方向及图像整体移动方向（默认游标右移，图像整体不动）
  delay_ms(3);  
}

void zhuanhuan_4(uchar x,uchar y,unsigned int shuju)
{
  unsigned char qian,bai,shi,ge;
  qian = shuju/1000;
  bai = (shuju%1000)/100;
  shi = (shuju%1000)%100/10;
  ge = shuju%10;
  LCD12864_writeAt(x,y);
  delay_us(10);
  LCD_write_data(qian+0x30);
  delay_us(20);
  LCD_write_data(bai+0x30);
  delay_us(20);
  LCD_write_data(shi+0x30);
  delay_us(20);
  LCD_write_data(ge+0x30);
  delay_us(20);
}

//被转换的温度变量是事先扩大了十倍，显示时带一位小数
void zhuanhuan_wendu(uchar x,uchar y,unsigned int wendu)
{
  unsigned char bai,shi,ge;
  bai = wendu/100;
  shi = (wendu%1000)%100/10;
  ge = wendu%10;
  LCD12864_writeAt(x,y);
  delay_us(10);
  LCD_write_data(bai+0x30);
  delay_us(20);
  LCD_write_data(shi+0x30);
  delay_us(20);
  LCD_write_data('.');
  delay_us(20);  
  LCD_write_data(ge+0x30);
  delay_us(20);
}

/*******************************************
功    能：清除液晶GDRAM内部的随机数据
********************************************/
void Clear_GDRAM(void)
{
    uchar i,j,k;
    LCD_write_com(0x34);
    delay_us(40);

	i = 0x80;
	for(j = 0;j < 32;j++)
	{
          LCD_write_com(i++);
          delay_us(40);
          LCD_write_com(0x80);
          delay_us(40);
          for(k = 0;k < 16;k++)
          {
             LCD_write_data(0x00);
            delay_us(30);
          }
	}
	i = 0x80;
 	for(j = 0;j < 32;j++)
	{
 	        LCD_write_com(i++);
                delay_us(40);
                LCD_write_com(0x88);
                delay_us(40); 	
  		for(k = 0;k < 16;k++)
  		{
   		    LCD_write_data(0x00);
                    delay_us(30);
   		}
	}
    LCD_write_com(0x36);
    delay_us(40);
    LCD_write_com(0x30);
    delay_us(40);
}

//***********************************************************************
//	读数据
//***********************************************************************
uchar LCD_read_data() 
{	
        uchar DataPort1;
        P4DIR = 0X00;
	RS_SET;
	RW_SET;
	EN_SET;
	DataPort1 = P4IN;
	delay_us(20);
	EN_CLR;
        P4DIR = 0XFF;
        return(DataPort1);
}

//***********************************************************************
//	画点
//***********************************************************************
void GUI_dot(uchar x,uchar y,uchar cl)
{
   uchar row,xlabel,xlabel_bit;
   uchar read_h,read_l;
   LCD_write_com(0x34);
   LCD_write_com(0x36);
   delay_us(50);

   xlabel= x>>4;
   xlabel_bit= x & 0x0f;

   if(y<32)
   {
      row=y;
    }
   else
   {
      row=y-32;
      xlabel += 8;
   }
   LCD_write_com(0x80+row);
   delay_us(50);
   LCD_write_com(0x80+xlabel);
   delay_us(50);

   LCD_read_data();
   delay_us(50);
   read_h=LCD_read_data();
   delay_us(50);
   read_l=LCD_read_data();
   delay_us(50);

   LCD_write_com(0x80+row);
   delay_us(50);
   LCD_write_com(0x80+xlabel);
   delay_us(50);

   if(cl==1)
   {
     if(xlabel_bit<8)
     {
         read_h|=((0x01<<(7-xlabel_bit)));
         LCD_write_data(read_h);
         delay_us(50);
         LCD_write_data(read_l);
         delay_us(50);
     }
     else
     {
         read_l|=((0x01<<(15-xlabel_bit)));
         LCD_write_data(read_h);
         delay_us(50);
         LCD_write_data(read_l);
         delay_us(50);
     }
   }
   else
   {
      if(xlabel_bit<8)
     {
  
         read_h&=(~(0x01<<(7-xlabel_bit)));
         LCD_write_data(read_h);
         delay_us(50);
         LCD_write_data(read_l);
         delay_us(50);
     }
     else
     {
         read_l&=(~(0x01<<(15-xlabel_bit)));
         LCD_write_data(read_h);
         delay_us(50);
         LCD_write_data(read_l);
         delay_us(50);
     }
   }
   LCD_write_com(0x36);
   delay_us(50);
   LCD_write_com(0x30);
   delay_us(50);
}

//***********************************************************************
//	任意两点间直线
//***********************************************************************
void huaxian(int x1,int y1,int x2,int y2)
{
   int dx = x2-x1;
   int dy = y2-y1;
   int ux = ((dx>0)<<1)-1;
   int uy = ((dy>0)<<1)-1;
   int x=x1,y=y1,eps;
   eps=0;dx=(int)fabs(dx);dy=(int)fabs(dy);
   if(dx>dy)
   {
      for(x=x1;x!=x2;x+=ux)
      {
        GUI_dot(x,y,1);
        eps+=dy;
         if((eps<<1)>=dx)
         {
            y+=uy;
            eps-=dx;
         }
      }
   }
   else
   {
     for(y=y1;y!=y2;y+=uy)
      {
        GUI_dot(x,y,1);
        eps+=dx;
         if((eps<<1)>=dy)
         {
            x+=ux;
            eps-=dy;
         }
      }
   }
}

void Line(	uchar X0,
			uchar Y0,
			uchar X1,
			uchar Y1)
	//		unsigned int color)
{
	int dx = X1 - X0;
	int dy = Y1 - Y0;
	int P  = 2 * dy - dx;
	int dobDy = 2 * dy;
	int dobD = 2 * (dy - dx);
	int PointX = 0,PointY = 0;
	int incx = 0,incy = 0;
	int distance = 0,xerr = 0,yerr = 0;
	unsigned int i = 0;

	if(dx == 0)		//k=1斜率为1
	{
		PointX = X0;
		if(Y0 < Y1)
		{
			PointY = Y0;
		}
		else
		{
			PointY = Y1;
		}
		for(i = 0;i <= ((Y0<Y1) ? (Y1-Y0) : (Y0-Y1));i++)
		{

			//	Put_pixel(PointX,PointY,color);
                  GUI_dot(PointX,PointY,1);
			PointY++;
		}
		return;
	}
	if(dy == 0)		//k=0斜率为0
	{
		PointY = Y0;
		if(X0 < X1)
		{
			PointX = X0;
		}
		else
		{
			PointX = X1;
		}
		for(i = 0;i <= ((X0<X1) ? (X1-X0) : (X0-X1));i++)
		{

				//Put_pixel(PointX,PointY,color);
                  GUI_dot(PointX,PointY,1);
			PointX++;
		}
		return;
	}

	if(dx > 0)
		incx = 1;
	else if(dx == 0)
		incx = 0;
	else
		incx = -1;

	if(dy > 0)
		incy = 1;
	else if(dy == 0)
		incy = 0;
	else
		incy = -1;

    dx = ((X0>X1) ? (X0-X1) : (X1-X0));
    dy = ((Y0>Y1) ? (Y0-Y1) : (Y1-Y0));

    if(dx>dy) distance=dx;
    else distance=dy;

    PointX = X0;
    PointY = Y0;
    for(i=0;i<=distance+1;i++)
    {

    		//Put_pixel(PointX,PointY,color);
      GUI_dot(PointX,PointY,1);
        xerr+=dx;
        yerr+=dy;
        if(xerr>distance)
        {
            xerr-=distance;
            PointX+=incx;
        }
        if(yerr>distance)
        {
            yerr-=distance;
            PointY+=incy;
        }
    }
}

//************画水平线函数**********************************//
//x0、x1为起始点和终点的水平坐标，y为垂直坐标***************//
//**********************************************************//
void gui_hline(uchar x0, uchar x1, uchar y)
{
	uchar bak;//用于对两个数互换的中间变量，使x1为大值
	if(x0 > x1)
	{
		bak = x1;
		x1 = x0;
		x0 = bak;
	}

	do
	{
		GUI_dot(x0,y,1);//从左到右逐点显示
		x0 ++;	
	}while(x1 >= x0);
}
//***********画竖直线函数***********************************//
//x为起始点和终点的水平坐标，y0、y1为垂直坐标***************//
//**********************************************************//
void gui_rline(uchar x, uchar y0, uchar y1)
{
	uchar bak;//用于对两个数互换的中间变量，使y1为大值
	if(y0 > y1)
	{
		bak = y1;
		y1 = y0;
		y0 = bak;
	}

	do
	{
		GUI_dot(x ,y0,1);//从上到下逐点显示
		y0 ++;	
	}while(y1 >= y0);
}

//***********画矩形函数*************************************//
//x0、y0为矩形左上角坐标值，x1、y1为矩形右下角坐标值********//
//**********************************************************//
void gui_rectangle(uchar x0 , uchar y0 , uchar x1 , uchar y1)
{
	gui_hline(x0 , x1 , y0);
	gui_rline(x0 , y0 , y1);
	gui_rline(x1 , y0 , y1);
	gui_hline(x0 , x1 , y1);
}


//****************画填充矩形函数****************************//
//x0、y0为矩形左上角坐标值，x1、y1为矩形右下角坐标值********//
//**********************************************************//
void gui_rectangle_fill(uchar x0 , uchar y0 , uchar x1 , uchar y1)
{
	uchar i;//转换数据的中间变量,使x1、y1大
	if(x0 > x1)
	{
		i = x0;
		x0 = x1;
		x1 = i;
	}
	if(y0 > y1)
	{
		i = y0;
		y0 = y1;
		y1 = i;
	}
	//***判断是否是直线***/
       if(y0 == y1)//画水平线
	{
		gui_hline(x0 , x1 , y0);
		return;
	}
	if(x0 == x1)//画竖直线
	{
		gui_rline(x0 , y0 , y1);
		return;
	}

	while(y0 <= y1)//画填充矩形
	{
		gui_hline(x0 , x1 , y0);
		y0 ++;
	}
}


//*******************画正方形函数*************************//
//x0、y0为正方形左上角坐标，with正方形边长****************//
//********************************************************//
void gui_square(uchar x0 , uchar y0 , uchar with)
{
	if(with == 0)
		return;
	if((x0 + with) > 127)//横轴超出液晶边界
		return;
	if((y0 + with) > 63)
		return;
	gui_rectangle(x0 , y0 , x0 + with , y0 + with);
}

//****************画填充正方形函数*************************//
//x0、y0为正方形左上角坐标，with正方形边长*****************//
//*********************************************************//
void gui_square_fill(uchar x0 , uchar y0 , uchar with)
{
	if(with == 0)
		return;
	if((x0 + with) > 127)//横轴超出液晶边界
		return;
	if((y0 + with) > 63)
		return;
	gui_rectangle_fill(x0 , y0 , x0 + with , y0 + with);	
}	


//***************************************************************************//
//*******************画指定宽度的任意两点之间的直线**************************//
//参数说明：x0、y0为起始点坐标，x1、y1为终点坐标，with为线宽*****************//
//***************************************************************************//

void gui_linewith(uchar x0 , uchar y0 , uchar x1 , uchar y1 , uchar with)
{
	signed char	dx;						// 直线x轴差值变量
	signed char	dy;          			// 直线y轴差值变量
	char	dx_sym;					// x轴增长方向，为-1时减值方向，为1时增值方向
	char	dy_sym;					// y轴增长方向，为-1时减值方向，为1时增值方向
	char	dx_x2;					// dx*2值变量，用于加快运算速度
	char	dy_x2;					// dy*2值变量，用于加快运算速度
	signed char	di;						// 决策变量

	signed char   wx, wy;					// 线宽变量
	signed char   draw_a, draw_b;

	// 参数过滤
	if(with==0) return;
	if(with>50) with = 50;

	dx = x1-x0;								// 求取两点之间的差值
	dy = y1-y0;

	wx = with/2;
	wy = with-wx-1;

	//判断增长方向，或是否为水平线、垂直线、点
	if(dx>0)								// 判断x轴方向
	{
		dx_sym = 1;							// dx>0，设置dx_sym=1
	}
	else
	{
		if(dx<0)
		{
			dx_sym = -1;					// dx<0，设置dx_sym=-1
		}
		else
		{
			//dx==0，画垂直线，或一点
			wx = x0-wx;
			if(wx<0) wx = 0;
			wy = x0+wy;

			while(1)
			{
				x0 = wx;
				gui_rline(x0, y0, y1);
				if(wx>=wy) break;
				wx++;
			}
			return;
		}
	}

	if(dy>0)								// 判断y轴方向
	{
		dy_sym = 1;							// dy>0，设置dy_sym=1
	}
	else
	{
		if(dy<0)
		{
			dy_sym = -1;					// dy<0，设置dy_sym=-1
		}
		else
		{
			//dy==0，画水平线，或一点
			wx = y0-wx;
			if(wx<0) wx = 0;
			wy = y0+wy;
			while(1)
			{
				y0 = wx;
				gui_hline(x0, x1, y1);
				if(wx>=wy) break;
				wx++;
			}
			return;
		}
	}

	// 将dx、dy取绝对值
	dx = dx_sym * dx;
	dy = dy_sym * dy;

	//计算2倍的dx及dy值
	dx_x2 = dx*2;
	dy_x2 = dy*2;

	//使用Bresenham法进行画直线
	if(dx>=dy)								// 对于dx>=dy，则使用x轴为基准
	{
		di = dy_x2 - dx;
		while(x0!=x1)
		{
			//x轴向增长，则宽度在y方向，即画垂直线
			draw_a = y0-wx;
			if(draw_a<0) draw_a = 0;
			draw_b = y0+wy;
			gui_rline(x0, draw_a, draw_b);
			x0 += dx_sym;				
			if(di<0)
			{
				di += dy_x2;				// 计算出下一步的决策值
			}
			else
			{
				di += dy_x2 - dx_x2;
				y0 += dy_sym;
			}
		}
		draw_a = y0-wx;
		if(draw_a<0) draw_a = 0;
		draw_b = y0+wy;
		gui_rline(x0, draw_a, draw_b);
	}
	else									// 对于dx<dy，则使用y轴为基准
	{
		di = dx_x2 - dy;
		while(y0!=y1)
		{
			//y轴向增长，则宽度在x方向，即画水平线
			draw_a = x0-wx;
			if(draw_a<0) draw_a = 0;
			draw_b = x0+wy;
			gui_hline(draw_a, y0, draw_b);

			y0 += dy_sym;
			if(di<0)
			{
				di += dx_x2;
			}
			else
			{
				di += dx_x2 - dy_x2;
				x0 += dx_sym;
			}
		}
		draw_a = x0-wx;
		if(draw_a<0) draw_a = 0;
		draw_b = x0+wy;
		gui_hline(draw_a, y0, draw_b);
	}

}

