/*
 * acp_can_process.c
 *
 *  Created on: 15 Mar 2023
 *      Author: LDavies
 */
#include "millis.h"
#include "acp_can.h"
#include "acp_commands.h"
#include "acp_notification.h"
#include "acp_board_addresses.h"
#include "acp_can_process.h"



/*****************************************************************************
   Defines
*****************************************************************************/


/*****************************************************************************
   Variables
*****************************************************************************/
static acp_can_t acp_can;


/*****************************************************************************
   Functions
*****************************************************************************/


/**
 * @brief Initializes ACP-CAN process
 *
 * This function initializes the ACP-CAN board address.
 *
 * @param[in] hcan Pointer to the ACP-CAN interface instance.
 * @param[in] board_address ACP CAN board address
 *
 * @return True if ACP-CAN process successfully initialised
 *
 */
bool acp_can_process_initialise(CAN_HandleTypeDef *hcan, uint16_t board_address)
{
	if (!acp_can_init(&acp_can, hcan, "ACPCAN"))
		return false;

	if (!acp_can_set_board_address(&acp_can, board_address))
		return false;

	return acp_notification_initialise(board_address);
}



/**
 * @brief Processes incoming ACP packets
 *
 * This function processes incoming commands/responses(notifications).
 *
 */
void acp_can_process_task(void)
{
	acp_packet_t acp_packet;

	if (acp_can_receive(&acp_can, &acp_packet))
	{
		if (acp_notification_packet(&acp_packet))
		{
			acp_notification_process_response(&acp_packet);
			return;
		}
		acp_command_process(&acp_packet);
		acp_can_transmit(&acp_can, &acp_packet);
	}

	// Service the ACP notification
	if (acp_nofification_get(&acp_packet))
		acp_can_transmit(&acp_can, &acp_packet);
}

