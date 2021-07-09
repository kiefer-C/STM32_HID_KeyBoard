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
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
//#include "stm32f103xb.h" 
//#include "stm32f1xx_hal_def.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;
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
#define BUFFER_SIZE  100
extern uint8_t rx_buffer_3[BUFFER_SIZE];
extern uint8_t aRxBuffer[1];
extern uint8_t KeyBuff[4];
void CmdHandle(void);
void ReadCustomData(void);
void CustomDataWriteToFlash(uint32_t DATA_64);
void UART_IRQ(UART_HandleTypeDef *huart);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SW2_Pin GPIO_PIN_13
#define SW2_GPIO_Port GPIOC
#define SW2_EXTI_IRQn EXTI15_10_IRQn
#define SW1_Pin GPIO_PIN_0
#define SW1_GPIO_Port GPIOA
#define SW1_EXTI_IRQn EXTI0_IRQn
#define LED_1_Pin GPIO_PIN_1
#define LED_1_GPIO_Port GPIOA
#define LED_2_Pin GPIO_PIN_2
#define LED_2_GPIO_Port GPIOA
#define LED_3_Pin GPIO_PIN_3
#define LED_3_GPIO_Port GPIOA
#define Touch_Pin GPIO_PIN_1
#define Touch_GPIO_Port GPIOB
#define Touch_EXTI_IRQn EXTI1_IRQn
#define RES_Pin GPIO_PIN_8
#define RES_GPIO_Port GPIOA
#define DC_Pin GPIO_PIN_9
#define DC_GPIO_Port GPIOA
#define CS_Pin GPIO_PIN_10
#define CS_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
