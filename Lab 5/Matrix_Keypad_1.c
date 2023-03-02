/*
 * Matrix_KeyPad_1.c
 *
 * Created: 10/2/2022 1:38:45 PM
 * Author : Jaswanth Kumaar
 *
 * Output button presses to Putty via USART
 * Fill in the missing code at every "xx"
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "uart_utility_functions.h"


//Function Prototypes
void init_hardware(void);
void set_row_low(int row);
int col_pushed(void);


int main(void)
{
	int row;
	int col;
	init_uart();

	printf("System Booted, built %s on %s\n ", __TIME__,__DATE__);
	printf("Simple button test\n");

	
	char buttons[4][3]={{'1', '2', '3'},
						{'4', '5', '6'},
						{'7', '8', '9'},
						{'*', '0', '#'}
						};//Link button push to a button label
	
	init_hardware();
		
	while (1) 
    	{
		//Cycle through the rows with the for loop
		for (row = 0 ; row < 4 ; row++)
		{
			set_row_low(row); 
			_delay_ms(20);
			
			col=col_pushed(); 

			
			if (col)
			{
				printf("%d-%d: %c\n", row, col, buttons[row][col-1]);
				_delay_ms(500);
			}
		}
	}
}

void init_hardware(void)
{
	//Set pins: rows as output and columns as inputs
	//Rows as outputs
	DDRB = 1;
		
	//Set outputs to be low
	PORTB &= ~(1<<PORTB3);
	PORTB &= ~(1<<PORTB2);
	PORTB &= ~(1<<PORTB1);
	PORTB &= ~(1<<PORTB0);
			
	//Set Columns as inputs
	DDRD = 0;
			
	//Enable pull-up resistors on the inputs
	PORTD |= (1<<PORTD7) | (1<<PORTD6) | (1<<PORTD5);
}



void set_row_low(int row)
{
	//Hi-Z the rows (make inputs without pull-ups)	
	DDRB &=~(1<< PORTB0 | 1<<PORTB1 | 1<<PORTB2 | 1<<PORTB3);
	
	//Drive the specified row low
	switch(row)
	{    
		case 0: //set Row 1 low
			DDRB |= (1<<PORTB3);
			PORTB &= ~(1<<PORTB3);
			PORTB |= (1<<PORTB2);
			PORTB |= (1<<PORTB1);
			PORTB |= (1<<PORTB0);
			break; 
		case 1: //set Row 2 low
		DDRB |= (1<<PORTB2);
			PORTB &= ~(1<<PORTB2);
			PORTB |= (1<<PORTB3);
			PORTB |= (1<<PORTB1);
			PORTB |= (1<<PORTB0);
			DDRB &= ~(1<<PORTB3);
			break; 
		case 2: //set Row 3 low
		DDRB |= (1<<PORTB1);
			PORTB &= ~(1<<PORTB1);
			PORTB |= (1<<PORTB2);
			PORTB |= (1<<PORTB3);
			PORTB |= (1<<PORTB0);
			DDRB &= ~(1<<PORTB2);
			break; 
		case 3: //set Row 4 low
		DDRB |= (1<<PORTB0);
			PORTB &= ~(1<<PORTB0);
			PORTB |= (1<<PORTB2);
			PORTB |= (1<<PORTB1);
			PORTB |= (1<<PORTB3);
			DDRB &= ~(1<<PORTB1);
			break; 
		default: printf("no row set\n");
	}
}

int col_pushed(void)
{
    //Return the column that was detected

	if ((PIND & (1<<PIND7))==0) //check column 1  
	{
		printf("column=1\n");
		return 1;
	}
	else if ((PIND & (1<<PIND6))==0) //check column 2
	{
		printf("column=2\n");
		return 2;
	}
	else if ((PIND & (1<<PIND5))==0) //check column 3
	{
		printf("column=3\n");
		return 3;
	}
	else
	{
		return 0;
	}
	
}
