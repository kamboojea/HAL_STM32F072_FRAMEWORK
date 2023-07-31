#include "circular_buf.h"

/*****************************************************************************
   Functions
*****************************************************************************/

/**
 * @brief Initialize a circular buffer
 *
 * This function initializes a circular buffer with the given size and buffer.
 *
 * @param me Pointer to the circular buffer to initialize
 * @param size Maximum number of elements in the buffer
 * @param buffer Pointer to the buffer
 */
void circular_buf_init(circular_buf_t * const me, const uint16_t size, uint8_t *buffer)
{
    me->head = 0;
    me->tail = 0;
    me->size = size;
    me->buffer = buffer;
}


/**
 * @brief Check if the buffer is empty
 *
 * @param me Pointer to the circular buffer
 * @return true if the buffer is empty, false otherwise
 */
bool circular_buf_is_empty(const circular_buf_t * const me)
{
    return (me->head == me->tail);
}


/**
 * @brief Check if the buffer is full
 *
 * @param me Pointer to the circular buffer
 * @return true if the buffer is full, false otherwise
 */
bool circular_buf_is_full(const circular_buf_t * const me)
{
    uint16_t next_head = me->head + 1;

    if (next_head == me->size)
        next_head = 0;

    return (next_head == me->tail);
}


/**
 * @brief Insert an element into the buffer
 *
 * This function inserts an element into the buffer.
 *
 * @param me Pointer to the circular buffer
 * @param data Element to insert
 * @return true on success, false if the buffer is full
 */
bool circular_buf_insert(circular_buf_t * const me, uint8_t data)
{
    if (circular_buf_is_full(me))
	    return false;

    me->buffer[me->head] = data;

    if (++me->head == me->size)
    	me->head = 0;

    return true;
}


/**
 * @brief Remove an element from the buffer
 *
 * This function removes an element from the buffer.
 *
 * @param me Pointer to the circular buffer
 * @param data Element that was removed.
 * @return true on success, false if the buffer is empty
 */
bool circular_buf_remove(circular_buf_t * const me, uint8_t *data)
{
    if (circular_buf_is_empty(me))
        return false;

    *data = me->buffer[me->tail];

    if (++me->tail == me->size)
    	me->tail = 0;

    return true;
}
