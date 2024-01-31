/*
 * one_wire.h
 *
 *  Created on: Jan 10, 2024
 *      Author: muntek
 */

#ifndef INC_ONE_WIRE_H_
#define INC_ONE_WIRE_H_

#include "stdint.h"

typedef enum
{
	  ONE_WIRE_OK       = 0x00U,
	  ONE_WIRE_ERROR    = 0x01U,
}one_wire_status_t;

/*
 * Send sequence RESET one wire
 */
one_wire_status_t one_wire_reset(void);

/*
 * Send 8bits of data by one wire
 * @param byte : 8bits of data
 */
void one_wire_write_byte(uint8_t byte);

/*
 * Read 8bits from one wire
 * @return: 8bits data
 */
uint8_t one_wire_read_byte(void);

/*
 * Calculate control summ
 * @param data : data
 * @param len  : data length
 */
uint8_t one_wire_crc(const uint8_t* data, uint8_t len);

#endif /* INC_ONE_WIRE_H_ */
