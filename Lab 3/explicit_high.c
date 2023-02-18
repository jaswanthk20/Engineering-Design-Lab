/*
 * blink.c
 *
 * Created: 2023-01-25 2:10:48 PM
 * Author : Dr. Sara.stout-grandy
 */ 
 
#define F_CPU 16000000UL //CPU frequency  16MHz
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	//DDRB |= (1<<DDB5); //set the data direction register at B5 so that it is in write mode
	PORTB = 0;//write 0 to portb
	
    while (1) 
    {
		PORTB = 0xFF;
		_delay_ms(1000);
		PORTB = 0x00;
		_delay_ms(1000);
    }
}
