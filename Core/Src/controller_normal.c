#include "controller_normal.h"
#include "utilities.h"
#include <stdlib.h>
#include <cmsis_os.h>
#include <stdbool.h>

static void _controller_normal_measure_temperature(controller_normal_t *self);
static bool _controller_normal_check_measure_correct(controller_normal_t *self);
static void _controller_normal_display(controller_normal_t *self);
static void _controller_normal_check_relay(controller_normal_t *self);
static void _controller_normal_check_boiler_relay(controller_normal_t *self);
static void _controller_normal_check_instant_relay(controller_normal_t *self);
static void _controller_normal_set_boiler_relay(controller_normal_t *self, bool state);
static void _controller_normal_set_instant_relay(controller_normal_t *self, bool state);
static void _controller_normal_set_boiler_led(controller_normal_t *self, bool state);
static void _controller_normal_set_instant_led(controller_normal_t *self, bool state);


#define TEMPERATURE_MEASURE_PERIOD 		1000
#define MAX_N_ERROR						60
#define BOILER_RELAY					0
#define INSTANT_RELAY					1
#define BOILER_LED						0
#define INSTANT_LED						7



void controller_normal_init(controller_normal_t **self,
							controller_devices_t *devices,
							menu_config_t *configuration)
{
	(*self) = calloc(1, sizeof(controller_normal_t));
	(*self)->devices = devices;  // Used controll_device_t structure from controller
	(*self)->configuration = configuration;

	(*self)->is_boiler_heating = false;
	(*self)->is_instant_heating = false;
	(*self)->count_error_period = 0;
}

void controller_normal_deinit(controller_normal_t *self)
{
	// Don't free controller_devices_t here (main controller free device)
	free(self);
}

void controller_normal_run(controller_normal_t *self)
{
	_controller_normal_measure_temperature(self);
	osDelay(TEMPERATURE_MEASURE_PERIOD);


	_controller_normal_display(self);
	if(_controller_normal_check_measure_correct(self))
		_controller_normal_check_relay(self);
}


static void _controller_normal_measure_temperature(controller_normal_t *self)
{
	ds18b20_device_read_temperature(self->devices->ds18b_20_pipe);
	ds18b20_device_read_temperature(self->devices->ds18b_20_boiler);
	ds18b20_start_measure_all();
}


static bool _controller_normal_check_measure_correct(controller_normal_t *self)
{
	if(self->count_error_period > MAX_N_ERROR)
	{
		NVIC_SystemReset();
	}

	if(self->devices->ds18b_20_boiler->status != DS18B20_OK)
	{
		++self->count_error_period;
		return false;
	}

	if(self->devices->ds18b_20_pipe->status != DS18B20_OK)
	{
		++self->count_error_period;
		return false;
	}

	self->count_error_period = 0;

	return true;
}

static void _controller_normal_display(controller_normal_t *self)
{
	char formating_char_arr[6];

	/* Display boiler temperature */
	if(self->devices->ds18b_20_boiler->status != DS18B20_OK)
	{
		/* If boiler temperature error */
		tm1638_display_string(0, "Err ", 4); // Display on 4 first 7segment block "Err "
	}else
	{
		/* If boiler temperature correct */
		float_to_char(formating_char_arr, self->devices->ds18b_20_boiler->temperature);
		tm1638_display_string(0, formating_char_arr, 5);
	}

	/* Display pipe temperature */
	if(self->devices->ds18b_20_pipe->status != DS18B20_OK)
	{
		/* If pipe temperature error */
		tm1638_display_string(4, "Err ", 4); // Display on 4 last 7segment block "Err "
	}else
	{
		/* If pipe temperature correct */
		float_to_char(formating_char_arr, self->devices->ds18b_20_pipe->temperature);
		tm1638_display_string(4, formating_char_arr, 5);
	}
}

static void _controller_normal_check_relay(controller_normal_t *self)
{
	_controller_normal_check_boiler_relay(self);
	_controller_normal_check_instant_relay(self);
}

static void _controller_normal_check_boiler_relay(controller_normal_t *self)
{
	float pipe_temperature = self->devices->ds18b_20_pipe->temperature;
	float boiler_temperature = self->devices->ds18b_20_boiler->temperature;

	/* Schema preventing boiling water in pipe */
	if (pipe_temperature > (*self->configuration)[MENU_OPTION_RTU])
	{
		if(self->is_boiler_heating)
		{
			if(boiler_temperature > (*self->configuration)[MENU_OPTION_BTU])
			{
				_controller_normal_set_boiler_relay(self, false);
				return;
			}else
				return;
		}else
		{
			if((boiler_temperature + (*self->configuration)[MENU_OPTION_BTH]) <
					(*self->configuration)[MENU_OPTION_BTU])
			{
				_controller_normal_set_boiler_relay(self, true);
				return;
			}else
				return;
		}
	}

	/* Differential dependency schema */
	float differentail_temperature = pipe_temperature - boiler_temperature;

	if(self->is_boiler_heating)
	{
		/* If boiler temperature bigger than set */
		if( boiler_temperature > (*self->configuration)[MENU_OPTION_BTS] )
		{
			_controller_normal_set_boiler_relay(self, false);
			return;
		}

		/* If differential temperature smaller than set + hysteresis */
		if (differentail_temperature + (*self->configuration)[MENU_OPTION_BRH] <
				(*self->configuration)[MENU_OPTION_BRT])
		{
			_controller_normal_set_boiler_relay(self, false);
			return;
		}
	}else
	{
		/* If differential temperature is not bigger than set - return*/
		if ( differentail_temperature <= (*self->configuration)[MENU_OPTION_BRT])
			return;

		/* If boiler temperature + hysteresis is bigger than set - return*/
		if ( (boiler_temperature + (*self->configuration)[MENU_OPTION_BRH]) >=
				(*self->configuration)[MENU_OPTION_BTS])
			return;

		/* If two above condition not exist turn on boiler heating */
		_controller_normal_set_boiler_relay(self, true);
	}
}

static void _controller_normal_check_instant_relay(controller_normal_t *self)
{
	float boiler_temperature = self->devices->ds18b_20_boiler->temperature;

	if(self->is_instant_heating)
	{
		/* If current state is true (instant heating ON) */
		if(boiler_temperature >  (*self->configuration)[MENU_OPTION_PTS])
			_controller_normal_set_instant_relay(self, false);
	}else
	{
		/* If current state is false (instant heating OFF) */
		if((boiler_temperature + (*self->configuration)[MENU_OPTION_PTH]) <
				(*self->configuration)[MENU_OPTION_PTS])
			_controller_normal_set_instant_relay(self, true);
	}
}

static void _controller_normal_set_boiler_relay(controller_normal_t *self, bool state)
{
	if( self->is_boiler_heating == state )
		return;

	self->is_boiler_heating = state;
	relay_set_status(self->devices->relay, BOILER_RELAY, state);
	_controller_normal_set_boiler_led(self, state);
}

static void _controller_normal_set_instant_relay(controller_normal_t *self, bool state)
{
	if( self->is_instant_heating == state )
		return;

	self->is_instant_heating = state;
	relay_set_status(self->devices->relay, INSTANT_RELAY, state);
	_controller_normal_set_instant_led(self, state);
}

static void _controller_normal_set_boiler_led(controller_normal_t *self, bool state)
{
	tm1638_led(BOILER_LED, state);
}

static void _controller_normal_set_instant_led(controller_normal_t *self, bool state)
{
	tm1638_led(INSTANT_LED, state);
}

