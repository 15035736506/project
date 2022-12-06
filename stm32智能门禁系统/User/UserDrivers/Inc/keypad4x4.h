/*
 * keypad4x4.h
 *
 *  Created on: 2021年10月12日
 *      Author: Naming8104
 */

#ifndef INC_KEY4X4_H_
#define INC_KEY4X4_H_

#include "main.h"
#include "usart.h"
#include "tim.h"

extern uint8_t key_confirmed;

uint8_t KeyPad_Scan();

#endif /* INC_KEYPAD4X4_H_ */
