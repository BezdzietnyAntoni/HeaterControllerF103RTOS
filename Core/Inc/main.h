/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#define BUILDIN_LED_Pin GPIO_PIN_13
#define BUILDIN_LED_GPIO_Port GPIOC
#define OW_Pin GPIO_PIN_2
#define OW_GPIO_Port GPIOA
#define TM1638_STB_Pin GPIO_PIN_3
#define TM1638_STB_GPIO_Port GPIOA
#define TM1638_CLK_Pin GPIO_PIN_4
#define TM1638_CLK_GPIO_Port GPIOA
#define TM1638_DIO_Pin GPIO_PIN_5
#define TM1638_DIO_GPIO_Port GPIOA
#define ENC_CH1_Pin GPIO_PIN_6
#define ENC_CH1_GPIO_Port GPIOA
#define ENC_CH2_Pin GPIO_PIN_7
#define ENC_CH2_GPIO_Port GPIOA
#define ENC_SW_Pin GPIO_PIN_0
#define ENC_SW_GPIO_Port GPIOB
#define ENC_SW_EXTI_IRQn EXTI0_IRQn
#define RELAY_IN0_Pin GPIO_PIN_1
#define RELAY_IN0_GPIO_Port GPIOB
#define RELAY_IN1_Pin GPIO_PIN_2
#define RELAY_IN1_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
extern TIM_HandleTypeDef htim1; //TODO : find better solution
extern TIM_HandleTypeDef htim3; //TODO : find better solution

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
