/**
 *
 * @file led_task.c
 * @brief Source file for controlling status and error LEDs
 * @author LDavies
 * @date 22 Feb 2023
 */
#include "led.h"
#include "led_task.h"
#include "main.h"


/*****************************************************************************
   Defines
*****************************************************************************/
#define ERROR_LED_PORT					ERROR_LED_GPIO_Port
#define ERROR_LED_PIN					ERROR_LED_Pin

#define STATUS_LED_PORT					STATUS_LED_GPIO_Port
#define STATUS_LED_PIN					STATUS_LED_Pin

#define INITIAL_STATUS_LED_ON_PERIOD 	1000
#define INITIAL_STATUS_LED_OFF_PERIOD 	1000

#define NORMAL_STATUS_LED_ON_PERIOD 	250
#define NORMAL_STATUS_LED_OFF_PERIOD 	2000


/*****************************************************************************
   Variables
*****************************************************************************/
led_t error_led;
led_t status_led;


/*****************************************************************************
   Functions Prototypes
*****************************************************************************/
static void led_task_init(void);
static void led_task_update(void);


/*****************************************************************************
   Public Functions
*****************************************************************************/

/**
 * @brief LED task called externally to update the error and status LEDs
 */
void led_task(void)
{
    static bool initialised = false;

    if (!initialised)
    {
        led_task_init();
        initialised = true;
    }
    led_task_update();
}


/**
 * @brief Switch on the error LED
 */
void led_task_error_led_on(void)
{
    led_on(&error_led);
}


/**
 * @brief Switch off the error LED
 */
void led_task_error_led_off(void)
{
    led_off(&error_led);
}


/**
 * @brief Set the status on/off flash period in millisecs
 * @param on_period The on period of the LED
 * @param off_period The off period of the LED
 */
void led_task_error_led_flash_rate(uint32_t on_period, uint32_t off_period)
{
    led_set_flash_rate(&error_led, on_period, off_period);
}


/**
 * @brief Switch on the status LED
 */
void led_task_status_led_on(void)
{
    led_on(&status_led);
}


/**
 * @brief Switch off the status LED
 */
void led_task_status_led_off(void)
{
    led_off(&status_led);
}


/**
 * @brief Set the status on/off flash period in millisecs
 * @param on_period The on period of the LED
 * @param off_period The off period of the LED
 */
void led_task_status_led_flash_rate(uint32_t on_period, uint32_t off_period)
{
    led_set_flash_rate(&status_led, on_period, off_period);
}


/**
 * @brief Set the status LED to initial flash rate
 */
void led_task_intial_led_startup_flash_rate(void)
{
	led_task_status_led_flash_rate(INITIAL_STATUS_LED_ON_PERIOD, INITIAL_STATUS_LED_OFF_PERIOD);
}


/**
 * @brief Set the status LED to normal flash rate
 */
void led_task_status_led_normal_flash_rate(void)
{
	led_task_status_led_flash_rate(NORMAL_STATUS_LED_ON_PERIOD, NORMAL_STATUS_LED_OFF_PERIOD);
}


/*****************************************************************************
   Private Functions
*****************************************************************************/

/**
 * @brief Initialise error and status LED classes
 */
static void led_task_init(void)
{
    led_init(&error_led, ERROR_LED_PORT, ERROR_LED_PIN);
    led_init(&status_led, STATUS_LED_PORT, STATUS_LED_PIN);
    led_task_intial_led_startup_flash_rate();
    led_task_error_led_off();
}


/**
 * @brief Update error and status LED classes
 */
static void led_task_update(void)
{
    // Update LED classes
	led_update(&status_led);
	led_update(&error_led);
}
