/*
 * This .h / .c files implements input controller.
 *
 * This controller is depends if controller is in input mode.
 *
 * @author : Antoni Bezdzietny
 * @create : 08.02.2024
 */


#ifndef INC_CONTROLLER_INPUT_H_
#define INC_CONTROLLER_INPUT_H_

#include "menu_config.h"
#include "controller_devices.h"
#include <stdbool.h>


typedef struct
{
	menu_config_t *configuration;
	controller_devices_t *devices;
	float input_value;
	menu_option_t menu_option;
}controller_input_t;

void controller_input_init(controller_input_t **self,
						   controller_devices_t *controller_devices,
						   menu_config_t *configuration);

void controller_input_deinit(controller_input_t *self);

void controller_input_run(controller_input_t *self);

/*
 * Function set menu state
 *
 * @param *self : Self pointer
 * @param menu_option : menu_option_t menu option to set
 */
void controller_input_set(controller_input_t *self, menu_option_t menu_option);

/*
 * Function check if setting value is different than saving in configuration
 *
 * @param *self : Self pointer
 */
bool controller_input_update_input(controller_input_t *self);

/*
 * Function return setting input value
 *
 * @param *self : Self pointer
 */
float controller_input_get_input(controller_input_t *self);


#endif /* INC_CONTROLLER_INPUT_H_ */
