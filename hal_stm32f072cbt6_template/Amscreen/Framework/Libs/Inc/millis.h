/**
 * @file millis.h
 * @brief This file contains the declarations for the millis functions.
 *
 * @author Lee
 * @date Jan 20, 2023
 */
#ifndef MILLIS_H_
#define MILLIS_H_

#include <stdbool.h>
#include <stdint.h>

/*****************************************************************************
   Defines
*****************************************************************************/

/**
 * @brief millis type used to store millisecond tick count
 */
typedef uint32_t	millis_t;


/*****************************************************************************
   Functions
*****************************************************************************/
bool millis_timeout(millis_t* reference_time, millis_t interval);
millis_t millis(void);

#endif /* MILLIS_H_ */
