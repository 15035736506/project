/**
 * @file smartlock.h
 * @author Naming8104
 * @brief 
 * @version 0.1
 * @date 2022-03-24
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef INC_SMARTLOCK_H_
#define INC_SMARTLOCK_H_

#include "main.h"

typedef enum
{
  LockScreen_UI = 0,
  LockScreen_Wrong_UI,
  SettingScreen_UI,
  SettingScreen_Opening_UI,
  ChangePW_New_UI,
  ChangePW_Confirm_UI,
  ChangePW_Sucess_UI,
  ChangePW_TooShort_UI,
  ChangePW_NotMatch_UI,
  ChangeFP_First_UI,
  ChangeFP_Second_UI,
  ChangeFP_Sucess_UI,
  ChangeFP_NotMatch_UI,

}LockUI_Type;

void SmartLock_Init();
void SmartLock_Loop();

#endif