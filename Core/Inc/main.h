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

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Emergency_button_Pin GPIO_PIN_1
#define Emergency_button_GPIO_Port GPIOC
#define H2_Analog_Pin GPIO_PIN_2
#define H2_Analog_GPIO_Port GPIOC
#define Speed_Sensor_Pin GPIO_PIN_3
#define Speed_Sensor_GPIO_Port GPIOC
#define Speed_Sensor_EXTI_IRQn EXTI3_IRQn
#define Data_Logger_TX_Pin GPIO_PIN_0
#define Data_Logger_TX_GPIO_Port GPIOA
#define Data_Logger_RX_Pin GPIO_PIN_1
#define Data_Logger_RX_GPIO_Port GPIOA
#define V_Battery_GPIO_Pin GPIO_PIN_4
#define V_Battery_GPIO_GPIO_Port GPIOA
#define Reset_button_Pin GPIO_PIN_4
#define Reset_button_GPIO_Port GPIOC
#define H2_Digital_Pin GPIO_PIN_0
#define H2_Digital_GPIO_Port GPIOB
#define H2_Digital_EXTI_IRQn EXTI0_IRQn
#define Solenoid_Valve_GPIO_Pin GPIO_PIN_9
#define Solenoid_Valve_GPIO_GPIO_Port GPIOC
#define Energy_Flow_TX_Pin GPIO_PIN_9
#define Energy_Flow_TX_GPIO_Port GPIOA
#define Energy_Flow_RX_Pin GPIO_PIN_10
#define Energy_Flow_RX_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define Steering_Wheel_TX_Pin GPIO_PIN_10
#define Steering_Wheel_TX_GPIO_Port GPIOC
#define Steering_Wheel_RX_Pin GPIO_PIN_11
#define Steering_Wheel_RX_GPIO_Port GPIOC
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define Horn_Pin GPIO_PIN_8
#define Horn_GPIO_Port GPIOB
#define Emergency_Relay_GPIO_Pin GPIO_PIN_9
#define Emergency_Relay_GPIO_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
