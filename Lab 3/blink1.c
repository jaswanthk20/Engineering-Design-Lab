/*
 * blink.c
 */ 
 
#define F_CPU 16000000UL //CPU frequency  16MHz
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRB |= (1<<DDB5); //set the data direction register at B5 so that it is in write mode
	PORTB = 0;//write 0 to portb
	
    while (1) 
    {
		_delay_ms(1000);//1s (1000ms) delay
		PORTB ^= (1<<PORTB5);//toggle portb5
		_delay_ms(1000);	
		PORTB ^= (1<<PORTB5);
		_delay_ms(1000);
		PORTB ^= (1<<PORTB5);
		_delay_ms(1000);
		PORTB ^= (1<<PORTB5);
		_delay_ms(500);
    }
}

