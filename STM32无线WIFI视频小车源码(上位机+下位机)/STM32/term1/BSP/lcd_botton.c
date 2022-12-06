/******************** (C) COPYRIGHT 2012 WildFire Team **************************
 * 文件名  ：lcdbotton.c
 * 描述    ：lcd底层应用函数库         
 * 实验平台：野火STM32开发板
 * 硬件连接：-----------------------
 *          |见函数LCD_GPIO_Config()|
 *           -----------------------
 * 库版本  ：ST3.5.0
 *
 * 作者    ：wildfire team 
 * 论坛    ：http://www.amobbs.com/forum-1008-1.html
 * 淘宝    ：http://firestm32.taobao.com
******************************************************************************/
#include "lcd_botton.h"
#include "core_cm3.h"
//#include "sysTick.h"

void Delay(__IO u32 nCount);



/*横竖屏标志位  0：GRAM指针扫描方向为横屏模式 1：GRAM指针扫描方向为竖屏模式*/
volatile u8 display_direction = 0;	 /*该标志位的作用方便用户识别当前究竟是处于哪种模式*/

#define LCD_ILI9341_CMD(index)       LCD_WR_REG(index)
#define LCD_ILI9341_Parameter(val)	 LCD_WR_Data(val)
//#define Delay_us(time)               delay_us(time)   	/*延时定义*/								  
#define DEBUG_DELAY()    //Delay_us(500) 	 /*调节延时，如果不延时则直接注释掉即可*/

/*******************************************
 * 函数名：LCD_GPIO_Config
 * 描述  ：根据FSMC配置LCD的I/O
 * 输入  : 无 
 * 输出  ：无
 * 举例  ：无
 * 注意  ：无
*********************************************/  
void LCD_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /* 使能FSMC时钟*/
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
    
    /* 使能FSMC对应相应管脚时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE , ENABLE);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    /* 配置LCD背光控制管脚*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;		
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    /* 配置LCD复位控制管脚*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ; 	 
    GPIO_Init(GPIOE, &GPIO_InitStructure);  		   
    
    /* 配置FSMC相对应的数据线,FSMC-D0~D15: PD 14 15 0 1,PE 7 8 9 10 11 12 13 14 15,PD 8 9 10*/	
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF_PP;
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 | 
                                  GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | 
                                  GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
                                  GPIO_Pin_15;
    GPIO_Init(GPIOE, &GPIO_InitStructure); 
    
    /* 配置FSMC相对应的控制线
	 * PD4-FSMC_NOE  :LCD-RD
   * PD5-FSMC_NWE  :LCD-WR
	 * PD7-FSMC_NE1  :LCD-CS
   * PD11-FSMC_A16 :LCD-DC
	 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);  
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);  
    
    /* tft control gpio init */	 
//    GPIO_SetBits(GPIOD, GPIO_Pin_13);		 // 背光打开 ----for 旧lcd 板
	GPIO_ResetBits(GPIOD, GPIO_Pin_13);		 // 修改---for新LCD板
   
	GPIO_SetBits(GPIOE, GPIO_Pin_1);		 
    GPIO_ResetBits(GPIOD, GPIO_Pin_3);
    GPIO_SetBits(GPIOD, GPIO_Pin_6);
	 
}



