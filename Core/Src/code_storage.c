/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : code_storage.c
  * @brief          : Flash storage implementation for access codes
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
#include "code_storage.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
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
static access_codes_t current_codes;
static uint8_t codes_loaded = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
static uint8_t CodeStorage_EraseSector(void);
static uint8_t CodeStorage_ProgramFlash(access_codes_t* codes);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  Initialize code storage system
  * @retval None
  */
void CodeStorage_Init(void)
{
    /* USER CODE BEGIN CodeStorage_Init */
    // Initialize current codes structure
    memset(&current_codes, 0, sizeof(access_codes_t));
    codes_loaded = 0;
    
    // Try to read codes from flash
    if (CodeStorage_ReadCodes(&current_codes) == 0)
    {
        codes_loaded = 1;
    }
    else
    {
        // Flash is empty or corrupted, initialize with empty codes
        current_codes.magic_number = MAGIC_NUMBER;
        codes_loaded = 1;
    }
    /* USER CODE END CodeStorage_Init */
}

/**
  * @brief  Read access codes from flash memory
  * @param  codes: Pointer to structure to store codes
  * @retval 0 if successful, 1 if error
  */
uint8_t CodeStorage_ReadCodes(access_codes_t* codes)
{
    /* USER CODE BEGIN CodeStorage_ReadCodes */
    if (codes == NULL)
        return 1;
    
    // Read data from flash
    memcpy(codes, (void*)FLASH_DATA_ADDRESS, sizeof(access_codes_t));
    
    // Validate the data
    if (!CodeStorage_ValidateFlashData(codes))
    {
        return 1; // Invalid data
    }
    
    return 0; // Success
    /* USER CODE END CodeStorage_ReadCodes */
}

/**
  * @brief  Write access codes to flash memory
  * @param  codes: Pointer to structure containing codes to write
  * @retval 0 if successful, 1 if error
  */
uint8_t CodeStorage_WriteCodes(access_codes_t* codes)
{
    /* USER CODE BEGIN CodeStorage_WriteCodes */
    if (codes == NULL)
        return 1;
    
    // Validate the data before writing
    if (!CodeStorage_ValidateFlashData(codes))
    {
        return 1; // Invalid data
    }
    
    // Erase the sector first
    if (CodeStorage_EraseSector() != 0)
    {
        return 1; // Erase failed
    }
    
    // Program the flash
    if (CodeStorage_ProgramFlash(codes) != 0)
    {
        return 1; // Program failed
    }
    
    // Update current codes in RAM
    memcpy(&current_codes, codes, sizeof(access_codes_t));
    
    return 0; // Success
    /* USER CODE END CodeStorage_WriteCodes */
}

/**
  * @brief  Get a specific access code
  * @param  codeIndex: Index of code to retrieve (0, 1, or 2)
  * @param  codeBuffer: Buffer to store the code
  * @retval 0 if successful, 1 if error
  */
uint8_t CodeStorage_GetCode(uint8_t codeIndex, char* codeBuffer)
{
    /* USER CODE BEGIN CodeStorage_GetCode */
    if (codeIndex >= 3 || codeBuffer == NULL)
        return 1;
    
    if (!codes_loaded)
        return 1;
    
    // Copy the requested code
    strncpy(codeBuffer, current_codes.code1 + (codeIndex * 32), MAX_CODE_LENGTH - 1);
    codeBuffer[MAX_CODE_LENGTH - 1] = '\0'; // Ensure null termination
    
    return 0; // Success
    /* USER CODE END CodeStorage_GetCode */
}

/**
  * @brief  Set a specific access code
  * @param  codeIndex: Index of code to set (0, 1, or 2)
  * @param  newCode: New code value
  * @retval 0 if successful, 1 if error
  */
uint8_t CodeStorage_SetCode(uint8_t codeIndex, char* newCode)
{
    /* USER CODE BEGIN CodeStorage_SetCode */
    if (codeIndex >= 3 || newCode == NULL)
        return 1;
    
    if (!codes_loaded)
        return 1;
    
    // Validate code length
    if (strlen(newCode) >= MAX_CODE_LENGTH)
        return 1;
    
    // Update the code in RAM
    strncpy(current_codes.code1 + (codeIndex * 32), newCode, MAX_CODE_LENGTH - 1);
    current_codes.code1[codeIndex * 32 + MAX_CODE_LENGTH - 1] = '\0';
    
    // Write to flash
    if (CodeStorage_WriteCodes(&current_codes) != 0)
    {
        return 1; // Flash write failed
    }
    
    return 0; // Success
    /* USER CODE END CodeStorage_SetCode */
}

