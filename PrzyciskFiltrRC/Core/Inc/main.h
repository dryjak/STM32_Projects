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

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LD2_Pin GPIO_PIN_13
#define LD2_GPIO_Port GPIOC
#define CzujnikL_Pin GPIO_PIN_0
#define CzujnikL_GPIO_Port GPIOA
#define CzujnikS_Pin GPIO_PIN_1
#define CzujnikS_GPIO_Port GPIOA
#define CzujnikP_Pin GPIO_PIN_2
#define CzujnikP_GPIO_Port GPIOA
#define PWM1_Pin GPIO_PIN_8
#define PWM1_GPIO_Port GPIOA
#define LedP_Pin GPIO_PIN_3
#define LedP_GPIO_Port GPIOB
#define LedS_Pin GPIO_PIN_4
#define LedS_GPIO_Port GPIOB
#define LedL_Pin GPIO_PIN_5
#define LedL_GPIO_Port GPIOB
#define Dir1_Pin GPIO_PIN_6
#define Dir1_GPIO_Port GPIOB
#define Dir2_Pin GPIO_PIN_7
#define Dir2_GPIO_Port GPIOB
#define DirMotor2V1_Pin GPIO_PIN_8
#define DirMotor2V1_GPIO_Port GPIOB
#define DirMotor2V2_Pin GPIO_PIN_9
#define DirMotor2V2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
