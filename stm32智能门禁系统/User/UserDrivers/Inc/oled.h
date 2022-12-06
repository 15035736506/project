/*
 * oled.h
 *
 *  Created on: 2022年3月12日
 *      Author: Naming8104
 */

#ifndef INC_OLED_H_
#define INC_OLED_H_

#include "main.h"

#define OLED_RST_CLR() HAL_GPIO_WritePin(OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_RESET)
#define OLED_RST_SET() HAL_GPIO_WritePin(OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_SET)

#define OLED_DC_CLR() HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_RESET)
#define OLED_DC_SET() HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_SET)

void OLED_WrDat(uint8_t data);
void OLED_WrCmd(uint8_t cmd);
void OLED_Start_DMA(uint8_t data[8][128], size_t size);
void OLED_Stop_DMA();

void OLED_Init(void);
void OLED_UpsideDown(uint8_t upsidedown);
void OLED_AddressMode(uint8_t address_mode);

void OLED_Set_Pos(uint8_t x, uint8_t y);
void OLED_Fill(uint8_t data);
void OLED_CLS(void);

void OLED_P6x8Str(uint8_t x, uint8_t y, const char *ch);
void OLED_P8x16Str(uint8_t x, uint8_t y, const char *ch);
void OLED_P16x16Str(uint8_t x, uint8_t y, const char *ch);
void OLED_BMP(uint8_t x, uint8_t y, const uint8_t *bmp);

#endif
