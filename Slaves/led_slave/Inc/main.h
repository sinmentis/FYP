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
#define d6_Pin GPIO_PIN_2
#define d6_GPIO_Port GPIOE
#define d7_Pin GPIO_PIN_3
#define d7_GPIO_Port GPIOE
#define con3_7_Pin GPIO_PIN_2
#define con3_7_GPIO_Port GPIOF
#define con3_5_Pin GPIO_PIN_4
#define con3_5_GPIO_Port GPIOF
#define con3_3_Pin GPIO_PIN_6
#define con3_3_GPIO_Port GPIOF
#define con3_1_Pin GPIO_PIN_10
#define con3_1_GPIO_Port GPIOF
#define con2_6_Pin GPIO_PIN_0
#define con2_6_GPIO_Port GPIOC
#define con2_5_Pin GPIO_PIN_1
#define con2_5_GPIO_Port GPIOC
#define con2_4_Pin GPIO_PIN_2
#define con2_4_GPIO_Port GPIOC
#define con2_3_Pin GPIO_PIN_3
#define con2_3_GPIO_Port GPIOC
#define con2_2_Pin GPIO_PIN_4
#define con2_2_GPIO_Port GPIOC
#define con2_1_Pin GPIO_PIN_5
#define con2_1_GPIO_Port GPIOC
#define d2_Pin GPIO_PIN_11
#define d2_GPIO_Port GPIOF
#define d3_Pin GPIO_PIN_12
#define d3_GPIO_Port GPIOF
#define d4_Pin GPIO_PIN_13
#define d4_GPIO_Port GPIOF
#define d5_Pin GPIO_PIN_14
#define d5_GPIO_Port GPIOF
#define uart_tx_Pin GPIO_PIN_8
#define uart_tx_GPIO_Port GPIOD
#define uat_rx_Pin GPIO_PIN_9
#define uat_rx_GPIO_Port GPIOD
#define con2_7_Pin GPIO_PIN_12
#define con2_7_GPIO_Port GPIOD
#define con2_8_Pin GPIO_PIN_13
#define con2_8_GPIO_Port GPIOD
#define con2_9_Pin GPIO_PIN_14
#define con2_9_GPIO_Port GPIOD
#define con2_10_Pin GPIO_PIN_15
#define con2_10_GPIO_Port GPIOD
#define d9_Pin GPIO_PIN_15
#define d9_GPIO_Port GPIOA
#define d8_Pin GPIO_PIN_10
#define d8_GPIO_Port GPIOC
#define board_type_4_Pin GPIO_PIN_11
#define board_type_4_GPIO_Port GPIOC
#define board_type_3_Pin GPIO_PIN_12
#define board_type_3_GPIO_Port GPIOC
#define board_type_2_Pin GPIO_PIN_0
#define board_type_2_GPIO_Port GPIOD
#define board_type_1_Pin GPIO_PIN_1
#define board_type_1_GPIO_Port GPIOD
#define board_num_4_Pin GPIO_PIN_2
#define board_num_4_GPIO_Port GPIOD
#define board_num_3_Pin GPIO_PIN_3
#define board_num_3_GPIO_Port GPIOD
#define board_num_2_Pin GPIO_PIN_4
#define board_num_2_GPIO_Port GPIOD
#define board_num_1_Pin GPIO_PIN_5
#define board_num_1_GPIO_Port GPIOD
#define status_led_Pin GPIO_PIN_3
#define status_led_GPIO_Port GPIOB
#define test3_Pin GPIO_PIN_9
#define test3_GPIO_Port GPIOB
#define test2_Pin GPIO_PIN_0
#define test2_GPIO_Port GPIOE
#define test1_Pin GPIO_PIN_1
#define test1_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
