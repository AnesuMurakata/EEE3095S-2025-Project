/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : uart_handler.c
  * @brief          : UART communication handler implementation
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

/* Includes ------------------------------------------------------------------*/
#include "uart_handler.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
char uart_rx_buffer[UART_RX_BUFFER_SIZE];
char uart_tx_buffer[UART_TX_BUFFER_SIZE];
uint16_t rx_buffer_index = 0;
uint8_t rx_complete = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  Initialize UART handler
  * @retval None
  */
void UART_Init(void)
{
    /* USER CODE BEGIN UART_Init */
    // Clear buffers
    memset(uart_rx_buffer, 0, UART_RX_BUFFER_SIZE);
    memset(uart_tx_buffer, 0, UART_TX_BUFFER_SIZE);
    rx_buffer_index = 0;
    rx_complete = 0;
    
    // Start receiving data
    HAL_UART_Receive_IT(&huart1, (uint8_t*)&uart_rx_buffer[rx_buffer_index], 1);
    /* USER CODE END UART_Init */
}

/**
  * @brief  Send a string via UART
  * @param  str: Pointer to string to send
  * @retval None
  */
void UART_SendString(char* str)
{
    /* USER CODE BEGIN UART_SendString */
    HAL_UART_Transmit(&huart1, (uint8_t*)str, strlen(str), HAL_MAX_DELAY);
    /* USER CODE END UART_SendString */
}

/**
  * @brief  Send a single character via UART
  * @param  ch: Character to send
  * @retval None
  */
void UART_SendChar(char ch)
{
    /* USER CODE BEGIN UART_SendChar */
    HAL_UART_Transmit(&huart1, (uint8_t*)&ch, 1, HAL_MAX_DELAY);
    /* USER CODE END UART_SendChar */
}

/**
  * @brief  Receive a string via UART (blocking)
  * @param  buffer: Buffer to store received string
  * @param  maxLength: Maximum length to receive
  * @retval None
  */
void UART_ReceiveString(char* buffer, uint16_t maxLength)
{
    /* USER CODE BEGIN UART_ReceiveString */
    HAL_UART_Receive(&huart1, (uint8_t*)buffer, maxLength, HAL_MAX_DELAY);
    /* USER CODE END UART_ReceiveString */
}

/**
  * @brief  Process received UART data
  * @retval None
  */
void UART_ProcessReceivedData(void)
{
    /* USER CODE BEGIN UART_ProcessReceivedData */
    if (rx_complete)
    {
        // Process the received data
        // For now, just echo it back
        UART_SendString("Echo: ");
        UART_SendString(uart_rx_buffer);
        UART_SendString("\r\n");
        
        // Clear buffer and reset
        memset(uart_rx_buffer, 0, UART_RX_BUFFER_SIZE);
        rx_buffer_index = 0;
        rx_complete = 0;
        
        // Restart receiving
        HAL_UART_Receive_IT(&huart1, (uint8_t*)&uart_rx_buffer[rx_buffer_index], 1);
    }
    /* USER CODE END UART_ProcessReceivedData */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  UART receive complete callback
  * @param  huart: UART handle
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    /* USER CODE BEGIN HAL_UART_RxCpltCallback */
    if (huart->Instance == USART1)
    {
        // Check for end of line
        if (uart_rx_buffer[rx_buffer_index] == '\r' || uart_rx_buffer[rx_buffer_index] == '\n')
        {
            uart_rx_buffer[rx_buffer_index] = '\0'; // Null terminate
            rx_complete = 1;
        }
        else
        {
            rx_buffer_index++;
            if (rx_buffer_index >= UART_RX_BUFFER_SIZE - 1)
            {
                // Buffer overflow, reset
                rx_buffer_index = 0;
                memset(uart_rx_buffer, 0, UART_RX_BUFFER_SIZE);
            }
            else
            {
                // Continue receiving
                HAL_UART_Receive_IT(&huart1, (uint8_t*)&uart_rx_buffer[rx_buffer_index], 1);
            }
        }
    }
    /* USER CODE END HAL_UART_RxCpltCallback */
}

/* USER CODE BEGIN 5 */

/* USER CODE END 5 */
