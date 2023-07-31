/**

 * @file sht4x_driver.h
 * @brief Header file for SHT4x sensor driver
 * @date Feb 17, 2023
 * @author ARahmatinia
*/
#ifndef INC_SHT4X_DRIVER_H_
#define INC_SHT4X_DRIVER_H_

#include "stm32f0xx_hal.h"


/*****************************************************************************
   typedefs
 *****************************************************************************/

/**
 * @brief Structure that holds the SHT4x device information.
 */
typedef struct
{
	const char *name; 			// The name of the device
    I2C_HandleTypeDef *i2c_bus; // Pointer to the I2C bus handle (exp:hi2c2)
} sht4x_device_t;


/*****************************************************************************
   Functions
*****************************************************************************/
HAL_StatusTypeDef sht4x_init(sht4x_device_t * const me, const char *name, I2C_HandleTypeDef * const i2c_bus);
HAL_StatusTypeDef sht4x_start_temperature_humidity_measurement(const sht4x_device_t * const me);
HAL_StatusTypeDef sht4x_read_temperature_humidity_measurements(const sht4x_device_t * const me, float *temperature, float *humidity);
HAL_StatusTypeDef sht4x_soft_reset(const sht4x_device_t * const me);

#endif /* INC_SHT4X_DRIVER_H_ */
