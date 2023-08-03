/**
 * @file acp_vectors.c
 * @author LDavies
 * @date 7 Mar 2017
 * @brief This file contains functions and definitions related to vector table manipulation in STM32F0 devices.
 */

#include <stdint.h>
#include "stm32f0xx_hal.h"

extern uint32_t _application_start_address; /**< Start address of the application in flash. */
extern uint32_t _estack;                    /**< Top address of the stack. */
extern uint32_t Reset_Handler;              /**< Address of the reset handler function. */

#define APPLICATION_START_ADDR             (uint32_t)(&_application_start_address) /**< Macro to define application start address. */
#define TOP_OF_STACK                       (uint32_t)(&_estack)                     /**< Macro to define the top of the stack. */
#define RESET_HANDLER                      (uint32_t)(&Reset_Handler)               /**< Macro to define the reset handler. */
#define INTERRUPT_VECTOR_SIZE 48            /**< Size of the interrupt vector table. */

/**
 * @brief Copies the interrupt vector table to RAM and remaps the SRAM into 0x0000 0000.
 */
void vectors_to_ram(void)
{
    // Copy interrupt vector table to the RAM.
    volatile uint32_t *vector_table = (volatile uint32_t *)0x20000000;

    for(uint32_t idx = 0; idx < INTERRUPT_VECTOR_SIZE; idx++)
    {
    	vector_table[idx] = *(__IO uint32_t*)((uint32_t)APPLICATION_START_ADDR + (idx << 2));
    }

    // Reset all AHB peripherals
    __HAL_RCC_AHB_FORCE_RESET();
    __HAL_RCC_AHB_RELEASE_RESET();

    // Enable SYSCFG clock
    __HAL_RCC_SYSCFG_CLK_ENABLE();

    // Remap SRAM into 0x0000 0000
    __HAL_SYSCFG_REMAPMEMORY_SRAM();

    __enable_irq();
}

/**
 * @brief Dummy vector table.
 */
uint32_t *dummy_vector_table[] __attribute__((section(".vector_table"))) =
{
    (uint32_t *)TOP_OF_STACK,   	// initial stack pointer
    (uint32_t *)RESET_HANDLER    	// main as Reset_Handler
};
