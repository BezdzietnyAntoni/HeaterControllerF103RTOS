/*
 * This .h / .c files implements service for n ports relay
 *
 *	GPIO pins used for relay ports
 *	should be set in open drain mode
 *
 * @author : Antoni Bezdzietny
 * @create : 17.01.2024
 */

#ifndef INC_RELAY_H_
#define INC_RELAY_H_

#include "utilities.h"

typedef struct
{
	uint8_t n_ports;
	gpio_t *ports_gpio;
}relay_config_t;

typedef struct
{
	relay_config_t *config;
	uint8_t *ports_state;
}relay_t;

typedef enum
{
	RELAY_OK 			= 0,
	RELAY_PORT_ERR 		= 1,
	RELAY_STATUS_ERR 	= 2,
}reley_err_t;

/*
 * Function initialize relay by passing configuration
 * @param **self 		: Pointer to self relay object
 * @param *relay_config : Pointer to relay configuration
 */
void relay_init(relay_t **self, const relay_config_t *relay_config);

/*
 * Function deinitialize relay object (free allocate memory)
 * @param *self : Pointer to self relay object
 */
void relay_deinit(relay_t *self);

/*
 * Function change relay port status
 * @param *self  : Pointer to self relay object
 * @param port   : n-th port
 * @param status : set concrete status 0/1
 */
reley_err_t relay_set_status(relay_t *self, uint8_t port, uint8_t status);

/*
 * Function return relay port status
 * @param *self  : Pointer to self relay object
 * @param port   : n-th port
 */
uint8_t relay_get_status(relay_t *self, uint8_t port);

#endif /* INC_RELAY_H_ */
