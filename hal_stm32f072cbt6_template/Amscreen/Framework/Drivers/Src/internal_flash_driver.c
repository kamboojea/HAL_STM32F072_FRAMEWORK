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
#include "stm32f0xx_hal.h"
#include "internal_flash_driver.h"
#include "millis.h"
#include "acp_types.h"
/*****************************************************************************
   Defines
******************************************************************************/
#define SAMPLE_TIMEOUT 50

/*****************************************************************************
   Function Prototypes
******************************************************************************/
uint32_t get_page(uint32_t address);
static bool_t flash_busy(void);
static bool_t eop_flag(void);
static HAL_StatusTypeDef is_flash_ready(void);
/*****************************************************************************
   Functions
******************************************************************************/

HAL_StatusTypeDef flash_driver_init(
		                            internal_flash_driver_t *dev,
		                            uint32_t start_sector,
									uint32_t num_sectors,
									HAL_StatusTypeDef (*erase)(internal_flash_driver_t *dev),
                                    HAL_StatusTypeDef (*write)(uint32_t address, uint16_t *data, uint16_t length),
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
	if (dev == NULL)
	{
		return  HAL_ERROR;
	}

    uint32_t sector_error = 0;
    FLASH_EraseInitTypeDef erase_init_struct;

    erase_init_struct.TypeErase     = FLASH_TYPEERASE_PAGES;
    erase_init_struct.PageAddress   = dev->start_sector;
    erase_init_struct.NbPages       = dev->num_sectors;

    HAL_StatusTypeDef status = HAL_FLASH_Unlock();

    if (status != HAL_OK)
    {
    	return status;
    }

    status = HAL_FLASHEx_Erase(&erase_init_struct, &sector_error);

    if (status != HAL_OK)
    {
    	return status;
    }

    status = HAL_FLASH_Lock();

    return status;
}


/**
 * @brief Writes data to the specified flash memory address.
 *
 * This function erases the flash pages within the given address range and then
 * writes the data in half-word (16-bit) format to the flash memory. The data is
 * swapped to ensure correct byte order.
 *
 * @param start_page_address The starting address of the flash memory.
 * @param data Pointer to the data array to be written to flash.
 * @param numberofbytes The number of bytes to be written.
 *
 * @return HAL_StatusTypeDef HAL_OK if the write operation is successful, an error code otherwise.
 */
/* Program the user Flash area half-word by half-word */
HAL_StatusTypeDef flash_driver_write_data(uint32_t start_page_address, uint16_t *data, uint16_t numberofbytes)
{
    /* Counter for the number of bytes written so far */
    int sofar = 0;

    /* Unlock the Flash to enable the flash control register access */
    HAL_StatusTypeDef status = HAL_FLASH_Unlock();

    if(status != HAL_OK)
    {
    	return status;
    }

    while (sofar < (numberofbytes/2))  // Dividing by 2 to write 2 bytes per iteration
    	{
		   /* Prepare the 16-bit data to write */
		   uint16_t halfword_data = data[sofar];
		   // Swap the byte order
		   uint16_t swapped_data = ((halfword_data & 0xFF) << 8) | ((halfword_data >> 8) & 0xFF);

		   status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, start_page_address, swapped_data);

		   if (status != HAL_OK)
		   {
			/* Error occurred while writing data in Flash memory. Return the HAL error code */
			return HAL_FLASH_GetError();
		   }
		   else
		   {
			/* Increment address index and written bytes count */
			start_page_address += 2;
			sofar++;

			status = is_flash_ready();

			if(status != HAL_OK)
			{
					return status;
			}
		   }
	   }

	   /* If the number of bytes is odd, write the remaining byte */
	   if (numberofbytes & 1) // Check if the LSB is set
	   {
		   /* Prepare the 16-bit data to write with padding byte (0xFF) */
		   uint16_t halfword_data = (data[sofar] & 0xFF) | 0xFFFF;

		   status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, start_page_address, halfword_data);

		   if (status != HAL_OK)
		   {
			/* Error occurred while writing data in Flash memory. Return the HAL error code */
			return HAL_FLASH_GetError();
		   }
		   else
		   {
			/* Increment address index and written bytes count */
			start_page_address += 2;
		   }
	   }
	   /* Lock the Flash to disable the flash control register access (recommended
	   to protect the FLASH memory against possible unwanted operation) */
	   status = HAL_FLASH_Lock();
	   /* Return HAL_OK to indicate success */
	   return status;
}


