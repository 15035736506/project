#include "lcd_botton.h"
#include "Touch.h"

typedef struct
{
	int x,y;
}point;
point LCD_XY,AD_XY;

typedef struct
{
	int x[5],xfb[5];
	int y[5],yfb[5];
	unsigned int a[7];
}calibration;
/*******************
其中，x和y分别表示5个点在触摸板上的坐标
xfb,yfb分别表示5个点在LCD上的坐标
a数组从a[0]到a[5]分别为A,B,C,D,E,F和一个除数，用于模拟浮点与不算
********************/
calibration cal;


//tilib算法源代码
int do_calibration(void);
{
	int j;
	float n,x,y,x2,y2,xy,z,zx,zy;
	float det,det1,det2,det3;
	float scaling = 65536.0;

	n = x = y = x2 = y2 = xy = 0;
	for(j = 0;j<5;j++)
	{
		n += 1.0;
		x += (float)cal.x[j];
		y += (float)cal.y[j];
		x2 += (float)(cal.x[j]*cal.x[j]);//X2项，具体参看算法
		y2 += (float)(cal.y[j]*cal.x[j]);//Y2项
		xy += (float)(cal.x[j]*cal.y[j]);//xy项
	}
	det = n*(x2*y2 - xy*xy) + x*(xy*y - x*y2) + y*(x*xy - y*x2);

	if(det < 0.1 && det >-0.1)
	{
		return 1;
	}

	//x
	z = zx = zy = 0;
	for(j = 0;j<5;j++)
	{
		z += (float)cal.xfb[j];
		zx += (float)(cal.xfb[j]*cal.x[j]);
		zy += (float)(cal.xfb[j]*cal.y[j]);
	}

	det1 = n*(zx*y2 - xy*zy) + z*(xy*y - x*y2) + y*(x*zy - y*zx);
	det2 = n*(x2*zy - zx*xy) + x*(zx*y - x*zy) + z*(x*xy - y*x2);
	det3 = z*(x2*y2 - xy*xy) + x*(xy*zy - zx*y2) + y*(zx*xy - zy*x2);

	cal.a[0] = (int)((det1/det)*scaling);
	cal.a[1] = (int)((det2/det)*scaling);
	cal.a[2] = (int)((det3/det)*scaling);


	//y
	z = zx = zy = 0;
	for(j = 0;j<5;j++)
	{
		z += (float)cal.yfb[j];
		zx += (float)(cal.yfb[j]*cal.x[j]);
		zy += (float)(cal.yfb[j]*cal.y[j]);
	}
	det1 = n*(zx*y2 - xy*zy) + z*(xy*y - x*y2) + y*(x*zy - y*zx);
	det2 = n*(x2*zy - zx*xy) + x*(zx*y - x*zy) + z*(x*xy - y*x2);
	det3 = z*(x2*y2 - xy*xy) + x*(xy*zy - zx*y2) + y*(zx*xy - zy*x2);

	cal.a[3] = (int)((det1/det)*scaling);
	cal.a[4] = (int)((det2/det)*scaling);
	cal.a[5] = (int)((det3/det)*scaling);

	cal.a[6] = (int)scaling;
	return 0;
}

//由触摸屏坐标转换到LCD屏坐标
point chuli(point XY)
{
	point xy;
	//Xl=Xt * A + Yt * B + C;
	//Yl=Xt * D + Yt * E + f;
	xy.x = XY.x*cal.a[0] + XY.y*cal[1] + cal[2];
	xy.y = XY.x*cal.a[3] + XY.y*cal[4] + cal[5];

	return xy;
} 

/******************************************************
* 函数名：DrawCross
* 描述  ：在LCD指定位置画十字
* 输入  : Xpos		--X方向位置
Ypos		--Y方向位置
* 输出  ：无
* 举例  ：DrawCross(100,100);
* 注意  ：触摸校正专用
*********************************************************/    
void DrawCross(uint16_t Xpos,uint16_t Ypos)
{
	LCD_WR_Data(0XF800);//red
	LCD_open_windows(Xpos-10,Ypos,20,1); 
	LCD_open_windows(Xpos,Ypos-10,1,20); 
}

void test()
{
	unsigned char n;

	DrawCross(50,50);
	cal.xfb[0] = 50;
	cal.xfb[0] = 50;
	for(n=0;n<5;n++)
	{
		while(flag==1)
		{
			AD_XY.x += Read_X();
			AD_XY.y += Read_Y();
			flag = 1;
		}
	}
	AD_XY.x = AD_XY.x/5; 
	AD_XY.y = AD_XY.y/5; 
}
