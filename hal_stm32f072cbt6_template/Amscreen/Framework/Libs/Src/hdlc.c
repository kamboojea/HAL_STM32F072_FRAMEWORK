/*
 * hdlc.c
 *
 *  Created on: 15 Mar 2023
 *      Author: LDavies
 */
#include "uart.h"
#include "crc8.h"
#include "hdlc.h"

/*****************************************************************************
   Defines
*****************************************************************************/
#define HDLC_NVERT_FLAG 	0x20
#define HDLC_START_FLAG		0x7F
#define HDLC_STOP_FLAG		0x7E
#define HDLC_ESC_FLAG		0x7D


/*****************************************************************************
   Prototypes
*****************************************************************************/
static void hdlc_reset(hdlc_t *me);
static inline bool is_hdlc_flag(uint8_t data);


/*****************************************************************************
   Functions
*****************************************************************************/

/**
 * @brief Initializes an HDLC frame with the given buffer and length.
 *
 * @param me Pointer to the HDLC frame to initialize.
 * @param buffer Pointer to the buffer used to store the HDLC frame.
 * @param len The length of the buffer.
 * @return true if the initialization is successful, false otherwise.
 */
bool hdlc_init(hdlc_t *me, uint8_t* buffer, uint16_t size)
{
	me->buffer = buffer;
	me->len = 0;
	me->size = size;
	hdlc_reset(me);
	me->state = HDLC_FIND_START;
	return true;
}


/**
 * @brief Pushes a byte onto the HDLC frame.
 *
 * @param byte The byte to push onto the HDLC frame.
 * @return true if the byte is successfully pushed onto the HDLC frame, false otherwise.
 */
bool hdlc_receive(uart_t *uart, hdlc_t *hdlc)
{
	uint8_t data;

	while (uart_rx_byte(uart, &data))
	{
		switch (hdlc->state)
		{
			/*
			 * Detect start flag to indicate the start of a new incoming packet
			 */
			case HDLC_FIND_START:
			{
				if (data == HDLC_START_FLAG)
				{
					hdlc_reset(hdlc);
					hdlc->state = HDLC_FIND_STOP;
				}
				break;
			}

			/*
			 * With start flag found, escape, accumulate and calculate
			 * the checksum of packet data until stop flag detected.
			 */
			case HDLC_FIND_STOP:
			{
				// End of packet detected, process content
				if (data == HDLC_STOP_FLAG)
				{
					hdlc->state = HDLC_FIND_START;

					// Check if any data has been received and the CRC is valid
					if ((hdlc->idx > 0) && !hdlc->crc)
					{
						// Remove the CRC
						hdlc->idx--;
						return true;
					}
					else
					{
						// Just to provide a debug point
						hdlc->state = HDLC_FIND_START;
					}
					break;
				}

				// Are we out-of-sync?
				if (data == HDLC_START_FLAG)
				{
					hdlc_reset(hdlc);
					hdlc->state = HDLC_FIND_STOP;
					break;
				}

				// Escape flag received?
				if (data == HDLC_ESC_FLAG)
				{
					hdlc->last_byte = HDLC_ESC_FLAG;
					break ;
				}

				// Check if previous byte was an escape flag, if so escape current byte
				if (hdlc->last_byte == HDLC_ESC_FLAG)
				{
					data ^= HDLC_NVERT_FLAG;
					hdlc->last_byte = 0;
				}

				// Drop from if buffer is full
				if (hdlc->idx >= hdlc->size)
				{
					hdlc->state = HDLC_FIND_START;
					break;
				}

				// Append buffer and update CRC
				hdlc->buffer[hdlc->idx++] = data;
				hdlc->crc = crc8_calculate(data, hdlc->crc);
				break;
			}
		}
	}

    // No complete HDLC frame received yet
    return false;
}



/**
 * @brief Transmit HDLC encapsulated buffer using specified UART.
 * @param[in] UART Pointer UART handle.
 * @param[in] ptr Pointer to buffer encapsulated in HDLC frame.
 * @param[in] len length of buffer.
 */
void hdlc_transmit(uart_t *uart, hdlc_t *hdlc)
{
    uint8_t crc = crc8_array(hdlc->buffer, hdlc->len);

	uart_tx_byte(uart, HDLC_START_FLAG);

    for (hdlc->idx = 0; hdlc->idx < hdlc->len; hdlc->idx++)
	{
    	uint8_t data = hdlc->buffer[hdlc->idx];

        if (is_hdlc_flag(data))
		{
			uart_tx_byte(uart, HDLC_ESC_FLAG);
			uart_tx_byte(uart, data ^ HDLC_NVERT_FLAG);
        }
		else
		{
			uart_tx_byte(uart, data);
        }
    }

    if (is_hdlc_flag(crc))
	{
		uart_tx_byte(uart, HDLC_ESC_FLAG);
		uart_tx_byte(uart, crc ^ HDLC_NVERT_FLAG);
    }
	else
	{
		uart_tx_byte(uart, crc);
    }
	uart_tx_byte(uart, HDLC_STOP_FLAG);
}



/*****************************************************************************
   Private Functions
*****************************************************************************/

/**
 * @brief Resets an HDLC frame to its initial state.
 *
 * This function resets the index, last byte, CRC, and state of the HDLC frame to their
 * initial values. This is useful when starting a new HDLC frame or when an error has
 * occurred and the current HDLC frame needs to be discarded.
 *
 * @param me Pointer to the HDLC frame to reset.
 */
static void hdlc_reset(hdlc_t *me)
{
	me->idx = 0;
	me->len = 0;
	me->last_byte = 0;
	me->crc = 0;
	me->state = HDLC_FIND_START;
}


/**
 * @brief Checks whether a given byte needs to be escaped in an HDLC frame.
 * @param[in] data The byte to check.
 * @return True if the byte needs to be escaped, false otherwise.
 */
static inline bool is_hdlc_flag(uint8_t data)
{
    return (data == HDLC_START_FLAG || data == HDLC_STOP_FLAG || data == HDLC_ESC_FLAG);
}
