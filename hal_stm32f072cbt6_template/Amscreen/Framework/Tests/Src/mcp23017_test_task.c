/*
 * mcp23017_task.c
 *
 *  Created on: 9 Mar 2023
 *      Author: ARahmatinia
 */
/*****************************************************************************
   Includes
*****************************************************************************/
#include <mcp23017_test_task.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "i2c.h"
#include "millis.h"
#include "usart.h"
#include "uart.h"
#include "stm32f1xx_hal.h"


/*****************************************************************************
   defines
*****************************************************************************/
#define IO_EXPANDER_NAME "MCP23017"
#define SAMPLE_INTERVAL 2
#define SENSOR_UPDATE_DELAY 5000 /**< Delay in msec */


/*****************************************************************************
   typedefs
*****************************************************************************/
typedef struct {
	mcp23017_device_t super;

	mcp23017_gpio_port_t port;
	mcp23017_gpio_pin_t pin;
	GPIO_PinState state;
}mcp23017_t;

typedef enum
{
	MCP23017_TASK_INITIALIZE,
	MCP23017_TASK_SETTING_PIN,
	MCP23017_TASK_WAITING,
	MCP23017_TASK_GETTING_PIN
}mcp23017_task_states_t;


/*****************************************************************************
   Variables
*****************************************************************************/
static I2C_HandleTypeDef * const io_expander_i2c_bus = &hi2c2;
static mcp23017_task_states_t mcp23017_task_state = MCP23017_TASK_INITIALIZE;
static millis_t sample_timer;
static millis_t sensor_update_timer;
static mcp23017_t mcp23017_io_expander;
static mcp23017_gpio_pin_t pins[] = {MCP23017_GP0, MCP23017_GP1, MCP23017_GP2, MCP23017_GP3, MCP23017_GP4, MCP23017_GP5, MCP23017_GP6, MCP23017_GP7};
static mcp23017_gpio_direction_t direction = MCP23017_IODIR_OUTPUT;
static uint8_t idx = 0;


/*****************************************************************************
   Functions Prototypes
*****************************************************************************/
static void mcp23017_test_initialize(mcp23017_t * const me, const char *name, I2C_HandleTypeDef * const i2c_bus, mcp23017_i2c_address_t i2c_address, mcp23017_gpio_port_t port, mcp23017_gpio_pin_t pin[], GPIO_PinState state);
static void mcp23017_setting(mcp23017_t * const me);
static void mcp23017_getting(mcp23017_t * const me);
static void mcp23017_waiting(mcp23017_t * const me);
//---------------- state machine functions ----------------
static void mcp23017_task_initialize(void);
static void mcp23017_task_setting_pin(void);
static void mcp23017_task_waiting(void);
static void mcp23017_task_getting_pin(void);
//---------------------------------------------------------


/*****************************************************************************
   Functions
*****************************************************************************/
void mcp23017_task(void)
{
	switch (mcp23017_task_state)
	{
	    case MCP23017_TASK_INITIALIZE:
	    	mcp23017_task_initialize();
	    break;

	    case MCP23017_TASK_SETTING_PIN:
	    	mcp23017_task_setting_pin();
	    break;

	    case MCP23017_TASK_GETTING_PIN:
	    	mcp23017_task_getting_pin();
	    break;

	    case MCP23017_TASK_WAITING:
	   	    	mcp23017_task_waiting();
	   	    break;
	}
}


/*****************************************************************************
  static functions
*****************************************************************************/
static void mcp23017_task_initialize()
{
	sample_timer = millis();
	mcp23017_test_initialize(&mcp23017_io_expander, IO_EXPANDER_NAME, io_expander_i2c_bus, MCP23017_ADDRESS_0, MCP23017_PORTA, pins, GPIO_PIN_RESET);
	mcp23017_task_state = MCP23017_TASK_SETTING_PIN;
}


static void mcp23017_task_setting_pin()
{
	if (millis_timeout(&sample_timer, SAMPLE_INTERVAL))
	{
		sensor_update_timer = millis();
		mcp23017_setting(&mcp23017_io_expander);
		mcp23017_task_state = MCP23017_TASK_GETTING_PIN;
	}
}


