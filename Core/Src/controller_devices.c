#include "controller_devices.h"

#include <stdlib.h>
#include "main.h"

static void _controller_devices_ds18b20_init(controller_devices_t *self);
static void _controller_devices_relay_init(controller_devices_t *self);
static void _controller_devices_flash_init(controller_devices_t *self);
static void _controller_devices_tm1638_init( void );
static void _controller_devices_one_wire_init( void );
static void _controller_devices_encoder( void );


void controller_devices_init(controller_devices_t **self)
{
	*self = calloc(1, sizeof(controller_devices_t));

	_controller_devices_encoder();
	_controller_devices_tm1638_init();
	_controller_devices_one_wire_init();

	_controller_devices_relay_init(*self);
	_controller_devices_flash_init(*self);
	_controller_devices_ds18b20_init(*self);

}

void controller_devices_deinit(controller_devices_t *self)
{
	relay_deinit(self->relay);
	flash_deinit(self->flash);
	ds18b20_device_deinit(self->ds18b_20_boiler);
	ds18b20_device_deinit(self->ds18b_20_pipe);
	free(self);
}


static void _controller_devices_relay_init(controller_devices_t *self)
{
	gpio_t relay_gpios[2] = {{RELAY_IN0_GPIO_Port, RELAY_IN0_Pin},
			  	  	  	     {RELAY_IN1_GPIO_Port, RELAY_IN1_Pin}};

	relay_config_t relay_configuration = {
		.n_ports = 2,
		.ports_gpio = (gpio_t*)relay_gpios,
	};

	relay_init(&self->relay, &relay_configuration);
}

static void _controller_devices_flash_init(controller_devices_t *self)
{
	flash_init(&self->flash);
}

static void _controller_devices_ds18b20_init(controller_devices_t *self)
{
	uint8_t boiler_rom_code [8] = { 0x28, 0xB5, 0xCA, 0x95, 0xF0, 0x01, 0x3C, 0x59 };
	uint8_t pipe_rom_code   [8] = { 0x28, 0xD7, 0x21, 0x95, 0xF0, 0x01, 0x3C, 0x5E };

	ds18b20_device_init(&self->ds18b_20_boiler, boiler_rom_code);
	ds18b20_device_init(&self->ds18b_20_pipe, pipe_rom_code);
}

static void _controller_devices_tm1638_init(void)
{
	/* TM1638 is build as singleton conception  */
	tm_1638_gpio_t tm_1638_gpio = {
		.clk = {TM1638_CLK_GPIO_Port, TM1638_CLK_Pin},
		.stb = {TM1638_STB_GPIO_Port, TM1638_STB_Pin},
		.dio = {TM1638_DIO_GPIO_Port, TM1638_DIO_Pin},
	};

	tm1638_init(&tm_1638_gpio);
}

static void _controller_devices_one_wire_init( void )
{
	one_wire_init(OW_GPIO_Port, OW_Pin);
}

static void _controller_devices_encoder( void )
{
	gpio_t encoder_sw = {
			.GPIOx = ENC_SW_GPIO_Port,
			.GPIO_Pin = ENC_SW_Pin,
	};

	encoder_init(&htim3, &encoder_sw);
}

