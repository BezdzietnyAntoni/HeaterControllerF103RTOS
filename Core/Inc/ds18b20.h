/*
 * ds18b20.h
 *
 *  Created on: Jan 10, 2024
 *      Author: muntek
 */

#ifndef INC_DS18B20_H_
#define INC_DS18B20_H_

#include <stdint.h>

typedef enum
{
	DS18B20_OK        = 0x00U,
	DS18B20_ERROR     = 0x01U,
	DS18B20_CRC_ERROR = 0x02U,
	DS18B20_INIT 	  = 0x03U,
}ds18b20_status_t;

typedef enum
{
	DS18B20_9B 	= 0x1F,	// MAX CONVERSION TIME ~94ms
	DS18B20_10B = 0x3F, // MAX CONVERSION TIME ~188ms
	DS18B20_11B	= 0x5F, // MAX CONVERSION TIME ~375ms
	DS18B20_12B = 0x7F, // MAX CONVERSION TIME ~750ms
}ds18b20_resolution_t;

typedef struct
{
	uint8_t rom_code[8];
	float temperature;
	ds18b20_status_t status;
}ds18b20_device_t;

/*
 *  Read DS18B20 thermometer address
 *  @param rom_code : pointers to variable where rom address will be stored
 */
ds18b20_status_t ds18b20_read_address(uint8_t* rom_code);

ds18b20_status_t ds18b20_start_measure(const uint8_t* rom_code);

ds18b20_status_t ds18b20_set_resolution(const uint8_t* rom_code, ds18b20_resolution_t resolution);

int16_t ds18b20_get_temp_int(const uint8_t* rom_code);

float ds18b20_get_temp_float(const uint8_t* rom_code);



#endif /* INC_DS18B20_H_ */
