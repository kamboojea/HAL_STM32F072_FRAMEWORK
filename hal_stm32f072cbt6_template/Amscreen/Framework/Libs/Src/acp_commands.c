/*
 * acp_commands.c
 *
 *  Created on: 18 Apr 2023
 *      Author: LDavies
 */
#include <string.h>
#include "debug.h"
#include "acp_commands.h"
#include "acp_result.h"
#include "board_info.h"
#include "internal_flash_driver.h"
#include "crc32.h"
#include "stm32f0xx_hal.h"

/*****************************************************************************
   Defines
*****************************************************************************/
#define OPTION_BYTES_DEFAULT_USER		0xFF
#define OPTION_BYTES_DEFAULT_RDP		0xAA
#define OPTION_BYTES_DEFAULT_DATA0		0xFF
#define OPTION_BYTES_DEFAULT_DATA1		0xFF
#define BOARD_ID_SIZE 8
/*****************************************************************************
   Variables
*****************************************************************************/

// command prototypes for the ACP framework command table
static acp_result_t cmd_reset(acp_packet_t *acp_packet);
static acp_result_t cmd_get_fw_version(acp_packet_t *acp_packet);
static acp_result_t cmd_get_board_id(acp_packet_t *acp_packet);
static acp_result_t cmd_erase_storage(acp_packet_t *acp_packet);
static acp_result_t cmd_set_flash_address(acp_packet_t *acp_packet);
static acp_result_t cmd_write_flash_data(acp_packet_t *acp_packet);
static acp_result_t cmd_validate_storage(acp_packet_t *acp_packet);
static acp_result_t cmd_echo_data(acp_packet_t *acp_packet);
static acp_result_t cmd_get_uid_lower(acp_packet_t *acp_packet);
static acp_result_t cmd_get_uid_upper(acp_packet_t *acp_packet);


// ACP framework command table
const acp_command_entry_t acp_framework_cmd_table []=
{
	{ 0x020, cmd_reset				},
	{ 0x021, cmd_get_fw_version 	},
	{ 0x022, cmd_get_board_id 		},
	{ 0x023, cmd_erase_storage 		},
	{ 0x024, cmd_set_flash_address 	},
	{ 0x025, cmd_write_flash_data 	},
	{ 0x026, cmd_validate_storage 	},
	{ 0x02A, cmd_echo_data 			},
	{ 0x02C, cmd_get_uid_lower		},
	{ 0x02D, cmd_get_uid_upper		},
	{ 0x000, NULL					}
};

// Application command table
const acp_command_entry_t *application_command_table = NULL;

// Flash write pointer
// ToDo: Flash support

static internal_flash_driver_t internal_flash_device;

/*****************************************************************************
   Functions
*****************************************************************************/

/*----------------------------------------------------------------------------
 *  Routine:        cmd_reset
 *
 *  Description:    Reset the board. There is no response sent, instead the
 *                  board will immediately reboot and send the start-up
 *                  message.
 *
 *  Parameter in:	acp_packet  ACP packet pointer. Not used
 *
 *  Return:			ACP_SUCCESS (in reality this function never returns)
 */
static acp_result_t cmd_reset(acp_packet_t *acp_packet)
{
	NVIC_SystemReset();
	return ACP_SUCCESS;
}


/*----------------------------------------------------------------------------
 *  Routine:        cmd_get_fw_version
 *
 *  Description:    Returns the application firmware version
 *
 *  Parameter in:	acp_packet  ACP packet pointer used to send the result
 *
 *  Return:			ACP error code
 */
static acp_result_t cmd_get_fw_version(acp_packet_t *acp_packet)
{
	memcpy(acp_packet->data, board_info.version + 4, 8);
	acp_packet->len = 8;
	return ACP_SUCCESS;
}


/*----------------------------------------------------------------------------
 *  Routine:        cmd_get_board_id
 *
 *  Description:    Returns the board ID
 *
 *  Parameter in:	acp_packet  ACP packet pointer used to send the result
 *
 *  Return:			ACP error code
 */
