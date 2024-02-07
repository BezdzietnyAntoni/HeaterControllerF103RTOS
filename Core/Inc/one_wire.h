/*
 * This .h / .c files implements simple one wire protocol
 *
 *	Remember before use to call function one_wire_init( GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin ).
 *	Passing as argument gpio argument (GPIO set as Output Open Drain without pull-up or pull-down).
 *
 * @author : Antoni Bezdzietny
 * @create : 10.01.2024
 */

#ifndef INC_ONE_WIRE_H_
#define INC_ONE_WIRE_H_

#include "utilities.h"

typedef enum
{
	  ONE_WIRE_OK        = 0x00U,
	  ONE_WIRE_ERROR     = 0x01U,
	  ONE_WIRE_CRC_ERROR = 0x02U,
}one_wire_status_t;

/*
 * Initialize one wire configuration gpio
 * @param GPIOx : one wire port
 * @param GPIO_Pin : one wire pin
 */
void one_wire_init(	GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin );

/*
 * Send sequence RESET one wire
 */
one_wire_status_t one_wire_write_reset( void );

/*
 * Send 8bits of data by one wire
 * @param byte : 8bits of data
 */
void one_wire_write_byte( uint8_t byte );

/*
 * Read 8bits from one wire
 * @return: 8bits data
 */
uint8_t one_wire_read_byte( void );

/*
 * Calculate one wire control sum
 * @param data : data
 * @param len  : data length
 */
uint8_t one_wire_crc( const uint8_t* data, uint8_t len );

#endif /* INC_ONE_WIRE_H_ */
