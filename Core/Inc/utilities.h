/*
 * utilities.h
 *
 *  Created on: Jan 10, 2024
 *      Author: muntek
 */

#ifndef INC_UTILITIES_H_
#define INC_UTILITIES_H_

#include "stdint.h"
#include "stm32f1xx.h"

 #define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

 #define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

void delay_us (uint16_t us);

void float_to_char(char *char_buff, float f_value);

typedef struct
{
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
}gpio_t;

#endif /* INC_UTILITIES_H_ */
