#include "Settings.h"


extern void RTC_Configuration(void);

struct {
	u8 Tiles;
	u8 Bk;
}ColorSel;

u8 hour,minite;
const char SystemInfo[] = 
{
"\
GUI : uC/GUI V3.90\n\
OS  : uC/OS-II V2.91\n\
FatFs : FatFs R0.09\n\
MCU : STM32F103ZET6\n\
LCD : ILI9320\n\
Keil: MDK-ARM V4.54\n\
author : Arvik\
"
};
const GUI_ConstString TilesColor[] = {
//"红","绿","蓝","紫","亮蓝","深蓝","暗红","暗黄",NULL
"Red","Blue","Green","Purple","LightGreen","DeepGreen","DarkRed","DarkYellow",NULL
};
const GUI_ConstString BkColor[] = {
//"黑","红","绿","蓝","黄","灰","亮蓝",NULL
"Black","Red","Blue","Green","Yellow","Gray","LightGreen",NULL
};

/********************************************建立资源表**************************20,90,200,180***************10,20,220,288*********/
const GUI_WIDGET_CREATE_INFO TimeSetDialog[] = {
//	{ FRAMEWIN_CreateIndirect,"时间设置",0,-200,90,200,180,FRAMEWIN_CF_MOVEABLE},
	{ FRAMEWIN_CreateIndirect,"Time Set",0,-200,90,200,180,FRAMEWIN_CF_MOVEABLE},
	{ BUTTON_CreateIndirect,"",Button_ID10,17, 10,24, 20},
	{ BUTTON_CreateIndirect,"",Button_ID11,75, 10,24, 20},	
	{ TEXT_CreateIndirect,"",GUI_ID_TEXT0,5,40,48,32,TEXT_CF_HCENTER},
	{ TEXT_CreateIndirect,"",GUI_ID_TEXT1,63,40,48,32,TEXT_CF_HCENTER},
	{ BUTTON_CreateIndirect,"",Button_ID12,17, 80,24, 20},
	{ BUTTON_CreateIndirect,"",Button_ID13,75, 80,24, 20},
//	{ BUTTON_CreateIndirect,"确定",GUI_ID_OK,33, 120,50, 26},
	{ BUTTON_CreateIndirect,"OK",GUI_ID_OK,33, 120,50, 26},
//	{ BUTTON_CreateIndirect,"取消",GUI_ID_CANCEL,116, 120,50,26}
	{ BUTTON_CreateIndirect,"CANCLE",GUI_ID_CANCEL,116, 120,50,26}
};

