/*
 * runtime_controller.h
 *
 *  Created on: Jan 11, 2024
 *      Author: muntek
 */

#ifndef INC_RUNTIME_CONTROLLER_H_
#define INC_RUNTIME_CONTROLLER_H_

#include <menu_config.h>
#include "flash.h"

#include "ds18b20.h"
#include "relay.h"


typedef enum
{
	RUNTIME_NORMAL = 0x01,
	RUNTIME_MENU   = 0x02,
	RUNTIME_INPUT  = 0x03,
}runtime_controller_mode_t;

typedef struct
{
	/* Configuration fields */
	runtime_controller_mode_t mode;
	uint32_t operation_time;

	/* Devices */
	relay_t *relay;
	flash_t *flash;
	ds18b20_device_t *ds18b_20_boiler;
	ds18b20_device_t *ds18b_20_pipe;

	/* Private fields*/
	uint32_t _last_action_tick;
	uint8_t _menu_option;
	menu_config_t _configuration;
	float _input_value;
	uint8_t _is_boiler_heating;
	uint8_t _is_extra_heating;
	uint8_t is_new_config;
}runtime_controller_t;

void runtime_controller_init(runtime_controller_t **self);

void runtime_controller_deinit(runtime_controller_t *self);

void runtime_controller_run(runtime_controller_t *self);



#endif /* INC_RUNTIME_CONTROLLER_H_ */
