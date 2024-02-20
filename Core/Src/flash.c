#include "flash.h"

#include <stm32f1xx.h>
#include <stdlib.h>

/* --- Private variable --- */

#define FLASH_HALFWORLD_OFFSET 	2
#define FLASH_PAGE_START 		0x0800FC00  //0x0801FC00
#define FLASH_PAGE_END			0x0800FFFF  //0x0801FFFF

/* --- Private function declaration --- */
static flash_status_t _flash_erase_page(flash_t *self);
static void _flash_find_current_address(flash_t *self);


/* --- Public function definition --- */

void flash_init(flash_t **self)
{
	(*self) = calloc(1, sizeof(flash_t));
	(*self)->_start_address_page = FLASH_PAGE_START;
	(*self)->_end_address_page   = FLASH_PAGE_END - 1;

	_flash_find_current_address((*self));
}


void flash_deinit(flash_t *self)
{
	free(self);
}


flash_status_t flash_write_data(flash_t *self, uint8_t *data, uint8_t len)
{
	if((self->current_address + (len * FLASH_HALFWORLD_OFFSET)) > FLASH_PAGE_END )
	{
		flash_status_t status = _flash_erase_page(self);
		if( status != FLASH_OK)
			return status;
	}


	HAL_FLASH_Unlock();
	for(uint8_t i = 0; i < len; ++i)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, self->current_address, data[i]);
		self->current_address += FLASH_HALFWORLD_OFFSET;
	}
	HAL_FLASH_Lock();

	return FLASH_OK;
}


flash_status_t flash_read_data(flash_t *self, uint8_t *data, uint8_t len)
{
	uint32_t start_read_address = (self->current_address - (len * FLASH_HALFWORLD_OFFSET));
	if(start_read_address < FLASH_PAGE_START)
		return FLASH_INVALID_LEN;

	for(uint8_t i = 0; i < len; ++i)
		data[i] = *(uint8_t*)(start_read_address + (i * 2) );

	return FLASH_OK;
}



/* --- Private function definition --- */

void _flash_find_current_address(flash_t *self)
{
	self->current_address = self->_start_address_page;

	// If all page is empty
	if(*(uint8_t*)(self->_start_address_page) == 0xFF)
	{
		self->current_address = self->_start_address_page;
		return;
	}

	// If all page is overwrite
	if(*(uint8_t*)(self->_end_address_page) != 0xFF)
	{
		self->current_address = self->_end_address_page + FLASH_HALFWORLD_OFFSET;
		return;
	}

	uint32_t start_addr   = self->_start_address_page;
	uint32_t end_addr     = self->_end_address_page;
	uint32_t current_addr;

	while((end_addr - start_addr) > 2)
	{
		current_addr = (end_addr + start_addr) / 2;
		current_addr -= current_addr % 2;

		if(*(uint8_t*)(current_addr) == 0xFF)
			end_addr = current_addr;
		else
			start_addr = current_addr;
	}

	self->current_address = end_addr;
}


flash_status_t _flash_erase_page(flash_t *self)
{
	FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t pageError;

	EraseInitStruct.TypeErase 	= FLASH_TYPEERASE_PAGES;
	EraseInitStruct.PageAddress = self->_start_address_page;
	EraseInitStruct.NbPages 	= 1;

	HAL_FLASH_Unlock();
	HAL_FLASHEx_Erase(&EraseInitStruct, &pageError);
	HAL_FLASH_Lock();

	if(pageError != HAL_OK)
		return FLASH_ERASE_ERROR;

	self->current_address = self->_start_address_page;

	return FLASH_OK;
}

