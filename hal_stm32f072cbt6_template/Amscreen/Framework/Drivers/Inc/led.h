/**
 * @file led.h
 * @brief LED driver
 * @date Feb 17, 2023
 * @author ARahmatinia
*/
#ifndef DRIVERS_INC_LED_H_
#define DRIVERS_INC_LED_H_

#include <stdbool.h>
#include "millis.h"
#include "stm32f0xx_hal.h"

/*****************************************************************************
   Define
*****************************************************************************/

/**
 * @brief  led structure definition
 */
typedef struct {
    GPIO_TypeDef* port; 	/*!< GPIO port */
    uint16_t pin; 			/*!< GPIO pin */
    uint32_t on_period; 	/*!< On period in milliseconds */
    uint32_t off_period; 	/*!< Off period in milliseconds */
    bool state; 			/*!< Current state of the led (on/off) */
    millis_t timer; 		/*!< LED timer used in flash mode */
    bool flash_mode; 		/*!< a flag to indicate whether the led is in flash mode or not */
} led_t;


/*****************************************************************************
   Functions
*****************************************************************************/
void led_init(led_t* const led, GPIO_TypeDef* port, uint16_t pin);
void led_update(led_t* const led);
void led_set_flash_rate(led_t* const led, const uint32_t on_period, const uint32_t off_period);
void led_on(led_t* const led);
void led_off(led_t* const led);

#endif /* DRIVERS_INC_LED_H_ */
