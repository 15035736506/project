/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              礐/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : LCDConf_1375_C8_C320x240.h
Purpose     : Sample configuration file
----------------------------------------------------------------------
*/

#ifndef LCDCONF_H
#define LCDCONF_H

/*********************************************************************
*
*                   General configuration of LCD
*
**********************************************************************
*/

#define LCD_XSIZE      (320)   /* X-resolution of LCD, Logical coor. */
#define LCD_YSIZE      (240)   /* Y-resolution of LCD, Logical coor. */

//#define LCD_BITSPERPIXEL (8)
#define LCD_BITSPERPIXEL (16)

#define LCD_CONTROLLER -1   /*用自己的驱动就填 -1 */

#define LCD_FIXEDPALETTE  565 //定义颜色查询表
#define LCD_SWAP_RB  1     //激活红，蓝基色的交换。
#define LCD_INIT_CONTROLLER() LCD_DriverInit()  

#define BUTTON_SUPPORT_STREAMED_BITMAP 1

 
#endif /* LCDCONF_H */
