/*
 * encoder.c
 *
 *  Created on: Jan 11, 2024
 *      Author: muntek
 */
#include "encoder.h"
#include "time.h"
#include "main.h"

typedef struct
{
	uint8_t is_pressed;
	uint32_t last_presed_time;
	uint32_t last_timmer_counter;
}encoder_t;

static encoder_t _encoder=
{
	.is_pressed = 0,
	.last_presed_time = 0,
	.last_timmer_counter = 0,
};

void encoder_set_sw_flag( void )
{
	// Button can not be pressed more often than 2Hz
	if(HAL_GetTick() - _encoder.last_presed_time < 500 )
		return;

	_encoder.is_pressed = 1;
	_encoder.last_presed_time = HAL_GetTick();
}

void encoder_reset_sw_falg( void ) { _encoder.is_pressed = 0; }

uint8_t encoder_get_sw_flag( void ) { return _encoder.is_pressed; }

void encoder_set_last_tim_count( uint16_t tim_count ) { _encoder.last_timmer_counter = tim_count; }

uint16_t encoder_get_last_tim_count( void ) { return _encoder.last_timmer_counter; }

uint16_t encoder_get_tim_count( void ) { return htim3.Instance->CNT; }

void encoder_update_last_time_count( void ) {encoder_set_last_tim_count(encoder_get_tim_count()); }

int8_t encoder_get_move_direction( void )
{
	int time_dif = encoder_get_tim_count() - encoder_get_last_tim_count();
	return time_dif / 4;  /*/4 because move 1 position counter 4 times in tim1 */
}
