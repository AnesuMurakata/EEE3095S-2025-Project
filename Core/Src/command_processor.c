/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : command_processor.c
  * @brief          : Command processor implementation for message protocol
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
#include "command_processor.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
#include "code_storage.h"
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
uint8_t connection_status = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  Initialize command processor
  * @retval None
  */
void CommandProcessor_Init(void)
{
    /* USER CODE BEGIN CommandProcessor_Init */
    // Initialize code storage system
    CodeStorage_Init();
    connection_status = 0;
    /* USER CODE END CommandProcessor_Init */
}

/**
  * @brief  Process incoming command
  * @param  command: Command string to process
  * @retval None
  */
void CommandProcessor_ProcessCommand(char* command)
{
    /* USER CODE BEGIN CommandProcessor_ProcessCommand */
    command_type_t cmd_type = CommandProcessor_ParseCommand(command);
    
    switch (cmd_type)
    {
        case CMD_CONNECT:
            CommandProcessor_HandleConnect();
            break;
            
        case CMD_GET_CODE_1:
            CommandProcessor_HandleGetCode(0);
            break;
            
        case CMD_GET_CODE_2:
            CommandProcessor_HandleGetCode(1);
            break;
            
        case CMD_GET_CODE_3:
            CommandProcessor_HandleGetCode(2);
            break;
            
        case CMD_SET_CODE_1:
            {
                char* param = CommandProcessor_ExtractParameter(command);
                if (param != NULL)
                {
                    CommandProcessor_HandleSetCode(0, param);
                }
                else
                {
                    CommandProcessor_SendResponse(RESP_ERROR_INVALID_PARAM, NULL);
                }
            }
            break;
            
        case CMD_SET_CODE_2:
            {
                char* param = CommandProcessor_ExtractParameter(command);
                if (param != NULL)
                {
                    CommandProcessor_HandleSetCode(1, param);
                }
                else
                {
                    CommandProcessor_SendResponse(RESP_ERROR_INVALID_PARAM, NULL);
                }
            }
            break;
            
        case CMD_SET_CODE_3:
            {
                char* param = CommandProcessor_ExtractParameter(command);
                if (param != NULL)
                {
                    CommandProcessor_HandleSetCode(2, param);
                }
                else
                {
                    CommandProcessor_SendResponse(RESP_ERROR_INVALID_PARAM, NULL);
                }
            }
            break;
            
        case CMD_DISCONNECT:
            CommandProcessor_HandleDisconnect();
            break;
            
        case CMD_UNKNOWN:
        default:
            CommandProcessor_SendResponse(RESP_ERROR_INVALID_CMD, NULL);
            break;
    }
    /* USER CODE END CommandProcessor_ProcessCommand */
}

/**
  * @brief  Send response via UART
  * @param  response: Response type
  * @param  data: Additional data (can be NULL)
  * @retval None
  */
void CommandProcessor_SendResponse(response_type_t response, char* data)
{
    /* USER CODE BEGIN CommandProcessor_SendResponse */
    char response_buffer[MAX_RESPONSE_LENGTH];
    
    switch (response)
    {
        case RESP_OK:
            if (data != NULL)
            {
                snprintf(response_buffer, MAX_RESPONSE_LENGTH, "%s:%s\r\n", RESP_OK_STR, data);
            }
            else
            {
                snprintf(response_buffer, MAX_RESPONSE_LENGTH, "%s\r\n", RESP_OK_STR);
            }
            break;
            
        case RESP_ERROR:
            snprintf(response_buffer, MAX_RESPONSE_LENGTH, "%s\r\n", RESP_ERROR_STR);
            break;
            
        case RESP_ERROR_NOT_SET:
            snprintf(response_buffer, MAX_RESPONSE_LENGTH, "%s\r\n", RESP_ERROR_NOT_SET_STR);
            break;
            
        case RESP_ERROR_INVALID_CMD:
            snprintf(response_buffer, MAX_RESPONSE_LENGTH, "%s\r\n", RESP_ERROR_INVALID_CMD_STR);
            break;
            
        case RESP_ERROR_INVALID_PARAM:
            snprintf(response_buffer, MAX_RESPONSE_LENGTH, "%s\r\n", RESP_ERROR_INVALID_PARAM_STR);
            break;
            
        default:
            snprintf(response_buffer, MAX_RESPONSE_LENGTH, "%s\r\n", RESP_ERROR_STR);
            break;
    }
    
    UART_SendString(response_buffer);
    /* USER CODE END CommandProcessor_SendResponse */
}

/**
  * @brief  Parse command string to determine command type
  * @param  command: Command string
  * @retval Command type
  */
