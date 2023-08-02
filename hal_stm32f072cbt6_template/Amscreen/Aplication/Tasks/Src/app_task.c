/*
 * application.c
 *
 *  Created on: Feb 22, 2023
 *      Author: LDavies
 *
 *  ToDo: Check resources available and investigate using FreeRTOS
 */
#include "led_task.h"
#include "acp_commands.h"
#include "acp_can_task.h"


/*****************************************************************************
   Defines
*****************************************************************************/
//#define SIMULATE_COLD_START


/*****************************************************************************
   Functions
*****************************************************************************/

#ifdef SIMULATE_COLD_START

void app_simulate_cold_start(void)
{
	board_gpio_deinit();
	HAL_Delay(3000);
}

#endif

/**
 * @brief The main application initialisation function.
 *
 * This function is responsible for coordinating the initialisation of all tasks needed for the application.
 */
void app_init(void)
{
#ifdef SIMULATE_COLD_START
	app_simulate_cold_start();
#endif
//	initial_condition_reset();
//	acp_command_add_application_commands(application_commands);
//	board_gpio_init();
//	production_test_link_init();
}



/**
 * @brief The main application task function.
 *
 * This function is responsible for coordinating the execution of all
 * tasks needed for the application.
 */
void app_task(void)
{
	acp_can_task();
	led_task();
}
