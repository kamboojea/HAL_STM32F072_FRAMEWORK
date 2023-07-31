/*
 * acp_packet.c
 *
 *  Created on: Apr 3, 2023
 *      Author: LDavies
 *
 */
#include <stdint.h>
#include <string.h>
#include "acp_packet.h"

/*****************************************************************************
   Functions
*****************************************************************************/

/**
 * @brief ACP packet compare.
 *
 * @param acp_packet_a ACP packet pointer.
 * @param acp_packet_b ACP packet pointer.
 *
 * @return True if packets are identical.
 */
bool acp_packet_compare(acp_packet_t *acp_packet_a, acp_packet_t *acp_packet_b)
{
	if (acp_packet_a->acp_header.destination != acp_packet_b->acp_header.destination)
		return false;

	if (acp_packet_a->acp_header.function != acp_packet_b->acp_header.function)
		return false;

	if (acp_packet_a->len != acp_packet_b->len)
		return false;

	return (memcmp(acp_packet_a->data, acp_packet_b->data, acp_packet_a->len) == 0);
}


/**
 * @brief ACP packet copy.
 *
 * @param acp_packet_dst Destination ACP packet pointer.
 * @param acp_packet_src Source ACP packet pointer.
 *
 */
void acp_packet_copy(acp_packet_t *acp_packet_dst, acp_packet_t *acp_packet_src)
{
	acp_packet_dst->acp_header.ext_id = acp_packet_src->acp_header.ext_id;
	acp_packet_dst->len = acp_packet_src->len;
	memcpy(acp_packet_dst->data, acp_packet_src->data, CAN_DATA_SIZE);
}
