/*
 * acp_commands.h
 *
 *  Created on: 18 Apr 2023
 *      Author: LDavies
 */

#ifndef FRAMEWORK_LIBS_INC_ACP_COMMANDS_H_
#define FRAMEWORK_LIBS_INC_ACP_COMMANDS_H_

#include <stdint.h>
#include <stdbool.h>
#include "acp_can.h"
#include "acp_result.h"


/*****************************************************************************
   Defines
*****************************************************************************/
#define ACP_COMMAND_RESPONSE	0x0100

// ACP command entry used in ACP framework and application command tables
typedef struct
{
    uint16_t function;
    acp_result_t (*fptr)(acp_packet_t *);
} acp_command_entry_t;



/*****************************************************************************
   Functions
*****************************************************************************/
acp_result_t acp_cmd_error(acp_packet_t *ptr, uint8_t error);
void acp_command_task(void);
acp_result_t acp_command_add_application_commands(const acp_command_entry_t *ptr);
acp_result_t acp_command_process(acp_packet_t *ptr);

#endif /* FRAMEWORK_LIBS_INC_ACP_COMMANDS_H_ */
