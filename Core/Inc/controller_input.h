/*
 * controller_input.h
 *
 *  Created on: Feb 8, 2024
 *      Author: muntek
 */

#ifndef INC_CONTROLLER_INPUT_H_
#define INC_CONTROLLER_INPUT_H_

#include "menu_config.h"
#include "controller_devices.h"
#include <stdbool.h>


typedef struct
{
	menu_config_t *configuration;
	controller_devices_t *devices;
	float input_value;
	menu_option_t menu_option;
}controller_input_t;

void controller_input_init(controller_input_t **self,
						   controller_devices_t *controller_devices,
						   menu_config_t *configuration);

void controller_input_deinit(controller_input_t *self);

void controller_input_run(controller_input_t *self);

void controller_input_set(controller_input_t *self, menu_option_t menu_option);

bool controller_input_update_input(controller_input_t *self);

float controller_input_get_input(controller_input_t *self);


#endif /* INC_CONTROLLER_INPUT_H_ */
