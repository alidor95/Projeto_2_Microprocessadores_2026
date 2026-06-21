/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_PILOTO_AUTO_Pin GPIO_PIN_13
#define LED_PILOTO_AUTO_GPIO_Port GPIOC
#define TACOMETRO_Pin GPIO_PIN_0
#define TACOMETRO_GPIO_Port GPIOA
#define POTENCIOMETRO_Pin GPIO_PIN_1
#define POTENCIOMETRO_GPIO_Port GPIOA
#define ULTRASSON_ECHO_Pin GPIO_PIN_2
#define ULTRASSON_ECHO_GPIO_Port GPIOA
#define ULTRASSON_GATE_Pin GPIO_PIN_3
#define ULTRASSON_GATE_GPIO_Port GPIOA
#define RELE_Pin GPIO_PIN_4
#define RELE_GPIO_Port GPIOA
#define LED_TEMP_Pin GPIO_PIN_0
#define LED_TEMP_GPIO_Port GPIOB
#define LED_SETAS_Pin GPIO_PIN_1
#define LED_SETAS_GPIO_Port GPIOB
#define PONTO_D_Pin GPIO_PIN_10
#define PONTO_D_GPIO_Port GPIOB
#define SELETOR_1_Pin GPIO_PIN_12
#define SELETOR_1_GPIO_Port GPIOB
#define SELETOR_2_Pin GPIO_PIN_13
#define SELETOR_2_GPIO_Port GPIOB
#define FREIO_Pin GPIO_PIN_14
#define FREIO_GPIO_Port GPIOB
#define SELETOR_3_Pin GPIO_PIN_15
#define SELETOR_3_GPIO_Port GPIOB
#define MOTOR_PWM_Pin GPIO_PIN_8
#define MOTOR_PWM_GPIO_Port GPIOA
#define FUNCAO_Pin GPIO_PIN_9
#define FUNCAO_GPIO_Port GPIOA
#define SELECIONA_Pin GPIO_PIN_10
#define SELECIONA_GPIO_Port GPIOA
#define ACELERADOR_Pin GPIO_PIN_15
#define ACELERADOR_GPIO_Port GPIOA
#define SEG_A_Pin GPIO_PIN_3
#define SEG_A_GPIO_Port GPIOB
#define SEG_B_Pin GPIO_PIN_4
#define SEG_B_GPIO_Port GPIOB
#define SEG_C_Pin GPIO_PIN_5
#define SEG_C_GPIO_Port GPIOB
#define SEG_D_Pin GPIO_PIN_6
#define SEG_D_GPIO_Port GPIOB
#define SEG_E_Pin GPIO_PIN_7
#define SEG_E_GPIO_Port GPIOB
#define SEG_F_Pin GPIO_PIN_8
#define SEG_F_GPIO_Port GPIOB
#define SEG_G_Pin GPIO_PIN_9
#define SEG_G_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
