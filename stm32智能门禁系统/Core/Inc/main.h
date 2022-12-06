/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
int RTT_printf(const char* sFormat, ...);
// uint32_t DWT_Delay_Init(void);
// void DWT_Delay_us(volatile uint32_t au32_microseconds);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define OPEN_THE_GATE_Pin GPIO_PIN_2
#define OPEN_THE_GATE_GPIO_Port GPIOE
#define KEY_1_Pin GPIO_PIN_3
#define KEY_1_GPIO_Port GPIOE
#define PAD_Y3_Pin GPIO_PIN_0
#define PAD_Y3_GPIO_Port GPIOC
#define PAD_Y2_Pin GPIO_PIN_1
#define PAD_Y2_GPIO_Port GPIOC
#define PAD_Y1_Pin GPIO_PIN_2
#define PAD_Y1_GPIO_Port GPIOC
#define PAD_Y0_Pin GPIO_PIN_3
#define PAD_Y0_GPIO_Port GPIOC
#define FP_WAK_Pin GPIO_PIN_1
#define FP_WAK_GPIO_Port GPIOA
#define LED_1_Pin GPIO_PIN_7
#define LED_1_GPIO_Port GPIOA
#define W25QXX_CS_Pin GPIO_PIN_0
#define W25QXX_CS_GPIO_Port GPIOB
#define LOCK_STAT_OUT_Pin GPIO_PIN_12
#define LOCK_STAT_OUT_GPIO_Port GPIOD
#define LOCK_STAT_IN_Pin GPIO_PIN_13
#define LOCK_STAT_IN_GPIO_Port GPIOD
#define ESP_RST_Pin GPIO_PIN_11
#define ESP_RST_GPIO_Port GPIOA
#define ESP_EN_Pin GPIO_PIN_12
#define ESP_EN_GPIO_Port GPIOA
#define OLED_RST_Pin GPIO_PIN_0
#define OLED_RST_GPIO_Port GPIOD
#define OLED_DC_Pin GPIO_PIN_1
#define OLED_DC_GPIO_Port GPIOD
#define PAD_X0_Pin GPIO_PIN_4
#define PAD_X0_GPIO_Port GPIOD
#define PAD_X1_Pin GPIO_PIN_5
#define PAD_X1_GPIO_Port GPIOD
#define PAD_X2_Pin GPIO_PIN_6
#define PAD_X2_GPIO_Port GPIOD
#define PAD_X3_Pin GPIO_PIN_7
#define PAD_X3_GPIO_Port GPIOD
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
