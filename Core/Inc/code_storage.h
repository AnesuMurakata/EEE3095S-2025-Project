/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : code_storage.h
  * @brief          : Header for code_storage.c file.
  *                   This file contains flash storage operations for access codes.
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
#ifndef __CODE_STORAGE_H
#define __CODE_STORAGE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef struct {
    char code1[32];
    char code2[32];
    char code3[32];
    uint32_t magic_number;  // 0x12345678 for validation
} access_codes_t;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
// Flash memory configuration
#define FLASH_DATA_SECTOR        7       // Sector 7 for data storage
#define FLASH_DATA_ADDRESS       0x08060000  // Start address of sector 7
#define FLASH_SECTOR_SIZE        0x20000    // 128KB sector size
#define MAGIC_NUMBER             0x12345678

// Code storage constants
#define MAX_CODE_LENGTH         32
#define CODES_NOT_SET            0
#define CODES_SET                1
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void CodeStorage_Init(void);
uint8_t CodeStorage_ReadCodes(access_codes_t* codes);
uint8_t CodeStorage_WriteCodes(access_codes_t* codes);
uint8_t CodeStorage_GetCode(uint8_t codeIndex, char* codeBuffer);
uint8_t CodeStorage_SetCode(uint8_t codeIndex, char* newCode);
uint8_t CodeStorage_IsCodeSet(uint8_t codeIndex);
void CodeStorage_ClearAllCodes(void);
uint8_t CodeStorage_ValidateFlashData(access_codes_t* codes);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __CODE_STORAGE_H */
