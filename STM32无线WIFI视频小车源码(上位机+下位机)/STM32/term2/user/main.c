/******************** (C) COPYRIGHT 2012 WildFire Team **************************
 * 文件名  ：main.c
 * 描述    ：LED流水灯，频率可调……         
 * 实验平台：野火STM32开发板
 * 库版本  ：ST3.0.0
 *
 * 作者    ：wildfire team 
 * 论坛    ：http://www.amobbs.com/forum-1008-1.html
 * 淘宝    ：http://firestm32.taobao.com
**********************************************************************************/	
#include "includes.h"
#include "lcd_botton.h"
#include "Touch.h"
#include "GUI.h"
#include "DIALOG.h"

static OS_STK startup_task_stk[STARTUP_TASK_STK_SIZE];		  //定义栈

  
int main(void)
{
  	BSP_Init();
/*	LCD_GPIO_Config();
	Touch_init();
	LED_GPIO_Config();

	GUI_Init();
	GUI_CURSOR_Show();		  */
//    WM_SetDesktopColor(GUI_WHITE);      /* Automacally update desktop window */
//    WM_SetCreateFlags(WM_CF_MEMDEV);
//	MainTaskbutton1();

	OSInit();
	OSTaskCreate(Task_LED,(void *)0,
	   &startup_task_stk[STARTUP_TASK_STK_SIZE-1], STARTUP_TASK_PRIO);

	OSStart();
    return 0;
 }

/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