static void mcp23017_task_getting_pin()
{
	mcp23017_getting(&mcp23017_io_expander);
	mcp23017_task_state = MCP23017_TASK_WAITING;
}


static void mcp23017_task_waiting(void)
{
	// Check for UART activity on the keyboard
	if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE) == SET)
	{
		// UART activity detected, process the incoming data
		uint8_t rx_data;
		HAL_UART_Receive(&huart1, &rx_data, 1, HAL_MAX_DELAY);
		mcp23017_waiting(&mcp23017_io_expander);
		mcp23017_task_state = MCP23017_TASK_SETTING_PIN;
	}

	/*if (mstimer_timeout(&sensor_update_timer, SENSOR_UPDATE_DELAY))
	{
		mcp23017_waiting(&mcp23017_io_expander);
	    mcp23017_task_state = MCP23017_TASK_SETTING_PIN;
	}*/
}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
static void mcp23017_test_initialize(mcp23017_t * const me,
		                        const char *name,
		                        I2C_HandleTypeDef * const i2c_bus,
								mcp23017_i2c_address_t i2c_address,
								mcp23017_gpio_port_t port,
								mcp23017_gpio_pin_t pins[],
								GPIO_PinState state)
{
	mcp23017_init(&me->super, name, i2c_bus, i2c_address);
	me->port = port;
    me->pin = pins[0];
    me->state = state;
    char buffer[100];
    uint8_t pins_array_size = 15;

    for (int8_t idx = 0; idx <= pins_array_size; ++idx)
    {
    	if (idx > 7)
    	{
    		me->port = MCP23017_PORTB;
    		me->pin = pins[idx - 8];
    	}
    	else
    	{
    		me->port = MCP23017_PORTA;
    		me->pin = pins[idx];
    	}

    	HAL_StatusTypeDef result = mcp23017_set_pin_direction(&me->super, me->port,  me->pin,  direction); // setting all pins as outputs
    	sprintf(buffer,"1)  result = %d...\r\n", result);
    	HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), 100);
    	result = mcp23017_set_pin(&me->super, me->port, me->pin, state);                // putting all pins to low
    }

    //---------------------------- debug ----------------------------

    sprintf(buffer,"1)  MCP23017_TASK_INITIALIZE...\r\n");
    HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), 100);

    //---------------------------------------------------------------
}


static void mcp23017_setting(mcp23017_t * const me)
{
	me->state = GPIO_PIN_SET;

	 if (idx > 7)
	 {
		 me->port = MCP23017_PORTB;
		 me->pin = pins[idx - 8];
	 }
	 else
	 {
		 me->port = MCP23017_PORTA;
	     me->pin = pins[idx];
	 }
	 mcp23017_set_pin(&me->super, me->port, me->pin, me->state);

	 //---------------------------- debug ----------------------------
	 char buffer[100];
	 sprintf(buffer,"2)  MCP23017_TASK_SETTING PORT[%d], PIN[%d], state[%d]...\r\n", me->port, me->pin, me->state);
	 HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), 100);
	 //---------------------------------------------------------------

	 idx++;
	 idx %= 16;
}


static void mcp23017_getting(mcp23017_t * const me)
{
	mcp23017_get_pin(&me->super, me->port, me->pin, &me->state);

	//---------------------------- debug ----------------------------
    char buffer[100];
    sprintf(buffer,"3.1)  MCP23017_TASK_GETTING PORT[%d], PIN[%d], state[%d]...\r\n", me->port, me->pin, me->state);
    HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), 100);
    //---------------------------------------------------------------
}

static void mcp23017_waiting(mcp23017_t * const me)
{
	//---------------------------- debug ----------------------------
	char buffer[100];
	sprintf(buffer,"4.1)  MCP23017_TASK_WAITING...\r\n");
	HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), 100);
	//---------------------------------------------------------------

	// Reset the pin
	me->state = GPIO_PIN_RESET;
	mcp23017_set_pin(&me->super, me->port, me->pin, me->state);
	//---------------------------- debug ----------------------------

	sprintf(buffer,"4.2)  MCP23017_TASK_RESETTING PORT[%d], PIN[%d], state[%d]...\r\n", me->port, me->pin, me->state);
	HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), 100);
	//---------------------------------------------------------------
}
