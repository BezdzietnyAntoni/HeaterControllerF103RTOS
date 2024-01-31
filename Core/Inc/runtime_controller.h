/*
 * runtime_controller.h
 *
 *  Created on: Jan 11, 2024
 *      Author: muntek
 */

#ifndef INC_RUNTIME_CONTROLLER_H_
#define INC_RUNTIME_CONTROLLER_H_

#include "configuration.h"
#include "flash.h"

typedef enum
{
	RUNTIME_NORMAL = 0x01,
	RUNTIME_MENU   = 0x02,
	RUNTIME_INPUT  = 0x03,
}runtime_controller_mode_t;

typedef struct
{
	/* Configuration fields */
	runtime_controller_mode_t  mode;
	uint32_t operation_time;

	/* Private fields*/
	uint32_t _last_action_tick;
	uint8_t _menu_option;
	configuration_t _configuration;
	float _input_value;
	float _temp_boiler;
	float _temp_pipe;
	uint8_t _is_boiler_heating;
	uint8_t _is_extra_heating;
	uint8_t is_new_config;
	flash_t *_flash;
}runtime_controller_t;

void runtime_controller_init(runtime_controller_t **self);

void runtime_controller_deinit(runtime_controller_t *self);

void runtime_controller_run(runtime_controller_t *self);



#endif /* INC_RUNTIME_CONTROLLER_H_ */
