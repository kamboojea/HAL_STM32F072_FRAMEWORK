/*
 * hdlc.h
 *
 *  Created on: 15 Mar 2023
 *      Author: LDavies
 */

#ifndef FRAMEWORK_LIBS_INC_HDLC_H_
#define FRAMEWORK_LIBS_INC_HDLC_H_

#include <stdint.h>
#include <stdbool.h>
#include <uart.h>

/*****************************************************************************
   Defines
*****************************************************************************/

/**
 * @brief Enumeration that defines the different states of an HDLC frame.
 */
typedef enum {
    HDLC_FIND_START,    		/**< Fin;ing the start flag of the HDLC frame */
    HDLC_FIND_STOP      		/**< Finding the stop flag of the HDLC frame */
} hdlc_state_t;


/**
 * @brief Structure that represents an HDLC frame.
 */
typedef struct {
    uint8_t *buffer;            /**< Pointer to the buffer used to store the HDLC frame */
    uint16_t len;               /**< The packet length */
    uint16_t size;				/**< The sized of the buffer */
    uint16_t idx;               /**< The current index in the buffer */
    int last_byte;              /**< The last byte received by the HDLC frame */
    char crc;                   /**< The CRC value of the HDLC frame */
    hdlc_state_t state;   		/**< The current state of the HDLC frame */
} hdlc_t;


/*****************************************************************************
   Functions
*****************************************************************************/
bool hdlc_init (hdlc_t *me, uint8_t* buffer, uint16_t size);
bool hdlc_receive(uart_t *uart, hdlc_t *hdlc);
void hdlc_transmit(uart_t *uart, hdlc_t *hdlc);

#endif /* FRAMEWORK_LIBS_INC_HDLC_H_ */
