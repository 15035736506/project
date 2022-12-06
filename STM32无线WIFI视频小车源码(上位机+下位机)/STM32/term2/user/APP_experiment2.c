#include "GUI.h"
#include "WM.h"

void main_APP1()
{
//	GUI_Init();
	WM_SetBkWindowColor(GUI_RED);
	/*建立消息框，并等待其关闭*/
//	GUI_MessageBox("This text is shown\nin a message box",
//					"Caption/Title",
//					GUI_MESSAGEBOX_CF_MOVEABLE);
	GUI_DispStringAt("welcome",10,10);
	
}
