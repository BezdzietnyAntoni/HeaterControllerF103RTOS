/*
 * flash.h
 *
 *  Created on: Jan 18, 2024
 *      Author: Antoni Bezdzietny
 */

#ifndef INC_FLASH_H_
#define INC_FLASH_H_

#include <stdint.h>

typedef struct
{
	uint32_t _start_address_page;
	uint32_t _end_address_page;
	uint32_t current_address;
}flash_t;

typedef enum
{
	FLASH_OK = 0,
	FLASH_INVALID_LEN = 1,
	FLASH_ERASE_ERROR = 2,
}flash_status_t;

void flash_init(flash_t **self);

void flash_deinit(flash_t *self);

void flash_find_current_address(flash_t *self);

flash_status_t flash_write_data(flash_t *self, uint8_t *data, uint8_t len);

flash_status_t flash_read_data(flash_t *self, uint8_t *data, uint8_t len);

flash_status_t flash_erase_page(flash_t *self);
#endif /* INC_FLASH_H_ */
