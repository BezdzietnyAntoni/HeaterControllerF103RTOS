#include <stdlib.h>
#include <string.h>
#include <main.h>
#include "runtime_controller.h"
#include "encoder.h"
#include "configuration.h"
#include "utilities.h"
#include "tm1638.h"
#include "ds18b20.h"
#include "cmsis_os.h"

void _runtime_controller_normal(runtime_controller_t *self);
void _runtime_controller_menu(runtime_controller_t *self);
void _runtime_controller_input(runtime_controller_t *self);

static uint8_t _runtime_is_out_of_operation_time(runtime_controller_t *self);
static void _runtime_load_configuration(runtime_controller_t *self);
static void _runtime_save_into_flash( runtime_controller_t *self );
static void _runtime_reset_encoder_settings( runtime_controller_t *self );
static void _runtime_change_is_boiler_heater( runtime_controller_t *self, uint8_t state );
static void _runtime_change_is_extra_heating( runtime_controller_t *self, uint8_t state );

static void _runtime_normal_measure_temperature( runtime_controller_t *self );
static void _runtime_normal_set_display( runtime_controller_t *self );
static void _runtime_normal_set_relay( runtime_controller_t *self );
static void _runtime_normal_on_click( runtime_controller_t *self );

static void _runtime_menu_on_click( runtime_controller_t *self );
static void _runtime_menu_on_move( runtime_controller_t *self );

static void _runtime_input_on_click( runtime_controller_t *self );
static void _runtime_input_on_move( runtime_controller_t *self );

static void _runtime_return_to_normal( runtime_controller_t *self );


const uint8_t _boiler_rom[] = { 0x28, 0xB5, 0xCA, 0x95, 0xF0, 0x01, 0x3C, 0x59 };
const uint8_t _pipe_rom[]   = { 0x28, 0xD7, 0x21, 0x95, 0xF0, 0x01, 0x3C, 0x5E };



/*
 * Definition runtime_controller functions
 */

void runtime_controller_init(runtime_controller_t **self)
{
	(*self) = calloc(1, sizeof(runtime_controller_t));

	/* Set default setting */
	(*self) -> mode = RUNTIME_NORMAL;
	(*self) -> operation_time = 10000; // Operation time in ms

	/* Initialization flash memory*/
	flash_init(&(*self)->_flash);

	_runtime_load_configuration( (*self) );

	/* Set resolution as 12bit in DS18B20*/
	ds18b20_set_resolution(_boiler_rom, DS18B20_11B);
	ds18b20_set_resolution(_pipe_rom,   DS18B20_11B);

	(*self) -> _is_boiler_heating = 0;
	(*self) -> _is_extra_heating = 0;
}


void runtime_controller_deinit(runtime_controller_t *self)
{
	flash_deinit(self->_flash);
	free(self);
}


void runtime_controller_run(runtime_controller_t *self)
{
	switch (self->mode) {
		case RUNTIME_NORMAL:
			_runtime_controller_normal(self);
			break;
		case RUNTIME_MENU:
			_runtime_controller_menu(self);
			break;
		case RUNTIME_INPUT:
			_runtime_controller_input(self);
			break;
	}
}


uint8_t _runtime_is_out_of_operation_time(runtime_controller_t *self)
{
	if (HAL_GetTick() - self->_last_action_tick > self->operation_time)
		return 1;
	else
		return 0;
}


static void _runtime_save_into_flash( runtime_controller_t *self )
{
	/* Write configuration array to flash memory */
	flash_write_data(self->_flash, self->_configuration, MENU_N_OPTION);
}


static void _runtime_load_configuration(runtime_controller_t *self)
{
	/* First read settings from flash memory*/
	flash_status_t status = flash_read_data(self->_flash, self->_configuration, MENU_N_OPTION);

	/* If some error in read from flash set default settings*/
	if(status != FLASH_OK)
	{
		memcpy(self->_configuration, config_get_default(), sizeof(uint8_t)*MENU_N_OPTION);
		return;
	}

	/* Check is configuration setting is in correct boundary if not set default */
	for(uint8_t i = 0; i < MENU_N_OPTION; ++i)
	{
		if( self->_configuration[i] < config_get_min( i ) )
			self->_configuration[i] = config_get_default_opt( i );
		if( self->_configuration[i] > config_get_max( i ) )
			self->_configuration[i] = config_get_default_opt( i );
	}
}


