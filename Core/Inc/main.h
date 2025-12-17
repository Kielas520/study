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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
extern uint8_t Key_Val;
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
#define MOTOR_STBY_Pin GPIO_PIN_5
#define MOTOR_STBY_GPIO_Port GPIOA
#define MOTOR_IN1_Pin GPIO_PIN_6
#define MOTOR_IN1_GPIO_Port GPIOA
#define MOTOR_IN2_Pin GPIO_PIN_7
#define MOTOR_IN2_GPIO_Port GPIOA
#define OLED_GND_Pin GPIO_PIN_0
#define OLED_GND_GPIO_Port GPIOB
#define OLED_VCC_Pin GPIO_PIN_1
#define OLED_VCC_GPIO_Port GPIOB
#define GPIO_INC_Pin GPIO_PIN_13
#define GPIO_INC_GPIO_Port GPIOB
#define GPIO_INC_EXTI_IRQn EXTI15_10_IRQn
#define GPIO_DEC_Pin GPIO_PIN_15
#define GPIO_DEC_GPIO_Port GPIOB
#define GPIO_DEC_EXTI_IRQn EXTI15_10_IRQn
#define GPIO_SWITCH_Pin GPIO_PIN_3
#define GPIO_SWITCH_GPIO_Port GPIOB
#define GPIO_SWITCH_EXTI_IRQn EXTI3_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
