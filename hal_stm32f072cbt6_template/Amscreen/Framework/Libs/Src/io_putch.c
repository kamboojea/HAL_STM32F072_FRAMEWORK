/*
 * io_putch.c
 *
 *  Created on: Jul 4, 2023
 *      Author: LDavies
 */


#ifdef STM32F105xC

#include "stm32f1xx.h"

int __io_putchar(int ch)
{
	// Write character to ITM ch.0
	ITM_SendChar(ch);
	return(ch);
}

#elif defined(STM32F072xB)

#include "stm32f0xx.h"
#include "stm32f0xx_hal_uart.h"

#define DATA_SIZE 1
#define UART_DELAY 100

UART_HandleTypeDef huart2; // assuming that you've already initialized this somewhere

int __io_putchar(int ch)
{
    HAL_UART_Transmit(&huart2, (uint8_t*)&ch, DATA_SIZE, UART_DELAY);
    return ch;
}

#endif