static void _runtime_return_to_normal( runtime_controller_t *self )
{
	if( self->is_new_config )
		_runtime_save_into_flash(self);

	self->is_new_config = 0;
	self->mode = RUNTIME_NORMAL;
}





void _runtime_controller_normal(runtime_controller_t *self)
{
	_runtime_normal_measure_temperature(self);

	osDelay(1000);

	_runtime_normal_set_display(self);
	_runtime_normal_set_relay(self);


	/* Check is button pressed -> change mode to menu */
	if(encoder_get_sw_flag())
		_runtime_normal_on_click(self);
}

void _runtime_controller_menu(runtime_controller_t *self)
{
	osDelay(200);
	/* Check encoder if is changed*/
	if(encoder_get_move_direction())
		_runtime_menu_on_move(self);

	/* Check is button pressed -> change mode to input */
	if(encoder_get_sw_flag())
		_runtime_menu_on_click(self);

	/* Return to normal mode when no action in operation time */
	if(_runtime_is_out_of_operation_time(self))
		_runtime_return_to_normal(self);
}

void _runtime_controller_input(runtime_controller_t *self)
{
	/* Check encoder if is changed*/
	if(encoder_get_move_direction())
		_runtime_input_on_move(self);

	/* Check is button pressed -> change mode to menu */
	if(encoder_get_sw_flag())
		_runtime_input_on_click(self);

	/* Return to normal mode when no action in operation time */
	if(_runtime_is_out_of_operation_time(self))
		_runtime_return_to_normal(self);
}


static void _runtime_reset_encoder_settings( runtime_controller_t *self )
{


	encoder_reset_sw_falg(); // Reset flag
	encoder_update_last_time_count(); // Reset encoder direction

	self->_last_action_tick = HAL_GetTick(); // Update last action time
}

static void _runtime_normal_measure_temperature( runtime_controller_t *self )
{
	self->_temp_boiler = ds18b20_get_temp_float(_boiler_rom);
	self->_temp_pipe   = ds18b20_get_temp_float(_pipe_rom);

	ds18b20_start_measure(_boiler_rom);
	ds18b20_start_measure(_pipe_rom);
}

static void _runtime_normal_set_display( runtime_controller_t *self )
{
	char str_from_f[6];

	//TODO Write a firewall from error ds18b20

	/* Check and display boiler temperature */
	if( self->_temp_boiler < -80.f )
		tm1638_set_display_string(0, "Err ", 4);
	else
	{
		float_to_char(str_from_f, self->_temp_boiler);
		tm1638_set_display_string(0, str_from_f, 5);
	}

	/* Check and display pipe temperature */
	if( self->_temp_pipe < -80.f )
		tm1638_set_display_string(4, "Err ", 4);
	else
	{
		float_to_char(str_from_f, self->_temp_pipe);
		tm1638_set_display_string(4, str_from_f, 5);
	}
}

