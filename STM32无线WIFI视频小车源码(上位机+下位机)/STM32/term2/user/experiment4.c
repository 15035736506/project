#include"GUI.h"
#include"WM.h"

/************背景窗口回调函数******************/
static void cbBackgroundWin(WM_MESSAGE* pMsg)
{
	switch(pMsg->MsgId)
	{
		case WM_PAINT:
			GUI_Clear();
		default:
			WM_DefaultProc(pMsg);
	}
}

/*************前景窗口回调函数***************/
static void cbForegroundWin(WM_MESSAGE* pMsg)
{
  switch(pMsg->MsgId)
  {
	case WM_PAINT:
		GUI_SetBkColor(GUI_GREEN);
		GUI_Clear();
		GUI_DispString("Foreground window");
		break;
	default:
		WM_DefaultProc(pMsg);
  }
}

/**********uC/GUI重绘机制展示************/
static void DemoRedraw(void)
{
	GUI_HWIN hWnd;
	while(1)
	{
		/***创建前景窗口***/
		hWnd = WM_CreateWindow(10,10,100,100,WM_CF_SHOW,cbForegroundWin,0);//显示前景窗口
		GUI_Delay(1000);
		/*删除前景窗口*/
		WM_DeleteWindow(hWnd);
		GUI_DispStringAt("Background of window has not been redrawn",10,10);
		//等待
		GUI_Delay(1000);
		GUI_Clear();
		/*设置背景窗口的回调函数*/
		WM_SetCallback(WM_HBKWIN,cbBackgroundWin);
		/*创建前景窗口*/
		hWnd = WM_CreateWindow(10,10,100,100,WM_CF_SHOW,
								cbForegroundWin,0);
		GUI_Delay(1000);
		/*删除前景窗口*/
		WM_DeleteWindow(hWnd);
		GUI_Delay(1000);
		/*删除背景窗口的回调函数*/
		WM_SetCallback(WM_HBKWIN,0);
	}
}

void _main_4()
{
	DemoRedraw();
}