/*******************************************
 * 函数名：LCD_FSMC_Config
 * 描述  ：LCD  FSMC 模式配置
 * 输入  : 无 
 * 输出  ：无
 * 举例  ：无
 * 注意  ：无
*********************************************/ 
void LCD_FSMC_Config(void)
{
    FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef  p; 
    
    
    p.FSMC_AddressSetupTime = 0x02;	 //地址建立时间
    p.FSMC_AddressHoldTime = 0x00;	 //地址保持时间
    p.FSMC_DataSetupTime = 0x05;		 //数据建立时间
    p.FSMC_BusTurnAroundDuration = 0x00;   //总线恢复时间
    p.FSMC_CLKDivision = 0x00;			  //时钟分频
    p.FSMC_DataLatency = 0x00;			  //数据保持时间
    p.FSMC_AccessMode = FSMC_AccessMode_B;	 //在地址数\据线不复用的情况下，ABCD模式的区别不大
											 //本成员配置只有使用扩展模式才有效
											
    
    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;			 //NOR FLASH的BANK1
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;	  //数据线与地址线不复用
    FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;				  //存储器类型NOR FLASH
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;		  //数据宽度为16位
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;	  //使用异步写模式，禁止突发模式
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;  //本成员的配置只在突发模式下有效，等待信号极性为低
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;				 //禁止非对齐突发模式
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;   //本成员配置仅在突发模式下有效。NWAIT信号在什么时期产生
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;			  //本成员的配置只在突发模式下有效，禁用NWAIT信号
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;			  //禁止突发写操作
   	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;		   //写使能
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;		  //禁止扩展模式，扩展模式可以使用独立的读、写模式
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;						   //配置读写时序
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p; 							   //配置写时序
   
    
    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 
    
    /* 使能 FSMC Bank1_SRAM Bank */
    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);  
}





/**********************************
 * 函数名：LCD_Rst
 * 描述  ：LCD 软件复位
 * 输入  : 无 
 * 输出  ：无
 * 举例  ：无
 * 注意  ：无
*************************************/
void LCD_Rst(void)
{	
	unsigned long i;		
	GPIO_ResetBits(GPIOE, GPIO_Pin_1);	 //低电平复位
//    Delay_us(10000);
	for(i=720000;i>0;i--);				   
    GPIO_SetBits(GPIOE, GPIO_Pin_1 );		 	 
//    Delay_us(12000);
		for(i=720000;i>0;i--);	 	
}




/**********************************
 * 函数名：LCD_RD_data
 * 描述  ：读 ILI9341 RAM 数据
 * 输入  : 无 
 * 输出  ：读取的数据,16bit *
 * 举例  ：无
 * 注意  ：无
*************************************/
u16 LCD_RD_data(void)
{
	u16 R=0, G=0, B=0 ;
	LCD_ILI9341_CMD(0x2e);	           /*LCD GRAM READ COMMAND--DATASHEET PAGE116*/
	R =(*(__IO u16 *) (Bank1_LCD_D));  /*FIRST READ OUT DUMMY DATA*/
	R =(*(__IO u16 *) (Bank1_LCD_D));  /*READ OUT RED DATA  */
	B =(*(__IO u16 *) (Bank1_LCD_D));  /*READ OUT BLACK DATA*/
	G =(*(__IO u16 *) (Bank1_LCD_D));  /*READ OUT GREEN DATA*/
	
    return (((R>>11)<<11) | ((G>>10)<<5) | (B>>11)) ;

}




/**********************************
 * 函数名：Set_direction
 * 描述  ：设置ILI9341GRAM指针扫描方向
 * 输入  : 0： 横向扫描
 		   1： 纵向扫描
 * 输出  ：无
 * 举例  ：无
 * 注意  ：无
*************************************/
void Set_direction(u8 option)
{

    switch(option)
    {
    case 0:
	{	    /*横屏*/
        LCD_ILI9341_CMD(0x36); 
		LCD_ILI9341_Parameter(0x68);    //横屏
		LCD_ILI9341_CMD(0X2A); 
		LCD_ILI9341_Parameter(0x00);	//start 
		LCD_ILI9341_Parameter(0x00);
		LCD_ILI9341_Parameter(0x01);	//end
		LCD_ILI9341_Parameter(0x3F);
		
		LCD_ILI9341_CMD(0X2B); 
		LCD_ILI9341_Parameter(0x00);   //start
		LCD_ILI9341_Parameter(0x00);
		LCD_ILI9341_Parameter(0x00);   //end
		LCD_ILI9341_Parameter(0xEF);
        display_direction = 0;		
        }break;
    case 1:
        {		/*竖屏*/
        LCD_ILI9341_CMD(0x36); 
	    LCD_ILI9341_Parameter(0x48);	//竖屏 
	    LCD_ILI9341_CMD(0X2A); 
		LCD_ILI9341_Parameter(0x00);
		LCD_ILI9341_Parameter(0x00);
		LCD_ILI9341_Parameter(0x00);
		LCD_ILI9341_Parameter(0xEF);	
		
		LCD_ILI9341_CMD(0X2B); 
		LCD_ILI9341_Parameter(0x00);
		LCD_ILI9341_Parameter(0x00);
		LCD_ILI9341_Parameter(0x01);
		LCD_ILI9341_Parameter(0x3F);
        display_direction = 1;
        }break;
        
    }

}




