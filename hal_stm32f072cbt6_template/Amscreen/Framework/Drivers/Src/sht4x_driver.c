/**
 * @file sht4x_driver.c
 * @brief Implementation file for SHT4x sensor driver
 * @date Feb 17, 2023
 * @author ARahmatinia
 */
#include "sht4x_driver.h"


/*****************************************************************************
   defines
*****************************************************************************/
#define SHT4x_ADDR 0x44 << 1               /**< SHT4x sensor address */
#define SHT4x_HIGH_PRECISION_COMMAND 0xFD  /**< Command for high precision measurement */
#define SHT4x_SOFT_RESET_COMMAND 0x94      /**< Command for soft resetting the sensor */
#define SHT4x_SERIAL_NUMBER_COMMAND 0x89
#define SHT4x_SERIAL_NUMBER_CRC 0x31
#define I2C_TIMEOUT 2                      /**< Timeout for I2C operations in milliseconds */
#define DATA_SIZE  6                       /**< Size of data buffer for I2C communication */


/*****************************************************************************
   Functions
*****************************************************************************/

/**
 * @brief Initializes the SHT4x device.
 * @param[in,out] me pointer to the SHT4x device structure
 * @param[in] name The name of the device
 * @param[in] i2c_bus pointer to the I2C bus handle (exp:hi2c2)
 * @return Returns the status of the initialization
 */
HAL_StatusTypeDef sht4x_init(sht4x_device_t * const me, const char *name, I2C_HandleTypeDef * const i2c_bus)
{
    me->name = name;
    me->i2c_bus = i2c_bus;
    return HAL_OK;
}


/**
 * @brief Starts temperature and humidity measurement for the SHT4x device.
 * @param[in] me pointer to the SHT4x device structure
 * @return Returns the status of starting the measurement
 */
HAL_StatusTypeDef sht4x_start_temperature_humidity_measurement(const sht4x_device_t * const me)
{
	uint8_t command = SHT4x_HIGH_PRECISION_COMMAND;
	return HAL_I2C_Master_Transmit(me -> i2c_bus, SHT4x_ADDR, &command, 1,I2C_TIMEOUT);
}


/**
 * @brief Reads the temperature & humidity measurements from the SHT4x device.
 * @param[in] me pointer to the SHT4x device structure
 * @param[out] temperature pointer to the location to store the temperature measurement
 * @param[out] humidity pointer to the location to store the humidity measurement
 * @return Returns the status of reading the temperature measurement
 */
HAL_StatusTypeDef sht4x_read_temperature_humidity_measurements(const sht4x_device_t * const me, float *temperature, float *humidity)
{
	uint8_t data[DATA_SIZE] = {0};

	HAL_StatusTypeDef result = HAL_I2C_Master_Receive(me -> i2c_bus, SHT4x_ADDR, data, DATA_SIZE, I2C_TIMEOUT);
	*temperature = (-45.0 + (175.0 *(float)((data[0]<<8) | data[1])/ 65535.0)) ;
	*humidity = ((100.0 * (float)((data[3] << 8) | data[4])) / 65535.0);
	return result;
}


/**
 * @brief Soft reset the SHT4x device.
 * @param[in] me pointer to the SHT4x device structure
 * @return Returns the status of starting the measurement
 */
HAL_StatusTypeDef sht4x_soft_reset(const sht4x_device_t * const me)
{
	uint8_t command = SHT4x_SOFT_RESET_COMMAND;
	return HAL_I2C_Master_Transmit(me -> i2c_bus, SHT4x_ADDR, &command, 1,I2C_TIMEOUT);
}

