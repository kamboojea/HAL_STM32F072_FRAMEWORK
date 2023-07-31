/*
 * acp_types.h
 *
 *  Created on: 8 Feb 2017
 *      Author: ldavies
 */

#ifndef ACP_TYPES_H_
#define ACP_TYPES_H_

/*****************************************************************************
   Defines
*****************************************************************************/

// Boolean
typedef enum
{
	FALSE = 0,
	TRUE = !FALSE
} bool_t;

// ACP packet type
#define ACP_PACKET_ID_SIZE 		5
#define ACP_PACKET_DATA_SIZE	8
#define ACP_PACKET_SIZE			(ACP_PACKET_ID_SIZE + ACP_PACKET_DATA_SIZE)

typedef struct __attribute((packed))
{
	uint16_t address;						// Source/destination address
	uint16_t board_address;					// Board address (added for PSEMB which emulates 2 boards)
	uint16_t function;						// Function - Command, respond or notification
	uint16_t len;							// Data length
	uint8_t  data[ACP_PACKET_DATA_SIZE];	// Data content
} acp_packet_t;


#endif /* ACP_TYPES_H_ */
