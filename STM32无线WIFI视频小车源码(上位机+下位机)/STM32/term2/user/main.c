/******************** (C) COPYRIGHT 2012 WildFire Team **************************
 * �ļ���  ��main.c
 * ����    ��LED��ˮ�ƣ�Ƶ�ʿɵ�����         
 * ʵ��ƽ̨��Ұ��STM32������
 * ��汾  ��ST3.0.0
 *
 * ����    ��wildfire team 
 * ��̳    ��http://www.amobbs.com/forum-1008-1.html
 * �Ա�    ��http://firestm32.taobao.com
**********************************************************************************/	
#include "includes.h"
#include "lcd_botton.h"
#include "Touch.h"
#include "GUI.h"
#include "DIALOG.h"

static OS_STK startup_task_stk[STARTUP_TASK_STK_SIZE];		  //����ջ

  
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
