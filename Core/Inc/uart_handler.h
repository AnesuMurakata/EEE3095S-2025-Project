/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : uart_handler.h
  * @brief          : Header for uart_handler.c file.
  *                   This file contains the common defines and function prototypes
  *                   for UART communication handling.
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
#ifndef __UART_HANDLER_H
#define __UART_HANDLER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

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
void UART_Init(void);
void UART_SendString(char* str);
void UART_SendChar(char ch);
void UART_ReceiveString(char* buffer, uint16_t maxLength);
void UART_ProcessReceivedData(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */
#define UART_RX_BUFFER_SIZE 256
#define UART_TX_BUFFER_SIZE 256

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __UART_HANDLER_H */
