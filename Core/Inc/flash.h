/*
 * This .h / .c files implements flash service
 *
 * Flash memory in this project is used to store
 * user configuration parameters.
 *
 * In STM32F1xx flash memory is organized by page.
 * Chosen page address: starting 0x0801FC00, ending 0x0801FFFF
 *
 * Erase data from memory is available only by clear all page.
 *
 * So the data is stored to fill in all the fields of the page.
 * The page is then cleaned and saved from the beginning again.
 *
 * @author : Antoni Bezdzietny
 * @create : 18.01.2024
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

/*
 * Function initialize flash memory
 */
void flash_init(flash_t **self);

/*
 * Function deinitialize flash memory
 */
void flash_deinit(flash_t *self);

/*
 * Write n data to flash memory in half world
 * @param *self : self pointer flash memory
 * @param *data : data array to store in flash
 * @param len   : data length
 */
flash_status_t flash_write_data(flash_t *self, uint8_t *data, uint8_t len);

/*
 * Read n data to flash memory in half world
 * @param *self : self pointer flash memory
 * @param *data : data array to store in flash
 * @param len   : data length
 */
flash_status_t flash_read_data(flash_t *self, uint8_t *data, uint8_t len);


#endif /* INC_FLASH_H_ */
