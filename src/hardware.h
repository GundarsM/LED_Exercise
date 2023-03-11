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

/* Definitions */
#define SYSCLK 8000000UL

/* Function declaration */
void init_clocks(void);		/* Initialize peripheral clocks */
void led_pin_setup(void);	/* Prepares pins for their operation modes */
void test_leds(void);		/* Test if all 9 LEDs turn on */
void button_pin_setup(void);	/* Setup button for reading, connected on PC13 */
void init_serial_USART1(void); 	/* Initialize serial interface on USART1 module as USART2 pins are disconnected on Nucleo-64 board */
void send_byte_USART1(uint8_t byte);	/* Send one character/byte over serial interface/USART1 */
void send_string_USART1(char data[]);	/* Send text over USART1 */
uint8_t receive_byte_USART1(void);		/* Receive one byte/symbol */

#endif /* HARDWARE_H_ */
