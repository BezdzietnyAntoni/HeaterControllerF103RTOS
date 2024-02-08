/*
 * This .h / .c files implements service for encoder with switch
 *
 * All encoder configuration was configured in STM32CubeIDE .ioc file
 * 1. Select timer
 * 		- Set in Encoder Mode
 * 		- Connect TI1 and TI2
 *  	- Set counter period: 0xffff (the biggest value as possible)
 *  	- Set encoder mode: Encoder Mode TI1 and TI2
 *
 * 2. Encoder switch
 * 		- Select GPIO pin and connect
 * 		- GPIO mode External Interrupt Mode with Falling
 * 		- Enable Pull-up
 *
 * 3. Enable global interrupt
 * 4. On interrupt handle encoder_set_sw_flag()
 *
 * @author : Antoni Bezdzietny
 * @create : 12.01.2024
 */


#ifndef INC_ENCODER_H_
#define INC_ENCODER_H_

#include "stm32f1xx.h"
#include "utilities.h"

/*
 * Initialization encoder
 * NOTE: Before use set GPIO and timer configuration (see above)
 *
 * @param *timer   : Encoder timer handle
 * @param *sw_gpio : Encoder switch gpio
 */
void encoder_init( TIM_HandleTypeDef* timer, gpio_t* sw_gpio  );

/* --- Encoder switch --- */

/*
 * Function set flag on encoder switch
 * NOTE: Use in interrupt handle
 */
void encoder_set_sw_flag( void );

/*
 * Function reset encoder switch flag
 */
void encoder_reset_sw_falg( void );

/*
 * Function return encoder switch flag
 */
uint8_t encoder_get_sw_flag( void );


/* --- Encoder timer (rotation) --- */

/*
 * Function set timer count (encoder value) to encoder struct
 */
void encoder_set_last_tim_count( uint16_t tim_count);

/*
 * Function return timer count from encoder struct
 */
uint16_t encoder_get_last_tim_count( void );

/*
 * Function return timer instance cnt
 */
uint16_t encoder_get_tim_count( void );

/*
 * Function returning encoder rotation in three option [-1, 0, 1]
 */
int8_t encoder_get_move_direction( void );

/*
 * Function update encoder cnt
 */
void encoder_update_last_time_count( void );

#endif /* INC_ENCODER_H_ */
