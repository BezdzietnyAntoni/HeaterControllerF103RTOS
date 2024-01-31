/*
 * configuration.h
 *
 *  Created on: Jan 31, 2024
 *      Author: Antoni Bezdzietny
 */

#ifndef INC_CONFIGURATION_H_
#define INC_CONFIGURATION_H_

/* Includes  */
#include <stdint.h>
#include "utilities.h"



/* Relay configuration */
/*
 *	Relay pins should be set as output push pull without pull-up and pull-down
 */
const uint8_t CONFIG_RELAY_PORTS = 2;
const gpio_t CONFIG_RELAY_GPIO[2] = {{RELAY_IN0_GPIO_Port, RELAY_IN0_Pin},
		  	  	  	  				 {RELAY_IN1_GPIO_Port, RELAY_IN1_Pin}};


/* Thermometers DS18B20 ROM configuration */
const uint8_t CONFIG_DS18B20_BOILER_ROM_CODE [8] = { 0x28, 0xB5, 0xCA, 0x95, 0xF0, 0x01, 0x3C, 0x59 };

const uint8_t CONFIG_DS18B20_PIPE_ROM_CODE   [8] = { 0x28, 0xD7, 0x21, 0x95, 0xF0, 0x01, 0x3C, 0x5E };


#endif /* INC_CONFIGURATION_H_ */
