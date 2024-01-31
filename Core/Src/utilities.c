/*
 * utilities.c
 *
 *  Created on: Jan 10, 2024
 *      Author: muntek
 */

#include "utilities.h"

#include <stdio.h>
#include "time.h"
#include "main.h"

void delay_us (uint16_t us)
{
	__HAL_TIM_SET_COUNTER(&htim1, 0);  // Reset counter (set value 0)
	while (__HAL_TIM_GET_COUNTER(&htim1) < us);  // Wait for delay

	__HAL_TIM_SET_COUNTER(&htim1, 0);  // Reset counter (security when irq)
}

void float_to_char(char *char_buff, float f_value)
{
	int value_i = (int) f_value;
	int value_f = (int) ((f_value - (float)value_i) * 10);

	sprintf(char_buff, "%3d.%d", value_i, value_f);
}
