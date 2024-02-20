/*
 * This .h / .c files implements service for thermometers DS18B20
 *
 * Before use initialize one wire protocol.
 *
 * @author : Antoni Bezdzietny
 * @create : 11.01.2024
 */

#ifndef INC_DS18B20_H_
#define INC_DS18B20_H_

#include "stdint.h"


typedef enum
{
	DS18B20_OK        = 0x00U,
	DS18B20_ERROR     = 0x01U,
	DS18B20_CRC_ERROR = 0x02U,
	DS18B20_ROM_CODE  = 0x03U,
	DS18B20_INIT 	  = 0x04U,
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

/* --- Public function declaration --- */

/*
 * Function read thermometer ROM CODE working only when one device connected to one wire GPIO
 * @param *rom_code : Array for store ROM CODE
 */
ds18b20_status_t ds18b20_read_address(uint8_t* rom_code);

/*
 * Function enables set thermometer resolution
 * If resolution is growing -> maximum read frequency is decreasing
 * @param *rom_code  : Device ROM_CODE
 * @param resolution : Resolution bits
 */
ds18b20_status_t ds18b20_set_resolution(const uint8_t* rom_code, ds18b20_resolution_t resolution);

/*
 * Function beginning measure all connected device
 */
ds18b20_status_t ds18b20_start_measure_all( void );

/*
 * Function beginning measure concrete device
 * @param *rom_code : Device ROM_CODE
 */
ds18b20_status_t ds18b20_start_measure(const uint8_t* rom_code);

/*
 * Function read temperature from concrete device
 * @param *rom_code : Device ROM_CODE
 * @param *temp		: Pointer for storing reading temperature
 */
ds18b20_status_t ds18b20_read_temperature(const uint8_t* rom_code, float *temp);

/*
 * Function read temperature from concrete device passing as parameters
 * @param *device : DS18B20_device (data returning in device structure)
 */
void ds18b20_device_read_temperature(ds18b20_device_t* device);

/*
 * Function allocate memory for ds18b20_device_t and set default value
 * @param **device : Pointer to pointer where device will be stored
 */
void ds18b20_device_init(ds18b20_device_t** device, uint8_t* rom_code);

/*
 * Function free ds18b20_device_t memory
 * @param *device : Pointer to devices
 */
void ds18b20_device_deinit(ds18b20_device_t* device);



#endif /* INC_DS18B20_H_ */
