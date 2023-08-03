/*
 * board_info.h
 *
 *  Created on: 18 Apr 2023
 *      Author: LDavies
 */

#ifndef APPLICATION_LIBS_INC_BOARD_INFO_H_
#define APPLICATION_LIBS_INC_BOARD_INFO_H_

/********************************************************************************
   Defines
********************************************************************************/

typedef struct __attribute((packed))
{
	// Both the Board ID and version strings are wrapped by unique tags, this
	// allows the HWM to extract the relevant information from the update file

	const char board_id[16];          	// Board ID  "****BBBBBBBB****"
    const char version[16];             // Version   "####MM.mm.pp####"
} board_info_t;

extern const board_info_t /* __attribute((section(".version"))) */ board_info;


#endif /* APPLICATION_LIBS_INC_BOARD_INFO_H_ */
