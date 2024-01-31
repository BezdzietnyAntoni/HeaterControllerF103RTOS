/*
 *	N_ports relay controller
 *
 *	GPIO pins used for relay ports
 *	should be set in open drain mode
 *
 *  Created on: 17.01.2024
 *      Author: Antoni Bezdzietny
 */

#ifndef INC_RELAY_H_
#define INC_RELAY_H_

#include "utilities.h"

typedef struct
{
	uint8_t n_ports;
	uint8_t *ports_state;
	gpio_t *ports_gpio;
}relay_t;

typedef enum
{
	RELAY_OK 			= 0,
	RELAY_PORT_ERR 		= 1,
	RELAY_STATUS_ERR 	= 2,
}reley_err_t;

void relay_init(relay_t **self, uint8_t n_ports, gpio_t *gpio);

void relay_deinit(relay_t *self);

reley_err_t relay_set_status(relay_t *self, uint8_t port, uint8_t status);

uint8_t relay_get_status(relay_t *self, uint8_t port);

#endif /* INC_RELAY_H_ */
