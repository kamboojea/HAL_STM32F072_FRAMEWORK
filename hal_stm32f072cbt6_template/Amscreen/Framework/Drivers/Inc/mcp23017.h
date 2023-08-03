#ifndef DRIVERS_INC_MCP23017_H_
#define DRIVERS_INC_MCP23017_H_

#include "stm32f0xx_hal.h"


/*****************************************************************************
  Defines
 *****************************************************************************/
#define MCP23017_BASE_I2C_ADDRESS 	0x20

/**
 * @enum mcp23017_i2c_address_t
 * @brief Enumeration representing the possible I2C addresses of an MCP23017 device.
 *
 * The enumeration defines the possible I2C addresses for an MCP23017 device.
 * The first address is fixed at 0x40 and the remaining addresses are incrementally
 * increased by 1, up to a maximum of 0x47.
 */
typedef enum {
	MCP23017_ADDRESS_0 = MCP23017_BASE_I2C_ADDRESS,
	MCP23017_ADDRESS_1,
	MCP23017_ADDRESS_2,
	MCP23017_ADDRESS_3,
	MCP23017_ADDRESS_4,
	MCP23017_ADDRESS_5,
	MCP23017_ADDRESS_6,
	MCP23017_ADDRESS_7
} mcp23017_i2c_address_t;


/**
 * @enum mcp23017_gpio_port_t
 * @brief Enumeration representing the two GPIO ports of an MCP23017 device.
 *
 * The enumeration defines the two possible GPIO ports for an MCP23017 device:
 * Port A and Port B.
 */
typedef enum
{
	MCP23017_PORTA = 0x00,
	MCP23017_PORTB = 0x01
} mcp23017_gpio_port_t;


/**
 * @enum mcp23017_gpio_pin_t
 * @brief Enumeration representing the GPIO pins of an MCP23017 device.
 *
 * The enumeration defines the possible GPIO pins for an MCP23017 device, numbered from 0 to 7.
 * The pin numbers correspond to the physical pin numbers on the device, from GP0 to GP7.
 */
typedef enum
{
	MCP23017_GP0,
	MCP23017_GP1,
	MCP23017_GP2,
	MCP23017_GP3,
	MCP23017_GP4,
	MCP23017_GP5,
	MCP23017_GP6,
	MCP23017_GP7
} mcp23017_gpio_pin_t;


/**
 * @enum mcp23017_gpio_direction_t
 * @brief Enumeration representing the possible GPIO directions of an MCP23017 device.
 *
 * The enumeration defines the possible GPIO directions for an MCP23017 device: Input and Output.
 * The direction values are used to configure the I/O direction register (IODIR) of the device.
 * MCP23017_IODIR_OUTPUT corresponds to a value of 0, and MCP23017_IODIR_INPUT corresponds to a value of 1.
 */
typedef enum
{
	MCP23017_IODIR_OUTPUT,
	MCP23017_IODIR_INPUT
} mcp23017_gpio_direction_t;


/**
 * @struct mcp23017_device_t
 * @brief Structure representing an MCP23017 device.
 *
 * The structure contains information about an MCP23017 device, including its name, the I2C bus to which it is
 * connected, and its I2C address. The I2C address is represented by the enumeration mcp23017_i2c_address_t.
 */
typedef struct {
    const char *name; 					/**< Name of the device */
    I2C_HandleTypeDef *i2c_bus; 		/**< I2C bus to which the device is connected */
    mcp23017_i2c_address_t i2c_address;	/**< I2C address of the device */
} mcp23017_device_t;


/*****************************************************************************
  Functions
 *****************************************************************************/
HAL_StatusTypeDef mcp23017_init(mcp23017_device_t * const me, const char *name, I2C_HandleTypeDef * const i2c_bus, mcp23017_i2c_address_t i2c_address);
HAL_StatusTypeDef mcp23017_set_pin_direction(mcp23017_device_t * const me, mcp23017_gpio_port_t port, mcp23017_gpio_pin_t pin, mcp23017_gpio_direction_t direction);
HAL_StatusTypeDef mcp23017_set_pin(mcp23017_device_t * const me, mcp23017_gpio_port_t port, mcp23017_gpio_pin_t pin, GPIO_PinState state);
HAL_StatusTypeDef mcp23017_get_pin(mcp23017_device_t * const me, mcp23017_gpio_port_t port, mcp23017_gpio_pin_t pin, GPIO_PinState *state);

#endif /* DRIVERS_INC_MCP23017_H_ */
