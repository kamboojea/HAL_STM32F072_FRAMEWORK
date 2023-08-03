/*
 * acp_notification.h
 *
 *  Created on: Apr 19, 2023
 *      Author: LDavies
 */

#ifndef FRAMEWORK_DRIVERS_INC_ACP_NOTIFICATION_H_
#define FRAMEWORK_DRIVERS_INC_ACP_NOTIFICATION_H_

#include <stdbool.h>
#include "acp_packet.h"


/*****************************************************************************
   Defines
*****************************************************************************/

// ACP notifications (currently in sequential order)
typedef enum {
	ACP_NOTIFICATION_NONE,
	ACP_NOTIFICATION_START_UP,
	ACP_NOTIFICATION_MAINS_FAILED,
	ACP_NOTIFICATION_SMS_RESET,
	ACP_NOTIFICATION_DOOR,
	ACP_NOTIFICATION_DISPLAY_PORT_CABLE,
	ACP_NOTIFICATION_CAN_TX_FULL,
	ACP_NOTIFICATION_RDM_PC_WATCHDOG,
	ACP_NOTIFICATION_DOOR_ACCELEROMETER,
} acp_notification_t;



/*****************************************************************************
   Functions
*****************************************************************************/
bool acp_notification_initialise(uint16_t address);
bool acp_notification_packet(acp_packet_t *acp_packet);
bool acp_notification_add(acp_packet_t *acp_packet);
bool acp_nofification_get(acp_packet_t *acp_packet);
bool acp_notification_process_response(acp_packet_t *acp_packet);

#endif /* FRAMEWORK_DRIVERS_INC_ACP_NOTIFICATION_H_ */
