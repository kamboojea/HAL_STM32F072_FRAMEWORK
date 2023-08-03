/*
 * internal_flash_test_task.c
 * STM32F103 have 128 PAGES (Page 0 to Page 127) of 1 KB each. This makes up 128 KB Flash Memory
 * Some STM32F103C8 have 64 KB FLASH Memory, so I guess they have Page 0 to Page 63 only.
 *
 *  Created on: 28 Jun 2023
 *      Author: ARahmatinia
 */

/*****************************************************************************
   Libraries
*****************************************************************************/
#include <string.h>
#include "stm32f0xx_hal.h"
#include "internal_flash_driver.h"
#include "internal_flash_test_task.h"
#include "stdio.h"
#include <millis.h>
#include <stdbool.h>

/*****************************************************************************
   Defines
******************************************************************************/
#define SAMPLE_INTERVAL 50

internal_flash_driver_t flash_driver;

/*****************************************************************************
   Functions
******************************************************************************/
HAL_StatusTypeDef flash_task(void)
{
    // Create instance of driver and initialize it

    HAL_StatusTypeDef status = HAL_OK;

    if(flash_driver_init(&flash_driver, START_SECTOR, NUM_SECTOR, flash_driver_erase, flash_driver_write_data, flash_driver_write_data_option_bytes, flash_driver_read_data) != HAL_OK)
    {
    	return HAL_ERROR;
    }

    // Prepare some data to write to flash
    uint16_t data_to_write[] = {0xABCD, 0xDEAD, 0xA5A5, 0xFACA, 0xCAAD, 0xDEBC, 0x5A5A, 0x0000, 0x6543};
    size_t length_data_to_write = sizeof(data_to_write) / sizeof(data_to_write[0]);

    if(flash_driver_erase(&flash_driver) != HAL_OK)
    {
    	return HAL_ERROR;
    }
    // Write the data to flash
    status = flash_driver.write(flash_driver.start_sector, data_to_write, length_data_to_write);

    if (status != HAL_OK)
    {
        return status;
    }

    if (status != HAL_OK)
    {
        return status;
    }

    volatile uint16_t data_to_read[length_data_to_write];

//    HAL_Delay(100);
    status = flash_driver.read(flash_driver.start_sector, data_to_read, length_data_to_write);
    data_to_read [length_data_to_write]= data_to_read[length_data_to_write];
    return status;
}
