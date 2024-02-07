#include "ds18b20.h"
#include "one_wire.h"


/* --- Private variable definition --- */

#define DS18B20_ROM_CODE_SIZE	   8
#define DS18B20_SCRATCHPAD_SIZE    9

#define DS18B20_READ_ROM           0x33
#define DS18B20_MATCH_ROM          0x55
#define DS18B20_SKIP_ROM           0xCC

#define DS18B20_CONVERT_T          0x44
#define DS18B20_READ_SCRATCHPAD    0xBE
#define DS18B20_WRITE_SCRATCHPAD   0x4E


/* --- Private function declaration --- */

static ds18b20_status_t _ds18b20_send_cmd(const uint8_t* rom_code, uint8_t cmd);
static ds18b20_status_t _ds18b20_read_scratchpad(const uint8_t* rom_code, uint8_t* scratchpad);



/* --- Public function definition --- */

ds18b20_status_t ds18b20_read_address(uint8_t* rom_code)
{
  if (one_wire_write_reset() != ONE_WIRE_OK)
    return DS18B20_ERROR;

  one_wire_write_byte(DS18B20_READ_ROM);

  for (uint8_t i = 0; i < DS18B20_ROM_CODE_SIZE; i++)
    rom_code[i] = one_wire_read_byte();

  uint8_t crc = one_wire_crc(rom_code, DS18B20_ROM_CODE_SIZE - 1);

  if (rom_code[DS18B20_ROM_CODE_SIZE - 1] == crc)
    return DS18B20_OK;
  else
    return DS18B20_CRC_ERROR;
}


ds18b20_status_t ds18b20_set_resolution(const uint8_t* rom_code, ds18b20_resolution_t resolution)
{
	if (_ds18b20_send_cmd(rom_code, DS18B20_WRITE_SCRATCHPAD) != DS18B20_OK)
	    return DS18B20_ERROR;

	one_wire_write_byte(0x00); // Th register - not used
	one_wire_write_byte(0x00); // Tl register - not used
	one_wire_write_byte(resolution);

	return DS18B20_OK;
}


ds18b20_status_t ds18b20_start_measure(const uint8_t* rom_code)
{
  return _ds18b20_send_cmd(rom_code, DS18B20_CONVERT_T);
}


ds18b20_status_t ds18b20_start_measure_all( void )
{
	return _ds18b20_send_cmd(0, DS18B20_CONVERT_T);
}


ds18b20_status_t ds18b20_read_temperature(const uint8_t* rom_code, float *temp)
{
	// Try read scratchpad
	uint8_t scratchpad[DS18B20_SCRATCHPAD_SIZE];
	ds18b20_status_t status = _ds18b20_read_scratchpad(rom_code, scratchpad);
	if( status != DS18B20_OK )
		return status;

	// If scratchpad read correct -> Convert to temperature
	int16_t temp_raw = (scratchpad[1] << 8) | scratchpad[0];
	*temp = temp_raw / 16.f;
	return DS18B20_OK;
}


void ds18b20_device_read_temperature(ds18b20_device_t* device)
{

	ds18b20_status_t status = ds18b20_read_temperature(device->rom_code, &device->temperature);
	device->status = status;

}



/* --- Private function definition --- */

static ds18b20_status_t _ds18b20_send_cmd(const uint8_t* rom_code, uint8_t cmd)
{
	// Beginning communication from one wire reset
	if( one_wire_write_reset() != ONE_WIRE_OK )
		return DS18B20_ERROR;

	if( rom_code )
	{
		// If ROM_CODE is set send command to selected device
		one_wire_write_byte(DS18B20_MATCH_ROM);
		for(uint8_t i = 0; i < DS18B20_ROM_CODE_SIZE; ++i) //
			one_wire_write_byte(rom_code[i]);
	}else
	{
		// If ROM_CODE is not set send to all device
		one_wire_write_byte(DS18B20_SKIP_ROM);
	}

	// Write command by one wire
	one_wire_write_byte(cmd);
	return DS18B20_OK;
}


static ds18b20_status_t _ds18b20_read_scratchpad(const uint8_t* rom_code, uint8_t* scratchpad)
{
	// Read scratchpad only concrete device
	if( rom_code == 0 )
		return DS18B20_ROM_CODE;

	// Send command `read scratchpad` to concrete device
	if (_ds18b20_send_cmd(rom_code, DS18B20_READ_SCRATCHPAD) != DS18B20_OK)
		return DS18B20_ERROR;

	// Pick up scratchpad
	for (uint8_t i = 0; i < DS18B20_SCRATCHPAD_SIZE; i++)
		scratchpad[i] = one_wire_read_byte();

	// Check CRC value
	uint8_t crc = one_wire_crc(scratchpad, DS18B20_SCRATCHPAD_SIZE - 1);
	if (scratchpad[DS18B20_SCRATCHPAD_SIZE - 1] == crc)
		return DS18B20_OK;

	return DS18B20_CRC_ERROR;
}