/**********************************
 * 函数名：LCD_open_windows
 * 描述  ：开窗(以x,y为坐标起点，长为len,高为wid)
 * 输入  : -x    窗户起点
           -y	   窗户起点
           -len  窗户长 
           -wid 窗户宽
 * 输出  ：无
 * 举例  ：无
 * 注意  ：无
*************************************/
void LCD_open_windows(u16 x,u16 y,u16 len,u16 wid)
{                    

    if(display_direction == 0)		/*如果是横屏选项*/
    {

		LCD_ILI9341_CMD(0X2A); 
		LCD_ILI9341_Parameter(x>>8);	//start 起始位置的高8位
		LCD_ILI9341_Parameter(x-((x>>8)<<8));  //起始位置的低8位
		LCD_ILI9341_Parameter((x+len-1)>>8);	//end 结束位置的高8位
		LCD_ILI9341_Parameter((x+len-1)-(((x+len-1)>>8)<<8));  //结束位置的低8位
		
		LCD_ILI9341_CMD(0X2B); 
		LCD_ILI9341_Parameter(y>>8);   //start
		LCD_ILI9341_Parameter(y-((y>>8)<<8));
		LCD_ILI9341_Parameter((y+wid-1)>>8);   //end
		LCD_ILI9341_Parameter((y+wid-1)-(((y+wid-1)>>8)<<8));
     
    }
    else
    {
		LCD_ILI9341_CMD(0X2B); 
		LCD_ILI9341_Parameter(x>>8);
		LCD_ILI9341_Parameter(x-((x>>8)<<8));
		LCD_ILI9341_Parameter((x+len-1)>>8);
		LCD_ILI9341_Parameter((x+len-1)-(((x+len-1)>>8)<<8));

	    LCD_ILI9341_CMD(0X2A); 
		LCD_ILI9341_Parameter(y>>8);
		LCD_ILI9341_Parameter(y-((y>>8)<<8));
		LCD_ILI9341_Parameter((y+wid-1)>>8);
		LCD_ILI9341_Parameter((y+wid-1)-(((y+wid-1)>>8)<<8));	
        
    }

      LCD_ILI9341_CMD(0x2c);     
}



void Lcd_data_start(void)
{
         LCD_WR_REG(0x2C);//开始写数据到GRAM
}

void LCD_Point(u16 x,u16 y,u16 POINT_COLOR)
{
    LCD_open_windows(x,y,1,1);   
    LCD_WR_Data(POINT_COLOR);	
}


