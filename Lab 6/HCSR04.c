/*
 * HC_SR04.c
 *
 * Author : Dr. Sara Stout-Grandy
 *
 * Code to make the HC-SR04 Ultrasonic Sensor Work
 * Complete the "xx" with the proper code
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "uart_utility_functions.h"

//Global variables
volatile unsigned char MIP; //Measurement-In-Progress Flag
volatile unsigned int ECHOHigh, ECHOHighTimes;
volatile unsigned int TimeOutCnt,Tick;
volatile double ECHOLow;


/***************************************************************************************
Interrupt Service Routine that is called when the echo length is longer than the counter duration.  
Use an additional byte to indicate how many times the maximum value is reached.
***************************************************************************************/
ISR (TIMER1_OVF_vect) // For long ECHO's
{	
	if(ECHOHighTimes >= 2)
	{
		cli();	// Disable interrupts. (xx)
		TCCR1B = 0; // Stop the counter (xx)
		MIP = 0xFF;	// End Measurement
	}
	
	ECHOHighTimes++;	// Add 1 to High byte.
}

/***************************************************************************************
Interrupt service routine called when the input capture pin state is changed
***************************************************************************************/
ISR (TIMER1_CAPT_vect)
{	
	// Start and Stop ECHO measurement;
	if((TCCR1B & (1 << ICES1)) != 0)  // a rising edge has been detected
	{
		TCCR1B &= ~(1 << ICES1);  // Configure Falling Edge Capture for end of echo pulse. (xx)
		TCCR1B |= (1 << CS11);  // Start counting with prescaler=8 (xx)
	
	}
	
	else // a falling edge has been detected
	{						
		ECHOLow = (double)(ICR1 * 0.000016); //See lecture notes for this equation, decimal places added to make calculation a decimal (xx)
		ECHOHigh = ECHOHighTimes; //EchoHigh will have a value if the count exceeded the size of the register (xx)
		OCR1B = 16000;			// set register to count the 10 mS Post echo Delay (xx)
		TIMSK1 |= (1<<OCIE1B) ;	// Enables the Compare B interrupt for POST Trigger Delay: Approx 10mS (xx)
		TCNT1 = 0;	//initialize the counter to zero (xx)
	}
}


/***************************************************************************************
Interrupt service routine called when the counter1 has reached the compare value (post echo delay is complete)
***************************************************************************************/
ISR (TIMER1_COMPB_vect) // Compare B: Post ECHO delay 10mS
{	
	MIP = 0;	// End Measurement
}

/***************************************************************************************
Interrupt service routine called when the counter 1 has reached the compare value (end of trigger pulse)
***************************************************************************************/
ISR (TIMER1_COMPA_vect) // Compare A : End of Trigger Pulse
{	
	PORTD &= ~(1<<DDD2);	//Clear the trigger pin (xx)
	TIMSK1 |= (1 << TOIE1) | (1 << ICIE1); // enables the Timer/Counter1 Overflow and Timer Counter1 Capture interrupt; (xx)
	TCCR1B |= (1 << ICES1);	// Set Positive edge for capture but Don't count yet...only start counting once the echo pulse goes high (xx)
}

/******************************************************************************
******************************************************************************/
void Trigger( void ) // Config Timer 1 for 10uS pulse.
{		
	if(MIP == 0) // Don't allow re-trigger.
	{	
		MIP = 1; // Set Measurement-In-Progress FLAG
		
		DDRD |= (1 << DDD2);		// Set Output pin for Trigger pulse. (xx)
		DDRD &= ~(1 << DDD3);		// Set Input pin for Input Capture (ECHO) (xx)
		PORTD |= (1 << DDD3);		//Set the pull-up resistor on Echo pin (xx)
		
		TCNT1 = 0;			// Clear last Echo times.
		ECHOHighTimes = 0;		// Keeps track of how many times the Timer1 counter overflows, this indicates that the echo time is greater than 30ms
		
		OCR1A = 160;			// set register to count the 10 us Trigger length (xx)

		TIFR1 = (1 << ICF1) | (1 << OCF1A) | (1 << OCF1B);			// Clear all timer interrupt flags (xx)
		TCCR1A = 0;			// Normal compare output mode, Normal timer/counter mode (xx)
		TIMSK1 |= (1 << OCIE1A);		// enable output compare A match interrupt enabled (xx)
	
		PORTD |= (1 << DDD2);		// Start the trigger pulse. (Output a HIGH to trigger pin) (xx)
		TCCR1B |= (1 << CS11) | (1 << WGM12);  		// Counting with prescaler=8, CTC Mode enabled, counting enabled (xx)

	}
	
}

/******************************************************************************
******************************************************************************/
int main(void){
	
    uint8_t key;
	
    init_uart();

    sei();
	
	printf("Press T to get a Distance Measurement or 0 to exit\n\n");

    while (1)
	{
	
		key = getchar();
	
		switch(key)
		{
			case '0':
				printf("\n Exiting!");
				break; //breaks out of the switch statement

			case 'T':
			case 't':
				Trigger(); //call the trigger function (xx)
				while (MIP == 1)
				{
				};
				printf("\nObject is %d & %f cm away\n\n",	ECHOHigh, ECHOLow); //Print to Putty the Distance
				break;
		
		}
		if (key=='0')
		{
			break; //Break out of the While loop
		}
    }
	return 0;
}
