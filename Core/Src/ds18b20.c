/*
 * ds18b20.c
 *
 *  Created on: Jan 10, 2024
 *      Author: Antoni Bezdzietny
 */


#include "ds18b20.h"
#include "one_wire.h"
#include <string.h>


/*
 * Private variable definition
 */
#define DS18B20_ROM_CODE_SIZE	   8
#define DS18B20_SCRATCHPAD_SIZE    9

#define DS18B20_READ_ROM           0x33
#define DS18B20_MATCH_ROM          0x55
#define DS18B20_SKIP_ROM           0xCC

#define DS18B20_CONVERT_T          0x44
#define DS18B20_READ_SCRATCHPAD    0xBE
#define DS18B20_WRITE_SCRATCHPAD   0x4E


/*
 * Private function definition
 */
static ds18b20_status_t ds18b20_send_cmd(const uint8_t* rom_code, uint8_t cmd)
{

  if (one_wire_write_reset() != ONE_WIRE_OK)
    return DS18B20_ERROR;

  if (!rom_code) {
    one_wire_write_byte(DS18B20_SKIP_ROM);
  } else {
	  one_wire_write_byte(DS18B20_MATCH_ROM);
    for (uint8_t i = 0; i < DS18B20_ROM_CODE_SIZE; i++)
    	one_wire_write_byte(rom_code[i]);
  }
  one_wire_write_byte(cmd);
  return DS18B20_OK;
}

static ds18b20_status_t ds18b20_read_scratchpad(const uint8_t* rom_code, uint8_t* scratchpad)
{
  uint8_t crc;

  if (ds18b20_send_cmd(rom_code, DS18B20_READ_SCRATCHPAD) != DS18B20_OK)
    return DS18B20_ERROR;

  for (uint8_t i = 0; i < DS18B20_SCRATCHPAD_SIZE; i++)
    scratchpad[i] = one_wire_read_byte();

  crc = one_wire_crc(scratchpad, DS18B20_SCRATCHPAD_SIZE - 1);
  if (scratchpad[DS18B20_SCRATCHPAD_SIZE - 1] == crc)
	    return DS18B20_OK;
	  else
	    return DS18B20_CRC_ERROR;
}



/*
 * Public function definition
 */


ds18b20_status_t ds18b20_read_address(uint8_t* rom_code)
{
  uint8_t crc;

  if (one_wire_write_reset() != ONE_WIRE_OK)
    return DS18B20_ERROR;

  one_wire_write_byte(DS18B20_READ_ROM);

  for (uint8_t i = 0; i < DS18B20_ROM_CODE_SIZE; i++)
    rom_code[i] = one_wire_read_byte();

  crc = one_wire_crc(rom_code, DS18B20_ROM_CODE_SIZE - 1);

  if (rom_code[DS18B20_ROM_CODE_SIZE - 1] == crc)
    return DS18B20_OK;
  else
    return DS18B20_CRC_ERROR;
}


ds18b20_status_t ds18b20_start_measure(const uint8_t* rom_code)
{
  return ds18b20_send_cmd(rom_code, DS18B20_CONVERT_T);
}


int16_t ds18b20_get_temp_int(const uint8_t* rom_code)
{
  uint8_t scratchpad[DS18B20_SCRATCHPAD_SIZE];

  if (ds18b20_read_scratchpad(rom_code, scratchpad) != DS18B20_OK)
    return -85.0f;

  int16_t temp_raw = (scratchpad[1] << 8) | scratchpad[0];

  return temp_raw;
}

float ds18b20_get_temp_float(const uint8_t* rom_code)
{
  uint8_t scratchpad[DS18B20_SCRATCHPAD_SIZE];

  if (ds18b20_read_scratchpad(rom_code, scratchpad) != DS18B20_OK)
    return -85.0f;

  int16_t temp_raw = (scratchpad[1] << 8) | scratchpad[0];

  return temp_raw / 16.f;
}

void ds18b20_get_temp(ds18b20_device_t *device)
{
	uint8_t scratchpad[DS18B20_SCRATCHPAD_SIZE];

	if (ds18b20_read_scratchpad(device->rom_code, scratchpad) != DS18B20_OK)
	{
		device->status = DS18B20_ERROR;
		device->temperature = -85.f;
		return;
	}

	device->status = DS18B20_OK;
	device->temperature = ((scratchpad[1] << 8) | scratchpad[0]) / 16.f;
}


ds18b20_status_t ds18b20_set_resolution(const uint8_t* rom_code, ds18b20_resolution_t resolution)
{
	if (ds18b20_send_cmd(rom_code, DS18B20_WRITE_SCRATCHPAD) != DS18B20_OK)
	    return DS18B20_ERROR;

	one_wire_write_byte(0x00); // Th register - not used
	one_wire_write_byte(0x00); // Tl register - not used
	one_wire_write_byte(resolution);

	return DS18B20_OK;
}

