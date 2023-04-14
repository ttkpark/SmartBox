/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32l4xx_hal_conf.h"
#include "stm32l4xx_it.h"
#include <stdlib.h>
#include <string.h>
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
#define LORA_DIO1_Pin GPIO_PIN_0
#define LORA_DIO1_GPIO_Port GPIOA
#define LORA_DIO1_EXTI_IRQn EXTI0_IRQn
#define NRF_CE_Pin GPIO_PIN_3
#define NRF_CE_GPIO_Port GPIOA
#define NRF_CSN_Pin GPIO_PIN_4
#define NRF_CSN_GPIO_Port GPIOA
#define LORA_NSS_Pin GPIO_PIN_0
#define LORA_NSS_GPIO_Port GPIOB
#define LORA_DIO0_Pin GPIO_PIN_1
#define LORA_DIO0_GPIO_Port GPIOB
#define LORA_DIO0_EXTI_IRQn EXTI1_IRQn
#define DebugLED_Pin GPIO_PIN_8
#define DebugLED_GPIO_Port GPIOA
#define SW_Reset_Pin GPIO_PIN_11
#define SW_Reset_GPIO_Port GPIOA
#define SW_Reset_EXTI_IRQn EXTI15_10_IRQn
#define SW_Emergency_Pin GPIO_PIN_12
#define SW_Emergency_GPIO_Port GPIOA
#define SW_Emergency_EXTI_IRQn EXTI15_10_IRQn
#define DSENSOR_SET_Pin GPIO_PIN_15
#define DSENSOR_SET_GPIO_Port GPIOA
#define LED_RED_Pin GPIO_PIN_4
#define LED_RED_GPIO_Port GPIOB
#define LED_GREEN_Pin GPIO_PIN_5
#define LED_GREEN_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/