/*
 *  Created on: Jan 12, 2024
 *      Author: Antoni Bezdzietny
 */

#ifndef INC_CONFIGURATION_H_
#define INC_CONFIGURATION_H_

#include <stdint.h>

typedef enum
{
	MENU_OPTION_BRT = 0,		//  Differential boiler temperature
	MENU_OPTION_BRH = 1, 		//  Differential boiler temperature hysteresis
	MENU_OPTION_BTS = 2,		//  Boiler temperature set
	MENU_OPTION_BTH = 3,		//  Boiler temperature set hysteresis
	MENU_OPTION_BTU = 4,    	//  Boiler temperature max
	MENU_OPTION_PTS = 5,    	//  Water heater temperature set
	MENU_OPTION_PTH = 6,		//  Water heater temperature hysteresis
	MENU_OPTION_RTU = 7,		//  Hot water max temperature
	MENU_N_OPTION, 	// Number of menu option
}menu_option_t;

typedef uint8_t configuration_t [MENU_N_OPTION] ;


const char* config_get_menu_char(menu_option_t menu_option);

const configuration_t* config_get_default( void );

uint8_t config_get_default_opt( menu_option_t menu_option );

uint8_t config_get_min(menu_option_t menu_option);

uint8_t config_get_max(menu_option_t menu_option);


#endif /* INC_CONFIGURATION_H_ */
