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
	RCC->APB2ENR |= 1<<14;		/* enable clock for USART1 */
}

/* Prepares pins for their operation modes */
void led_pin_setup(void)
{
	/* PORTA pins 5,6,7,8,10 */
	GPIOA->MODER &= ~((0b11<<10)|(0b11<<12)|(0b11<<14)|(0b11<<16));		/* Reset pin 5,6,7,8 state */
	GPIOA->MODER |=  0b01<<10|0b01<<12|0b01<<14|0b01<<16;				/* Setup pin 5,6,7,8 as output */
	GPIOA->OTYPER &=~(0b1<<5|0b1<<6|0b1<<7|0b1<<8);						/* set pin   5,6,7,8 as push-pull pin */

	/* PORTB pin 4,6,8,9*/
	GPIOB->MODER &= ~((0b11<<8)|(0b11<<12)|(0b11<<16)|(0b11<<18)|(0b11<<20));	/* Reset pin 4,6,8,9,10 state */
	GPIOB->MODER |=  0b01<<8|0b01<<12|0b01<<16|0b01<<18|0b01<<20;				/* Setup pin 4,6,8,9,10 as output */
	GPIOB->OTYPER &=~(0b1<<4|0b1<<6|0b1<<8|0b1<<9|0b1<<10);						/* set pin   4,6,8,9,10 as push-pull pin */

	/* PORTC pin 7 */
	GPIOC->MODER &= ~(0b11<<14);		/* Reset pin 7 state */
	GPIOC->MODER |=  0b01<<14;			/* Setup pin 7 as output */
	GPIOC->OTYPER &=~0b1<<7;			/* set pin   7 as push-pull pin */
}

/* Test if all 9 LEDs turn on */
void test_leds(void)
{
	/* Turn on LEDs PORTA pins 5,6,7,8 */
	GPIOA->ODR |= 0b1<<5;
	GPIOA->ODR |= 0b1<<6;
	GPIOA->ODR |= 0b1<<7;
	GPIOA->ODR |= 0b1<<8;


	/* Turn on LEDs PORTB pin 4,6,8,9,10 */
	GPIOB->ODR |= 0b1<<4;
	GPIOB->ODR |= 0b1<<8;
	GPIOB->ODR |= 0b1<<9;
	GPIOB->ODR |= 0b1<<6;
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

void init_serial_USART1(void)
{

    /* Configure GPIO pins for USART1 */
    GPIOA->MODER |= GPIO_MODER_MODER9_1 | GPIO_MODER_MODER10_1;  	/* Set PA9 and PA10 to alternate function mode */
    GPIOA->AFR[1] |= (1 << 4)|(1 << 8);  							/* Set PA9 and PA10 alternate function to USART1 */

    /* Configure USART1 */
    USART1->BRR = SYSCLK / 9600.0;  								/* Set baud rate to 9600 */
    USART1->CR2 |= USART_CR2_STOP_1;								/* Set 2 stop bits */
    USART1->CR1 = USART_CR1_TE | USART_CR1_UE | USART_CR1_RE ;  	/* Enable transmitter, receiver and USART1 */
}

/* Send one character/byte over serial interface/USART1 */
void send_byte_USART1(uint8_t byte)
{
    while (!(USART1->ISR & USART_ISR_TXE));			/* Wait for TXE (transmit data register empty) flag to be set */
    USART1->TDR = byte;								/* Write byte to data register */
    while (!(USART1->ISR & USART_ISR_TC));			/* Wait for TC (transmission complete) flag to be set */
}

/* Send text over USART1 */
void send_string_USART1(char data[])
{
	uint32_t counter = 0;
	while(data[counter] != 0){				/* Send data while '/0' not reached */
		send_byte_USART1(data[counter++]);	/* Send one byte*/
	}
	send_byte_USART1('\r');					/* Reset cursor in terminal to start of line*/
	send_byte_USART1('\n');
}

/* Receive one byte/symbol */
uint8_t receive_byte_USART1(void)
{
	uint8_t chartoreceive = 0;

	if ((USART1->ISR & USART_ISR_RXNE) == USART_ISR_RXNE){		/* If RXE (receive data register full) flag is set */
		chartoreceive = (uint8_t)(USART1->RDR); 				/* Receive data, clear flag */
		return chartoreceive;									/* Return received data */
		}
	else {
		return 0;	/* Nothing has been received, return 0 */
	}
}
