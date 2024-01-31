/*
 * tm1638.c
 *
 *  Created on: Jan 10, 2024
 *      Author: muntek
 */

/*
 * Private variable definition
 */

#include "tm1638.h"
#include "utilities.h"
#include "main.h"
#include "ascii_7segment.h"
#include "string.h"


#define TM1638_SETTINGS_ON  			0x88
#define TM1638_SETTINGS_OFF 			0x80
#define TM1638_SETTINGS_BRIGHTNES_MIN 	0x00
#define TM1638_SETTINGS_BRIGHTNES_MAX 	0x07

#define TM1638_SINGLE_WR_MODE  			0x44
#define TM1638_SEQUENTIAL_WR_MODE  		0x40

#define TM1638_ADDRESSING_COMMAND 		0xC0

#define TM1638_BUTTONS_READ				0x42 /* IMPORTANT not support mode*/

#define TM1638_N_LEDS 					0x08
#define TM1638_N_SEGMENTS 				0x08


static void _tm1638_wirte_byte(uint8_t byte)
{
	for(uint8_t i = 0; i < 8; ++i)
	{
		HAL_GPIO_WritePin(TM1638_DIO_GPIO_Port, TM1638_DIO_Pin, (byte & 0x01) );
		byte >>= 1;
		delay_us(1);
		HAL_GPIO_WritePin(TM1638_CLK_GPIO_Port, TM1638_CLK_Pin, 1);
		delay_us(1);
		HAL_GPIO_WritePin(TM1638_CLK_GPIO_Port, TM1638_CLK_Pin, 0);
	}
}

static tm_1638_status_t _tm1638_single_mode(uint8_t address, uint8_t data)
{
	if(address > 0x0F)
		return TM1638_ERROR;

	HAL_GPIO_WritePin(TM1638_STB_GPIO_Port, TM1638_STB_Pin, 0);
	delay_us(2);
	_tm1638_wirte_byte(TM1638_SINGLE_WR_MODE);		/* Write command */
	HAL_GPIO_WritePin(TM1638_STB_GPIO_Port, TM1638_STB_Pin, 1);
	delay_us(2);
	HAL_GPIO_WritePin(TM1638_STB_GPIO_Port, TM1638_STB_Pin, 0);
	delay_us(2);
	_tm1638_wirte_byte(TM1638_ADDRESSING_COMMAND | address);  /* Write address */
	_tm1638_wirte_byte(data);  /* Write data */
	delay_us(2);
	HAL_GPIO_WritePin(TM1638_STB_GPIO_Port, TM1638_STB_Pin, 1);
	delay_us(1);  /* Preventing time rules when single_mode run in loop*/

	return TM1638_OK;
}

static tm_1638_status_t _tm1638_sequential_mode(uint8_t address, uint8_t* data, uint8_t len)
{
	if(address > 0x0F)
		return TM1638_ERROR;

	uint8_t remaining_field = 0x10 - address;
	uint8_t data_to_send = min(remaining_field, len);

	HAL_GPIO_WritePin(TM1638_STB_GPIO_Port, TM1638_STB_Pin, 0);
	delay_us(2);
	_tm1638_wirte_byte(TM1638_SINGLE_WR_MODE);		/* Write command */
	HAL_GPIO_WritePin(TM1638_STB_GPIO_Port, TM1638_STB_Pin, 1);
	delay_us(2);
	HAL_GPIO_WritePin(TM1638_STB_GPIO_Port, TM1638_STB_Pin, 0);
	delay_us(2);
	_tm1638_wirte_byte(TM1638_SEQUENTIAL_WR_MODE | address);  /* Write address */
	for (int i = 0; i < data_to_send; ++i)
		_tm1638_wirte_byte(data[i]);  /* Write data */
	delay_us(2);
	HAL_GPIO_WritePin(TM1638_STB_GPIO_Port, TM1638_STB_Pin, 1);
	delay_us(1);  /* Preventing time rules when single_mode run in loop*/

	return TM1638_OK;
}

