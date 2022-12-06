/*
 * keypad4x4.c
 *
 * 4*4矩阵键盘驱动
 * 
 * X0~3 -> 1~4列
 * Y0~3 -> 1~4行
 * 
 * 例如S0对应(X0,Y0),S11对应(X2,Y2)
 * 
 *  Created on: 2021年10月12日
 *      Author: Naming8104
 */
#include "keypad4x4.h"

uint8_t key_col;
uint16_t key_buf;
uint8_t key_confirmed_flag;
uint8_t scan_step;

uint8_t keyParser(uint16_t key_buf)
{
  uint8_t key_num = 0;
  uint8_t i;
  for (i = 0; i < 16; i++)
  {
    if ((key_buf >> i) & 0x0001)
      key_num = ++i;
  }
  RTT_printf("[矩阵键盘] 按下了 S%d \r\n", key_num);
  return key_num;
}

uint8_t keyPadColScan(uint8_t row)
{
  switch(row)
  {
  case 0:
    HAL_GPIO_WritePin(GPIOC, PAD_Y1_Pin | PAD_Y2_Pin | PAD_Y3_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOC, PAD_Y0_Pin, GPIO_PIN_RESET);
    break;
  case 1:
    HAL_GPIO_WritePin(GPIOC, PAD_Y0_Pin | PAD_Y2_Pin | PAD_Y3_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOC, PAD_Y1_Pin, GPIO_PIN_RESET);
    break;
  case 2:
    HAL_GPIO_WritePin(GPIOC, PAD_Y0_Pin | PAD_Y1_Pin | PAD_Y3_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOC, PAD_Y2_Pin, GPIO_PIN_RESET);
    break;
  case 3:
    HAL_GPIO_WritePin(GPIOC, PAD_Y0_Pin | PAD_Y1_Pin | PAD_Y2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOC, PAD_Y3_Pin, GPIO_PIN_RESET);
    break;
  }
  key_col = HAL_GPIO_ReadPin(GPIOD, PAD_X0_Pin) << 3;
  key_col = key_col | HAL_GPIO_ReadPin(GPIOD, PAD_X1_Pin) << 2;
  key_col = key_col | HAL_GPIO_ReadPin(GPIOD, PAD_X2_Pin) << 1;
  key_col = key_col | HAL_GPIO_ReadPin(GPIOD, PAD_X3_Pin);
  return key_col;
}

uint8_t KeyPad_Scan()
{
  uint8_t i;
  switch (scan_step)
  {
  //按键还没有按下
  case 0:
    key_buf = 0x0000;//清除按键

    for (i = 0; i < 4; i++)//4行
    {
      keyPadColScan(i);
      if (key_col != 0x0f)//有按键按下
      {
        switch (key_col)//哪个按键
        {
        case 0x07: key_buf |= 0x0001 << (i << 2); break;//0000 0111
        case 0x0b: key_buf |= 0x0002 << (i << 2); break;//0000 1011
        case 0x0d: key_buf |= 0x0004 << (i << 2); break;//0000 1101
        case 0x0e: key_buf |= 0x0008 << (i << 2); break;//0000 1110
        }
        scan_step = 1;//下一步
        HAL_TIM_Base_Start_IT(&htim13);//开始消抖计时
        break;
      }
    }
    break;

  //有按键按下或抖动
  case 1:
    for (i = 0; i < 4; i++)//4行
    {
      if ((key_buf & (0x000f << (i << 2))) != 0x0000) //是第i+1行
      {
        keyPadColScan(i);//重新扫描该行
        if (key_col != 0x0f && key_confirmed_flag == 1) //按键未抬起，且完成消抖
        {
          key_confirmed_flag = 0;//重置 完成消抖标志
          scan_step = 2;//下一步
          break;
        }
        else if (key_col == 0x0f) //按键抖动
        {
          HAL_TIM_Base_Stop_IT(&htim13);    //定时器停止
          __HAL_TIM_SetCounter(&htim13, 0); //定时器清零
          scan_step = 0;//回到第一步
          break;
        }
      }
    }
    break;

  //确认有按键按下，等待按键抬起
  case 2:
    for (i = 0; i < 4; i++)//4行
    {
      if ((key_buf & (0x000f << (i << 2))) != 0x0000) //是第i+1行
      {
        keyPadColScan(i);
        if (key_col != 0x0f) break;//按键未抬起
      }
    }
    if (key_col != 0x0f) break;//按键未抬起
    scan_step = 0;//回到第一步
    return keyParser(key_buf);
  }
  return 0;
}
