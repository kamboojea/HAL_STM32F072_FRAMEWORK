/**
 *
 * @file led_task.h
 * @brief Header file for controlling status and error LEDs
 * @author LDavies
 * @date 22 Feb 2023
 */
#include <stdint.h>

#ifndef TASKS_INC_LED_TASK_H_
#define TASKS_INC_LED_TASK_H_

void led_task(void);
void led_task_status_led_on(void);
void led_task_status_led_off(void);
void led_task_error_led_on(void);
void led_task_error_led_off(void);
void led_task_status_led_flash_rate(uint32_t on_period, uint32_t off_period);
void led_task_error_led_flash_rate(uint32_t on_period, uint32_t off_period);
void led_task_intial_led_startup_flash_rate(void);
void led_task_status_led_normal_flash_rate(void);

#endif /* TASKS_INC_LED_TASK_H_ */
