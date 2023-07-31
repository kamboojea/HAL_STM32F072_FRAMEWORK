/*
 * acp_rulst.h
 *
 *  Created on: Apr 4, 2023
 *      Author: LDavies
 */

#ifndef APPLICATION_LIBS_INC_ACP_RESULT_H_
#define APPLICATION_LIBS_INC_ACP_RESULT_H_

#include "stm32f1xx.h"


/*****************************************************************************
   Defines
*****************************************************************************/

/**
 * @brief acp_result_t enumeration
 *
 * This enumeration defines the ACP result codes that are used by ACP framework
 * functions and commands. The same table is used by the firmware and the HWM.
 */
typedef enum  {
	ACP_SUCCESS = 0x01,
	ACP_ERROR_UNKNOWN_COMMAND,
	ACP_ERROR_INVALID_INDEX,
	ACP_ERROR_INVALID_VALUE,
	ACP_ERROR_NULL_POINTER,
	ACP_ERROR_INVALID_DLC,
	ACP_ERROR_FAULTY_SENSOR,
	ACP_ERROR_PACKET_SIZE_EXCEEDED,
	ACP_ERROR_UNSUPPORTED_DEVICE,
	ACP_ERROR_HAL_ERROR,
	ACP_ERROR_HAL_TIMEOUT,
	ACP_ERROR_HAL_BUSY,

	ACP_ERROR_MEMORY_NOT_ERASED = 0x10,
	ACP_ERROR_ADDRESS_INVALID,
	ACP_ERROR_FLASH_WRITE_FAILED,
	ACP_ERROR_CHECKSUM_ERROR,
	ACP_ERROR_INVALID_IMAGE,
	ACP_ERROR_FLASH_READ_FAILED,

	ACP_ERROR_INVALID_I2C_ADDRESS = 0x20,
	ACP_ERROR_INVALID_I2C_COUNT,
	ACP_ERROR_I2C_DEVICE_FAULT,

	ACP_ERROR_NO_BOARD_ADDRESS = 0x30,
	ACP_ERROR_CAN_DISABLED,
	ACP_ERROR_CAN_RX_EMPTY,
	ACP_ERROR_CAN_TX_BUFFER_FULL,
	ACP_ERROR_CAN_TX_FAILED,
	ACP_ERROR_HAL_UNKNOWN
} acp_result_t;


/*****************************************************************************
   Functions
*****************************************************************************/
acp_result_t hal_status_to_acp_result(HAL_StatusTypeDef status);

#endif /* APPLICATION_LIBS_INC_ACP_RESULT_H_ */
