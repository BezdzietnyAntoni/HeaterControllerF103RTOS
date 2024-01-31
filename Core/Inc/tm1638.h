/*
 * tm1638.h
 *
 *  Created on: Jan 10, 2024
 *      Author: muntek
 */

#ifndef INC_TM1638_H_
#define INC_TM1638_H_

#include <stdint.h>

typedef enum
{
	TM1638_OK        = 0x00U,
	TM1638_ERROR     = 0x01U,
}tm_1638_status_t;

void tm1638_init(void);

void tm1638_set_display(uint8_t is_display_on, uint8_t display_brightness);

tm_1638_status_t tm1638_set_display_clear(void);

tm_1638_status_t tm1638_set_display_char(uint8_t n_seven_segment, char c, uint8_t is_dot);

tm_1638_status_t tm1638_set_display_string(uint8_t n_seven_segment, const char* str, uint16_t len);

tm_1638_status_t tm1638_set_led_clear(void);

tm_1638_status_t tm1638_set_led(uint8_t n_led, uint8_t state);

tm_1638_status_t tm1638_set_leds(uint8_t state);

tm_1638_status_t tm1638_set_clear_all(void);

#endif /* INC_TM1638_H_ */
