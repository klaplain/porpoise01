/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32h7xx_hal.h"

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

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SD_Card_Detect_Pin GPIO_PIN_0
#define SD_Card_Detect_GPIO_Port GPIOA
#define Record_Enable_Pin GPIO_PIN_7
#define Record_Enable_GPIO_Port GPIOE
#define Acq_Busy_Pin GPIO_PIN_8
#define Acq_Busy_GPIO_Port GPIOE
#define GAINA0_Pin GPIO_PIN_10
#define GAINA0_GPIO_Port GPIOD
#define GAINA1_Pin GPIO_PIN_11
#define GAINA1_GPIO_Port GPIOD
#define GAINB0_Pin GPIO_PIN_6
#define GAINB0_GPIO_Port GPIOC
#define GAINB1_Pin GPIO_PIN_7
#define GAINB1_GPIO_Port GPIOC
#define GAINB2_Pin GPIO_PIN_8
#define GAINB2_GPIO_Port GPIOC
#define RED_LED_Pin GPIO_PIN_5
#define RED_LED_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
