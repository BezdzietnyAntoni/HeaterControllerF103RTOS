#include "encoder.h"
#include "time.h"


/* --- Private variable definition and declaration --- */
typedef struct
{
	/* Configuration fields */
	TIM_HandleTypeDef *tim;
	gpio_t sw_gpio;

	/* Encoder used fields */
	uint8_t is_pressed;
	uint32_t last_presed_time;
	uint32_t last_timmer_counter;
}encoder_t;


static encoder_t _encoder;


/* --- Public function definition --- */

void encoder_init( TIM_HandleTypeDef* timer, gpio_t* sw_gpio  )
{
	_encoder.tim = timer;
	_encoder.sw_gpio.GPIOx = sw_gpio->GPIOx;
	_encoder.sw_gpio.GPIO_Pin = sw_gpio->GPIO_Pin;

	_encoder.is_pressed = 0;
	_encoder.last_presed_time = 0;
	_encoder.last_timmer_counter = 0;
}

void encoder_set_sw_flag( void )
{
	// Button can not be pressed more often than 2Hz (debouncing)
	if(HAL_GetTick() - _encoder.last_presed_time < 500 )
		return;

	_encoder.is_pressed = 1;
	_encoder.last_presed_time = HAL_GetTick();
}

void encoder_reset_sw_falg( void ) { _encoder.is_pressed = 0; }

uint8_t encoder_get_sw_flag( void ) { return _encoder.is_pressed; }

void encoder_set_last_tim_count( uint16_t tim_count ) { _encoder.last_timmer_counter = tim_count; }

uint16_t encoder_get_last_tim_count( void ) { return _encoder.last_timmer_counter; }

uint16_t encoder_get_tim_count( void ) { return _encoder.tim->Instance->CNT; }

void encoder_update_last_time_count( void ) {encoder_set_last_tim_count(encoder_get_tim_count()); }

int8_t encoder_get_move_direction( void )
{
	int time_dif = encoder_get_tim_count() - encoder_get_last_tim_count();
	return time_dif / 4;  /*/4 because move 1 position counter 4 times in tim1 */
}
