/*
 * as608.h
 *
 *  Created on: 2022年3月5日
 *      Author: Naming8104
 */

#ifndef INC_AS608_H_
#define INC_AS608_H_

#include "main.h"

const char *SensorType(uint8_t data);
const char *EnsureMessage(uint8_t ensure);

uint8_t RecvPkg(uint8_t *recv_data);
uint8_t PS_GetImage(void);
uint8_t PS_GenChar(uint8_t bufferId);
uint8_t PS_Match(void);
uint8_t PS_Search(uint8_t bufferId, uint16_t startPage, uint16_t pageNum);
uint8_t PS_RegModel(void);
uint8_t PS_StoreChar(uint8_t bufferId, uint16_t pageId);
uint8_t PS_LoadChar(uint8_t bufferId, uint16_t pageId);
uint8_t PS_UpChar(uint8_t bufferId);
uint8_t PS_DeletChar(uint16_t pageId, uint16_t num);
uint8_t PS_Empty(void);
uint8_t PS_WriteReg(uint8_t regId, uint16_t param);
uint8_t PS_ReadSysPara(void);
uint8_t PS_HighSpeedSearch(uint8_t bufferId, uint16_t startPage, uint16_t pageNum);
uint8_t PS_ValidTempleteNum(void);

// void FP_UART_Rx_Callback(void);

#endif /* INC_AS608_H_ */
