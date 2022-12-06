#ifndef _APP1_H
#define _APP1_H
#include "stm32f10x.h"
#include "includes.h"
#include "GUI.H"
#include "My_UI.h"
//#include "Initialize.h"

extern TEXT_Handle hText;
extern u8 Menu_ID;                 //菜单界面的ID
extern WM_HWIN hDesktopWindow;     //桌面的句柄
extern u8 TouchCmd;                //标志着是点触还是滑动操作。
extern u8 TouchFlag;
/************************************************************************************************
**任务的优先级声明
************************************************************************************************/
#define Touch_TASK_Prio   	      3        //触摸检测任务
#define Menu_TASK_Prio   	      30       //GUI菜单运行任务
#define START_TASK_Prio           55       //起始任务
#define LED_DEMO_TASK_Prio        50       //LED任务
#define Setting_TASK_Prio         12       //设置任务
#define FileBrowser_TASK_Prio     14       //文件浏览任务
#define MusicPlayer_TASK_Prio     16       //音乐播放器任务
#define PhotoBrowser_TASK_Prio    18       //图片浏览器任务
#define TextBrowser_TASK_Prio     20       //文本浏览器任务
#define ComPort_TASK_Prio         22       //串口任务
/************************************************************************************************
**任务堆栈容量声明（字节）
************************************************************************************************/
#define Touch_STK_SIZE            128      //触摸检测任务
#define Menu_STK_SIZE             256      //GUI菜单运行任务
#define START_STK_SIZE            100       //起始任务
#define LED_STK_SIZE              64       //LED任务
#define Setting_STK_SIZE          256      //设置任务
#define FileBrowser_STK_SIZE      256      //文件浏览器任务
#define MusicPlayer_STK_SIZE      320      //音乐播放器任务
#define PhotoBrowser_STK_SIZE     256      //图片浏览器任务
#define TextBrowser_STK_SIZE      256      //文本浏览器任务
#define ComPort_STK_SIZE          256      //串口任务
/************************************************************************************************
**任务声明
************************************************************************************************/
void TaskStart(void * pdata);              //起始任务
void Task_Touch(void *pdata);              //触摸检测任务
void Task_Menu(void *pdata);               //GUI菜单运行任务
void Task_LED_DEMO(void *pdata);           //LED任务
extern void Task_Setting(void *pdata);     //设置任务
//extern void Task_FileBrowser(void *pdata); //文件浏览器任务
//extern void Task_PhotoBrowser(void *pdata);//图片浏览器任务
//extern void Task_TextBrowser(void *pdata); //文本浏览器任务
//extern void Task_ComPort(void *pdata);     //串口任务
#endif
