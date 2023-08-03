/*
 * acp_can_task.h
 *
 *  Created on: 12 Apr 2023
 *      Author: LDavies
 */

#ifndef APPLICATION_TASKS_INC_ACP_CAN_PROCESS_H_
#define APPLICATION_TASKS_INC_ACP_CAN_PROCESS_H_

#include <stdbool.h>
#include "stm32f0xx.h"

bool acp_can_process_initialise(CAN_HandleTypeDef *hcan, uint16_t board_address);
void acp_can_process_task(void);

#endif /* APPLICATION_TASKS_INC_ACP_CAN_PROCESS_H_ */
