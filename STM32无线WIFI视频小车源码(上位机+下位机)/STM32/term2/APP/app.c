#include "includes.h"
#include "WM.h"
#include "DIALOG.h"

#include "lcd_botton.h"
#include "Touch.h"
#include "GUI.h"
//#include "DIALOG.h"


OS_STK task_led2_stk[TASK_LED2_STK_SIZE];
OS_STK task_led3_stk[TASK_LED3_STK_SIZE];

extern void Main1(void);
extern void MainTaskbutton1(void);
extern void main_APP1();
extern void MainTask_MultiEdit(void);
extern void GUIDEMO_Dialog(void); 
extern void _main_4();

void Task_LED(void *p_arg)
{

    (void)p_arg;                		// 'p_arg' 并没有用到，防止编译器提示警告
//	Main1();
//	main_APP1();


	LED_GPIO_Config();
	LCD_GPIO_Config();
	Touch_init();

   	OSTaskCreate(Task_LED2,(void *)0,
	   &task_led2_stk[TASK_LED2_STK_SIZE-1], TASK_LED2_PRIO);
	OSTaskCreate(Task_LED3,(void *)0,
	   &task_led3_stk[TASK_LED3_STK_SIZE-1], TASK_LED3_PRIO);
//	   	MainTaskbutton1();
//	main_APP1();

    while (1)
    {
        LED1( ON );
        OSTimeDlyHMSM(0, 0,0,100);
        LED1( OFF);
		OSTimeDlyHMSM(0, 0,0,100);
//MainTask_MultiEdit();
//GUIDEMO_Dialog();
//Main1();
_main_4();		
 //       WM_Exec();
/*        LED2( ON );
        OSTimeDlyHMSM(0, 0, 0,500);
        LED2( OFF);
        
        LED3( ON );
        OSTimeDlyHMSM(0, 0,0,500);
        LED3( OFF);			*/
    }
}

void Task_LED2(void *p_arg)
{
	(void)p_arg;

	while(1)
	{
		LED2(ON);
		OSTimeDlyHMSM(0,0,0,30);
		LED2(OFF);
		GUI_TOUCH_Exec();//获取坐标
//		OSTimeDlyHMSM(0,0,0,30);
	}
}

void Task_LED3(void *p_arg)
{
	(void)p_arg;
//	  	BSP_Init();
//	LED_GPIO_Config();

	GUI_Init();
	GUI_CURSOR_Show();
	
//	Main1();
	while(1)
	{
		LED3(ON);
		OSTimeDlyHMSM(0,0,0,800);
		LED3(OFF);
		OSTimeDlyHMSM(0,0,0,800);
						
	}
}
