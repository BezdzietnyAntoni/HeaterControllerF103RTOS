/*
 * controller_device.h
 *
 *  Created on: Feb 8, 2024
 *      Author: muntek
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