const GUI_WIDGET_CREATE_INFO TilesSetDialog[] = {
	{ FRAMEWIN_CreateIndirect,"System Color",0,-220,20,220,288,FRAMEWIN_CF_MOVEABLE},
	{ TEXT_CreateIndirect,"Set the theme color:",GUI_ID_TEXT0,2,2,70,20,TEXT_CF_HCENTER},
	{ TEXT_CreateIndirect,"Set the background color:",GUI_ID_TEXT1,2,113,70,20,TEXT_CF_HCENTER},
  { LISTBOX_CreateIndirect,0,GUI_ID_LISTBOX0,2,22,210,85,WM_CF_SHOW},
  { LISTBOX_CreateIndirect,0,GUI_ID_LISTBOX1,2,137,210,85,WM_CF_SHOW},
	{ BUTTON_CreateIndirect,"OK",GUI_ID_OK,40, 230,50, 25},
	{ BUTTON_CreateIndirect,"CANCLE",GUI_ID_CANCEL,130,230,50,25}
};
/********************************************************************************************************** 
** Function name:      TimeSetCallBack
** Descriptions:       时间设置对话框回调函数
** input parameters:   WM_MESSAGE *pMsg：消息
** output parameters:  无
** Returned value:     无
**********************************************************************************************************/
static void TimeSetCallBack(WM_MESSAGE *pMsg)
{
	int Id,NCode;
	WM_HWIN hWin = pMsg->hWin;
	u32 time_now ;
	TEXT_Handle hTextelse[2];
	char  time_chars_hour[] = {0,0,0};
  char  time_chars_minite[] = {0,0,0};
  hTextelse[0] = WM_GetDialogItem(hWin,GUI_ID_TEXT0);
  hTextelse[1] = WM_GetDialogItem(hWin,GUI_ID_TEXT1);
	switch(pMsg -> MsgId) {
		case WM_INIT_DIALOG:
			MenuDisnable++;
			time_now = RTC_GetCounter();				  
			hour = time_now / 3600;
			minite = time_now % 3600 / 60;				
			time_chars_hour[0] = hour/10+0x30;
		  time_chars_hour[1] = hour%10+0x30;
			time_chars_minite[0] = minite/10+0x30;
			time_chars_minite[1] = minite%10+0x30;
		
		  TEXT_SetFont(hTextelse[0],&GUI_FontD24x32);
		  TEXT_SetFont(hTextelse[1],&GUI_FontD24x32);
		  TEXT_SetTextColor(hTextelse[0], ThemeColor);
		  TEXT_SetTextColor(hTextelse[1], ThemeColor);
		  TEXT_SetText(hTextelse[0],time_chars_hour);
		  TEXT_SetText(hTextelse[1],time_chars_minite);
		
/*			BUTTON_SetBitmapEx (WM_GetDialogItem(hWin,Button_ID10),0, &bmButtonBmp1_1bpp, 4, 7);
			BUTTON_SetBitmapEx (WM_GetDialogItem(hWin,Button_ID11),0, &bmButtonBmp1_1bpp, 4, 7);
			BUTTON_SetBitmapEx (WM_GetDialogItem(hWin,Button_ID12),0, &bmButtonBmp2_1bpp, 4, 7);
			BUTTON_SetBitmapEx (WM_GetDialogItem(hWin,Button_ID13),0, &bmButtonBmp2_1bpp, 4, 7);
*/

   	  FRAMEWIN_AddMaxButton(pMsg->hWin, FRAMEWIN_BUTTON_RIGHT, 0);
 	    FRAMEWIN_AddMinButton(pMsg->hWin, FRAMEWIN_BUTTON_RIGHT, 0);
			break;
	  case WM_NOTIFY_PARENT:
		  Id = WM_GetId(pMsg ->hWinSrc);
		  NCode = pMsg ->Data.v;
		  switch(NCode) {
		    case WM_NOTIFICATION_RELEASED:
			    if(Id == Button_ID10) {
					   hour++;
					   if(hour == 24)hour = 0;
			       time_chars_hour[0] = hour/10+0x30;
		         time_chars_hour[1] = hour%10+0x30;
             TEXT_SetText(WM_GetDialogItem(hWin,GUI_ID_TEXT0),time_chars_hour);
			    } else if (Id == Button_ID11) {
					   minite++;
					   if(minite == 60)minite = 0;
			       time_chars_minite[0] = minite/10+0x30;
			       time_chars_minite[1] = minite%10+0x30;
						 TEXT_SetText(WM_GetDialogItem(hWin,GUI_ID_TEXT1),time_chars_minite);
			    } else if(Id == Button_ID12) {
				   	 if(hour == 0)hour = 23;
					   else hour--;
				     time_chars_hour[0] = hour/10+0x30;
		         time_chars_hour[1] = hour%10+0x30;
             TEXT_SetText(WM_GetDialogItem(hWin,GUI_ID_TEXT0),time_chars_hour);
			    } else if (Id == Button_ID13) {
					   if(minite == 0)minite = 59;
					   else	minite--;
				     time_chars_minite[0] = minite/10+0x30;
			       time_chars_minite[1] = minite%10+0x30;
						 TEXT_SetText(WM_GetDialogItem(hWin,GUI_ID_TEXT1),time_chars_minite);
			    } else if (Id == GUI_ID_OK) {
						 RTC_Configuration();
			       RTC_SetCounter((u32)hour * 3600 + (u32)minite * 60);
						 GUI_EndDialog(pMsg->hWin, 0);
						 MenuDisnable--;
					} else if (Id == GUI_ID_CANCEL) {
						 GUI_EndDialog(pMsg->hWin, 1);
						 MenuDisnable--;
					}
					break;
				default:
				  break;
		  }		
        break;			
			default:
        WM_DefaultProc(pMsg);
				break;
	 }
}
/********************************************************************************************************** 
** Function name:      ThemeSetCallBack
** Descriptions:       主题色设置对话框回调函数
** input parameters:   WM_MESSAGE *pMsg：消息
** output parameters:  无
** Returned value:     无
**********************************************************************************************************/
static void ThemeSetCallBack(WM_MESSAGE *pMsg)
{
	int Id,NCode;
	WM_HWIN hWin = pMsg->hWin;
	switch(pMsg -> MsgId) {
		
		case WM_INIT_DIALOG:
			MenuDisnable++;
			LISTBOX_SetText(WM_GetDialogItem(hWin,GUI_ID_LISTBOX0), TilesColor);
		  LISTBOX_SetAutoScrollV (WM_GetDialogItem(hWin,GUI_ID_LISTBOX0), 1);
			LISTBOX_SetText(WM_GetDialogItem(hWin,GUI_ID_LISTBOX1), BkColor);
		  LISTBOX_SetAutoScrollV (WM_GetDialogItem(hWin,GUI_ID_LISTBOX1), 1);
		  LISTBOX_SetSel(WM_GetDialogItem(hWin,GUI_ID_LISTBOX0), ColorSel.Tiles);
		  LISTBOX_SetSel(WM_GetDialogItem(hWin,GUI_ID_LISTBOX1), ColorSel.Bk);
		  TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT0), ThemeColor);
		  TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT1), ThemeColor);