static acp_result_t cmd_get_board_id(acp_packet_t *acp_packet)
{
	memcpy(acp_packet->data, board_info.board_id + 4, 8);
	acp_packet->len = 8;
	return ACP_SUCCESS;
}


/*----------------------------------------------------------------------------
 *  Routine:        cmd_erase_storage
 *
 *  Description:    Erase the flash storage area used to store firmware
 *  				updates.
 *
 *  Parameter in:	acp_packet  ACP packet pointer used to send the result
 *
 *  Return:			ACP error code
 */
static acp_result_t cmd_erase_storage(acp_packet_t *acp_packet)
{
	// Initialize internal_flash_device object with start sector, number of sectors, erase, write, and read function pointers.
	flash_driver_init(&internal_flash_device, START_SECTOR, NUM_SECTOR, flash_driver_erase, flash_driver_write_data, flash_driver_write_data_option_bytes, flash_driver_read_data);

	// Erase application area
	if (flash_driver_erase(&internal_flash_device) == HAL_OK)
	{
		debug_printf("Storage area erased\r\n");

		acp_packet->len = 0;
		return (ACP_SUCCESS);
	}
	debug_printf("ERROR: Failed to erase storage area\r\n");
	return (acp_cmd_error(acp_packet, ACP_ERROR_MEMORY_NOT_ERASED));
}


/*----------------------------------------------------------------------------
 *  Routine:        cmd_set_flash_address
 *
 *  Description:    Not currently supported
 *
 *  Parameter in:	acp_packet  ACP packet pointer used to send the result
 *
 *  Return:			ACP error code
 */
static acp_result_t cmd_set_flash_address(acp_packet_t *acp_packet)
{
	return ACP_SUCCESS;
}


/*----------------------------------------------------------------------------
 *  Routine:        cmd_write_flash_data
 *
 *  Description:    Write data to flash.
 *
 *  Parameter in:	acp_packet  ACP packet pointer used to send the result
 *
 *  Return:			ACP error code
 */
static acp_result_t cmd_write_flash_data(acp_packet_t *acp_packet)
{
	if (acp_packet->len < 2)
	{
		debug_printf("ERROR: Invalid address\r\n");
		return (acp_cmd_error(acp_packet, ACP_ERROR_ADDRESS_INVALID));
	}

	uint32_t flash_write_ptr = (acp_packet->data[0] << 8) | acp_packet->data[1];
	flash_write_ptr += STORAGE_AREA_START_ADDRESS;
	acp_packet->len -= 2;

	if (((flash_write_ptr + acp_packet->len) > (FLASH_BANK1_END + 1)) || (flash_write_ptr & 0x01))
	{
		debug_printf("ERROR: Write pointer %d\r\n", flash_write_ptr);
		debug_printf("ERROR: Write address %d > %d\r\n", flash_write_ptr + acp_packet->len, FLASH_BANK1_END);
		return (acp_cmd_error(acp_packet, ACP_ERROR_ADDRESS_INVALID));
    }

	if (memcmp((uint8_t *)flash_write_ptr, (uint8_t *)&acp_packet->data[2], acp_packet->len) != 0)
	{
		HAL_StatusTypeDef status = internal_flash_device.write(flash_write_ptr, (uint8_t *)&acp_packet->data[2], acp_packet->len);

		if (status != HAL_OK)
		{
			debug_printf("ERROR: Flash write failed\r\n");
			return (acp_cmd_error(acp_packet, ACP_ERROR_FLASH_WRITE_FAILED));
		}
	}

	acp_packet->len = 0;
	return (ACP_SUCCESS);
}


/*----------------------------------------------------------------------------
 *  Routine:        cmd_validate_storage
 *
 *  Description:    Validate the firmware update in the storage area using
 *                  the provided CRC32
 *
 *  Parameter in:	acp_packet  ACP packet pointer used to send the result
 *
 *  Return:			ACP error code
 */
