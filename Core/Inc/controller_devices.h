/*
 * This .h / .c files implements devices using by controllers.
 *
 * This structures and files configuration and store fields for
 * devices communication.
 *
 * In .c file are stored all devices configuration.
 *
 * @author : Antoni Bezdzietny
 * @create : 08.02.2024
 */


#ifndef INC_CONTROLLER_DEVICES_H_
#define INC_CONTROLLER_DEVICES_H_

#include "ds18b20.h"
#include "flash.h"
#include "relay.h"

#include "one_wire.h"
#include "encoder.h"
#include "tm1638.h"

typedef struct
{
	ds18b20_device_t *ds18b_20_pipe;
	ds18b20_device_t *ds18b_20_boiler;
	relay_t *relay;
	flash_t *flash;
}controller_devices_t;

void controller_devices_init(controller_devices_t **self);

void controller_devices_deinit(controller_devices_t *self);

#endif /* INC_CONTROLLER_DEVICES_H_ */
