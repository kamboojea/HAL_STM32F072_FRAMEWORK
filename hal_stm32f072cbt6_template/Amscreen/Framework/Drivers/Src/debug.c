/*
 * debug.c
 *
 *  Created on: 18 May 2023
 *      Author: LDavies
 *
 *  ToDo: Needs improvment!
 */
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "debug.h"

/*****************************************************************************
   defines
*****************************************************************************/


/*****************************************************************************
   Variables
*****************************************************************************/


/*****************************************************************************
   Functions
*****************************************************************************/
#ifdef DEBUG_ENABLED

/**
 * @brief Print formatted output to the debug UART
 * @param format Format string for printf
 * @param ... Variable arguments for printf
 * @return True if the printf output is successfully transmitted, False otherwise
 */
bool debug_printf(const char *format, ... )
{
    printf ("%08d - ", (int)HAL_GetTick());

    va_list aptr;
    va_start(aptr, format);
    vprintf(format, aptr);
    printf("\r\n");
    va_end(aptr);
    return true;
}


char* debug_acp_packet_to_string(acp_packet_t *acp_packet)
{
	static char buffer[100];
	snprintf(buffer, sizeof(buffer), "S%02x D%02x F%03x L%02d %02x %02x %02x %02x %02x %02x %02x %02x",
			acp_packet->acp_header.source, acp_packet->acp_header.destination, acp_packet->acp_header.function, acp_packet->len,
			acp_packet->data[0], acp_packet->data[1], acp_packet->data[2], acp_packet->data[3], acp_packet->data[4], acp_packet->data[5], acp_packet->data[6], acp_packet->data[7]);
	return buffer;
}

#else

/**
 * @brief Empty function
 * @param huart Pointer to the UART handle
 * @return True if the UART initialization is successful, False otherwise
 */
bool debug_set_uart(UART_HandleTypeDef *huart)
{
	return false;
}


/**
 * @brief Empty function
 * @param format Format string for printf
 * @param ... Variable arguments for printf
 * @return True if the printf output is successfully transmitted, False otherwise
 */
bool debug_printf(const char *format, ... )
{
	return false;
}

#endif


