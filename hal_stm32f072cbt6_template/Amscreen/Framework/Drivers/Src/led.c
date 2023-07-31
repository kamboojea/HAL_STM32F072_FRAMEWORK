/**
 * @file led.c
 * @brief LED driver
 * @date Feb 17, 2023
 * @author LDavies
*/
#include <millis.h>
#include "led.h"


/*****************************************************************************
   Functions
*****************************************************************************/

/**
 * @brief  Initializes an led
 * @param  led: pointer to the led to initialize
 * @param  port: pointer to the GPIO port to which the led is connected
 * @param  pin: pin number of the GPIO port to which the led is connected
 * @retval None
 */
void led_init(led_t* const led, GPIO_TypeDef* port, uint16_t pin)
{
    led->port = port;
    led->pin = pin;
    led->flash_mode = false;
    led->state = false;
    led->timer = millis();
    HAL_GPIO_WritePin(led->port, led->pin, GPIO_PIN_RESET);
}


/**
 * @brief  Updates the state of an led
 * @param  led: pointer to the led to update
 * @retval None
 */
void led_update(led_t* const led)
{
    if (!led->flash_mode)
	    return;
	
    if (led->state)
    {
        if (millis_timeout(&led->timer, led->on_period))
        {
            led->state = false;
            HAL_GPIO_WritePin(led->port, led->pin, GPIO_PIN_RESET);
        }
    }
    else
    {
        if (millis_timeout(&led->timer, led->on_period))
        {
            led->state = true;
            HAL_GPIO_WritePin(led->port, led->pin, GPIO_PIN_SET);
        }
    }
}


/**
 * @brief  Set the flash rate of an led
 * @param  led: pointer to the led to flash
 * @param  on_period: on period of the led in milliseconds
 * @param  off_period: off period of the led in milliseconds
 * @retval None
 */
void led_set_flash_rate(led_t* const led, const uint32_t on_period, const uint32_t off_period)
{
    led->on_period = on_period;
    led->off_period = off_period;
    led->flash_mode = true;
    led->timer = millis();
}


/**
 * @brief  Turns on an led
 * @param  led: pointer to the led to turn on
 * @retval None
 */
void led_on(led_t* const led)
{
    led->state = true;
    led->flash_mode = false;
    HAL_GPIO_WritePin(led->port, led->pin, GPIO_PIN_SET);
}


/**
 * @brief  Turns off an led
 * @param  led: pointer to the led to turn off
 * @retval None
 */
void led_off(led_t* const led)
{
    led->state = false;
    led->flash_mode = false;
    HAL_GPIO_WritePin(led->port, led->pin, GPIO_PIN_RESET);
}
