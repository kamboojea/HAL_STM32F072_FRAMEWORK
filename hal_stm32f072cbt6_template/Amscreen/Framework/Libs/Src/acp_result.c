/**
 * @file acp_result.c
 * @brief Source file for ACP result codes
 *
 * This file contains the implementation of the function for converting the HAL status to the ACP result result.
 */

#include "acp_result.h"

/*****************************************************************************
   Functions
*****************************************************************************/

/**
 * @brief Convert HAL status to ACP result
 *
 * This function converts the HAL status to the ACP result code.
 *
 * @param status The HAL status to convert.
 * @return The corresponding ACP result.
 */
acp_result_t hal_status_to_acp_result(HAL_StatusTypeDef status)
{
    switch (status)
    {
        case HAL_OK:
            return ACP_SUCCESS;

        case HAL_ERROR:
            return ACP_ERROR_HAL_ERROR;

        case HAL_BUSY:
            return ACP_ERROR_HAL_BUSY;

        case HAL_TIMEOUT:
            return ACP_ERROR_HAL_TIMEOUT;

        default:
        	break;
    }
    return ACP_ERROR_HAL_UNKNOWN;
}