void tm1638_init(void)
{
	/* Set GPIO to default sleep mode */
	HAL_GPIO_WritePin(TM1638_STB_GPIO_Port, TM1638_STB_Pin, 1);
	HAL_GPIO_WritePin(TM1638_CLK_GPIO_Port, TM1638_CLK_Pin, 0);
	HAL_GPIO_WritePin(TM1638_DIO_GPIO_Port, TM1638_DIO_Pin, 0);
}

void tm1638_set_display(uint8_t is_display_on, uint8_t display_brightness)
{
	if(display_brightness > 0x07)
		display_brightness = 0x07;

	HAL_GPIO_WritePin(TM1638_STB_GPIO_Port, TM1638_STB_Pin, 0);
	delay_us(2);
	if(is_display_on)
	{
		_tm1638_wirte_byte(TM1638_SETTINGS_ON | display_brightness);
	}else
	{
		_tm1638_wirte_byte(TM1638_SETTINGS_OFF | display_brightness);
	}
	delay_us(2);
	HAL_GPIO_WritePin(TM1638_STB_GPIO_Port, TM1638_STB_Pin, 1);
}

tm_1638_status_t tm1638_set_display_clear(void)
{
	tm_1638_status_t st;

	for (uint8_t i = 0; i < TM1638_N_SEGMENTS; ++i) {
		st = _tm1638_single_mode(i << 1, 0x00);

		if(st != TM1638_OK)
			return st;
	}
	return TM1638_OK;
}

tm_1638_status_t tm1638_set_display_char(uint8_t n_seven_segment, char c, uint8_t is_dot)
{
	if(n_seven_segment > TM1638_N_SEGMENTS - 1)
			return TM1638_ERROR;

	uint8_t seven_segment_address = n_seven_segment << 1;

	uint8_t seven_segment_char_code = seven_segment_ASCII[((int)c) - SEVEN_SEGMENT_ASCII_OFFSET];

	if(is_dot)
		seven_segment_char_code |= seven_segment_ASCII[((int)'.') - SEVEN_SEGMENT_ASCII_OFFSET];



	return _tm1638_single_mode(seven_segment_address, seven_segment_char_code);
}

tm_1638_status_t tm1638_set_display_string(uint8_t n_seven_segment, const char* str, uint16_t len)
{
	tm_1638_status_t st;

	for(uint16_t i = 0; i < len; ++i)
	{
		// Check next symbol
		if(str[min(len-1, i+1)] == '.')
		{
			st = tm1638_set_display_char(n_seven_segment, str[i], 1);
			++i;
		}
		else
			st = tm1638_set_display_char(n_seven_segment, str[i], 0);

		++n_seven_segment;

		if(st != TM1638_OK)
			return st;

		if(n_seven_segment > TM1638_N_SEGMENTS - 1)
			break;
	}

	return TM1638_OK;
}

tm_1638_status_t tm1638_set_led_clear(void)
{
	tm_1638_status_t st;

	for (uint8_t i = 0; i < TM1638_N_LEDS; ++i) {
		st = _tm1638_single_mode((i << 1) + 0x01, 0x00);

		if(st != TM1638_OK)
			return st;
	}
	return TM1638_OK;
}

tm_1638_status_t tm1638_set_led(uint8_t n_led, uint8_t state)
{
	if(n_led > TM1638_N_LEDS - 1)
		return TM1638_ERROR;

	uint8_t led_address = (n_led << 1) + 0x01;

	if(state)
		return _tm1638_single_mode(led_address, 0x01);
	else
		return _tm1638_single_mode(led_address, 0x00);
}

tm_1638_status_t tm1638_set_leds(uint8_t leds_states){
	tm_1638_status_t st;
	for(uint8_t i = 0; i < TM1638_N_LEDS; ++i)
	{
		st = tm1638_set_led(i, (leds_states & 0x01) );
		leds_states >>= 1;
		if(st != TM1638_OK)
			return st;
	}
	return TM1638_OK;
}

tm_1638_status_t tm1638_set_clear_all(void)
{
	uint8_t n_data = 16;
	uint8_t data[n_data];
	memset(data, 0, n_data);
	return _tm1638_sequential_mode(0, data, n_data);
}

