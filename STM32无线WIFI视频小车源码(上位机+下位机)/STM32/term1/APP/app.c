#include "APP.h"
#include "includes.h"
#include "Touch.h"
#include "lcd_botton.h"

/*************************************************任务堆栈声明********************************************/
OS_STK  TASK_Touch_STK[Touch_STK_SIZE];
OS_STK  TASK_Menu_STK[Menu_STK_SIZE];
OS_STK  TASK_LED_STK[LED_STK_SIZE];
OS_STK  TASK_Setting_STK[Setting_STK_SIZE];
OS_STK  TASK_FileBrowser_STK[FileBrowser_STK_SIZE];
OS_STK  TASK_MusicPlayer_STK[MusicPlayer_STK_SIZE];
OS_STK  TASK_PhotoBrowser_STK[PhotoBrowser_STK_SIZE];
OS_STK  TASK_TextBrowser_STK[TextBrowser_STK_SIZE];
OS_STK  TASK_ComPort_STK[ComPort_STK_SIZE];
// OS_STK  TASK_STAT_STK[OS_TASK_IDLE_STK_SIZE]; //统计任务堆栈容量

/******************************************************** 
** Function name:      TaskStart
** Descriptions:       起始任务，用于建立最初的系统必备任务
** input parameters:   void *pdata
** output parameters:  无
** Returned value:     无
********************************************************/
void TaskStart(void * pdata)
{
	pdata = pdata;

//	LED_GPIO_Config();
	LCD_GPIO_Config();
	Touch_init();

	OSTaskCreate(Task_LED_DEMO, (void * )0, (OS_STK *)&TASK_LED_STK[LED_STK_SIZE-1], LED_DEMO_TASK_Prio);
//	OSTaskCreate(Task_Menu, (void * )0, (OS_STK *)&TASK_Menu_STK[Menu_STK_SIZE-1], Menu_TASK_Prio);
//	OSTaskCreate(Task_Touch, (void * )0, (OS_STK *)&TASK_Touch_STK[Touch_STK_SIZE-1], Touch_TASK_Prio);

	OSTaskDel(OS_PRIO_SELF);//删除自己	OSTaskSuspend(START_TASK_Prio);
}

/********************************************************************************************************** 
** Function name:      Task_Touch
** Descriptions:       触摸屏检测任务，此任务必须建立，否则将不能响应触摸屏
** input parameters:   void *pdata
** output parameters:  无
** Returned value:     无
**********************************************************************************************************/
void Task_Touch(void *pdata)
{
	GUI_PID_STATE  TouchPoint;
	while(1){
		GUI_TOUCH_Exec() ;
		GUI_TOUCH_GetState(&TouchPoint);
		if (TouchPoint.Pressed) {
			if(TouchFlag == TouchUnPressed){
				TouchFlag = TouchPress;
			}
		} else {
			if(TouchFlag == TouchPressed){
				TouchFlag = TouchUnPress;	
			}
		}
//	  OSTimeDly(10);   //保证50Hz以上的执行频率
	  OSTimeDlyHMSM(0,0,0,10);
	}
}

/********************************************************************************************************** 
** Function name:      Task_Menu
** Descriptions:       菜单运行任务，此任务必须建立，否则将不能支持滑动菜单操作。主菜单没有使用回调函数。
                       将你自己的任务添加到相应的按钮下，所以你只需完成各个分任务即可。
** input parameters:   void *pdata
** output parameters:  无
** Returned value:     无
**********************************************************************************************************/
void Task_Menu(void *pdata)
{
  GUI_Init();	
  MainMenu_Init();
	while(1)
	{
		if(TouchCmd == 1 && Menu_ID == MainMenu_ID)
		{
		    TouchCmd = 0;	
		  	switch(GUI_GetKey()) 
			{			
			  case GUI_ID_BUTTON0:
					Out_MainMenu();
//			    	OSTaskCreate(Task_MusicPlayer, (void * )0, (OS_STK *)&TASK_MusicPlayer_STK[MusicPlayer_STK_SIZE-1], MusicPlayer_TASK_Prio);
				  break;
			  case GUI_ID_BUTTON1: //退出主界面并创建文件浏览任务
          			Out_MainMenu();
//				    OSTaskCreate(Task_FileBrowser, (void * )0, (OS_STK *)&TASK_FileBrowser_STK[FileBrowser_STK_SIZE-1], FileBrowser_TASK_Prio);
				  break;
			  case GUI_ID_BUTTON2:
				    Out_MainMenu();
//				    OSTaskCreate(Task_TextBrowser, (void * )0, (OS_STK *)&TASK_TextBrowser_STK[TextBrowser_STK_SIZE-1], TextBrowser_TASK_Prio);
				  break;
			  case GUI_ID_BUTTON3: //退出主界面并创建设置任务
				    Out_MainMenu();
//				    OSTaskCreate(Task_Setting, (void * )0, (OS_STK *)&TASK_Setting_STK[Setting_STK_SIZE-1], Setting_TASK_Prio);
				  break;
			  case GUI_ID_BUTTON4:
				    Out_MainMenu();
//				    OSTaskCreate(Task_PhotoBrowser, (void * )0, (OS_STK *)&TASK_PhotoBrowser_STK[PhotoBrowser_STK_SIZE-1], PhotoBrowser_TASK_Prio);
				  break;
				case GUI_ID_BUTTON5:
//					GUI_MessageBox("Function does not support!","FFT",GUI_MESSAGEBOX_CF_MOVEABLE);
					break;
				case GUI_ID_BUTTON6:
					Out_MainMenu();
//				    OSTaskCreate(Task_ComPort, (void * )0, (OS_STK *)&TASK_ComPort_STK[ComPort_STK_SIZE-1], ComPort_TASK_Prio);
					break;
				case GUI_ID_BUTTON7:
//					GUI_MessageBox("Function does not support!","示波器",GUI_MESSAGEBOX_CF_MOVEABLE);
					break;				  
				default :	
					TouchCmd = 1;
					break;
		  }
	  }
	  WM_MoveCtrl();
		GUI_Exec();//重绘
	  OSTimeDly(3);
	}
}

/********************************************************************************************************** 
** Function name:      Task_LED_DEMO
** Descriptions:       LED闪烁，并且更新时间任务
** input parameters:   void *pdata
** output parameters:  无
** Returned value:     无
**********************************************************************************************************/
void Task_LED_DEMO(void *pdata)
{
	pdata = pdata;
/*	char  time_chars[] = {0,0,':',0,0,0};
    u32   time_now;
	while(1){ 
		GPIO_ResetBits(GPIOE, GPIO_Pin_2);
    OSTimeDly(250);
		GPIO_SetBits(GPIOE, GPIO_Pin_2);
	  OSTimeDly(250);
		time_now = RTC_GetCounter();
		if((time_now/3600)>23){	
			RTC_Configuration();
			RTC_SetCounter(time_now-24*3600);
    }			
		time_chars[0] = time_now / 36000 + 0x30;
		time_chars[1] = time_now / 3600 % 10 + 0x30;
		time_chars[3] = time_now % 3600 / 600 + 0x30;
		time_chars[4] = time_now % 3600 / 60 % 10 + 0x30;
		TEXT_SetText(hText,time_chars);
// // 		printf("OSCPUUsage = %d\r\n",OSCPUUsage);

	}				 */
	while(1)
	{
		LED3(ON);
		OSTimeDlyHMSM(0,0,0,800);
		LED3(OFF);
		OSTimeDlyHMSM(0,0,0,800);						
	}
}



