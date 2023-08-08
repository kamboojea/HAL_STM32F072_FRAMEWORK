/**
 * @file flash_driver.c
 * @brief This file provides a set of firmware functions to manage the internal
 * flash memory of STM32F105xx devices.
 * @version 0.1
 * @date 23 Jun 2023
 * @author ARahmatinia
 */

/*****************************************************************************
   Libraries
*****************************************************************************/
#include <stdbool.h>
#include "stm32f0xx_hal.h"
#include "internal_flash_driver.h"
#include "millis.h"

/*****************************************************************************
   Defines
******************************************************************************/
#define SAMPLE_TIMEOUT 50

/*****************************************************************************
   Function Prototypes
******************************************************************************/
uint32_t get_page(uint32_t address);
static bool flash_busy(void);
static bool eop_flag(void);
static HAL_StatusTypeDef is_flash_ready(void);

/*****************************************************************************
   Functions
******************************************************************************/
HAL_StatusTypeDef flash_driver_init(
		                            internal_flash_driver_t *dev,
		                            uint32_t start_sector,
									uint32_t num_sectors,
									HAL_StatusTypeDef (*erase)(internal_flash_driver_t *dev),
                                    HAL_StatusTypeDef (*write)(uint32_t address, uint8_t *data, uint16_t length),
									HAL_StatusTypeDef (*write_option_byte)(OptionByteData* option_bytes, uint8_t num_bytes),
                                    HAL_StatusTypeDef (*read)(uint32_t address, volatile uint16_t *data, uint16_t length)
									)
{
    if (dev == NULL)
	{
	    return HAL_ERROR;
    }

    dev->start_sector = start_sector;
    dev->num_sectors = num_sectors;
    dev->erase = erase;
    dev->write = write;
    dev->write_option_byte = write_option_byte;
    dev->read = read;

    return HAL_OK;
}


/**
 * @brief  Erases the specified sectors in flash memory.
 * @param  dev: The flash driver instance.
 * @retval HAL status
 */
HAL_StatusTypeDef flash_driver_erase(internal_flash_driver_t *dev)
{
	// Check if the device pointer is NULL
	if (dev == NULL)
	{
		return HAL_ERROR; // Return error status if device pointer is NULL
	}

    uint32_t sector_error = 0;
    FLASH_EraseInitTypeDef erase_init_struct;

    erase_init_struct.TypeErase     = FLASH_TYPEERASE_PAGES; // Set the erase type to erase pages
    erase_init_struct.PageAddress   = dev->start_sector; // Set the start address of the sector to be erased
    erase_init_struct.NbPages       = dev->num_sectors; // Set the number of sectors to be erased

    HAL_StatusTypeDef status = HAL_FLASH_Unlock(); // Unlock the flash for erase operation

    if (status != HAL_OK)
    {
    	return status; // Return status if unlocking the flash failed
    }

    status = HAL_FLASHEx_Erase(&erase_init_struct, &sector_error); // Erase the specified flash sectors

    if (status != HAL_OK)
    {
    	return status; // Return status if erasing the sectors failed
    }

    // Lock the flash after erase operation
    return HAL_FLASH_Lock();
}


/**
 * @brief Writes data to the specified flash memory address.
 *
 * This function erases the flash pages within the given address range and then
 * writes the data in half-word (16-bit) format to the flash memory. The data is
 * swapped to ensure correct byte order.
 *
 * @param destination_start_page_address The starting address of the flash memory.
 * @param data Pointer to the data array to be written to flash.
 * @param numberofbytes The number of bytes to be written.
 *
 * @return HAL_StatusTypeDef HAL_OK if the write operation is successful, an error code otherwise.
 */
