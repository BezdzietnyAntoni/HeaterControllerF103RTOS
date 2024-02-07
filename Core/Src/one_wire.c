#include "one_wire.h"

#include "stdint.h"
#include "stm32f1xx.h"

/* --- Private one wire variable --- */
static gpio_t one_wire_gpio;

/* ---  Private one wire function  --- */

static void _one_wire_write_bit(uint8_t value)
{
  if (value)
  {
	// Write 1 value
    HAL_GPIO_WritePin(one_wire_gpio.GPIOx, one_wire_gpio.GPIO_Pin, GPIO_PIN_RESET);
    delay_us(6);
    HAL_GPIO_WritePin(one_wire_gpio.GPIOx, one_wire_gpio.GPIO_Pin, GPIO_PIN_SET);
    delay_us(64);
  } else
  {
	// Write 0 value
    HAL_GPIO_WritePin(one_wire_gpio.GPIOx, one_wire_gpio.GPIO_Pin, GPIO_PIN_RESET);
    delay_us(60);
    HAL_GPIO_WritePin(one_wire_gpio.GPIOx, one_wire_gpio.GPIO_Pin, GPIO_PIN_SET);
    delay_us(10);
  }
}


static int _one_wire_read_bit(void)
{
  int read_value;
  HAL_GPIO_WritePin(one_wire_gpio.GPIOx, one_wire_gpio.GPIO_Pin, GPIO_PIN_RESET);
  delay_us(6);
  HAL_GPIO_WritePin(one_wire_gpio.GPIOx, one_wire_gpio.GPIO_Pin, GPIO_PIN_SET);
  delay_us(9);
  read_value = HAL_GPIO_ReadPin(one_wire_gpio.GPIOx, one_wire_gpio.GPIO_Pin);
  delay_us(55);
  return read_value;
}


/* ---  Public one wire function  --- */

void one_wire_init(	GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	// Set one wire GPIO
	one_wire_gpio.GPIOx    = GPIOx;
	one_wire_gpio.GPIO_Pin = GPIO_Pin;
}


one_wire_status_t one_wire_write_reset(void)
{
  //Send reset sequence
  HAL_GPIO_WritePin(one_wire_gpio.GPIOx, one_wire_gpio.GPIO_Pin, GPIO_PIN_RESET);
  delay_us(480);
  HAL_GPIO_WritePin(one_wire_gpio.GPIOx, one_wire_gpio.GPIO_Pin, GPIO_PIN_SET);
  delay_us(70);

  // Read status flag
  int flag = HAL_GPIO_ReadPin(one_wire_gpio.GPIOx, one_wire_gpio.GPIO_Pin);
  delay_us(410);

  if (flag == 0)
    return ONE_WIRE_OK;
  else
    return ONE_WIRE_ERROR;
}


void one_wire_write_byte(uint8_t byte)
{
  for (uint8_t i = 0; i < 8; i++)
  {
	  _one_wire_write_bit(byte & 0x01);
    byte >>= 1;
  }
}


uint8_t one_wire_read_byte(void)
{
  uint8_t value = 0;

  for (uint8_t i = 0; i < 8; i++) {
	value >>= 1;
	if (_one_wire_read_bit())
		value |= 0x80;
  }

  return value;
}


uint8_t one_wire_crc(const uint8_t* data, uint8_t len)
{
	uint8_t crc = 0;

    for (uint8_t i = 0; i < len; ++i)
    {
    	uint8_t byte = data[i];
    	for(uint8_t j = 0; j < 8; ++j)
    	{
    		uint8_t b = crc ^ byte;
    		crc >>= 1;
    		if(b & 0x01)
    			crc ^= 0x8C;

    		byte >>= 1;
    	}
    }

    return crc;
}


