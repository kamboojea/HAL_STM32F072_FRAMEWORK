/*
 * board_info.c
 *
 *  Created on: 18 Apr 2023
 *      Author: LDavies
 */
#include "board_info.h"

const board_info_t __attribute((section(".version"))) board_info =
{
    // board ID, the format should be ****BBBBBBBB****,
    // where BBBBBBBB if the board ID. For example the
    // board ID for the PCB_NAME is ****201849PM****
    "****TEMPLATE****",

    // Application version, the format should be ****MM.mm.bb****
    // where:
    //    MM    Major
    //    mm    Minor
    //    PP    Patch
    //
    "####00.00.00####",
};



