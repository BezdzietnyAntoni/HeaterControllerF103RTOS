/*
 * controller_menu.h
 *
 *  Created on: Feb 8, 2024
 *      Author: muntek
 */

#ifndef INC_CONTROLLER_MENU_H_
#define INC_CONTROLLER_MENU_H_

#include "menu_config.h"
#include "controller_devices.h"
#include <stdbool.h>

typedef struct
{
	menu_config_t *configuration;
	controller_devices_t *devices;
	menu_option_t menu_option;
	bool is_new_configuration;
}controller_menu_t;

void controller_menu_init(controller_menu_t **self,
						  controller_devices_t *controller_devices,
						  menu_config_t *configuration);

void controller_menu_deinit(controller_menu_t *self);

void controller_menu_run(controller_menu_t *self);

void controller_menu_set(controller_menu_t *self, menu_option_t menu_option);

void controller_menu_set_is_new_configuration(controller_menu_t *self, bool state);

bool controller_menu_get_is_new_configuration(controller_menu_t *self);


#endif /* INC_CONTROLLER_MENU_H_ */
