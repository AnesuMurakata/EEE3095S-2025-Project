/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : command_processor.h
  * @brief          : Header for command_processor.c file.
  *                   This file contains the message protocol definitions and
  *                   function prototypes for command processing.
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
#ifndef __COMMAND_PROCESSOR_H
#define __COMMAND_PROCESSOR_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "uart_handler.h"

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef enum {
    CMD_CONNECT = 0,
    CMD_GET_CODE_1,
    CMD_GET_CODE_2,
    CMD_GET_CODE_3,
    CMD_SET_CODE_1,
    CMD_SET_CODE_2,
    CMD_SET_CODE_3,
    CMD_DISCONNECT,
    CMD_UNKNOWN
} command_type_t;

typedef enum {
    RESP_OK = 0,
    RESP_ERROR,
    RESP_ERROR_NOT_SET,
    RESP_ERROR_INVALID_CMD,
    RESP_ERROR_INVALID_PARAM
} response_type_t;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
// Command strings
#define CMD_CONNECT_STR          "CONNECT"
#define CMD_GET_CODE_1_STR       "GET_CODE_1"
#define CMD_GET_CODE_2_STR       "GET_CODE_2"
#define CMD_GET_CODE_3_STR       "GET_CODE_3"
#define CMD_SET_CODE_1_STR       "SET_CODE_1"
#define CMD_SET_CODE_2_STR       "SET_CODE_2"
#define CMD_SET_CODE_3_STR       "SET_CODE_3"
#define CMD_DISCONNECT_STR       "DISCONNECT"

// Response strings
#define RESP_OK_STR              "OK"
#define RESP_ERROR_STR           "ERROR"
#define RESP_ERROR_NOT_SET_STR   "ERROR:NOT_SET"
#define RESP_ERROR_INVALID_CMD_STR "ERROR:INVALID_CMD"
#define RESP_ERROR_INVALID_PARAM_STR "ERROR:INVALID_PARAM"

// Protocol constants
#define MAX_CODE_LENGTH          32
#define MAX_COMMAND_LENGTH       64
#define MAX_RESPONSE_LENGTH      128
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void CommandProcessor_Init(void);
void CommandProcessor_ProcessCommand(char* command);
void CommandProcessor_SendResponse(response_type_t response, char* data);
command_type_t CommandProcessor_ParseCommand(char* command);
char* CommandProcessor_ExtractParameter(char* command);
void CommandProcessor_HandleConnect(void);
void CommandProcessor_HandleGetCode(uint8_t codeIndex);
void CommandProcessor_HandleSetCode(uint8_t codeIndex, char* newCode);
void CommandProcessor_HandleDisconnect(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __COMMAND_PROCESSOR_H */
