/**
 * @file pca9546a.h
 * @brief Header file for PCA9546A I2C multiplexer device driver
 * @data-sheet https://www.mouser.co.uk/datasheet/2/302/nxp_pca9546a-1189027.pdf
 * @author ARahmatinia
 * @date 27 Feb 2023
 */

#ifndef DRIVERS_INC_PCA9546A_H_
#define DRIVERS_INC_PCA9546A_H_

#include "stm32f0xx_hal.h"

/*****************************************************************************
   Typedefs
*****************************************************************************/

/**
 * @brief Enumerates the possible PCA9546A I2C address values
 */
typedef enum {
   PCA9546A_ADDRESS_0 = 0X70,	/**< I2C address 0x70 */
   PCA9546A_ADDRESS_1,			/**< I2C address 0x71 */
   PCA9546A_ADDRESS_2,			/**< I2C address 0x72 */
   PCA9546A_ADDRESS_3,			/**< I2C address 0x73 */
   PCA9546A_ADDRESS_4,			/**< I2C address 0x74 */
   PCA9546A_ADDRESS_5,			/**< I2C address 0x75 */
   PCA9546A_ADDRESS_6,			/**< I2C address 0x76 */
   PCA9546A_ADDRESS_7			/**< I2C address 0x77 */
} pca9546a_i2c_address_t;


/**
 * @brief Enumerates the possible PCA9546A channels
 */
typedef enum {
	PCA9546A_NO_CHANNEL,
	PCA9546A_CHANNEL_0,
	PCA9546A_CHANNEL_1,
    PCA9546A_CHANNEL_2,
	PCA9546A_CHANNEL_3,
} pca9546a_channel_t;


/**
 * @brief Struct that represents a PCA9546A device
 */
typedef struct {
    const char *name; 			/**< Name of the device */
    I2C_HandleTypeDef *i2c_bus; /**< I2C bus to which the device is connected */
    uint8_t i2c_address; 		/**< I2C address of the device */
} pca9546a_device_t;


/*****************************************************************************
   Functions
*****************************************************************************/
HAL_StatusTypeDef pca9546a_init(pca9546a_device_t * const me, const char *name, I2C_HandleTypeDef * const i2c_bus, pca9546a_i2c_address_t i2c_address);
HAL_StatusTypeDef pca9546a_set_channel(pca9546a_device_t * const me,pca9546a_channel_t channel);
HAL_StatusTypeDef pca9546a_get_channel(pca9546a_device_t * const me, pca9546a_channel_t *channel);

#endif /* DRIVERS_INC_PCA9546A_H_ */
