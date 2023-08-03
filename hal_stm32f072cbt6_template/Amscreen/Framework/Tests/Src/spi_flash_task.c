// Including the spi_flash.h file
#include "spi_flash.h"
#include "spi.h"
#include "string.h"
#include "strings.h"
#include "debug.h"

// Definitions for SPI handle, CS GPIO port and pin
#define SPI_FLASH_SPI_HANDLE hspi1
#define SPI_FLASH_CS_GPIO_PORT GPIOA
#define SPI_FLASH_CS_GPIO_PIN GPIO_PIN_4

// Buffer for testing read/write operations



int failed = 0;

HAL_StatusTypeDef flash_test_task(void)
{
    // Declare an external_flash_device_t instance
	  external_flash_device_t dev;





	  /*******************************************************************************************************************************************************************************************************************
	                                                                                                              Initialize
	  ********************************************************************************************************************************************************************************************************************/
    // Initialize the flash memory device
    HAL_StatusTypeDef status = external_flash_init(&dev, "AT25SF041B", &SPI_FLASH_SPI_HANDLE, SPI_FLASH_CS_GPIO_PORT, SPI_FLASH_CS_GPIO_PIN);
    if (status != HAL_OK)
    {
       return status;
    	// Handle the error
    }

        /*******************************************************************************************************************************************************************************************************************
        	                                                                                                       Device ID
        ********************************************************************************************************************************************************************************************************************/

        uint8_t manufacturer_id;
        uint16_t device_id;
        status = external_flash_read_manufacturer_device_id(&dev, &manufacturer_id, &device_id);

        if (status != HAL_OK)
        {
			return status;
		}

        /*******************************************************************************************************************************************************************************************************************
          	                                                                                                   Block Erase the flash memory
        ********************************************************************************************************************************************************************************************************************/
        // Define the address where you want to write the data
        uint32_t address = 0x00000000; // Replace with your desired address

        // Define the data you want to write
        uint8_t write_buffer[] = "hello";
        uint16_t buffer_length = (sizeof(write_buffer));



        block_erase_size_t block_erase_size = block_erase_4k;

        if (external_flash_block_erase(&dev, block_erase_size, address) != HAL_OK)
        {
        	//printf("Error: Failed to erase data to the external flash device.\n");
        	return HAL_ERROR;
        }
        /********************************************************************************************************************************************************************************************************************
                	                                                                                              Chip Erase the flash memory
         ********************************************************************************************************************************************************************************************************************/
//        status = external_flash_chip_erase(&dev) ;
//
//         if (status != HAL_OK)
//         {
//        	 return;
//         }

        /********************************************************************************************************************************************************************************************************************
                                	                                                                               Read form the flash memory
         ********************************************************************************************************************************************************************************************************************/
               // Define a buffer to read the data into
               uint8_t read_buffer[buffer_length];
               memset(read_buffer, 0x00, buffer_length);

               // Read the data from the external flash device
               if (external_flash_read_page(&dev, address, read_buffer, buffer_length) != HAL_OK)
               {
                   debug_printf("Error: Failed to read data from the external flash device.\n");
                   return HAL_ERROR;
               }

               // Print the read data
             //  printf("Read data: %s\n", read_buffer);


               if (external_flash_write_status_register(&dev))
               {

               }

        /*******************************************************************************************************************************************************************************************************************
                 	                                                                                                Write to the flash memory
        ********************************************************************************************************************************************************************************************************************/
        // Write the data to the external flash device
        if (external_flash_write_page(&dev, address, write_buffer, buffer_length) != HAL_OK)
        {
          //  printf("Error: Failed to write data to the external flash device.\n");
            return HAL_ERROR;
        }

        /*******************************************************************************************************************************************************************************************************************
                         	                                                                                         Read form the flash memory
        ********************************************************************************************************************************************************************************************************************/
        // Define a buffer to read the data into
       // uint8_t read_buffer[buffer_length];
        uint8_t read_buffer1[buffer_length];
        // Read the data from the external flash device
        if (external_flash_read_page(&dev, address, read_buffer1, buffer_length) != HAL_OK)
        {
          //  printf("Error: Failed to read data from the external flash device.\n");
            return HAL_ERROR;
        }

        // Print the read data
        debug_printf("Read data: %s\n", read_buffer1);

        return HAL_OK;

}








