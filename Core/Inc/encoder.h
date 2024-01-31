/*
 * encoder.h
 *
 *  Created on: Jan 11, 2024
 *      Author: muntek
 */

#ifndef INC_ENCODER_H_
#define INC_ENCODER_H_

#include <stdint.h>

void encoder_set_sw_flag( void );

void encoder_reset_sw_falg( void );

uint8_t encoder_get_sw_flag( void );

void encoder_set_last_tim_count( uint16_t tim_count);

uint16_t encoder_get_last_tim_count( void );

uint16_t encoder_get_tim_count( void );

int8_t encoder_get_move_direction( void );

void encoder_update_last_time_count( void );

#endif /* INC_ENCODER_H_ */
