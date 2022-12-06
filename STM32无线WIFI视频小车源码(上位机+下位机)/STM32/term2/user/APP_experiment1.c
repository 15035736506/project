/*
******************************************************************
**                      uCGUIBuilder                            **
**                  Version:   3.0.0.0                          **
**                     2011 / 10                                **
**                   CpoyRight to: wyl                          **
**              Email:wyl2008056@163.com                        **
**          This text was Created by uCGUIBuilder               **
******************************************************************/

#include <stddef.h>
#include "GUI.h"
#include "DIALOG.h"

//#include "WM.h"
//#include "BUTTON.h"
//#include "CHECKBOX.h"
//#include "DROPDOWN.h"
//#include "EDIT.h"
//#include "FRAMEWIN.h"
//#include "LISTBOX.h"
//#include "MULTIEDIT.h"
//#include "RADIO.h"
//#include "SLIDER.h"
//#include "TEXT.h"
//#include "PROGBAR.h"
//#include "SCROLLBAR.h"
//#include "LISTVIEW.h"

/*********************************************************************
*
*       static data
*
**********************************************************************
*/




/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created by ucGUIbuilder.
*/

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { FRAMEWIN_CreateIndirect, "Caption",   0,                0,  0,  320,240,FRAMEWIN_CF_MOVEABLE,0},
    { BUTTON_CreateIndirect,   "BUTTON1",   GUI_ID_BUTTON0,   44, 65, 101,48, 0,0}
};



/*****************************************************************
**      FunctionName:void InitDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**                                                      
**      call this function in _cbCallback --> WM_INIT_DIALOG
*****************************************************************/

void InitDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;
    //
    //FRAMEWIN
    //
    FRAMEWIN_SetClientColor(hWin,GUI_CYAN);
    FRAMEWIN_SetTextColor(hWin,GUI_RED);
    FRAMEWIN_SetFont(hWin,&GUI_Font8_ASCII);
    FRAMEWIN_SetTitleVis(hWin,0);
    //
    //GUI_ID_BUTTON0
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON0),BUTTON_CI_UNPRESSED,GUI_MAGENTA);
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON0),BUTTON_CI_PRESSED,GUI_BLUE);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON0),&GUI_Font10_ASCII);

}




/*********************************************************************
*
*       Dialog callback routine
*/
static void _cbCallback(WM_MESSAGE * pMsg) 
{
    int NCode, Id;
    WM_HWIN hWin = pMsg->hWin;
    switch (pMsg->MsgId) 
    {
        case WM_INIT_DIALOG:
            InitDialog(pMsg);
            break;
        case WM_KEY:
            switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
            {
                case GUI_KEY_ESCAPE:
                    GUI_EndDialog(hWin, 1);
                    break;
                case GUI_KEY_ENTER:
                    GUI_EndDialog(hWin, 0);
                    break;
            }
            break;
        case WM_NOTIFY_PARENT:
            Id = WM_GetId(pMsg->hWinSrc); 
            NCode = pMsg->Data.v;        
            switch (Id) 
            {
                case GUI_ID_OK:
                    if(NCode==WM_NOTIFICATION_RELEASED)
                        GUI_EndDialog(hWin, 0);
                    break;
                case GUI_ID_CANCEL:
                    if(NCode==WM_NOTIFICATION_RELEASED)
                        GUI_EndDialog(hWin, 0);
                    break;
                case GUI_ID_BUTTON0:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            //add codes here
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            //add codes here
                            break;
                        case WM_NOTIFICATION_MOVED_OUT:
                            //add codes here
                            break;
                    }
                    break;

            }
            break;
        default:
            WM_DefaultProc(pMsg);
    }
}



/*********************************************************************
*
*       MainTask
*
**********************************************************************
*/
void MainTaskbutton1(void) 
{ 
//    GUI_Init();
//	GUI_CURSOR_Show();
    WM_SetDesktopColor(GUI_WHITE);      /* Automacally update desktop window */
    WM_SetCreateFlags(WM_CF_MEMDEV);  /* Use memory devices on all windows to avoid flicker */
    //PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
    //FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
    //PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
    //BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
    //CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
    //DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
    //SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
    //SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
    //HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
    //RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
    GUI_ExecDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, 0, 0, 0);
}