static acp_result_t cmd_validate_storage(acp_packet_t *acp_packet)
{
    // Validate image is appropriate for the board

    const char* new_board_id = (board_info.board_id + 4 + STORAGE_AREA_SIZE);
    const char* old_board_id = (board_info.board_id + 4);

    if (memcmp(new_board_id, old_board_id, BOARD_ID_SIZE) != 0)
    {
        return (acp_cmd_error(acp_packet, ACP_ERROR_INVALID_IMAGE));
    }

    uint32_t flash_crc  = crc32(0, (uint8_t *) STORAGE_AREA_START_ADDRESS, STORAGE_AREA_SIZE);
    uint32_t remote_crc = (acp_packet->data[0] << 24) | (acp_packet->data[1] << 16) | (acp_packet->data[2] << 8) | acp_packet->data[3];

    debug_printf("Flash  CRC %08X\r\n", flash_crc);
    debug_printf("Remote CRC %08X\r\n", remote_crc);

    if (flash_crc != remote_crc)
    {
        debug_printf("ERROR: Checksum mismatch\r\n");
        return (acp_cmd_error(acp_packet, ACP_ERROR_CHECKSUM_ERROR));
    }

	OptionByteData option_bytes[NUM_OPTION_BYTES] = {
	    {OB_DATA_ADDRESS_DATA0, DATA0_FLAG},
	    {OB_DATA_ADDRESS_DATA1, DATA1_FLAG}
	};

	if (flash_driver_write_data_option_bytes(option_bytes, NUM_OPTION_BYTES) != HAL_OK)
    {
		debug_printf("ERROR: flash_driver_write_data_option_bytes() failed.\r\n");
		return (acp_cmd_error(acp_packet, ACP_ERROR_HAL_ERROR));
	}
    acp_packet->len = 0;
    return (ACP_SUCCESS);
}


/*----------------------------------------------------------------------------
 *  Routine:        cmd_get_uid_lower
 *
 *  Description:    Returns the lower half of the 96-Bit STM32F0 unique ID
 *
 *  Parameter in:	acp_packet  ACP packet pointer used to send the result
 *
 *  Return:			ACP_SUCCESS
 */
static acp_result_t cmd_get_uid_lower(acp_packet_t *acp_packet)
{
	uint32_t uuid_w0 = HAL_GetUIDw0();
	uint32_t uuid_w1 = HAL_GetUIDw1();

	acp_packet->data[0] = (uint8_t)((uuid_w0>>0) & 0xff);
	acp_packet->data[1] = (uint8_t)((uuid_w0>>8) & 0xff);
	acp_packet->data[2] = (uint8_t)((uuid_w0>>16) & 0xff);
	acp_packet->data[3] = (uint8_t)((uuid_w0>>24) & 0xff);
	acp_packet->data[4] = (uint8_t)((uuid_w1>>0) & 0xff);
	acp_packet->data[5] = (uint8_t)((uuid_w1>>8) & 0xff);
	acp_packet->data[6] = (uint8_t)((uuid_w1>>16) & 0xff);
	acp_packet->data[7] = (uint8_t)((uuid_w1>>24) & 0xff);

	acp_packet->len = 8;

	return ACP_SUCCESS;
}


/*----------------------------------------------------------------------------
 *  Routine:        cmd_get_uid_upper
 *
 *  Description:    Returns the upper half of the 96-Bit STM32F0 unique ID
 *
 *  Parameter in:	acp_packet  ACP packet pointer used to send the result
 *
 *  Return:			ACP_SUCCESS
 */
static acp_result_t cmd_get_uid_upper(acp_packet_t *acp_packet)
{
	uint32_t uuid_w2 = HAL_GetUIDw2();

	acp_packet->data[0] = (uint8_t)((uuid_w2>>0) & 0xff);
	acp_packet->data[1] = (uint8_t)((uuid_w2>>8) & 0xff);
	acp_packet->data[2] = (uint8_t)((uuid_w2>>16) & 0xff);
	acp_packet->data[3] = (uint8_t)((uuid_w2>>24) & 0xff);
	acp_packet->len = 4;
	return ACP_SUCCESS;
}


