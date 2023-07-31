#ifndef CIRCULAR_BUF_H
#define CIRCULAR_BUF_H

#include <stdint.h>
#include <stdbool.h>

/*****************************************************************************
   Defines
*****************************************************************************/

/**
 * @brief Circular buffer structure
 */
typedef struct {
    volatile uint16_t head;   	/**< Index of the first element */
    volatile uint16_t tail;   	/**< Index of the last element */
    uint16_t size; 				/**< Maximum number of elements */
    uint8_t *buffer; 			/**< Pointer to the buffer */
} circular_buf_t;


/*****************************************************************************
   Functions
*****************************************************************************/
void circular_buf_init(circular_buf_t * const me, const uint16_t size, uint8_t *buffer);
bool circular_buf_is_empty(const circular_buf_t * const me);
bool circular_buf_is_full(const circular_buf_t * const me);
uint16_t circular_buf_remaining(const circular_buf_t * const me);
bool circular_buf_insert_array(circular_buf_t * const me, uint8_t *buffer, uint16_t size);
bool circular_buf_insert(circular_buf_t * const me, uint8_t data);
bool circular_buf_remove(circular_buf_t * const me, uint8_t *data);

#endif
