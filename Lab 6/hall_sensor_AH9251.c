/*
 * Hall_Sensor_AH9251.c
 *
 * Created: 10/7/2022 5:19:24 PM
 * Author : Dr. sara.stout-grandy
 *
 * Takes output from Digital Hall Effect Sensor, outputs status on an LED and USART
 *
 * Complete the missing code everywhere there is an "xx"
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <util/delay.h>

void init_hardware(void);
void init_uart(void);

int main(void)
{
    init_hardware();
	init_uart();
	
	printf("System Booted, built %s on %s\n ", __TIME__,__DATE__);
	printf("Bring a Magnet Close to the Hall Sensor, see LED come on!\n");

	
    while (1) 
    {
		//Read the sensor, if it goes low, it has detected the magnet
		if ((PIND & (1<<DDD2))==0)
		{
			//Magnet detected, drive the LED high to turn it on
			PORTD |= (1<<DDD3); //drive LED High
			printf("Magnet Detected!\n");

		}
		else
		{
			//No Magnet detected, drive the LED low to turn it off
			PORTD &= ~(1<<DDD3); //drive LED low
			//printf("No Magnet\n");
		
		}
    }
}

void init_hardware(void)
{
	//Set pin as output to LED
	DDRD |= (1<<DDD3);
	
	//Set pin output to be low
	PORTD &= ~(1<<DDD3);
	
	//Set pin as input from sensor
	DDRD &= ~(1<<DDD2); 
	
	//Enable pull-up resistors on the input
	PORTD |= (1<<DDD2);

}
