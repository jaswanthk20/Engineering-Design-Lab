/*
 * trafficcontrol.c
 *
 * Created: 17/02/2023 10:04:31 PM
 * Author : jaswanth
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define LED1 PB0
#define LED2 PB1
#define LED3 PB2
#define LED4 PB3

int main(void)
{
	// Initialization Stage
	DDRB |= (1<<LED1) | (1<<LED2) | (1<<LED3) | (1<<LED4); // Set LEDs as output
	PORTB &= ~((1<<LED1) | (1<<LED2) | (1<<LED3) | (1<<LED4)); // Turn off LEDs
	uint8_t counter = 0; // Initialize counter to zero
	
	// Infinite Loop
	while(1)
	{
		if(counter == 0)
		{
			PORTB |= (1<<LED1); // Turn on LED1
		}
		else if(counter == 1)
		{
			PORTB |= (1<<LED2); // Turn on LED2
		}
		else if(counter == 2)
		{
			PORTB |= (1<<LED3); // Turn on LED3
		}
		else if(counter == 3)
		{
			PORTB |= (1<<LED4); // Turn on LED4
		}
		counter++; // Increment counter
		if(counter == 4)
		{
			counter = 0; // Reset counter when it reaches 4
		}
		_delay_ms(1000); // Delay for 1 second
		PORTB &= ~((1<<LED1) | (1<<LED2) | (1<<LED3) | (1<<LED4)); // Turn off all LEDs
	}
	return 0;
}
