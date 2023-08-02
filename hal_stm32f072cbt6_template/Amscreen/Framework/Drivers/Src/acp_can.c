/*
 * acp_can.c
 *
 *  Created on: Apr 3, 2023
 *      Author: LDavies
 *
 *  ToDo: first packet received function that alters LED flash pattern
 */
#include <string.h>
#include "acp_can.h"

/*****************************************************************************
   Defines
*****************************************************************************/

#define NOT_ASSIGNED    			NULL
#define MAX_CANS					2		// current limit on STM32s
#define SLAVE_START_FILTER_BANK		14		// Start of the filter banks used by CAN2

/**
 * @brief Enumeration for CAN assignment indices
 */
typedef enum {
    CAN1_ASSIGNMENT_INDEX, /**< Index for CAN1 assignment */
    CAN2_ASSIGNMENT_INDEX, /**< Index for CAN2 assignment */
} can_assignment_index_t;



/*****************************************************************************
   Variables
*****************************************************************************/

/**
 * @brief Array of CAN driver instances for assignments
 * @details This array stores pointers to instances of CAN drivers.
 * The array is initialized to contain NULL pointers for all CANs.
 * The pointers are updated to point to the relevant CAN drivers
 * during runtime when an assignment is made.
 */
static acp_can_t *(can_assignment[MAX_CANS]) = {NOT_ASSIGNED};


/*****************************************************************************
   Static Functions
*****************************************************************************/
static can_assignment_index_t can_handle_to_can_assignment_index(CAN_HandleTypeDef *hcan);
static inline bool can_assigned(CAN_HandleTypeDef *can);
static inline void assign_can(acp_can_t *can);


/*****************************************************************************
   Functions
*****************************************************************************/

/**
 * @brief Initializes CAN driver with specified buffers and device name
 *
 * @param me Pointer to ACP CAN peripheral structure
 * @param hcan Pointer to CAN handle
 * @param name Device name
 * @param rx_buffer Pointer to receive circular buffer
 * @param tx_buffer Pointer to transmit circular buffer
 *
 * @return true if initialization successful, false otherwise
 */
bool acp_can_init(acp_can_t * const me, CAN_HandleTypeDef *hcan, const char *name)
{
	// check if UART is already assigned
	if (can_assigned(hcan))
		return false;

	me->hcan = hcan;
	me->name = name;
	assign_can(me);
	return true;
}


/**
 * @brief Sets the address of the board for CAN communication
 *
 * @param me Pointer to ACP CAN peripheral structure
 * @param address Address of the board
 *
 * @return true if setting board address successful, false otherwise
 */
bool acp_can_set_board_address(acp_can_t * me, uint16_t address)
{
	HAL_CAN_Stop(me->hcan);

	can_assignment_index_t can = can_handle_to_can_assignment_index(me->hcan);
	uint32_t ext_filter = address << 12;

	/* Configure the CAN Filter */
	CAN_FilterTypeDef  filter_config;
	filter_config.FilterBank = ((can == CAN1_ASSIGNMENT_INDEX) ? 0 : SLAVE_START_FILTER_BANK);
	filter_config.FilterMode = CAN_FILTERMODE_IDMASK;
	filter_config.FilterScale = CAN_FILTERSCALE_32BIT;
	filter_config.FilterIdHigh = (ext_filter >> 16) & 0xFFFF;
	filter_config.FilterIdLow = ext_filter & 0xFFFF;
	filter_config.FilterMaskIdHigh = (ext_filter >> 16) & 0xFFFF;
	filter_config.FilterMaskIdLow = ext_filter & 0xFFFF;
	filter_config.FilterFIFOAssignment = ((can == CAN1_ASSIGNMENT_INDEX) ? CAN_RX_FIFO0 : CAN_RX_FIFO1);
	filter_config.FilterActivation = ENABLE;
	filter_config.SlaveStartFilterBank = SLAVE_START_FILTER_BANK;

	if (HAL_CAN_ConfigFilter(me->hcan, &filter_config) != HAL_OK)
		return false;

	if (HAL_CAN_Start(me->hcan) != HAL_OK)
		return false;

	me->address = address;
	return true;
}


/**
 * @brief Transmits an ACP packet through CAN peripheral
 *
 * @param me Pointer to ACP CAN peripheral structure
 * @param acp Pointer to ACP packet to transmit
 *
 * @return true if transmit successful, false otherwise
 */
bool acp_can_transmit(acp_can_t * me, acp_packet_t * acp_packet)
{
	CAN_TxHeaderTypeDef   tx_header;
	uint32_t              tx_mailbox_used;

	tx_header.IDE = CAN_ID_EXT;
	tx_header.RTR = CAN_RTR_DATA;
	tx_header.ExtId = acp_packet->acp_header.ext_id;
	tx_header.DLC = acp_packet->len;
	return HAL_CAN_AddTxMessage(me->hcan, &tx_header, acp_packet->data, &tx_mailbox_used) == HAL_OK;
}


/**
 * @brief Receives an ACP packet through CAN peripheral
 *
 * @param me Pointer to ACP CAN peripheral structure
 * @param acp Pointer to ACP packet to receive
 *
 * @return true if receive successful, false otherwise
 */
bool acp_can_receive(acp_can_t * me, acp_packet_t * acp_packet)
{
	CAN_RxHeaderTypeDef rx_header;
	can_assignment_index_t can = can_handle_to_can_assignment_index(me->hcan);

	HAL_StatusTypeDef status = HAL_CAN_GetRxMessage(me->hcan, ((can == CAN1_ASSIGNMENT_INDEX) ? CAN_RX_FIFO0 : CAN_RX_FIFO1), &rx_header, acp_packet->data);

	if ( status != HAL_OK)
	{
		return false;
	}

	acp_packet->acp_header.ext_id = rx_header.ExtId;
	acp_packet->len = rx_header.DLC;

	memset (&acp_packet->data[acp_packet->len], 0, (CAN_DATA_SIZE - acp_packet->len));
	return true;
}


/**
 * @brief Converts a CAN handle to its corresponding assignment index
 * @param can Pointer to the CAN handle
 * @return The CAN assignment index
 */
static can_assignment_index_t can_handle_to_can_assignment_index(CAN_HandleTypeDef *hcan)
{
    if (hcan->Instance == CAN)
    {
        return CAN1_ASSIGNMENT_INDEX;
    }
   	return CAN2_ASSIGNMENT_INDEX;
}


/**
 * @brief Checks if a CAN is assigned to a particular handle
 * @param hcan Pointer to the CAN handle
 * @return true if the UART is assigned, false otherwise
 */
static inline bool can_assigned(CAN_HandleTypeDef *hcan)
{
    return can_assignment[can_handle_to_can_assignment_index(hcan)] != NOT_ASSIGNED;
}


/**
 * @brief Assigns a CAN to its corresponding handle
 * @param can Pointer to the CAN driver instance
 * @details This function assigns a CAN to its corresponding handle
 * in the @c can_assignment array. The array is used to keep track of
 * which UART driver is assigned to which UART handle.
 */
static inline void assign_can(acp_can_t *acp_can)
{
	can_assignment[can_handle_to_can_assignment_index(acp_can->hcan)] = acp_can;
}
