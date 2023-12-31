/*
 * internal_flash_driver.h
 *
 *  Created on: 28 Jun 2023
 *      Author: ARahmatinia
 */

#ifndef FRAMEWORK_DRIVERS_INC_INTERNAL_FLASH_DRIVER_H_
#define FRAMEWORK_DRIVERS_INC_INTERNAL_FLASH_DRIVER_H_

/*****************************************************************************
   Libraries
*****************************************************************************/
#include "stm32f0xx_hal.h"

/*------------------------------------------------------------------------------
 *
 *  Internal flash usage:
 *
 *
 *          0x08020000  +----------------------------+  0x0801FFFF
 *                      |                            |
 *                      |                            |
 *                      |                            |
 *                      |    Storage Area (62KB)     |
 *                      |                            |
 *                      |                            |
 *                      |                            |
 *          0x08010800  +----------------------------+  0x080107FF
 *                      |                            |
 *                      |                            |
 *                      |                            |
 *                      |     Application (62KB)     |
 *                      |                            |
 *                      |                            |
 *                      |                            |
 *          0x08001000  +----------------------------+  0x08000FFF
 *                      |                            |
 *                      |      Bootloader (4KB)      |
 *                      |                            |
 *          0x08000000  +----------------------------+
 *
 *----------------------------------------------------------------------------*/

/*****************************************************************************
   Defines
******************************************************************************/
#define BOOTLOADER_NUM_PAGES			2
#define APPLICATION_NUM_PAGES			31
#define STORAGE_AREA_NUM_PAGES			31

#define BOOTLOADER_SIZE					BOOTLOADER_NUM_PAGES * FLASH_PAGE_SIZE
#define APPLICATION_SIZE				APPLICATION_NUM_PAGES * FLASH_PAGE_SIZE
#define STORAGE_AREA_SIZE				STORAGE_AREA_NUM_PAGES * FLASH_PAGE_SIZE

#define BOOTLOADER_START_ADDRESS		FLASH_BASE
#define APPLICATION_START_ADDRESS		BOOTLOADER_START_ADDRESS + BOOTLOADER_SIZE
#define STORAGE_AREA_START_ADDRESS		APPLICATION_START_ADDRESS + APPLICATION_SIZE

// Firmware update required, stored in the Option flags
#define FIRMWARE_UPDATE_REQUEST_HIGH	0xAA
#define FIRMWARE_UPDATE_REQUEST_LOW		0x55

#define START_SECTOR STORAGE_AREA_START_ADDRESS
#define NUM_SECTOR   APPLICATION_NUM_PAGES

#define NUM_OPTION_BYTES 2
#define DATA0_FLAG 0xAA
#define DATA1_FLAG 0x55

/*****************************************************************************
   Types
*****************************************************************************/
typedef struct {
    uint32_t address;
    uint8_t data;
} OptionByteData;

typedef struct flash_s internal_flash_driver_t;  // Forward declaration of dev_t

struct flash_s {
    uint32_t start_sector;
    uint32_t num_sectors;
    HAL_StatusTypeDef (*erase)(internal_flash_driver_t *dev);
    HAL_StatusTypeDef (*write)(uint32_t address, uint8_t *data, uint16_t numberofbytes);
    HAL_StatusTypeDef (*read)(uint32_t address, volatile uint16_t *data, uint16_t length);
    HAL_StatusTypeDef (*write_option_byte)(OptionByteData* option_bytes, uint8_t num_bytes);
};

/*****************************************************************************
   Function prototypes
******************************************************************************/
HAL_StatusTypeDef flash_driver_init(internal_flash_driver_t *dev, uint32_t start_sector, uint32_t num_sectors,
		                            HAL_StatusTypeDef (*erase)(internal_flash_driver_t *dev),
		                            HAL_StatusTypeDef (*write)(uint32_t address, uint8_t *data, uint16_t numberofbytes),
									HAL_StatusTypeDef (*write_option_byte)(OptionByteData* option_bytes, uint8_t num_bytes),
                                    HAL_StatusTypeDef (*read)(uint32_t address, volatile uint16_t *data, uint16_t numberofwords)
									);
HAL_StatusTypeDef flash_driver_erase(internal_flash_driver_t *dev);
HAL_StatusTypeDef flash_driver_write_data(uint32_t start_page_address, uint8_t *data, uint16_t numberofbytes);
HAL_StatusTypeDef flash_driver_write_data_option_bytes(OptionByteData* option_bytes, uint8_t num_bytes);
HAL_StatusTypeDef flash_driver_read_data(uint32_t start_page_address, volatile uint16_t *rx_buffer, uint16_t numberofwords);


#endif /* FRAMEWORK_DRIVERS_INC_INTERNAL_FLASH_DRIVER_H_ */
