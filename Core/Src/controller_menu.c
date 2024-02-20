#include "controller_menu.h"
#include "cmsis_os.h"
#include <stdlib.h>

static void _controller_menu_display(controller_menu_t *self);
static void _controller_menu_on_encoder_move(controller_menu_t *self);


void controller_menu_init(controller_menu_t **self,
						  controller_devices_t *controller_devices,
						  menu_config_t *configuration)
{
	(*self) = calloc(1, sizeof(controller_menu_t));
	(*self)->devices = controller_devices;
	(*self)->configuration = configuration;
}

void controller_menu_deinit(controller_menu_t *self)
{
	free(self);
}

void controller_menu_run(controller_menu_t *self)
{
	osDelay(200);

	/* Check encoder if is changed*/
	if(encoder_get_move_direction())
		_controller_menu_on_encoder_move(self);
}


void controller_menu_set(controller_menu_t *self, menu_option_t menu_option)
{
	self->menu_option = menu_option;
	tm1638_display_clear();
	_controller_menu_display(self);
}


static void _controller_menu_display(controller_menu_t *self)
{
	tm1638_display_string(0, menu_config_get_menu_char(self->menu_option), 3);
}

static void _controller_menu_on_encoder_move(controller_menu_t *self)
{
	int8_t encoder_move = encoder_get_move_direction();
	self->menu_option = max(0, min(MENU_N_OPTION-1, self->menu_option + encoder_move));
	_controller_menu_display(self);
}

void controller_menu_set_is_new_configuration(controller_menu_t *self, bool state)
{
	self->is_new_configuration = state;
}

bool controller_menu_get_is_new_configuration(controller_menu_t *self)
{
	return self->is_new_configuration;
}
