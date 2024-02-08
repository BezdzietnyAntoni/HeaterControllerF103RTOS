/*
 * This .h / .c files implements service for display TM1638_PLUS
 *
 * Application only as send command into device
 *
 * @author : Antoni Bezdzietny
 * @create : 11.01.2024
 */

#ifndef INC_TM1638_H_
#define INC_TM1638_H_

#include "utilities.h"

/* --- Public variable declaration --- */
typedef struct
{
	gpio_t dio;
	gpio_t stb;
	gpio_t clk;
}tm_1638_gpio_t;

typedef enum
{
	TM1638_OK        		 = 0x00U,
	TM1638_ERROR			 = 0x01U,
	TM1638_DISPLAY_ERROR     = 0x02U,
	TM1638_LED_ERROR 	     = 0x03U,
}tm_1638_status_t;


/* --- Public function declaration --- */

/*
 *	Function sets TM1638 GPIO
 *	For handling communication set:
 *	- DIO - output push-pull, without pull up / down
 *	- STB - output push-pull, without pull up / down
 *	- CLK - output push-pull, without pull up / down
 *	@param gpio : GPIO configuration
 */
void tm1638_init( tm_1638_gpio_t* gpio );

/*
 * Function set TM1638 configuration: ON/OFF output, brightness
 * @param is_display_on 	 : 0 - display and LEDs OFF, 1 - display and leds ON
 * @param display_brightness : brightness value range [0...7]
 */
void tm1638_configuration( uint8_t is_display_on, uint8_t display_brightness );

/*
 * Function clear display and LEDs (extinction lighting)
 */
tm_1638_status_t tm1638_clear(void);


/* --- Functions for display 8 x 7 segments --- */

/*
 * Function clear display
 */
tm_1638_status_t tm1638_display_clear(void);

/*
 * Function set concrete char with or without dot on concrete 7 segment display
 * @param n_seven_segment : n-th 7 segment block [0...7]
 * @param c				  : char to set
 * @param is_dot		  : 0 / 1 is extra dot
 */
tm_1638_status_t tm1638_display_char(uint8_t n_seven_segment, char c, uint8_t is_dot);

/*
 * Function set string on display starting from n_seven_segment block
 * @param n_seven_segment : n-th starting 7 segment block [0...7]
 * @param *str			  : string to set
 * @param len		  	  : str length
 */
tm_1638_status_t tm1638_display_string(uint8_t n_seven_segment, const char* str, uint16_t len);


/* --- Functions for LEDs --- */

/*
 * Function clear LEDs
 */
tm_1638_status_t tm1638_led_clear(void);

/*
 * Function set concrete LED [0...7] to one state OFF(0)/ON(1)
 * @param n_led : n-th LED [0...7]
 * @param state : state OFF(0)/ON(1)
 */
tm_1638_status_t tm1638_led(uint8_t n_led, uint8_t state);

/*
 * Function set all 0..7 LEDs
 * @param state : all LEDs states ex. 0x11110000
 */
tm_1638_status_t tm1638_leds(uint8_t state);

#endif /* INC_TM1638_H_ */
