/*
 * debug_printf.h
 *
 *  Created on: 18 May 2023
 *      Author: LDavies
 */

#ifndef FRAMEWORK_DRIVERS_INC_DEBUG_H_
#define FRAMEWORK_DRIVERS_INC_DEBUG_H_

#include <stdarg.h>
#include "stm32f0xx.h"
#include "uart.h"
#include <acp_can.h>

#define DEBUG_ENABLED

bool debug_set_uart(UART_HandleTypeDef *huart);
bool debug_printf(const char *format, ... );
char* debug_acp_packet_to_string(acp_packet_t *acp_packet);

#endif /* FRAMEWORK_DRIVERS_INC_DEBUG_H_ */
