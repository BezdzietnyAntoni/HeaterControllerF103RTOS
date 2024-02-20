#include "controller_input.h"
#include <stdlib.h>
#include "cmsis_os.h"
#include <stdbool.h>


static void _controller_input_display(controller_input_t *self);
static void _controller_input_on_encoder_move(controller_input_t *self);

void controller_input_init(controller_input_t **self,
						   controller_devices_t *controller_devices,
						   menu_config_t *configuration)
{
	(*self) = calloc(1, sizeof(controller_input_t));
	(*self)->devices = controller_devices;
	(*self)->configuration = configuration;
}

void controller_input_deinit(controller_input_t *self)
{
	free(self);
}

void controller_input_run(controller_input_t *self)
{
	osDelay(200);

	/* Check encoder if is changed*/
	if(encoder_get_move_direction())
		_controller_input_on_encoder_move(self);
}

void controller_input_set(controller_input_t *self, menu_option_t menu_option)
{
	self->menu_option = menu_option;
	self->input_value = (*self->configuration)[menu_option];
	_controller_input_display(self);
}

bool controller_input_update_input(controller_input_t *self)
{
	if(self->input_value == (*self->configuration)[self->menu_option])
		return false;

	(*self->configuration)[self->menu_option] = self->input_value;
	return true;
}


float controller_input_get_input(controller_input_t *self)
{
	return self->input_value;
}

static void _controller_input_display(controller_input_t *self)
{
	char buff[6];
	float_to_char(buff, self->input_value);
	tm1638_display_string(4, buff, 5);
}

static void _controller_input_on_encoder_move(controller_input_t *self)
{
	int8_t encoder_move = encoder_get_move_direction();
	self->input_value = max(menu_config_get_min(self->menu_option),
								 min(menu_config_get_max(self->menu_option), self->input_value + encoder_move));
	_controller_input_display(self);
}