HAL_StatusTypeDef flash_driver_write_data(uint32_t start_page_address, uint8_t *data, uint16_t numberofbytes)
{
    /* Counter for the number of bytes written so far */
    int sofar = 0;

    /* Unlock the Flash to enable the flash control register access */
    HAL_StatusTypeDef status = HAL_FLASH_Unlock();

    if(status != HAL_OK)
    {
        return status;
    }

    while (sofar < (numberofbytes - 1)) // Ensure we don't overrun the data buffer
    {
        /* Prepare the 16-bit data to write */
        uint16_t halfword_data = (uint16_t)((data[sofar + 1] << 8) | data[sofar]);

        status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, start_page_address, halfword_data);

        if (status != HAL_OK)
        {
            /* Error occurred while writing data in Flash memory. Return the HAL error code */
            return HAL_FLASH_GetError();
        }

        /* Increment address index and written bytes count */
        start_page_address += 2;
        sofar += 2;

        status = is_flash_ready();

        if (status != HAL_OK)
        {
            return status;
        }
    }

    /* If the number of bytes is odd, write the remaining byte */
    if (numberofbytes & 1)
    {
        /* Prepare the 16-bit data to write with padding byte (0xFF) */
        uint16_t halfword_data = (uint16_t)((data[sofar] << 8 ) | 0xFF);

        status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, start_page_address, halfword_data);

        if (status != HAL_OK)
        {
            /* Error occurred while writing data in Flash memory. Return the HAL error code */
            return HAL_FLASH_GetError();
        }
    }
    // Lock the flash to disable the flash control register access
    return HAL_FLASH_Lock();
}


/**
 * @brief Writes data to the option bytes of the FLASH memory on the STM32F072 microcontroller.
 *
 * This function performs the following sequence:
 *   - Unlocks the FLASH memory.
 *   - Unlocks the option bytes.
 *   - Erases existing option bytes.
 *   - Programs the new option bytes with the specified data.
 *   - Locks the option bytes and FLASH memory back.
 *
 * @param option_bytes Pointer to an array of OptionByteData structures containing the address and data for each option byte.
 * @param num_bytes The number of option bytes to write.
 *
 * @retval HAL_OK if the option bytes were successfully written.
 * @retval HAL_ERROR if there was an error during any of the operations.
 */
HAL_StatusTypeDef flash_driver_write_data_option_bytes(OptionByteData* option_bytes, uint8_t num_bytes)
{
    // Check if the input parameters are valid
    if (option_bytes == NULL || num_bytes == 0)
    {
        return HAL_ERROR; // Return error if the parameters are not valid
    }

    // Unlock the FLASH memory
    if (HAL_FLASH_Unlock() != HAL_OK)
    {
        return HAL_ERROR; // Return error if FLASH memory unlock fails
    }

    // Unlock the option bytes
    if (HAL_FLASH_OB_Unlock() != HAL_OK)
    {
        return HAL_ERROR; // Return error if option bytes unlock fails
    }

    // Erase existing option bytes
    if (HAL_FLASHEx_OBErase() != HAL_OK)
    {
        return HAL_ERROR; // Return error if option bytes erase fails
    }

    // Program each option byte with the given data
    for (uint8_t i = 0; i < num_bytes; i++)
    {
        FLASH_OBProgramInitTypeDef option_byte; // Declare option byte structure
        option_byte.DATAAddress = option_bytes[i].address; // Set the address for the option byte
        option_byte.DATAData = option_bytes[i].data;       // Set the data for the option byte
        option_byte.OptionType = OPTIONBYTE_DATA;          // Set the option type as DATA

        if (HAL_FLASHEx_OBProgram(&option_byte) != HAL_OK)
        {
            return HAL_ERROR; // Return error if option byte programming fails
        }
    }

    // Lock the option bytes
    if (HAL_FLASH_OB_Lock() != HAL_OK)
    {
        return HAL_ERROR; // Return error if option bytes lock fails
    }

    // Lock the FLASH memory
    if (HAL_FLASH_Lock() != HAL_OK)
    {
        return HAL_ERROR; // Return error if FLASH memory lock fails
    }

    return HAL_OK; // Return success if all operations were successful
}


/**
 * @brief  Read data from flash memory
 *
 * @param  destination_destination_start_page_address Start address of the page from where the reading starts in flash
 * @param  rx_buffer           Pointer to a buffer where read data will be stored
 * @param  numberofwords   Number of words to read from flash
 *
 * @note   This function reads 32-bit words from flash memory starting from the provided address
 *         and stores them into the provided buffer. It reads a specified number of words.
 *         If the function reaches the end of flash memory before reading the specified number
 *         of words, it will stop reading.
 */
