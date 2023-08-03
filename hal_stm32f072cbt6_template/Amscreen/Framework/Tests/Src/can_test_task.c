/*
 * uart_test_task.c
 *
 *  Created on: Mar 1, 2023
 *      Author: LDavies
 */
#include "circular_buf.h"
#include "can.h"
#include "acp_can.h"


void can_test_task(void)
{
	static bool initialised = false;
	static acp_can_t acp_can1;
	static acp_can_t acp_can2;

	if (!initialised)
	{
		acp_can_init(&acp_can1, &hcan1, "CAN1", NULL, NULL);
		acp_can_init(&acp_can2, &hcan2, "CAN2", NULL, NULL);
		acp_can_set_board_address(&acp_can1, 0);
		acp_can_set_board_address(&acp_can2, 0);

		initialised = true;
	}
}
