/*
 * harware.c
 *
 *  Created on: Mar 10, 2023
 *      Author: GundarsMiezitis
 */

#include "hardware.h"

/* Function declarations */

/* Initialize peripheral clocks */
void init_clocks(void)
{
	RCC->AHBENR |= 0x1<<17;		/* Enable port A */
	RCC->AHBENR |= 0x1<<18;		/* Enable port B */
	RCC->AHBENR |= 0x1<<19;		/* Enable port C */
}

/* Prepares pins for their operation modes */
void led_pin_setup(void)
{
	/* PORTA pins 5,6,7,8,9 */
	GPIOA->MODER &= ~((0b11<<10)|(0b11<<12)|(0b11<<14)|(0b11<<16)|(0b11<<18));		/* Reset pin 5,6,7,8,9 state */
	GPIOA->MODER |=  0b01<<10|0b01<<12|0b01<<14|0b01<<16|0b01<<18;					/* Setup pin 5,6,7,8,9 as output */
	GPIOA->OTYPER &=~(0b1<<5|0b1<<6|0b1<<7|0b1<<8|0b1<<9);							/* Set pin 5,6,7,8,9 as push-pull pin, reset state */

	/* PORTB pin 6,8,9,10 */
	GPIOB->MODER &= ~((0b11<<12)|(0b11<<16)|(0b11<<18)|(0b11<<20));		/* Reset pin 6,8,9,10 state */
	GPIOB->MODER |=  0b01<<12|0b01<<16|0b01<<18|0b01<<20;				/* Setup pin 6,8,9,10 as output */
	GPIOB->OTYPER &=~(0b1<<6|0b1<<8|0b1<<9|0b1<<10);					/* Set pin 6,8,9,10 as push-pull pin, reset state */

	/* PORTC pin 7 */
	GPIOC->MODER &= ~(0b11<<14);	/* Reset pin 6,8,9,10 state */
	GPIOC->MODER |=  0b01<<14;		/* Setup pin 6,8,9,10 as output */
	GPIOC->OTYPER &=~0b1<<7;		/* Set pin 6,8,9,10 as push-pull pin, reset state */
}

/* Test if all 9 LEDs turn on */
void test_leds(void)
{
	/* Turn on LEDs PORTA pins 5,6,7,8,9 */
	GPIOA->ODR |= 0b1<<5;
	GPIOA->ODR |= 0b1<<6;
	GPIOA->ODR |= 0b1<<7;
	GPIOA->ODR |= 0b1<<8;
	GPIOA->ODR |= 0b1<<9;

	/* Turn on LEDs PORTB pin 6,8,9,10 */
	GPIOB->ODR |= 0b1<<6;
	GPIOB->ODR |= 0b1<<8;
	GPIOB->ODR |= 0b1<<9;
	GPIOB->ODR |= 0b1<<10;

	/* Turn on LED PORTC pin 7 */
	GPIOC->ODR |= 0b1<<7;
}

/* Setup button for reading, connected on PC13 */
void button_pin_setup(void)
{
	/* setup for reading button */
	GPIOC->MODER &= ~(0b11<<26);	/* Reset pin 13 state aka input */
	GPIOC->OTYPER &=~0b1<<13;		/* set as push-pull pin */
	GPIOC->OSPEEDR |= 0b11<<26;		/* set high-speed */
	GPIOC->PUPDR |= 0b01<<26;		/* set pull-up, technically no need as there is an external pull-up connected to the button */
}
