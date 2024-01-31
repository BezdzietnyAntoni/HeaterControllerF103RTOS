/*
 * one_wire.c
 *
 *  Created on: Jan 10, 2024
 *      Author: muntek
 */

#include "one_wire.h"

#include "utilities.h"
#include "stm32f1xx.h"
#include "main.h"


static void _one_wire_write_bit(int value)
{
  if (value)
  {
	// Write 1 value
    HAL_GPIO_WritePin(OW_GPIO_Port, OW_Pin, GPIO_PIN_RESET);
    delay_us(6);
    HAL_GPIO_WritePin(OW_GPIO_Port, OW_Pin, GPIO_PIN_SET);
    delay_us(64);
  } else
  {
	// Write 0 value
    HAL_GPIO_WritePin(OW_GPIO_Port, OW_Pin, GPIO_PIN_RESET);
    delay_us(60);
    HAL_GPIO_WritePin(OW_GPIO_Port, OW_Pin, GPIO_PIN_SET);
    delay_us(10);
  }
}

static int _one_wire_read_bit(void)
{
  int read_value;
  HAL_GPIO_WritePin(OW_GPIO_Port, OW_Pin, GPIO_PIN_RESET);
  delay_us(6);
  HAL_GPIO_WritePin(OW_GPIO_Port, OW_Pin, GPIO_PIN_SET);
  delay_us(9);
  read_value = HAL_GPIO_ReadPin(OW_GPIO_Port, OW_Pin);
  delay_us(55);
  return read_value;
}


one_wire_status_t one_wire_reset(void)
{
  int flag;

  //Send reset sequence
  HAL_GPIO_WritePin(OW_GPIO_Port, OW_Pin, GPIO_PIN_RESET);
  delay_us(480);
  HAL_GPIO_WritePin(OW_GPIO_Port, OW_Pin, GPIO_PIN_SET);
  delay_us(70);

  // Read status flag
  flag = HAL_GPIO_ReadPin(OW_GPIO_Port, OW_Pin);
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

uint8_t byte_crc(uint8_t crc, uint8_t byte)
{
  int i;
  for (i = 0; i < 8; i++) {
    uint8_t b = crc ^ byte;
    crc >>= 1;
    if (b & 0x01)
      crc ^= 0x8c;
    byte >>= 1;
  }
  return crc;
}

uint8_t one_wire_crc(const uint8_t* data, uint8_t len)
{
  int i;
    uint8_t crc = 0;

    for (i = 0; i < len; i++)
      crc = byte_crc(crc, data[i]);

    return crc;
}