command_type_t CommandProcessor_ParseCommand(char* command)
{
    /* USER CODE BEGIN CommandProcessor_ParseCommand */
    if (strncmp(command, CMD_CONNECT_STR, strlen(CMD_CONNECT_STR)) == 0)
        return CMD_CONNECT;
    else if (strncmp(command, CMD_GET_CODE_1_STR, strlen(CMD_GET_CODE_1_STR)) == 0)
        return CMD_GET_CODE_1;
    else if (strncmp(command, CMD_GET_CODE_2_STR, strlen(CMD_GET_CODE_2_STR)) == 0)
        return CMD_GET_CODE_2;
    else if (strncmp(command, CMD_GET_CODE_3_STR, strlen(CMD_GET_CODE_3_STR)) == 0)
        return CMD_GET_CODE_3;
    else if (strncmp(command, CMD_SET_CODE_1_STR, strlen(CMD_SET_CODE_1_STR)) == 0)
        return CMD_SET_CODE_1;
    else if (strncmp(command, CMD_SET_CODE_2_STR, strlen(CMD_SET_CODE_2_STR)) == 0)
        return CMD_SET_CODE_2;
    else if (strncmp(command, CMD_SET_CODE_3_STR, strlen(CMD_SET_CODE_3_STR)) == 0)
        return CMD_SET_CODE_3;
    else if (strncmp(command, CMD_DISCONNECT_STR, strlen(CMD_DISCONNECT_STR)) == 0)
        return CMD_DISCONNECT;
    else
        return CMD_UNKNOWN;
    /* USER CODE END CommandProcessor_ParseCommand */
}

/**
  * @brief  Extract parameter from command string
  * @param  command: Command string with parameter
  * @retval Pointer to parameter string (NULL if no parameter)
  */
char* CommandProcessor_ExtractParameter(char* command)
{
    /* USER CODE BEGIN CommandProcessor_ExtractParameter */
    char* colon_pos = strchr(command, ':');
    if (colon_pos != NULL)
    {
        return colon_pos + 1; // Return string after ':'
    }
    return NULL;
    /* USER CODE END CommandProcessor_ExtractParameter */
}

/**
  * @brief  Handle CONNECT command
  * @retval None
  */
void CommandProcessor_HandleConnect(void)
{
    /* USER CODE BEGIN CommandProcessor_HandleConnect */
    connection_status = 1;
    CommandProcessor_SendResponse(RESP_OK, NULL);
    /* USER CODE END CommandProcessor_HandleConnect */
}

/**
  * @brief  Handle GET_CODE command
  * @param  codeIndex: Index of code to retrieve (0, 1, or 2)
  * @retval None
  */
void CommandProcessor_HandleGetCode(uint8_t codeIndex)
{
    /* USER CODE BEGIN CommandProcessor_HandleGetCode */
    if (!connection_status)
    {
        CommandProcessor_SendResponse(RESP_ERROR, NULL);
        return;
    }
    
    if (codeIndex >= 3)
    {
        CommandProcessor_SendResponse(RESP_ERROR_INVALID_PARAM, NULL);
        return;
    }
    
    // Check if code is set using flash storage
    if (!CodeStorage_IsCodeSet(codeIndex))
    {
        CommandProcessor_SendResponse(RESP_ERROR_NOT_SET, NULL);
    }
    else
    {
        char code_buffer[MAX_CODE_LENGTH];
        if (CodeStorage_GetCode(codeIndex, code_buffer) == 0)
        {
            CommandProcessor_SendResponse(RESP_OK, code_buffer);
        }
        else
        {
            CommandProcessor_SendResponse(RESP_ERROR, NULL);
        }
    }
    /* USER CODE END CommandProcessor_HandleGetCode */
}

/**
  * @brief  Handle SET_CODE command
  * @param  codeIndex: Index of code to set (0, 1, or 2)
  * @param  newCode: New code value
  * @retval None
  */
void CommandProcessor_HandleSetCode(uint8_t codeIndex, char* newCode)
{
    /* USER CODE BEGIN CommandProcessor_HandleSetCode */
    if (!connection_status)
    {
        CommandProcessor_SendResponse(RESP_ERROR, NULL);
        return;
    }
    
    if (codeIndex >= 3)
    {
        CommandProcessor_SendResponse(RESP_ERROR_INVALID_PARAM, NULL);
        return;
    }
    
    if (newCode == NULL || strlen(newCode) == 0)
    {
        CommandProcessor_SendResponse(RESP_ERROR_INVALID_PARAM, NULL);
        return;
    }
    
    // Validate code length
    if (strlen(newCode) >= MAX_CODE_LENGTH)
    {
        CommandProcessor_SendResponse(RESP_ERROR_INVALID_PARAM, NULL);
        return;
    }
    
    // Store code using flash storage
    if (CodeStorage_SetCode(codeIndex, newCode) == 0)
    {
        CommandProcessor_SendResponse(RESP_OK, NULL);
    }
    else
    {
        CommandProcessor_SendResponse(RESP_ERROR, NULL);
    }
    /* USER CODE END CommandProcessor_HandleSetCode */
}

/**
  * @brief  Handle DISCONNECT command
  * @retval None
  */
void CommandProcessor_HandleDisconnect(void)
{
    /* USER CODE BEGIN CommandProcessor_HandleDisconnect */
    connection_status = 0;
    CommandProcessor_SendResponse(RESP_OK, NULL);
    /* USER CODE END CommandProcessor_HandleDisconnect */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN 5 */

/* USER CODE END 5 */
