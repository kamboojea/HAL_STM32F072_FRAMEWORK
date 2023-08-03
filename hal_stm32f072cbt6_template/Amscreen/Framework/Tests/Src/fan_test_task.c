/*
 * fan_test_task.c
 *
 *  Created on: May 3, 2023
 *      Author: LDavies
 */
#include "tacho.h"
#include "tim.h"

void fan_test_task(void)
{
	static bool initialised = false;
	static tacho_t tacho1;
	//static tacho_t tacho2;
	static uint32_t value;

	if (!initialised)
	{
		// Start PWM
		TIM2->CCR2 = 2880 - 576;
		HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
		board_gpio_set_fan1_power_pin(GPIO_PIN_SET);
		//HAL_TIM_Base_Start(&htim2);
		initialised = true;
		// Initialise tacho
		//initialised = tacho_init(&tacho1, &htim2, TIM_CHANNEL_1);
		//tacho_start(&tacho1);
	}

	//if (tacho_read(&tacho1, &value))
	//{
		//tacho_start(&tacho1);
	//}
}

