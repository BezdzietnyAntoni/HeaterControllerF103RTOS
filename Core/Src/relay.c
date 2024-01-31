#include <stdlib.h>
#include <string.h>
#include "relay.h"


void relay_init(relay_t **self, const relay_config_t *relay_config)
{
	(*self) = calloc(1, sizeof(relay_t));

	// Copy relay configuration
	(*self)->config = calloc(1, sizeof(relay_config_t));
	(*self)->config->n_ports = relay_config->n_ports;

	(*self)->config->ports_gpio = calloc((*self)->config->n_ports, sizeof(gpio_t));
	memcpy((*self)->config->ports_gpio, relay_config->ports_gpio, sizeof(gpio_t)*(*self)->config->n_ports);

	// Allocate and set default state as 0
	(*self)->ports_state = calloc((*self)->config->n_ports, sizeof(uint8_t));
}


void relay_deinit(relay_t *self)
{
	free(self->config->ports_gpio);
	free(self->ports_state);
	free(self->config);
	free(self);
}


reley_err_t relay_set_status(relay_t *self, uint8_t port, uint8_t state)
{
	if(port >= self->config->n_ports)
		return RELAY_PORT_ERR;

	if(!(state == 0 || state == 1))
		return RELAY_STATUS_ERR;

	self->ports_state[port] = state;
	HAL_GPIO_WritePin(self->config->ports_gpio[port].GPIOx,
			self->config->ports_gpio[port].GPIO_Pin, state);

	return RELAY_OK;
}


uint8_t relay_get_status(relay_t *self, uint8_t port)
{
	if(port >= self->config->n_ports)
		return RELAY_PORT_ERR;

	return self->ports_state[port];
}


