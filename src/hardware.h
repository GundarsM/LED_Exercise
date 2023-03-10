/*
 * hardware.h
 *
 *  Created on: Mar 10, 2023
 *      Author: GundarsMiezitis
 */

#ifndef HARDWARE_H_
#define HARDWARE_H_

/* Includes */
#include "stm32f0xx.h"

/* Global variables */
volatile unsigned int tim6IntCounter;


/* Function declaration */
void init_clocks(void);		/* Initialize peripheral clocks */
void led_pin_setup(void);	/* Prepares pins for their operation modes */
void test_leds(void);		/* Test if all 9 LEDs turn on */
void button_pin_setup(void);	/* Setup button for reading, connected on PC13 */
void init_timer_6(void);	/* Initialize timer 6 for generating delay*/

#endif /* HARDWARE_H_ */
