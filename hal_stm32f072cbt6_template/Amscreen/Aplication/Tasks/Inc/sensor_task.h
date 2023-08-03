/*
 * sensor_task.h
 *
 *  Created on: Feb 23, 2023
 *      Author: ARahmatinia
 */

#ifndef TASKS_INC_SENSOR_TASK_H_
#define TASKS_INC_SENSOR_TASK_H_

#include "stm32f0xx_hal.h"
#include "acp_result.h"

void sensor_task(void);
acp_result_t sensor_task_read_temperature(int16_t *temperature);
acp_result_t sensor_task_read_humidity(uint16_t *humidity);


#endif /* TASKS_INC_SENSOR_TASK_H_ */

