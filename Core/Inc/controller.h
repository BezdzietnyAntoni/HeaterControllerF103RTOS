/*
 * Main project controller
 *
 * @author : Antoni Bezdzietny
 * @create : 08.02.2024
 */

#ifndef INC_CONTROLLER_H_
#define INC_CONTROLLER_H_

#include "menu_config.h"
#include "controller_devices.h"
#include "controller_normal.h"
#include "controller_menu.h"
#include "controller_input.h"


typedef enum
{
	CONTROLLER_MODE_NORMAL,
	CONTROLLER_MODE_MENU,
	CONTROLLER_MODE_INPUT,
}controller_mode_t;


typedef struct
{
	controller_mode_t controller_mode;
	controller_normal_t *controller_normal;
	controller_menu_t *controller_menu;
	controller_input_t *controller_input;

	uint32_t last_action_tick;
	menu_config_t configuration;
	controller_devices_t *controller_devices;
}controller_t;

void controller_init(controller_t **self);

void controller_deinit(controller_t *self);

void controller_run(controller_t *self);

#endif /* INC_CONTROLLER_H_ */
