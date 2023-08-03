/*
 * acp_notifications.c
 *
 *  Created on: Apr 19, 2023
 *      Author: LDavies
 */
#include <string.h>
#include "stm32f0xx.h"
#include "millis.h"
#include "acp_commands.h"
#include "acp_board_addresses.h"
#include "acp_notification.h"


/*****************************************************************************
   Defines
*****************************************************************************/
#define MAX_ACP_NOTIFICATIONS				3
#define ACP_NOTIFICATION_RESEND_INTERVAL	1000



/*****************************************************************************
   Variables
*****************************************************************************/

// ACP notification variables
static acp_packet_t *acp_notification[MAX_ACP_NOTIFICATIONS];
static uint8_t num_acp_notifications;
static millis_t acp_notification_resend_timer;
static uint16_t board_address;


// Start up ACP notification packet
static acp_packet_t start_up_notication;



/*****************************************************************************
   Prototypes
*****************************************************************************/
static void clear_acp_notifications(void);
static bool new_acp_notification(acp_packet_t *acp_packet);
static void create_start_up_notification(void);



/*****************************************************************************
   Public Functions
*****************************************************************************/

/**
 * @brief Initialise the ACP notification task
 *
 * @param address Board address used as source of notification packet
 *
 * @return True if initialised successfully
 */
bool acp_notification_initialise(uint16_t address)
{
	board_address = address;
	clear_acp_notifications();
	create_start_up_notification();
	return (acp_notification_add(&start_up_notication));
}


/**
 * @brief Check if ACP packet is a response/notification
 *
 * @param acp_packet ACP-CAN packet containing notification.
 *
 * @return True if ACP packet is a response/notification
 */
bool acp_notification_packet(acp_packet_t *acp_packet)
{
	return (acp_packet->acp_header.function >= ACP_COMMAND_RESPONSE);
}


/**
 * @brief Add ACP notifications to active notification list.
 *
 * This function inserts the ACP-CAN notification into the notification list.
 * All notifications in the list are repeatly transmitted until acknowledged
 *
 * @param acp_packet ACP notification packet pointer.
 *
 * @return True if notifications added to active list.
 */
bool acp_notification_add(acp_packet_t *acp_packet)
{
	if (new_acp_notification(acp_packet))
	{
		for(int idx = 0; idx < MAX_ACP_NOTIFICATIONS; idx++)
		{
			if (acp_notification[idx] == NULL)
			{
				acp_notification[idx] = acp_packet;
				num_acp_notifications++;
				acp_notification_resend_timer = millis();
				return true;
			}
		}
	}
	return false;
}


/**
 * @brief Get ACP notification to transmit
 *
 * @param acp_packet Pointer to ACP packet
 *
 * @return True ACP notification to transmit
 */
bool acp_nofification_get(acp_packet_t *acp_packet)
{
	if (num_acp_notifications == 0)
		return false;

	if (!millis_timeout(&acp_notification_resend_timer, ACP_NOTIFICATION_RESEND_INTERVAL))
		return false;

	for(int idx = 0; idx < MAX_ACP_NOTIFICATIONS; idx++)
	{
		if (acp_notification[idx] != NULL)
		{
			acp_packet_copy(acp_packet, acp_notification[idx]);
//			acp_packet = acp_notification[idx];
			return true;
		}
	}
	return false;
}


/**
 * @brief Process any incoming packet for a matching active notifications
 *
 * @param acp_packet Pointer to incoming ACP packet
 *
 * @return True if match found
 */
bool acp_notification_process_response(acp_packet_t *acp_packet)
{
	if (num_acp_notifications == 0)
		return false;

	for(int idx = 0; idx < MAX_ACP_NOTIFICATIONS; idx++)
	{
		if (acp_notification[idx] != NULL)
		{
			// Notification match the response?
			if (acp_packet->acp_header.function == (acp_notification[idx]->acp_header.function | ACP_COMMAND_RESPONSE))
			{
				// Notification has been acknowledged,
				// remove notification from active list
				acp_notification[idx] = NULL;
				num_acp_notifications--;
				return true;
			}
		}
	}
	return false;
}



/*****************************************************************************
   Private Functions
*****************************************************************************/

/**
 * @brief Clear all current ACP notifcations
 *
 */
static void clear_acp_notifications(void)
{
	// Clear all ACP notifications
	for(int idx = 0; idx < MAX_ACP_NOTIFICATIONS; idx++)
		acp_notification[idx] = NULL;

	num_acp_notifications = 0;

	// Restart the ACP resend timer
	acp_notification_resend_timer = millis();
}



/**
 * @brief Check if notification already active.
 *
 * @param acp_packet ACP-CAN notification packet pointer.
 *
 * @return True if notification is new.
 */
static bool new_acp_notification(acp_packet_t *acp_packet)
{
	for(int idx = 0; idx < MAX_ACP_NOTIFICATIONS; idx++)
	{
		if (acp_notification[idx] != NULL)
		{
			if (acp_packet_compare(acp_notification[idx], acp_packet))
				return false;
		}
	}
	return true;
}


/**
 * @brief Create the startup notification.
 *
 */
static void create_start_up_notification(void)
{
	// ToDo: Startup reason
	// Record the top byte in the Control/Status register containing the reset flags.
    // The reset flags are detailed in the STM32 reference manual (RM0091) page 128.
    // The remove reset bit (RMVF) is removed
	uint8_t reset_flags = 0;

    // Clear the reset flags
    // RCC_PERIPHERAL->CSR.rmvf = SET;

    // Setup the startup ACP notification
    start_up_notication.acp_header.source = board_address;
    start_up_notication.acp_header.destination = ACP_ADDRESS_HWM;
    start_up_notication.acp_header.function = ACP_NOTIFICATION_START_UP;
    start_up_notication.len = 1;
    start_up_notication.data[0] = reset_flags;
}
