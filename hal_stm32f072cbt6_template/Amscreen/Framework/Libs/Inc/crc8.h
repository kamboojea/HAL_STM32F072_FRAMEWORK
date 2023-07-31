/*
 * crc.h
 *
 *  Created on: 15 Mar 2023
 *      Author: LDavies
 */

#ifndef FRAMEWORK_LIBS_INC_CRC8_H_
#define FRAMEWORK_LIBS_INC_CRC8_H_

#include <stdint.h>

uint8_t crc8_calculate(uint8_t crc, uint8_t data);
uint8_t crc8_array(uint8_t *data, uint16_t len);

#endif /* FRAMEWORK_LIBS_INC_CRC8_H_ */
