/*
 * controller_normal.h
 *
 *  Created on: Feb 8, 2024
 *      Author: muntek
 */

#ifndef INC_CONTROLLER_NORMAL_H_
#define INC_CONTROLLER_NORMAL_H_

#include "controller_devices.h"
#include "menu_config.h"
#include "stdbool.h"


typedef struct
{
	menu_config_t *configuration;
	controller_devices_t *devices;
	bool is_boiler_heating;
	bool is_instant_heating;
	uint8_t count_error_period;

}controller_normal_t;

void controller_normal_init(controller_normal_t **self,
							controller_devices_t *controller_devices,
							menu_config_t *configuration);

void controller_normal_deinit(controller_normal_t *self);

void controller_normal_run(controller_normal_t *self);

#endif /* INC_CONTROLLER_NORMAL_H_ */
