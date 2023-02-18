/*
 * trafficcontrol.c
 *
 * Created: 17/02/2023 10:04:31 PM
 * Author : jaswanth
 */

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "uart_utility_functions.h"

typedef enum {
	RED,
	GREEN,
	ORANGE
} State;

#define LED_GREEN PB0
#define LED_ORANGE PB1
#define LED_RED PB2
#define LED_PEDESTRIAN PB3
#define BUTTON PD2

volatile uint16_t timer = 0; // Global variable to store timer value
volatile uint8_t state = 0; // Global variable to store the current state
volatile uint8_t pedestrian_state = 0; // Global variable to store the current pedestrian state
volatile uint8_t button_pressed = 0; // Global variable to store whether the button is pressed

// Function to control the traffic light schedule
void run_scheduler()
{
    // State 0: Red light is on
    if (state == 0)
    {
        PORTB |= (1<<LED_RED);
        PORTB &= ~((1<<LED_GREEN) | (1<<LED_ORANGE) | (1<<LED_PEDESTRIAN));
        pedestrian_state = 0;
        if (timer >= 8000) // If 8 seconds have passed
        {
            state = 1; // Change to next state
            timer = 0; // Reset timer
        }
    }
    // State 1: Green light is on
    else if (state == 1)
    {
        PORTB |= (1<<LED_GREEN);
        PORTB &= ~((1<<LED_RED) | (1<<LED_ORANGE) | (1<<LED_PEDESTRIAN));
        if (pedestrian_state == 0 && timer >= 2000) // If 2 seconds have passed and pedestrian light is off
        {
            PORTB |= (1<<LED_PEDESTRIAN); // Turn on pedestrian light
            pedestrian_state = 1;
        }
        else if (pedestrian_state == 1 && timer >= 8000) // If 6 seconds have passed and pedestrian light is on
        {
            PORTB &= ~(1<<LED_PEDESTRIAN); // Turn off pedestrian light
            pedestrian_state = 2;
            timer = 0; // Reset timer
        }
    }
    // State 2: Orange light is on
    else if (state == 2)
    {
        PORTB |= (1<<LED_ORANGE);
        PORTB &= ~((1<<LED_RED) | (1<<LED_GREEN) | (1<<LED_PEDESTRIAN));
        pedestrian_state = 0;
        if (timer >= 2000) // If 2 seconds have passed
        {
            state = 0; // Change to next state
            timer = 0; // Reset timer
        }
    }
}

// ISR for timer 0 compare match A interrupt
ISR(TIMER0_COMPA_vect)
{
    timer++; // Increment timer value
}

State current_state = RED;

int main(void)
{
	init_uart();
	
	// Initialization Stage
	// Configure timer 0 with CTC mode and prescaler of 1024
	TCCR0A |= (1<<WGM01);
	TCCR0B |= (1<<CS02) | (1<<CS00);
	OCR0A = 156; // Set compare match value for 1 ms interrupt
	TIMSK0 |= (1<<OCIE0A); // Enable interrupt for compare match A

	sei(); // Enable global interrupts

	// Initialize LEDs
	DDRB |= (1<<LED_RED) | (1<<LED_ORANGE) | (1<<LED_GREEN) | (1<<LED_PEDESTRIAN);
	PORTB &= ~((1<<LED_RED) | (1<<LED_ORANGE) | (1<<LED_GREEN) | (1<<LED_PEDESTRIAN)); // Turn off all LEDs

	// Initialize button
	DDRD &= ~(1<<BUTTON);
	PORTD |= (1<<BUTTON); // Enable pull-up resistor

	// Initialize state machine

	// Infinite Loop
	while(1)
	{
		// Run the scheduler
		run_scheduler();

		// Check if button is pressed
		if ((PIND & (1<<BUTTON)) == 0 && !button_pressed)
		{
			// If button is pressed and not already pressed
			current_state = RED; // Reset state machine to initial state
			state = 0;
			timer = 0; // Reset timer
			button_pressed = 1; // Set button_pressed flag
		}
		else if ((PIND & (1<<BUTTON)) != 0)
		{
			// If button is not pressed
			button_pressed = 0; // Reset button_pressed flag
		}

		// Print current state to UART for testing
		printf("Current state: %d\n", state);

		_delay_ms(10); // Small delay to prevent switch bouncing
	}
}
