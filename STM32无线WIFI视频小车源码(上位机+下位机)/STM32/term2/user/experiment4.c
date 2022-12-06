#include"GUI.h"
#include"WM.h"

/************�������ڻص�����******************/
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

/*************ǰ�����ڻص�����***************/
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

/**********uC/GUI�ػ����չʾ************/
static void DemoRedraw(void)
{
	GUI_HWIN hWnd;
	while(1)
	{
		/***����ǰ������***/
		hWnd = WM_CreateWindow(10,10,100,100,WM_CF_SHOW,cbForegroundWin,0);//��ʾǰ������
		GUI_Delay(1000);
		/*ɾ��ǰ������*/
		WM_DeleteWindow(hWnd);
		GUI_DispStringAt("Background of window has not been redrawn",10,10);
		//�ȴ�
		GUI_Delay(1000);
		GUI_Clear();
		/*���ñ������ڵĻص�����*/
		WM_SetCallback(WM_HBKWIN,cbBackgroundWin);
		/*����ǰ������*/
		hWnd = WM_CreateWindow(10,10,100,100,WM_CF_SHOW,
								cbForegroundWin,0);
		GUI_Delay(1000);
		/*ɾ��ǰ������*/
		WM_DeleteWindow(hWnd);
		GUI_Delay(1000);
		/*ɾ���������ڵĻص�����*/
		WM_SetCallback(WM_HBKWIN,0);
	}
}

void _main_4()
{
	DemoRedraw();
}









