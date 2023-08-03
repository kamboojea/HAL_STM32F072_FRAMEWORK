/*
 * uart_test_task.c
 *
 *  Created on: Mar 1, 2023
 *      Author: LDavies
 */
#include "circular_buf.h"
#include "uart.h"

#define UART1_RX_BUFFER_SIZE	512
#define UART1_TX_BUFFER_SIZE	512
#define UART3_RX_BUFFER_SIZE	512
#define UART3_TX_BUFFER_SIZE	512

uart_t uart1;
uint8_t uart1_rx_buffer[UART1_RX_BUFFER_SIZE];
uint8_t uart1_tx_buffer[UART1_TX_BUFFER_SIZE];
circular_buf_t uart1_rx_circular_buffer;
circular_buf_t uart1_tx_circular_buffer;

uart_t uart3;
uint8_t uart3_rx_buffer[UART3_RX_BUFFER_SIZE];
uint8_t uart3_tx_buffer[UART3_RX_BUFFER_SIZE];
circular_buf_t uart3_rx_circular_buffer;
circular_buf_t uart3_tx_circular_buffer;


void uart_test_task(void)
{
	static bool initialised = false;
	uint8_t byte;

	if (!initialised)
	{
		circular_buf_init(&uart1_rx_circular_buffer, UART1_RX_BUFFER_SIZE, uart1_rx_buffer);
		circular_buf_init(&uart1_tx_circular_buffer, UART1_TX_BUFFER_SIZE, uart1_tx_buffer);

		if (uart_init(&uart1, &huart1, "UART1", &uart1_rx_circular_buffer, &uart1_tx_circular_buffer))
			uart_tx_string(&uart1, "PASS - UART1 assigned\r\n");
		else
			uart_tx_string(&uart1, "FAIL - Failed to assign UART1\r\n");

		circular_buf_init(&uart3_rx_circular_buffer, UART3_RX_BUFFER_SIZE, uart3_rx_buffer);
		circular_buf_init(&uart3_tx_circular_buffer, UART3_TX_BUFFER_SIZE, uart3_tx_buffer);

		if (uart_init(&uart3, &huart3, "UART3", &uart3_rx_circular_buffer, &uart3_tx_circular_buffer))
			uart_tx_string(&uart1, "PASS - UART3 assigned\r\n");
		else
			uart_tx_string(&uart1, "FAIL - Failed to assign UART3\r\n");

		// Try reassigning UART1
		uart_t uart_test;
		if (!uart_init(&uart_test, &huart1, "UART1", &uart1_rx_circular_buffer, &uart1_tx_circular_buffer))
			uart_tx_string(&uart1, "PASS - (uart_test) UART1 already assigned\r\n");
		else
			uart_tx_string(&uart1, "FAIL - (uart_test) Should not be possible to reuse UART1\r\n");

		if (!uart_init(&uart3, &huart1, "UART1", &uart1_rx_circular_buffer, &uart1_tx_circular_buffer))
			uart_tx_string(&uart1, "PASS - (uart3) UART1 already assigned\r\n");
		else
			uart_tx_string(&uart1, "FAIL - (uart3) Should not be possible to reuse UART1\r\n");

		initialised = true;
	}


	while (uart_rx_byte(&uart1, &byte))
		uart_tx_byte(&uart3, byte);

	while (uart_rx_byte(&uart3, &byte))
		uart_tx_byte(&uart1, byte);
}