//		  TEXT_SetFont(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT0),&GUI_FontHZ_MicrosoftFont_13);
		  TEXT_SetFont(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT0),&GUI_Font13_ASCII);
//		  TEXT_SetFont(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT1),&GUI_FontHZ_MicrosoftFont_13);
		  TEXT_SetFont(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT1),&GUI_Font13_ASCII);


   	  FRAMEWIN_AddMaxButton(pMsg->hWin, FRAMEWIN_BUTTON_RIGHT, 0);
 	    FRAMEWIN_AddMinButton(pMsg->hWin, FRAMEWIN_BUTTON_RIGHT, 0);
			break;
	  case WM_NOTIFY_PARENT:
		  Id = WM_GetId(pMsg ->hWinSrc);
		  NCode = pMsg ->Data.v;
		  switch(NCode) {
		    case WM_NOTIFICATION_RELEASED:
			  
			     if (Id == GUI_ID_OK) {
						 ColorSel.Tiles = LISTBOX_GetSel(WM_GetDialogItem(hWin,GUI_ID_LISTBOX0));
						 ColorSel.Bk    = LISTBOX_GetSel(WM_GetDialogItem(hWin,GUI_ID_LISTBOX1));
						 switch(ColorSel.Tiles) {

							 case 0:SetThemeColor(GUI_RED); 
								 break;
							 case 1:SetThemeColor(GUI_GREEN);
								 break;
							 case 2:SetThemeColor(GUI_BLUE);
								 break;
							 case 3:SetThemeColor(GUI_DARKMAGENTA);
								 break;
							 case 4:SetThemeColor(GUI_LIGHTBLUE);
								break;
							 case 5:SetThemeColor(GUI_DARKBLUE);
								 break;						 
							 case 6:SetThemeColor(GUI_DARKRED);
								 break;				
							 case 7:SetThemeColor(GUI_DARKYELLOW);
								 break;								 
							 default:
								 break;
						 }
						 switch(ColorSel.Bk) {
					
							 case 0:DesktopColor = GUI_BLACK;
								 break;
							 case 1:DesktopColor = GUI_RED;
								 break;
							 case 2:DesktopColor = GUI_GREEN ;
								 break;
							 case 3:DesktopColor = GUI_BLUE;
								 break;
							 case 4:DesktopColor = GUI_YELLOW;
								 break;
							 case 5:DesktopColor = GUI_GRAY;
								break;
							 case 6:DesktopColor = GUI_LIGHTBLUE;
								 break;							 
							 default:
								 break;
						 }
						 WM_SetDesktopColor(DesktopColor); 
						 GUI_EndDialog(pMsg->hWin, 0);
						 MenuDisnable--;
					} else if (Id == GUI_ID_CANCEL) {
						 GUI_EndDialog(pMsg->hWin, 1);
						 MenuDisnable--;
					} 
					break;
				default:
				  break;
		  }		
      break;			
		default:
      WM_DefaultProc(pMsg);
		  break;
	 }
}
/********************************************************************************************************** 
** Function name:      Task_Setting
** Descriptions:       设置任务
** input parameters:   void *pdata
** output parameters:  无
** Returned value:     无
**********************************************************************************************************/
void Task_Setting(void *pdata)
{
	WM_HWIN WM_Handle;
	BUTTON_Handle hButton_else[6];
  TEXT_Handle hTextelse[6];
	FRAMEWIN_Handle hFrame;
	MULTIEDIT_HANDLE hMultiEdit;
	u8 i ;
	SingleList   pos   = NULL;
	Menu_ID = SetMenu_ID;
	OtherMenu_pHead = SingleListNodeCreate();        /* 创建单向链表表头节点    */
	pos = OtherMenu_pHead;
	/*******************************************设置界面初始化*********************************************/

  hTextelse[0] = TEXT_Create(5,20,200, 20, 2, WM_CF_SHOW, "Setting options",TEXT_CF_LEFT);
//	TEXT_SetFont(hTextelse[0],&GUI_FontHZ_MicrosoftFont_13);
	TEXT_SetFont(hTextelse[0],&GUI_Font13_ASCII);
	OtherMenu_Info.FirstHandle = hTextelse[0];
	hTextelse[1] = TEXT_Create(5,50,200, 25, 3, WM_CF_SHOW, "Set time",TEXT_CF_LEFT);
	TEXT_SetFont(hTextelse[1],&GUI_Font24_ASCII);
	hTextelse[2] = TEXT_Create(5,110,200, 25, 4, WM_CF_SHOW, "Set system color",TEXT_CF_LEFT);
	TEXT_SetFont(hTextelse[2],&GUI_Font24_ASCII);
	hTextelse[3] = TEXT_Create(5,170,200, 25, 5, WM_CF_SHOW, "System message",TEXT_CF_LEFT);
	TEXT_SetFont(hTextelse[3],&GUI_Font24_ASCII);
	hTextelse[4] = TEXT_Create(5,300,200, 25, 6, WM_CF_SHOW, "Back to mainmenu",TEXT_CF_LEFT);
	TEXT_SetFont(hTextelse[4],&GUI_Font24_ASCII);
	
	for (i=0;i<5;i++) {
		TEXT_SetTextColor(hTextelse[i],GUI_WHITE);
// 	  TEXT_SetBkColor(hTextelse[i],GUI_BLACK);
		pos = SingleListInsert(OtherMenu_pHead,pos,hTextelse[i]);//将所有文本框句柄插入链表
	}
	
	hButton_else[0] = BUTTON_Create (5, 80,70, 25, Button_ID0, WM_CF_SHOW);
	hButton_else[1] = BUTTON_Create (5, 140,70, 25, Button_ID1, WM_CF_SHOW);
	hButton_else[2] = BUTTON_Create (5, 200,70, 25, Button_ID2, WM_CF_SHOW);
	hButton_else[3] = BUTTON_Create (5, 330,70, 25, Button_ID3, WM_CF_SHOW);
	
	OtherMenu_Info.EndHandle = hButton_else[3];
	
	for (i=0;i<4;i++) {
		BUTTON_SetTextColor(hButton_else[i],0,GUI_WHITE);
	  BUTTON_SetBkColor  (hButton_else[i],0,ThemeColor);
    BUTTON_SetBkColor  (hButton_else[i],1,GUI_GRAY);
		pos = SingleListInsert(OtherMenu_pHead,pos,hButton_else[i]);//将所有按钮句柄插入链表
	}
	BUTTON_SetText(hButton_else[0],"Set");
	BUTTON_SetText(hButton_else[1],"Set");
	BUTTON_SetText(hButton_else[2],"Check");
	BUTTON_SetText(hButton_else[3],"desktop");
	
	ViscousWM(OtherMenu_pHead);//实现界面的回弹效果
	/*******************************************************************************************************/
	while(1){
		if(TouchCmd == 1){
			TouchCmd = 0;	
		  switch(GUI_GetKey()) {
				case Button_ID0:          
				  WM_Handle = GUI_CreateDialogBox(TimeSetDialog, GUI_COUNTOF(TimeSetDialog), TimeSetCallBack, 0, 0, 0);
				  MyCreatDialog(20,90,WM_Handle);
					break;
				case Button_ID1:       
				  WM_Handle = GUI_CreateDialogBox(TilesSetDialog, GUI_COUNTOF(TilesSetDialog), ThemeSetCallBack, 0, 0, 0);
				  MyCreatDialog(10,20,WM_Handle);
					break;
			  case Button_ID2:
					
				  hFrame = FRAMEWIN_Create("System message",NULL,WM_CF_SHOW,-220,60,220,200);
			    FRAMEWIN_AddCloseButton(hFrame, FRAMEWIN_BUTTON_RIGHT,0);
		      FRAMEWIN_AddMaxButton(hFrame, FRAMEWIN_BUTTON_RIGHT, 0);
		      FRAMEWIN_AddMinButton(hFrame, FRAMEWIN_BUTTON_RIGHT, 0);
		      FRAMEWIN_SetMoveable(hFrame, 1);
			    hMultiEdit = MULTIEDIT_Create(5, 25,210,170, hFrame,20, WM_CF_SHOW, WM_CF_SHOW,SystemInfo,50);
				  MULTIEDIT_SetReadOnly(hMultiEdit, MULTIEDIT_CF_READONLY);
//				  MULTIEDIT_SetFont(hMultiEdit,&GUI_FontHZ_MicrosoftFont_13);
				  MULTIEDIT_SetFont(hMultiEdit,&GUI_Font13_ASCII);
				  MyCreatDialog(10,60,hFrame);
					break;
			  case Button_ID3:
					Out_OtherMenu();            //退出设置界面
				  Go_MainMenu();              //进入主界面
				  OSTaskDel(OS_PRIO_SELF);    //删除自己
					break;
				default :	
					TouchCmd = 1;
					break;
		  }
	  }
	  OSTimeDly(10);   //保证50Hz的执行频率
	}
}
