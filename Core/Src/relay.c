#include <stdlib.h>
#include <string.h>
#include "relay.h"


void relay_init(relay_t **self, uint8_t n_ports, gpio_t *gpio)
{
	(*self) = calloc(1, sizeof(relay_t));
	(*self)->n_ports = n_ports;

	// Allocate and set default state as 0
	(*self)->ports_state = calloc(n_ports, sizeof(uint8_t));

	// Copy GPIO configuration
	(*self)->ports_gpio = calloc(n_ports, sizeof(gpio_t));
	memcpy((*self)->ports_gpio, gpio, sizeof(gpio_t)*n_ports);
}


void relay_deinit(relay_t *self)
{
	free(self->ports_state);
	free(self->ports_gpio);
	free(self);
}


reley_err_t relay_set_status(relay_t *self, uint8_t port, uint8_t state)
{
	if(port >= self->n_ports)
		return RELAY_PORT_ERR;

	if(!(state == 0 || state == 1))
		return RELAY_STATUS_ERR;

	self->ports_state[port] = state;
	HAL_GPIO_WritePin(self->ports_gpio[port].GPIOx,
			self->ports_gpio[port].GPIO_Pin, state);

	return RELAY_OK;
}


uint8_t relay_get_status(relay_t *self, uint8_t port)
{
	if(port >= self->n_ports)
		return RELAY_PORT_ERR;

	return self->ports_state[port];
}