HAL_StatusTypeDef flash_driver_read_data(uint32_t destination_destination_start_page_address, volatile uint16_t *rx_buffer, uint16_t numberofwords)
{
	 // Check if the device instance is valid
	if (rx_buffer == NULL)
	{
		return HAL_ERROR; // Return error if the device instance is not valid
    }

	int16_t index = 0;

    while (true)
    {
   	    HAL_StatusTypeDef status = is_flash_ready();

   	    if(status != HAL_OK)
    	{
			return status;
		}

        // Dereference the destination_destination_start_page_address, read the 32-bit word,
        // and assign it to the location pointed to by rx_buffer
    	uint16_t data = *(__IO uint16_t *)destination_destination_start_page_address;
        uint16_t swapped_data = ((data & 0xFF) << 8) | ((data >> 8) & 0xFF);

        // Move to the next word in flash memory by adding 4 (size of uint32_t) to destination_destination_start_page_address
        destination_destination_start_page_address += 2;

        // Move to the next location in rx_buffer
        rx_buffer[index] = swapped_data;
        index++;

        // Decrement numberofwords. If it reaches 0, break out of the loop.
        if (!(numberofwords--))
        {
        	break;
        }
    }
    return HAL_OK;
}


/*****************************************************************************
  Static Functions
******************************************************************************/
/**
 * @brief Retrieves the page start address for a given flash memory address.
 *
 * The function calculates the page start address of the flash memory
 * for the provided address. The flash memory is divided into pages
 * of size FLASH_PAGE_SIZE and this function will return the starting address
 * of the page that contains the input address.
 *
 * If the address falls outside the expected range (0x08000000 to
 * 0x08000000 + 128*FLASH_PAGE_SIZE), the function will return 0.
 *
 * @param address The flash memory address for which to calculate the page start address.
 *
 * @return The start address of the page that contains the input address,
 * or 0 if the address is out of range.
 */
uint32_t get_page(uint32_t address)
{
    /* Loop over the 128 pages */
    for (int indx=0; indx<128; indx++)
    {
        /* Check if the address falls within the current page */
        if ((address < (0x08000000 + (FLASH_PAGE_SIZE *(indx+1))) ) && (address >= (0x08000000 + FLASH_PAGE_SIZE*indx)))
        {
            /* If the address is within the current page, return the page start address */
            return (0x08000000 + FLASH_PAGE_SIZE*indx);
        }
    }
    /* If the address does not fall within any page, return 0 */
    return 0;
}


/**
 * @brief Checks if the flash memory is busy with a write operation.
 *
 * This function checks the busy flag of the flash memory and waits until the write operation is completed
 * or a timeout occurs. The timeout duration is defined by SAMPLE_TIMEOUT.
 *
 * @return TRUE if the flash memory is still busy, FALSE if the write operation is completed or a timeout occurs.
 */
static bool flash_busy(void)
{
    uint32_t sample_timer = millis(); // Start the timer
    volatile bool busy_flag; // Check the busy flag

    do
    {
        busy_flag = __HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY); // Check the busy flag

    } while (busy_flag || millis_timeout(&sample_timer, SAMPLE_TIMEOUT));

    return busy_flag ; // Write operation completed
}


/**
 * @brief Checks the end-of-program (EOP) flag status.
 *
 * This function checks the EOP flag status by repeatedly sampling the flag
 * until it is cleared or a timeout occurs.
 *
 * @return False if the EOP flag is cleared within the timeout, True otherwise.
 */
static bool eop_flag(void)
{
    uint32_t sample_timer = millis(); /**< Variable to store the current time in milliseconds. */
    volatile bool eop_flag ; /**< Flag indicating the EOP status. */

    do
    {
        eop_flag = __HAL_FLASH_GET_FLAG(FLASH_FLAG_EOP); /**< Update the EOP flag status. */

    } while (eop_flag || millis_timeout(&sample_timer, SAMPLE_TIMEOUT));

    return eop_flag; /**< EOP flag cleared within the timeout, return 0 indicating success. */
}


/**
 * @brief Checks if the flash memory is ready for operations.
 *
 * This function checks if the flash memory is busy or if the end-of-program (EOP) flag is set.
 * If the flash memory is busy, it returns HAL_BUSY. If the EOP flag is set, it clears the flag and returns HAL_BUSY.
 * Otherwise, it returns HAL_OK indicating that the flash memory is ready for operations.
 *
 * @return HAL_StatusTypeDef The status indicating if the flash memory is ready (HAL_OK) or busy (HAL_BUSY).
 */
static HAL_StatusTypeDef is_flash_ready(void)
{
    if (flash_busy())
    {
        return HAL_BUSY; /**< Flash memory is busy, return HAL_BUSY. */
    }

    // Check the EOP flag in the FLASH_SR register
    if (eop_flag())
    {
        /* Clear the EOP flag */
        __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP);
        return HAL_BUSY; /**< EOP flag is set, return HAL_BUSY. */
    }
    return HAL_OK; /**< Flash memory is ready for operations, return HAL_OK. */
}
