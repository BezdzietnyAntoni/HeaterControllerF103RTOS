#include "configuration.h"

/* Shortcuts display in menu mode */
static const char * CONFIG_MENU_CHAR [MENU_N_OPTION] =
{
	"bdt",
	"bdh",
	"btS",
	"btH",
	"btU",
	"PtS",
	"PtH",
	"HtU",
};

/* Available parameters range */
static const uint8_t CONFIG_RANGE [MENU_N_OPTION][2] =
{
	{0, 20},	//  Differential boiler temperature
	{0, 10},	//  Differential boiler temperature hysteresis
	{0, 90},	//  Boiler temperature set
	{0, 10},	//  Boiler temperature set hysteresis
	{0, 90},	//  Boiler temperature max
	{0, 90},	//  Water heater temperature set
	{0, 10},	//  Water heater temperature hysteresis
	{60, 120},	//  Hot water max temperature
};

/* Default setting using when some issue with as flash memory */
static const configuration_t CONFIG_DEFAULT = {5, 5, 30, 2, 80, 30, 5, 90};

/*
 * Public function definition
 */
const char* config_get_menu_char(menu_option_t menu_option) { return CONFIG_MENU_CHAR[menu_option]; }

const configuration_t* config_get_default( void ) { return &CONFIG_DEFAULT; }

uint8_t config_get_default_opt( menu_option_t menu_option ) { return CONFIG_DEFAULT[menu_option]; }

uint8_t config_get_min(menu_option_t menu_option) {return CONFIG_RANGE[menu_option][0]; }

uint8_t config_get_max(menu_option_t menu_option) {return CONFIG_RANGE[menu_option][1]; }