/**
* @brief  Read data from flash memory
*
* @param  start_page_address Start address of the page from where the reading starts in flash
* @param  rx_buffer           Pointer to a buffer where read data will be stored
* @param  numberofwords   Number of words to read from flash
*
* @note   This function reads 32-bit words from flash memory starting from the provided address
*         and stores them into the provided buffer. It reads a specified number of words.
*         If the function reaches the end of flash memory before reading the specified number
*         of words, it will stop reading.
*/
HAL_StatusTypeDef flash_driver_read_data(uint32_t start_page_address, volatile uint16_t *rx_buffer, uint16_t numberofwords)
{
	 // Check if the device instance is valid
	if (rx_buffer == NULL)
	{
		return HAL_ERROR; // Return error if the device instance is not valid
	}

	int16_t index = 0;

   while (TRUE)
   {
   	HAL_StatusTypeDef status = is_flash_ready();

   	if(status != HAL_OK)
   	{
			return status;
		}
       // Dereference the start_page_address, read the 32-bit word,
       // and assign it to the location pointed to by rx_buffer
   	uint16_t data = *(__IO uint16_t *)start_page_address;
       uint16_t swapped_data = ((data & 0xFF) << 8) | ((data >> 8) & 0xFF);

       // Move to the next word in flash memory by adding 4 (size of uint32_t) to start_page_address
       start_page_address += 2;

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
    if((address < (0x08000000 + (FLASH_PAGE_SIZE *(indx+1))) ) && (address >= (0x08000000 + FLASH_PAGE_SIZE*indx)))
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
 * @return 1 if the flash memory is still busy, 0 if the write operation is completed or a timeout occurs.
 */
static bool_t flash_busy(void)
{
    uint32_t sample_timer = millis(); // Start the timer
    volatile int busy_flag = (__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY) == TRUE); // Check the busy flag

    while (busy_flag)
    {
        busy_flag = (__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY) == TRUE); // Check the busy flag

        if (millis_timeout(&sample_timer, SAMPLE_TIMEOUT))
        {
            return SET; // Timeout occurred
        }
    }
    return RESET ; // Write operation completed
}


/**
 * @brief Checks the end-of-program (EOP) flag status.
 *
 * This function checks the EOP flag status by repeatedly sampling the flag
 * until it is cleared or a timeout occurs.
 *
 * @return 0 if the EOP flag is cleared within the timeout, 1 otherwise.
 */
static bool_t eop_flag(void)
{
    uint32_t sample_timer = millis(); /**< Variable to store the current time in milliseconds. */
    volatile int eop_flag = (__HAL_FLASH_GET_FLAG(FLASH_FLAG_EOP) == FALSE); /**< Flag indicating the EOP status. */

    while (eop_flag)
    {
        eop_flag = (__HAL_FLASH_GET_FLAG(FLASH_FLAG_EOP) == FALSE); /**< Update the EOP flag status. */

        if (millis_timeout(&sample_timer, SAMPLE_TIMEOUT))
        {
            return SET; /**< Timeout occurred, return 1 indicating failure. */
        }
    }
    return RESET; /**< EOP flag cleared within the timeout, return 0 indicating success. */
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
    }
    else
    {
        return HAL_BUSY; /**< EOP flag is set, return HAL_BUSY. */
    }
    return HAL_OK; /**< Flash memory is ready for operations, return HAL_OK. */
}

