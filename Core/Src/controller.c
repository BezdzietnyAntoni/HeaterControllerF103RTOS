#include "controller.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>



/* --- Private variable (configuration controller) --- */

#define MAX_PERIOD_WITHOUT_ACTION 		10000



/* --- Private function declaration --- */

void _controller_on_click(controller_t *self);
void _controller_change_mode_normal(controller_t *self);
void _controller_change_mode_menu(controller_t *self);
void _controller_change_mode_input(controller_t *self);
void _controller_change_if_without_action(controller_t *self);

void _controller_reset_encoder(controller_t *self);

void _controller_devices_init(controller_t *self);
void _controller_configuration_init(controller_t *self);



/* --- Public interface definition --- */

void controller_init(controller_t **self)
{
	(*self) = calloc(1, sizeof(controller_t));

	_controller_devices_init((*self));

	(*self)->controller_mode = CONTROLLER_MODE_NORMAL;

	_controller_configuration_init((*self));

	controller_normal_init(&(*self)->controller_normal, (*self)->controller_devices, &(*self)->configuration);
	controller_menu_init(&(*self)->controller_menu, (*self)->controller_devices, &(*self)->configuration);
	controller_input_init(&(*self)->controller_input, (*self)->controller_devices, &(*self)->configuration);
}

void controller_deinit(controller_t *self)
{
	controller_normal_deinit(self->controller_normal);
	controller_menu_deinit(self->controller_menu);
	controller_input_deinit(self->controller_input);
	controller_devices_deinit(self->controller_devices);
	free(self);
}

void controller_run(controller_t *self)
{
	switch (self->controller_mode)
	{
		case CONTROLLER_MODE_NORMAL:
			controller_normal_run(self->controller_normal);
			break;
		case CONTROLLER_MODE_MENU:
			controller_menu_run(self->controller_menu);
			_controller_reset_encoder(self);
			_controller_change_if_without_action(self);
			break;
		case CONTROLLER_MODE_INPUT:
			controller_input_run(self->controller_input);
			_controller_reset_encoder(self);
			_controller_change_if_without_action(self);
			break;
	}

	_controller_on_click(self);
}


/* --- Private function definition --- */

/*
 * Function check if encoder switch was pressed change mode
 * Note: Only this function can reset encoder flag
 */
void _controller_on_click(controller_t *self)
{
	// If switch was not pressed -> return
	if( !encoder_get_sw_flag() )
		return;

	encoder_reset_sw_falg();
	self->last_action_tick = HAL_GetTick();

	switch (self->controller_mode)
	{
		case CONTROLLER_MODE_NORMAL:
			_controller_change_mode_normal(self);
			break;
		case CONTROLLER_MODE_MENU:
			_controller_change_mode_menu(self);
			break;
		case CONTROLLER_MODE_INPUT:
			_controller_change_mode_input(self);
			break;
	}
}


void _controller_change_mode_normal(controller_t *self)
{
	/*Change mode from normal -> menu */
	controller_menu_set(self->controller_menu, 0);
	controller_menu_set_is_new_configuration(self->controller_menu, false);
	_controller_reset_encoder(self);
	self->controller_mode = CONTROLLER_MODE_MENU;
}


void _controller_change_mode_menu(controller_t *self)
{
	controller_input_set(self->controller_input,
						 self->controller_menu->menu_option);
	_controller_reset_encoder(self);
	self->controller_mode = CONTROLLER_MODE_INPUT;
}


void _controller_change_mode_input(controller_t *self)
{
	/* */
	if(controller_input_update_input(self->controller_input))
		controller_menu_set_is_new_configuration(self->controller_menu, true);

	controller_menu_set(self->controller_menu,
						self->controller_input->menu_option);
	_controller_reset_encoder(self);
	self->controller_mode = CONTROLLER_MODE_MENU;
}

void _controller_change_if_without_action(controller_t *self)
{
	if (HAL_GetTick() - self->last_action_tick < MAX_PERIOD_WITHOUT_ACTION)
		return;

	if( controller_menu_get_is_new_configuration(self->controller_menu) )
		flash_write_data(self->controller_devices->flash, self->configuration, MENU_N_OPTION);

	self->controller_mode = CONTROLLER_MODE_NORMAL;
}

void _controller_reset_encoder(controller_t *self)
{
	if(!encoder_get_move_direction())
		return;

	encoder_update_last_time_count();
	self->last_action_tick = HAL_GetTick();
}

void _controller_devices_init(controller_t *self)
{
	controller_devices_init(&self->controller_devices);

	/* Beginning communication with thermometers */
	ds18b20_set_resolution(self->controller_devices->ds18b_20_pipe->rom_code, DS18B20_11B);
	ds18b20_set_resolution(self->controller_devices->ds18b_20_boiler->rom_code, DS18B20_11B);
	ds18b20_start_measure_all();

	/* Beginning communication with display */
	tm1638_configuration(1, 3);
	tm1638_clear();
}

void _controller_configuration_init(controller_t *self)
{
	/* Try read data from flash, if flash error set default */
	if(flash_read_data(self->controller_devices->flash, self->configuration, MENU_N_OPTION) != FLASH_OK)
	{
		memcpy(self->configuration, menu_config_get_default(), sizeof(uint8_t)*MENU_N_OPTION);
		return; //Return with default value
	}

	/* Check configuration setting is in correct boundary if not set default (extra safety if flash read not correct)*/
	for(uint8_t i = 0; i < MENU_N_OPTION; ++i)
	{
		if(self->configuration[i] < menu_config_get_min(i) )
			self->configuration[i] = menu_config_get_default_opt(i);

		if(self->configuration[i] > menu_config_get_max(i) )
			self->configuration[i] = menu_config_get_default_opt(i);
	}


}
