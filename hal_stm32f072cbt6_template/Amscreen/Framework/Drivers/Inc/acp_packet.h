/*
 * acp_packet.h
 *
 *  Created on: Apr 24, 2023
 *      Author: LDavies
 */

#ifndef FRAMEWORK_DRIVERS_INC_ACP_PACKET_H_
#define FRAMEWORK_DRIVERS_INC_ACP_PACKET_H_

#include <stdbool.h>


/*****************************************************************************
   Defines
*****************************************************************************/
#define CAN_DATA_SIZE	8

/**
 * @brief Structure representing ACP header and its fields
 */
typedef union {
	struct __attribute((packed)) {
		unsigned function:9;		/**< Function code of the packet */
		unsigned destination:10;	/**< Destination ID of the packet */
		unsigned source:10;			/**< Source ID of the packet */
	};
	uint32_t ext_id;				/**< Extended ID of the packet */
} acp_header_t;


/**
 * @brief Structure representing an ACP packet and its fields
 */
typedef struct {
	acp_header_t acp_header;		/**< Header of the packet */
	uint8_t len;					/**< Length of the data */
	uint8_t data[CAN_DATA_SIZE];	/**< Data buffer of the packet */
} acp_packet_t;


/*****************************************************************************
   functions
*****************************************************************************/
bool acp_packet_compare(acp_packet_t *acp_packet_a, acp_packet_t *acp_packet_b);
void acp_packet_copy(acp_packet_t *acp_packet_dst, acp_packet_t *acp_packet_src);

#endif /* FRAMEWORK_DRIVERS_INC_ACP_PACKET_H_ */