/*----------------------------------------------------------------------------
 *  Routine:        acp_cmd_error
 *
 *  Description:    Populate the ACP packet with the specified ACP error code
 *
 *  Parameter in:	acp_packet  ACP packet pointer used to send the ACP error
 *                          code response
 *
 *					error	ACP error code
 *
 *  Return:			ACP Error code
 */
acp_result_t acp_cmd_error(acp_packet_t *acp_packet, acp_result_t error)
{
	acp_packet->len = 6;
	acp_packet->data[0] = 'E';
	acp_packet->data[1] = 'R';
	acp_packet->data[2] = 'R';
	acp_packet->data[3] = 'O';
	acp_packet->data[4] = 'R';
	acp_packet->data[5] = error;
	return error;
}



/*----------------------------------------------------------------------------
 *  Routine:        acp_command_add_application_commands
 *
 *  Description:    Set the application command table pointer
 *
 *  Parameter in:	acp_command_entry  pointer to application command table
 *
 *  Return:			ACP error code
 */
acp_result_t acp_command_add_application_commands(const acp_command_entry_t *acp_command_entry)
{
	if (acp_command_entry != NULL)
	{
		application_command_table = acp_command_entry;
		return ACP_SUCCESS;
	}
	return ACP_ERROR_NULL_POINTER;
}



/*----------------------------------------------------------------------------
 *  Routine:        acp_command_process
 *
 *  Description:    Process command that matches function field within
 *  				the ACP packet.
 *
 *  Parameter in:	acp_packet  ACP packet pointer. The same pointer is used
 *  							to place the response in.
 *
 *  Return:			ACP result code
 */
acp_result_t acp_command_process(acp_packet_t *acp_packet)
{
	const acp_command_entry_t *cmd_entry_ptr;

    // Swap destination and source, since the response is in the opposite direction
    uint16_t temp = acp_packet->acp_header.source;
    acp_packet->acp_header.source = acp_packet->acp_header.destination;
    acp_packet->acp_header.destination = temp;

	// The function field within the application command table is checked
	// first. This allows the application to override framework commands
	if (application_command_table != NULL)
	{
		for (cmd_entry_ptr = application_command_table; cmd_entry_ptr->fptr != NULL; cmd_entry_ptr++)
		{
			if (acp_packet->acp_header.function == cmd_entry_ptr->function)
			{
			    // Set the response flag
				acp_packet->acp_header.function |= ACP_COMMAND_RESPONSE;
				return (cmd_entry_ptr->fptr(acp_packet));
			}
		}
	}

	// If there are no matching functions within the application
	// command table, check the ACP framework command table
    for (cmd_entry_ptr = acp_framework_cmd_table; cmd_entry_ptr->fptr != NULL; cmd_entry_ptr++)
    {
    	if (acp_packet->acp_header.function == cmd_entry_ptr->function)
    	{
    	    // Set the response flag
    		acp_packet->acp_header.function |= ACP_COMMAND_RESPONSE;
    		return (cmd_entry_ptr->fptr(acp_packet));
    	}
    }

    // Set the response flag
	acp_packet->acp_header.function |= ACP_COMMAND_RESPONSE;
    return (acp_cmd_error(acp_packet, ACP_ERROR_UNKNOWN_COMMAND));
}


/*----------------------------------------------------------------------------
 *  Routine:        cmd_echo_data
 *
 *  Description:    echo data back
 *
 *  Parameter in:	ptr		ACP packet pointer used to send the result
 *
 *  Return:			ACP_SUCCESS
 */
static uint8_t cmd_echo_data(acp_packet_t *ptr)
{
	return (ACP_SUCCESS);
}