/**********************************
 * 函数名：Lcd_init_conf
 * 描述  ：ILI9341 LCD寄存器初始配置
 * 输入  : 无
 * 输出  ：无
 * 举例  ：无
 * 注意  ：无
*************************************/
void Lcd_init_conf(void)
{
	unsigned long i;
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0xCF);
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x81);
	LCD_ILI9341_Parameter(0x30);
	
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0xED);
	LCD_ILI9341_Parameter(0x64);
	LCD_ILI9341_Parameter(0x03);
	LCD_ILI9341_Parameter(0x12);
	LCD_ILI9341_Parameter(0x81);
	
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0xE8);
	LCD_ILI9341_Parameter(0x85);
	LCD_ILI9341_Parameter(0x10);
	LCD_ILI9341_Parameter(0x78);
	
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0xCB);
	LCD_ILI9341_Parameter(0x39);
	LCD_ILI9341_Parameter(0x2C);
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x34);
	LCD_ILI9341_Parameter(0x02);
	
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0xF7);
	LCD_ILI9341_Parameter(0x20);
	
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0xEA);
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x00);
	
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0xB1);
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x1B);
	
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0xB6);
	LCD_ILI9341_Parameter(0x0A);
	LCD_ILI9341_Parameter(0xA2);
	
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0xC0);
	LCD_ILI9341_Parameter(0x35);
	
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0xC1);
	LCD_ILI9341_Parameter(0x11);
	
	LCD_ILI9341_CMD(0xC5);
	LCD_ILI9341_Parameter(0x45);
	LCD_ILI9341_Parameter(0x45);
	
	LCD_ILI9341_CMD(0xC7);
	LCD_ILI9341_Parameter(0xA2);
	
	LCD_ILI9341_CMD(0xF2);
	LCD_ILI9341_Parameter(0x00);
	
	LCD_ILI9341_CMD(0x26);
	LCD_ILI9341_Parameter(0x01);
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0xE0); //Set Gamma
	LCD_ILI9341_Parameter(0x0F);
	LCD_ILI9341_Parameter(0x26);
	LCD_ILI9341_Parameter(0x24);
	LCD_ILI9341_Parameter(0x0B);
	LCD_ILI9341_Parameter(0x0E);
	LCD_ILI9341_Parameter(0x09);
	LCD_ILI9341_Parameter(0x54);
	LCD_ILI9341_Parameter(0xA8);
	LCD_ILI9341_Parameter(0x46);
	LCD_ILI9341_Parameter(0x0C);
	LCD_ILI9341_Parameter(0x17);
	LCD_ILI9341_Parameter(0x09);
	LCD_ILI9341_Parameter(0x0F);
	LCD_ILI9341_Parameter(0x07);
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_CMD(0XE1); //Set Gamma
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x19);
	LCD_ILI9341_Parameter(0x1B);
	LCD_ILI9341_Parameter(0x04);
	LCD_ILI9341_Parameter(0x10);
	LCD_ILI9341_Parameter(0x07);
	LCD_ILI9341_Parameter(0x2A);
	LCD_ILI9341_Parameter(0x47);
	LCD_ILI9341_Parameter(0x39);
	LCD_ILI9341_Parameter(0x03);
	LCD_ILI9341_Parameter(0x06);
	LCD_ILI9341_Parameter(0x06);
	LCD_ILI9341_Parameter(0x30);
	LCD_ILI9341_Parameter(0x38);
	LCD_ILI9341_Parameter(0x0F);
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0x36); 
	LCD_ILI9341_Parameter(0x68);    /*横屏  左上角到(起点)到右下角(终点)扫描方式*/
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0X2A); 
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x01);
	LCD_ILI9341_Parameter(0x3F);
	
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0X2B); 
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0xEF);

	DEBUG_DELAY();
	LCD_ILI9341_CMD(0x3a); // Memory Access Control
	LCD_ILI9341_Parameter(0x55);
	LCD_ILI9341_CMD(0x11); //Exit Sleep
//	Delay_us(1200);
	for(i=720000;i>0;i--);
	LCD_ILI9341_CMD(0x29); //display on
    
	Set_direction(0);	  //传入0-->初始化默认为横屏方式
	LCD_ILI9341_CMD(0x2c); 
}

/*****************************************
 * 函数名：LCD_Init
 * 描述  ：LCD 控制 I/O 初始化
 *         LCD FSMC 初始化
 *         LCD 控制器 HX8347 初始化 
 * 输入  : 无
 * 输出  ：无
 * 举例  ：无
 * 注意  ：无
******************************************/    
void LCD_DriverInit(void)
{
    unsigned long i; 

    LCD_GPIO_Config();	  	//初始化使用到的GPIO
    LCD_FSMC_Config();		//初始化FSMC模式
    LCD_Rst();				//复位LCD液晶屏
	Lcd_init_conf();	    //写入命令参数，对液晶屏进行基本的初始化配置
    Lcd_data_start();   	//发送写GRAM命令
    for(i=0; i<(320*240); i++)		 		
    {
        LCD_WR_Data(GBLUE); //发送颜色数据，使屏幕初始化为GBLUE颜色					    
    }
}
/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
