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
#include "stm32g4xx_hal.h"

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
#define DRV_PWM_LC_Pin GPIO_PIN_13
#define DRV_PWM_LC_GPIO_Port GPIOC
#define SPI1_AUX1_CS_Pin GPIO_PIN_14
#define SPI1_AUX1_CS_GPIO_Port GPIOC
#define DRV_PWM_HC_Pin GPIO_PIN_0
#define DRV_PWM_HC_GPIO_Port GPIOC
#define DRV_PWM_HB_Pin GPIO_PIN_1
#define DRV_PWM_HB_GPIO_Port GPIOC
#define PWM_DRV_HA_Pin GPIO_PIN_2
#define PWM_DRV_HA_GPIO_Port GPIOC
#define SENSE_U_A_Pin GPIO_PIN_3
#define SENSE_U_A_GPIO_Port GPIOC
#define SENSE_U_C_Pin GPIO_PIN_0
#define SENSE_U_C_GPIO_Port GPIOA
#define SENSE_U_B_Pin GPIO_PIN_1
#define SENSE_U_B_GPIO_Port GPIOA
#define DRV_CAL_Pin GPIO_PIN_2
#define DRV_CAL_GPIO_Port GPIOA
#define DRV_EN_Pin GPIO_PIN_3
#define DRV_EN_GPIO_Port GPIOA
#define SPI1_DRV_CS_Pin GPIO_PIN_4
#define SPI1_DRV_CS_GPIO_Port GPIOA
#define DRV_nFAULT_Pin GPIO_PIN_4
#define DRV_nFAULT_GPIO_Port GPIOC
#define DRV_nFAULT_EXTI_IRQn EXTI4_IRQn
#define SPI1_MAG_CS_Pin GPIO_PIN_5
#define SPI1_MAG_CS_GPIO_Port GPIOC
#define DRV_SOA_Pin GPIO_PIN_0
#define DRV_SOA_GPIO_Port GPIOB
#define DRV_SOB_Pin GPIO_PIN_1
#define DRV_SOB_GPIO_Port GPIOB
#define DRV_SOC_Pin GPIO_PIN_2
#define DRV_SOC_GPIO_Port GPIOB
#define EXT_FAN_Pin GPIO_PIN_11
#define EXT_FAN_GPIO_Port GPIOB
#define SENSE_TEMP_BRAKE_Pin GPIO_PIN_12
#define SENSE_TEMP_BRAKE_GPIO_Port GPIOB
#define SENSE_VBAT_Pin GPIO_PIN_13
#define SENSE_VBAT_GPIO_Port GPIOB
#define DRV_PWM_LB_Pin GPIO_PIN_14
#define DRV_PWM_LB_GPIO_Port GPIOB
#define SENSE_USB_VBUS_Pin GPIO_PIN_15
#define SENSE_USB_VBUS_GPIO_Port GPIOB
#define BRAKE_PWM_Pin GPIO_PIN_9
#define BRAKE_PWM_GPIO_Port GPIOC
#define SENSE_TEMP_FET_Pin GPIO_PIN_8
#define SENSE_TEMP_FET_GPIO_Port GPIOA
#define SENSE_TEMP_MOT_Pin GPIO_PIN_9
#define SENSE_TEMP_MOT_GPIO_Port GPIOA
#define LED_DBG_Pin GPIO_PIN_2
#define LED_DBG_GPIO_Port GPIOD
#define LED_PWR_Pin GPIO_PIN_4
#define LED_PWR_GPIO_Port GPIOB
#define DRV_PWM_LA_Pin GPIO_PIN_9
#define DRV_PWM_LA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
