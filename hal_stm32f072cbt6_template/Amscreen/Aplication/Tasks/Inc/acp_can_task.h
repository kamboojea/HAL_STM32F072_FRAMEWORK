/*
 * acp_can_task.h
 *
 *  Created on: 12 Apr 2023
 *      Author: LDavies
 */

#ifndef APPLICATION_TASKS_INC_ACP_CAN_TASK_H_
#define APPLICATION_TASKS_INC_ACP_CAN_TASK_H_

#include <stdbool.h>
#include "acp_can.h"

void acp_can_task(void);
bool acp_can_task_tx_packet_side_a(acp_packet_t *acp_packet);
bool acp_can_task_tx_packet_side_b(acp_packet_t *acp_packet);

#endif /* APPLICATION_TASKS_INC_ACP_CAN_TASK_H_ */
