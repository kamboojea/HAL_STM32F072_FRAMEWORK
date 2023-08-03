/*
 * spi_flash_task.h
 *
 *  Created on: 9 Jun 2023
 *      Author: ARahmatinia
 */

#ifndef FRAMEWORK_TESTS_INC_SPI_FLASH_TASK_H_
#define FRAMEWORK_TESTS_INC_SPI_FLASH_TASK_H_

/*****************************************************************************
   Libraries
*****************************************************************************/
#include "stm32f1xx_hal.h"
#include <stdbool.h>
#include "spi_flash.h"
#include "acp_result.h"
#include "spi.h"
/*****************************************************************************
   Functions
*****************************************************************************/
void flash_test_task(void);

#endif /* FRAMEWORK_TESTS_INC_SPI_FLASH_TASK_H_ */
