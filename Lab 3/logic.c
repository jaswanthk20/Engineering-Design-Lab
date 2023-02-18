/*
* 4.3 Implementation of Logic Circuit
  
*/
#define F_CPU 16000000UL //CPU frequency  16MHz

#include <util/delay.h>
#include <avr/io.h>

int main(void)
{
	// Initialize Ports as inputs
	DDRC &= ~((1<<DDC0) | (1<<DDC1)); // port C0,1 as inputs
	DDRD &= ~((1<<DDD2) | (1<<DDD3) | (1<<DDD4) | (1<<DDD5) | (1<<DDD6) | (1<<DDD7)); // port D2,3,4,5,6,7 as inputs
	
	// Initialize Ports as outputs
	DDRB |= (1<<DDB0) | (1<<DDB1) | (1<<DDB2); // port B0,1 and 2 as outputs	
	
	// Setting all inputs as HIGH
	PORTC |= (1<<PINC0) | (1<<PINC1);
	PORTD |= (1<<PIND2) | (1<<PIND3) | (1<<PIND4) | (1<<PIND5) | (1<<PIND6) | (1<<PIND7);
	
	while (1) {
		
		//AND gate
		//when all inputs are HIGH, output is LOW
		if ((PINC & (1<<PINC0)) && (PINC & (1<<PINC1)) && (PIND & (1<<PIND7))) {
			PORTB &= ~(1<<PB0); 
		}			
		else {
			PORTB |= (1<<PB0);
		}
		
		//INVERTER gate
		//when all inputs are HIGH, output is LOW
		if (PIND & (1<<PIND2)) {
			PORTB &= ~(1<<PB1); 
		}
		else {
			PORTB |= (1<<PB1);
		}
		
		//NOR gate
		//when all inputs are HIGH, output is LOW
		if ((PIND & (1<<PIND3)) && (PIND & (1<<PIND5))) {
			PORTB &= ~(1<<PB2);
		}
		else {
			PORTB |= (1<<PB2);
		}
		
		//XNOR gate
		// when all inputs are HIGH, output is HIGH	
		if ((PIND & (1<<PIND4)) && (PIND & (1<<PIND6))) {
			PORTB |= (1<<PB3);
		}
		else {
			PORTB &= ~(1<<PB3);
		}		
	}
	return 0;
}

