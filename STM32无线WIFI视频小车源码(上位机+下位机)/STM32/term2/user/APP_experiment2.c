#include "GUI.h"
#include "WM.h"

void main_APP1()
{
//	GUI_Init();
	WM_SetBkWindowColor(GUI_RED);
	/*������Ϣ�򣬲��ȴ���ر�*/
//	GUI_MessageBox("This text is shown\nin a message box",
//					"Caption/Title",
//					GUI_MESSAGEBOX_CF_MOVEABLE);
	GUI_DispStringAt("welcome",10,10);
	
}
