/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#define LED1_Pin GPIO_PIN_2
#define LED1_GPIO_Port GPIOE
#define LED2_Pin GPIO_PIN_3
#define LED2_GPIO_Port GPIOE
#define LED3_Pin GPIO_PIN_4
#define LED3_GPIO_Port GPIOE
#define LED4_Pin GPIO_PIN_5
#define LED4_GPIO_Port GPIOE
#define TEST_Pin GPIO_PIN_6
#define TEST_GPIO_Port GPIOE
#define EN8_Pin GPIO_PIN_0
#define EN8_GPIO_Port GPIOD
#define EN7_Pin GPIO_PIN_1
#define EN7_GPIO_Port GPIOD
#define EN6_Pin GPIO_PIN_2
#define EN6_GPIO_Port GPIOD
#define EN5_Pin GPIO_PIN_3
#define EN5_GPIO_Port GPIOD
#define EN4_Pin GPIO_PIN_4
#define EN4_GPIO_Port GPIOD
#define EN3_Pin GPIO_PIN_5
#define EN3_GPIO_Port GPIOD
#define EN2_Pin GPIO_PIN_6
#define EN2_GPIO_Port GPIOD
#define EN1_Pin GPIO_PIN_7
#define EN1_GPIO_Port GPIOD
#define RESET2_Pin GPIO_PIN_9
#define RESET2_GPIO_Port GPIOG
#define RESET1_Pin GPIO_PIN_10
#define RESET1_GPIO_Port GPIOG
#define OUT4_Pin GPIO_PIN_11
#define OUT4_GPIO_Port GPIOG
#define OUT3_Pin GPIO_PIN_12
#define OUT3_GPIO_Port GPIOG
#define OUT2_Pin GPIO_PIN_13
#define OUT2_GPIO_Port GPIOG
#define OUT1_Pin GPIO_PIN_14
#define OUT1_GPIO_Port GPIOG
#define FAULT2_Pin GPIO_PIN_4
#define FAULT2_GPIO_Port GPIOB
#define FAULT1_Pin GPIO_PIN_5
#define FAULT1_GPIO_Port GPIOB
#define RESET_FB2_Pin GPIO_PIN_6
#define RESET_FB2_GPIO_Port GPIOB
#define RESET_FB1_Pin GPIO_PIN_7
#define RESET_FB1_GPIO_Port GPIOB
#define IN4_Pin GPIO_PIN_8
#define IN4_GPIO_Port GPIOB
#define IN3_Pin GPIO_PIN_9
#define IN3_GPIO_Port GPIOB
#define IN2_Pin GPIO_PIN_0
#define IN2_GPIO_Port GPIOE
#define IN1_Pin GPIO_PIN_1
#define IN1_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
