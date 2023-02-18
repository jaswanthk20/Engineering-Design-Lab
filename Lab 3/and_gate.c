/*
 * and_gate.c
 */ 
 
#define F_CPU 16000000UL //CPU frequency  16MHz

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{	
	DDRC &= ~(1<<DDC0); // set pin C0 on PORT C as input
	DDRC &= ~(1<<DDC1); // set pin C1 on PORT C as input
	PORTC |= (1<<DDC0) | (1<<DDD1); // set pull-up resistor on PortC input pins

	DDRD &= ~(1<<DDD7); // set pin D7 on PORT D as input
	PORTD |= (1<<DDD7); // set pull-up resistor on PortD input pins

	
	// output pin to the AND gate is Port B
	DDRB |= (1<<DDB0);
	PORTB = 0x0; // write zero to the port to initialize it
	
	
    while (1) 
    {
		if ((PINC & (1<<PINC0)) && (PINC & (1<<PINC1)) && (PIND & (1<<PIND7))) {
			PORTB |= (1<<PORTB0); // set output as 1
		}
		
		else {
			PORTB &= ~(1<<PORTB0); // clear the output as 0
		}
    }
}
