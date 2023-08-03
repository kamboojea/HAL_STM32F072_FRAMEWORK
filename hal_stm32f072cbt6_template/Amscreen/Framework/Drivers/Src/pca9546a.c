/*
 * pca9546A.c
 *
 *  Created on: 27 Feb 2023
 *      Author: ARahmatinia
 */
#include "pca9546a.h"


/*****************************************************************************
   defines
*****************************************************************************/
/* Define TIMEOUT constant */
#define I2C_TIMEOUT 2


/*****************************************************************************
   Functions
*****************************************************************************/

/**
 * @brief Initializes a PCA9546A device
 * @param me Pointer to the PCA9546A device to initialize
 * @param name Name of the device
 * @param i2c_bus Pointer to the I2C bus to which the device is connected
 * @param i2c_address I2C address of the device
 * @return HAL status code
 */
HAL_StatusTypeDef pca9546a_init(pca9546a_device_t * const me, const char *name, I2C_HandleTypeDef * const i2c_bus, pca9546a_i2c_address_t i2c_address)
{
	me -> name = name;
	me -> i2c_bus = i2c_bus;
	me -> i2c_address = i2c_address << 1;
	return HAL_OK;
}


/**
 * @brief Sets the PCA9546A channel
 * @param me Pointer to the PCA9546A device
 * @param channel Channel to set
 * @return HAL status code
 */
HAL_StatusTypeDef pca9546a_set_channel(pca9546a_device_t * const me, pca9546a_channel_t channel)
{
	uint8_t control_register;

	if(channel > PCA9546A_CHANNEL_3)
	{
		return HAL_ERROR;
	}

	if (channel == PCA9546A_NO_CHANNEL)
	{
		control_register = 0;
	}
	else
	{
		control_register =  1 << (channel - 1);
	}

	return HAL_I2C_Master_Transmit(me -> i2c_bus, me -> i2c_address, &control_register, 1,I2C_TIMEOUT);
}



/**
 * @brief Gets the PCA9546A channel
 * @param me Pointer to the PCA9546A device
 * @param channel Channel to set
 * @return HAL status code
 */
HAL_StatusTypeDef pca9546a_get_channel(pca9546a_device_t * const me, pca9546a_channel_t *channel)
{
	uint8_t control_register;
	HAL_StatusTypeDef result = HAL_I2C_Master_Receive(me -> i2c_bus, me -> i2c_address, &control_register, 1, I2C_TIMEOUT);

	if (result == HAL_OK)
	{
		if(control_register == 8)
			*channel = PCA9546A_CHANNEL_3;
		else if(control_register == 4)
			*channel = PCA9546A_CHANNEL_2;
		else if(control_register == 2)
			*channel = PCA9546A_CHANNEL_1;
		else if(control_register == 1)
			*channel = PCA9546A_CHANNEL_0;
		else
			*channel = PCA9546A_NO_CHANNEL;
	}
	return result;
}
