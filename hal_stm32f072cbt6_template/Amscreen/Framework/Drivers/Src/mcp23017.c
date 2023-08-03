/*
 * MCP23017.c
 *
 *  Created on: 6 Mar 2023
 *      Author: ARahmatinia
 */
#include "mcp23017.h"


/*****************************************************************************
   defines
*****************************************************************************/
#define I2C_TIMEOUT 			2


/*****************************************************************************
   typedefs
*****************************************************************************/
// MCP23017 GPx lines
typedef enum
{
    MCP23017_IODIRA = 0x00,
    MCP23017_IODIRB = 0x01,
    MCP23017_GPIOA  = 0x12,
    MCP23017_GPIOB  = 0x13
} mcp23017_reg_t;


/*****************************************************************************
   Static Functions
*****************************************************************************/

/**
 * @brief Read a register from an MCP23017 device.
 *
 * @param[in] me Pointer to an mcp23017_device_t structure representing the MCP23017 device.
 * @param[in] reg The 16-bit register address to be read.
 * @param[out] data Pointer to a buffer where the read result will be stored.
 *
 * @retval HAL_OK if the operation is successful.
 */
static HAL_StatusTypeDef mcp23017_read_register(mcp23017_device_t * const me, uint16_t reg, uint8_t *data)
{
    return HAL_I2C_Mem_Read(me->i2c_bus, me->i2c_address, reg, 1, data, 1, I2C_TIMEOUT);
}


/**
 * @brief Write to a register of an MCP23017 device.
 *
 * @param[in] me Pointer to an mcp23017_device_t structure representing the MCP23017 device.
 * @param[in] reg The 16-bit register address to be written to.
 * @param[in] data Pointer to a mcp23017_gpio_direction_t containing the data to be written.
 *
 * @retval HAL_OK if the operation is successful.
 */
static HAL_StatusTypeDef mcp23017_write_register(mcp23017_device_t * const me, uint16_t reg, mcp23017_gpio_direction_t *data)
{
    return HAL_I2C_Mem_Write(me->i2c_bus, me->i2c_address, reg, 1, data, 1, I2C_TIMEOUT);
}


/**
 * @brief Set a specific bit of an MCP23017 register.
 *
 * @param[in] me Pointer to an mcp23017_device_t structure representing the MCP23017 device.
 * @param[in] reg The register to be written to, specified by an mcp23017_reg_t enum member.
 * @param[in] port The GPIO port to be modified, specified by an mcp23017_gpio_port_t enum member.
 * @param[in] pin The GPIO pin to be modified, specified by an mcp23017_gpio_pin_t enum member.
 * @param[in] value The value to be written to the specified bit (0 or 1).
 *
 * @retval HAL_OK if the operation is successful.
 */
static HAL_StatusTypeDef mcp23017_set_register_bit(mcp23017_device_t * const me, mcp23017_reg_t reg, mcp23017_gpio_port_t port, mcp23017_gpio_pin_t pin, uint8_t value)
{
    // validation
    if (port > MCP23017_PORTB  || pin > MCP23017_GP7)
    {
        return HAL_ERROR;
    }

    // read the current IODIR
    uint8_t data;
    HAL_StatusTypeDef result = mcp23017_read_register(me, reg | port, &data);

    if (result != HAL_OK)
    {
        return result;
    }

    // set pin direction
    data = value ? (data | (1 << pin)) : (data & ~(1 << pin));

    //write a new value to IODIR
    return  mcp23017_write_register(me, reg | port, &data);
}


/*****************************************************************************
   Functions
*****************************************************************************/

/**
 * @brief Initializes the MCP23017 device
 *
 * @param me Pointer to the device instance
 * @param name Device name
 * @param i2c_bus Pointer to the I2C bus instance
 * @param i2c_address I2C address of the device
 * @return HAL_StatusTypeDef HAL status
 */
HAL_StatusTypeDef mcp23017_init(mcp23017_device_t * const me, const char *name, I2C_HandleTypeDef * const i2c_bus,mcp23017_i2c_address_t i2c_address)
{
    me ->name = name;
    me ->i2c_bus = i2c_bus;
    me ->i2c_address = i2c_address << 1;
    return HAL_OK;
}


/**
 * @brief Sets the direction of a MCP23017 pin
 *
 * @param me Pointer to the device instance
 * @param port Port of the pin
 * @param pin Pin number
 * @param direction Pin direction
 * @return HAL_StatusTypeDef HAL status
 */
HAL_StatusTypeDef mcp23017_set_pin_direction(mcp23017_device_t * const me, mcp23017_gpio_port_t port, mcp23017_gpio_pin_t pin, mcp23017_gpio_direction_t direction)
{
    return mcp23017_set_register_bit(me, MCP23017_IODIRA, port, pin,(uint8_t)direction);
}


/**
 * @brief Sets the state of a MCP23017 pin
 *
 * @param me Pointer to the device instance
 * @param port Port of the pin
 * @param pin Pin number
 * @param state Pin state
 * @return HAL_StatusTypeDef HAL status
 */
HAL_StatusTypeDef mcp23017_set_pin(mcp23017_device_t * const me, mcp23017_gpio_port_t port, mcp23017_gpio_pin_t pin, GPIO_PinState state)
{
    return mcp23017_set_register_bit(me, MCP23017_GPIOA, port, pin,(uint8_t) state);
}


/**
 * @brief Gets the state of a MCP23017 pin
 *
 * @param me Pointer to the device instance
 * @param port Port of the pin
 * @param pin Pin number
 * @param state Pointer to the variable where the pin state will be stored
 * @return HAL_StatusTypeDef HAL status
 */
HAL_StatusTypeDef mcp23017_get_pin(mcp23017_device_t * const me, mcp23017_gpio_port_t port, mcp23017_gpio_pin_t pin, GPIO_PinState *state)
{
    uint8_t data;
    HAL_StatusTypeDef result = mcp23017_read_register(me, MCP23017_GPIOA | port, &data);
    *state = (data & (1 <<pin)) ? GPIO_PIN_SET : GPIO_PIN_RESET; // Check if the desired pin is high or low
    return result;
}
