/*
 * acp_can_task.c
 *
 *  Created on: 15 Mar 2023
 *      Author: LDavies
 *
 *  ToDo: Refactor into a class with two instances (SideA & SideB)
 */
#include "can.h"
#include "debug.h"
#include "millis.h"
#include "acp_can_task.h"
#include "acp_can.h"
#include "acp_commands.h"
#include "acp_board_addresses.h"
#include "acp_notification.h"


/*****************************************************************************
   Defines
*****************************************************************************/
//#define ENABLE_CAN_PACKETS_DEBUGGING

typedef enum {
	ACP_CAN_TASK_INITIALISE,
	ACP_CAN_TASK_SERVICE
} acp_can_task_state_t;


/*****************************************************************************
   Variables
*****************************************************************************/
static acp_can_task_state_t acp_can_task_state = ACP_CAN_TASK_INITIALISE;
static millis_t acp_can_task_timer;

CAN_HandleTypeDef *hcan_side_a = &hcan;


static acp_can_t acp_can_side_a;

/*****************************************************************************
   Prototypes
*****************************************************************************/
static void set_acp_can_task_state (acp_can_task_state_t state);
static inline void acp_can_task_initialise();
static inline void acp_can_task_service();


/*****************************************************************************
   Functions
*****************************************************************************/


/**
 * @brief Processes PC comms interface tasks.
 *
 * This function processes the UART-CAN interface tasks.
 *
 * @param[in] me Pointer to the UART-CAN interface instance.
 *
 * @return True if the task processing was successful, false otherwise.
 */
void acp_can_task(void)
{
	switch (acp_can_task_state)
	{
		case ACP_CAN_TASK_INITIALISE:
			acp_can_task_initialise();
			break;

		case ACP_CAN_TASK_SERVICE:
			acp_can_task_service();
			break;
	}
}


/**
 * @brief Transmits an ACP packet to SIDEA via the ACP-CAN interface.
 *
 * This function transmits an ACP packet to SIDEA via the ACP-CAN interface by calling the acp_can_transmit() function with the acp_packet parameter and the ACP-CAN interface instance for SIDEA.
 *
 * @param[in] acp_packet Pointer to the ACP packet to transmit.
 *
 * @return True if the ACP packet was successfully transmitted to SIDEA, false
 */
bool acp_can_task_tx_packet_side_a(acp_packet_t *acp_packet)
{
#ifdef ENABLE_CAN_PACKETS_DEBUGGING
	debug_printf("PMB(A)->CB: %s", debug_acp_packet_to_string(acp_packet));
#endif
	return acp_can_transmit(&acp_can_side_a, acp_packet);
}



/*****************************************************************************
   Private Functions
*****************************************************************************/

/**
 * @brief Sets the state of the ACP-CAN task.
 *
 * This function sets the state of the ACP-CAN task to the specified state and updates the timer.
 *
 * @param[in] state The new state to set for the ACP-CAN task.
 */
static void set_acp_can_task_state (acp_can_task_state_t state)
{
    acp_can_task_state = state;
    acp_can_task_timer = millis();

}


/**
 * @brief Initializes the ACP-CAN task.
 *
 * This function initializes the ACP-CAN task by initializing the ACP-CAN interface instances for SIDEA and SIDEB.
 */
void acp_can_task_initialise(void)
{
	acp_can_init(&acp_can_side_a, hcan_side_a, "CANSIDA");
	acp_can_set_board_address(&acp_can_side_a, 0x91);
	set_acp_can_task_state(ACP_CAN_TASK_SERVICE);
}


/**
 * @brief Services the ACP-CAN task.
 *
 * This function services the ACP-CAN task by receiving ACP packets from the ACP-CAN interfaces for SIDEA and SIDEB and transmitting them to the PC comms task.
 */
void acp_can_task_service(void)
{
	acp_packet_t acp_packet;

	if (acp_can_receive(&acp_can_side_a, &acp_packet))
	{
#ifdef ENABLE_CAN_PACKETS_DEBUGGING
		debug_printf("CB->Pfeb(A): %s", debug_acp_packet_to_string(&acp_packet));
#endif
			if (!acp_notification_packet(&acp_packet))
			{
				acp_command_process(&acp_packet);
				acp_can_task_tx_packet_side_a(&acp_packet);
			}
	}
}