static void _runtime_normal_set_relay( runtime_controller_t *self )
{
	// Extra water heater
	if(self->_is_extra_heating)
	{
		if(self->_temp_boiler > self->_configuration[MENU_OPTION_PTS])
			_runtime_change_is_extra_heating(self, 0);
	}else
	{
		if((self->_temp_boiler + self->_configuration[MENU_OPTION_PTH]) <
				self->_configuration[MENU_OPTION_PTS])
			_runtime_change_is_extra_heating(self, 1);
	}


	// Preventing boiling water in pipe
	if(self->_temp_pipe > self->_configuration[MENU_OPTION_RTU])
	{
		if(self->_is_boiler_heating)
		{
			if(self->_temp_boiler > self->_configuration[MENU_OPTION_BTU])
			{
				_runtime_change_is_boiler_heater(self, 0);
				return;
			}else
			{
				return;
			}
		}else
		{
			if((self->_temp_boiler + self->_configuration[MENU_OPTION_BTH]) <
					self->_configuration[MENU_OPTION_BTU])
			{
				_runtime_change_is_boiler_heater(self, 1);
				return;
			}else
			{
				return;
			}
		}
	}

	float diff = self->_temp_pipe - self->_temp_boiler;
	if(self->_is_boiler_heating)
	{
		if((self->_temp_boiler > self->_configuration[MENU_OPTION_BTS]) ||
				((diff + self->_configuration[MENU_OPTION_BRH])
						< self->_configuration[MENU_OPTION_BRT]) )
		{
			_runtime_change_is_boiler_heater(self, 0);
			return;
		}
	}else
	{
		if(self->_temp_boiler + self->_configuration[MENU_OPTION_BRH] <
				self->_configuration[MENU_OPTION_BTS] &&
				diff > self->_configuration[MENU_OPTION_BRT])
		{
			_runtime_change_is_boiler_heater(self, 1);
			return;
		}
	}
}

static void _runtime_change_is_boiler_heater( runtime_controller_t *self, uint8_t state )
{
	if(self->_is_boiler_heating == state)
		return;

	self->_is_boiler_heating = state;
	//TODO Change relay state
}

static void _runtime_change_is_extra_heating( runtime_controller_t *self, uint8_t state )
{
	if(self->_is_extra_heating == state)
		return;

	self->_is_extra_heating = state;
	//TODO Change relay state
}


static void _runtime_normal_on_click( runtime_controller_t *self )
{
	_runtime_reset_encoder_settings(self);
	self->mode = RUNTIME_MENU; // Change mode to menu
	self->_menu_option = MENU_OPTION_BRT; // First menu option (0)

	// Update display to menu mode
	tm1638_set_clear_all();
	tm1638_set_display_string(0, config_get_menu_char(self->_menu_option), 3);
}



static void _runtime_menu_on_click( runtime_controller_t *self)
{
	_runtime_reset_encoder_settings(self);
	self->mode = RUNTIME_INPUT; // Change mode to input

	// Update display to input
	char buff[6];
	tm1638_set_clear_all();
	self->_input_value = self->_configuration[self->_menu_option];
	float_to_char(buff, self->_input_value);
	tm1638_set_display_string(0, config_get_menu_char(self->_menu_option), 3);
	tm1638_set_display_string(4, buff, 5);
}

static void _runtime_menu_on_move( runtime_controller_t *self )
{
	int8_t enc_move = encoder_get_move_direction();
	encoder_update_last_time_count(); // Reset encoder direction
	self->_menu_option = max(0, min(MENU_N_OPTION-1, self->_menu_option + enc_move));
	self->_last_action_tick = HAL_GetTick(); // Update last action time
	// Update display()
	tm1638_set_clear_all();
	tm1638_set_display_string(0, config_get_menu_char(self->_menu_option), 3);
}

static void _runtime_input_on_click( runtime_controller_t *self )
{
	_runtime_reset_encoder_settings(self);

	self->mode = RUNTIME_MENU; // Change mode to menu

	tm1638_set_clear_all();
	tm1638_set_display_string(0, config_get_menu_char(self->_menu_option), 3);


	if(self->_input_value != self->_configuration[self->_menu_option])
	{
		self->is_new_config = 1;
		self->_configuration[self->_menu_option] = self->_input_value;
	}

}


static void _runtime_input_on_move( runtime_controller_t *self)
{
	int8_t enc_move = encoder_get_move_direction(); // Get direction

	encoder_update_last_time_count(); // Reset encoder direction
	self->_last_action_tick = HAL_GetTick(); // Update last action time

	self->_input_value = max(config_get_min(self->_menu_option),
								 min(config_get_max(self->_menu_option), self->_input_value + enc_move));

	// Update display()
	char buff[6];
	float_to_char(buff, self->_input_value);
	tm1638_set_display_string(4, buff, 5);
}