/**
  * @brief  Check if a specific code is set
  * @param  codeIndex: Index of code to check (0, 1, or 2)
  * @retval 1 if code is set, 0 if not set
  */
uint8_t CodeStorage_IsCodeSet(uint8_t codeIndex)
{
    /* USER CODE BEGIN CodeStorage_IsCodeSet */
    if (codeIndex >= 3 || !codes_loaded)
        return 0;
    
    char* code_ptr = current_codes.code1 + (codeIndex * 32);
    return (strlen(code_ptr) > 0) ? 1 : 0;
    /* USER CODE END CodeStorage_IsCodeSet */
}

/**
  * @brief  Clear all access codes
  * @retval None
  */
void CodeStorage_ClearAllCodes(void)
{
    /* USER CODE BEGIN CodeStorage_ClearAllCodes */
    // Clear all codes in RAM
    memset(&current_codes, 0, sizeof(access_codes_t));
    current_codes.magic_number = MAGIC_NUMBER;
    
    // Write to flash
    CodeStorage_WriteCodes(&current_codes);
    /* USER CODE END CodeStorage_ClearAllCodes */
}

/**
  * @brief  Validate flash data integrity
  * @param  codes: Pointer to codes structure to validate
  * @retval 1 if valid, 0 if invalid
  */
uint8_t CodeStorage_ValidateFlashData(access_codes_t* codes)
{
    /* USER CODE BEGIN CodeStorage_ValidateFlashData */
    if (codes == NULL)
        return 0;
    
    // Check magic number
    if (codes->magic_number != MAGIC_NUMBER)
        return 0;
    
    // Validate code lengths
    for (int i = 0; i < 3; i++)
    {
        char* code_ptr = codes->code1 + (i * 32);
        if (strlen(code_ptr) >= MAX_CODE_LENGTH)
            return 0;
    }
    
    return 1; // Valid
    /* USER CODE END CodeStorage_ValidateFlashData */
}

/* USER CODE BEGIN 4 */
/**
  * @brief  Erase flash sector for data storage
  * @retval 0 if successful, 1 if error
  */
static uint8_t CodeStorage_EraseSector(void)
{
    /* USER CODE BEGIN CodeStorage_EraseSector */
    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t SectorError = 0;
    
    // Unlock the Flash
    HAL_FLASH_Unlock();
    
    // Clear any pending flags
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGSERR);
    
    // Configure erase operation
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.Sector = FLASH_DATA_SECTOR;
    EraseInitStruct.NbSectors = 1;
    EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    
    // Perform erase
    if (HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK)
    {
        HAL_FLASH_Lock();
        return 1; // Erase failed
    }
    
    HAL_FLASH_Lock();
    return 0; // Success
    /* USER CODE END CodeStorage_EraseSector */
}

/**
  * @brief  Program flash memory with codes data
  * @param  codes: Pointer to codes structure to program
  * @retval 0 if successful, 1 if error
  */
static uint8_t CodeStorage_ProgramFlash(access_codes_t* codes)
{
    /* USER CODE BEGIN CodeStorage_ProgramFlash */
    uint32_t* data_ptr = (uint32_t*)codes;
    uint32_t data_size = sizeof(access_codes_t);
    uint32_t words_to_write = (data_size + 3) / 4; // Round up to word boundary
    
    // Unlock the Flash
    HAL_FLASH_Unlock();
    
    // Program the data word by word
    for (uint32_t i = 0; i < words_to_write; i++)
    {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, FLASH_DATA_ADDRESS + (i * 4), data_ptr[i]) != HAL_OK)
        {
            HAL_FLASH_Lock();
            return 1; // Program failed
        }
    }
    
    HAL_FLASH_Lock();
    return 0; // Success
    /* USER CODE END CodeStorage_ProgramFlash */
}
/* USER CODE END 4 */

/* USER CODE BEGIN 5 */

/* USER CODE END 5 */
