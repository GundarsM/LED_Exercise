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
	RCC->APB1ENR |= (1<<4); 	/* enable clock for timer6 */
}

/* Prepares pins for their operation modes */
void init_led_pins(void)
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

/* Turn on necessary LED */
void tunr_on_led(uint32_t led)

	{
	switch(led){
	case 0b0000000001: LEDS_OFF; LED0_ON; break;
	case 0b0000000010: LEDS_OFF; LED1_ON; break;
	case 0b0000000100: LEDS_OFF; LED2_ON; break;
	case 0b0000001000: LEDS_OFF; LED3_ON; break;
	case 0b0000010000: LEDS_OFF; LED4_ON; break;
	case 0b0000100000: LEDS_OFF; LED5_ON; break;
	case 0b0001000000: LEDS_OFF; LED6_ON; break;
	case 0b0010000000: LEDS_OFF; LED7_ON; break;
	case 0b0100000000: LEDS_OFF; LED8_ON; break;
	case 0b1000000000: LEDS_OFF; LED9_ON; break;
	default:LEDS_OFF; break;
	}//end of switch
}

/* Setup button for reading, connected on PC13 */
void init_button_pin(void)
{
	/* setup for reading button */
	GPIOC->MODER &= ~(0b11<<26);	/* Reset pin 13 state aka input */
	GPIOC->OTYPER &=~0b1<<13;		/* set as push-pull pin */
	GPIOC->OSPEEDR |= 0b11<<26;		/* set high-speed */
	GPIOC->PUPDR |= 0b01<<26;		/* set pull-up, technically no need as there is an external pull-up connected to the button */
}

/* Initialize timer6 for generating delay*/
void init_timer_6(void)
{
	TIM6->PSC = ((DELAY/1000*SYS_CLK)/TIM6_MAX_VAL)-1;	/* Set prescaler -> PSC+1 */
	TIM6->ARR = TIM6_MAX_VAL;        					/* Set timer to reset after CNT = 0xC350 */
	TIM6->DIER |= 1;									/* Enable interrupt */

	/* setup interrupt */
	NVIC->IP[TIM6_IRQn] =  (1 << 4); 						/* Set priority to 2 */
	NVIC->ISER[TIM6_IRQn >> 5] |= (1 << (TIM6_IRQn % 32)); 	/* Enable interrupt */
	TIM6->SR &= ~(1<<0);									/* Clear status register*/

	TIM6->EGR |= (1<<0);    	/* Reset timer counter registers */
	TIM6->CNT = 0;          	/* Manually reset CNT */
	TIM6->CR1 |= 1; 			/* enable timer6 */

	tim6_int_counter = 0;		/* Initialize interrupt counter */
}

/* Timer 6 interrupt service routine */
void TIM6_IRQHandler(void){
	tim6_int_counter++;		/* Increase interrupt counter */
	TIM6->SR &= ~(1<<0); 	/* Clear UIF update interrupt flag */
}

/* Change Timer6 parameters to adjust for inputed delay */
void change_delay(int delay)
{
	TIM6->PSC = ((delay/1000.0*SYS_CLK)/TIM6_MAX_VAL)-1;		/* Calculate new prescaler value PSC+1 */
	TIM6->CNT = 0;          									/* Manually reset CNT */
}

/* Initialize serial interface on USART1 module as USART2 pins are disconnected on Nucleo-64 board */
void init_serial_USART1(void)
{

    /* Configure GPIO pins for USART1 */
    GPIOA->MODER |= GPIO_MODER_MODER9_1 | GPIO_MODER_MODER10_1;  	/* Set PA9 and PA10 to alternate function mode */
    GPIOA->AFR[1] |= (1 << 4)|(1 << 8);  							/* Set PA9 and PA10 alternate function to USART1 */

    /* Configure USART1 */
    USART1->BRR = SYS_CLK / 9600.0;  								/* Set baud rate to 9600 */
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
