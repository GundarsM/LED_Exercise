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
volatile unsigned int tim6_int_counter;


/* Definitions */
#define SYS_CLK 8000000UL

/* To get precise delay we need certain timer value */
#define TIM6_MAX_VAL 0xC350		/* Timer 6 max value will be set to 50'000 */
#define DELAY 500.0				/* Delay in ms */

/* Define last LED position for running light */
#define MAX_LED 0x200

/* Define first LED position for running light */
#define MIN_LED 0x01

/* Define macro to turn on one led, in visual order of connection */
#define LED0_ON  GPIOB->ODR |= 0b1<<8
#define LED1_ON  GPIOB->ODR |= 0b1<<9
#define LED2_ON  GPIOA->ODR |= 0b1<<5
#define LED3_ON  GPIOA->ODR |= 0b1<<6
#define LED4_ON  GPIOA->ODR |= 0b1<<7
#define LED5_ON  GPIOB->ODR |= 0b1<<6
#define LED6_ON  GPIOC->ODR |= 0b1<<7
#define LED7_ON  GPIOA->ODR |= 0b1<<8
#define LED8_ON  GPIOB->ODR |= 0b1<<10
#define LED9_ON  GPIOB->ODR |= 0b1<<4

/* Switch of all LEDs in running light */
#define LEDS_OFF  GPIOB->ODR &= ~0b1<<8;\
  GPIOB->ODR &= ~0b1<<9;\
  GPIOA->ODR &= ~0b1<<5;\
  GPIOA->ODR &= ~0b1<<6;\
  GPIOA->ODR &= ~0b1<<7;\
  GPIOB->ODR &= ~0b1<<6;\
  GPIOC->ODR &= ~0b1<<7;\
  GPIOB->ODR &= ~0b1<<10;\
  GPIOA->ODR &= ~0b1<<8;\
  GPIOB->ODR &= ~0b1<<4;

/* Define macro for reading the pushbutton at PORTC pin 13 */
#define BTN_RD !(GPIOC->IDR&(1<<13))

/* Function declaration */
void init_clocks(void);			/* Initialize peripheral clocks */
void init_led_pins(void);		/* Prepares pins for their operation modes */
void init_button_pin(void);		/* Setup button for reading, connected on PC13 */
void tunr_on_led(uint32_t led); /* Turn on necessary LED */
void init_timer_6(void);		/* Initialize timer 6 for generating delay*/
void init_serial_USART1(void); 	/* Initialize serial interface on USART1 module as USART2 pins are disconnected on Nucleo-64 board */
void send_byte_USART1(uint8_t byte);	/* Send one character/byte over serial interface/USART1 */
void send_string_USART1(char data[]);	/* Send text over USART1 */
uint8_t receive_byte_USART1(void);		/* Receive one byte/symbol */

#endif /* HARDWARE_H_ */
