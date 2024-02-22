/*
 * This .h / .c files implements menu controller.
 *
 * This controller is depends if controller is in menu mode.
 *
 * @author : Antoni Bezdzietny
 * @create : 08.02.2024
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

/*
 * Function set menu state
 *
 * @param *self : Self pointer
 * @param menu_option : menu_option_t menu option to set
 */
void controller_menu_set(controller_menu_t *self, menu_option_t menu_option);

/*
 * Function set flag new_configuration state
 *
 * @param *self : Self pointer
 * @param state : State to set
 */
void controller_menu_set_is_new_configuration(controller_menu_t *self, bool state);

/*
 * Function return new_configuration flag
 *
 * @param *self : Self pointer
 */
bool controller_menu_get_is_new_configuration(controller_menu_t *self);


#endif /* INC_CONTROLLER_MENU_H_ */
