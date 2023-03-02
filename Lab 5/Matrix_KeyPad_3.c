/*
 * Matrix_KeyPad_3.c
 *
 * Created: 10/2/2022 1:38:45 PM
 * Author : Jaswanth Kumaar
 *
 * Move the Button Press/Release to a function
 * Fill in the missing code at every "xx"
 */ 
#define F_CPU 16000000UL

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "uart_utility_functions.h"

//Volatile Variable
volatile int row;
volatile int col;


//Function Prototypes
void init_hardware(void);
void set_row_low(int row);
int col_pushed(void);
char get_button(void);
char get_new_button(void);

int main(void)
{
	char b=0;
	init_uart();

	printf("System Booted, built %s on %s\n ", __TIME__,__DATE__);
	printf("Move the button press/release to a function\n");
	
	init_hardware();

	while (1) 
    	{
		b=get_new_button();
 	    	
		if(b)
		{
		    printf("Button: %c\n", b);
	    	}
	}
	return 0;
}


char get_button(void)
{
	int key_pressed=0;
	char b;
	char buttons[4][3]={{'1', '2', '3'},
	{'4', '5', '6'},
	{'7', '8', '9'},
	{'*', '0', '#'}};//Link button push to a button label
	
	//Check for button push
	//Cycle through the rows with the for loop
	for (row=0; row < 4; row++) {
		set_row_low (row);
		_delay_ms(20);
		
		col=col_pushed();

		if (col) {
			b=buttons[row][col-1]; //b= look-up table in matrix
			printf("%d-%d: %c\n", row, col,b );
			_delay_ms(500);
			key_pressed=1; //A key was pressed
		}
	}
	
	if (key_pressed) {
		return b;
	}
	else {
		return 0;
	}

}

char get_new_button(void)
{
	static char last_button=0;
	char b;
	
	b=get_button(); //Call get_button function
	
	//Check if we held button down, if yes, return 0
	if (b == last_button) {
		return 0;
	}

	last_button=b;
	return b;
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
	switch(row) {
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

int col_pushed(void) {
    //Return the column that was detected

    if ((PIND & (1<<PIND7))==0) { //check column 1
	    printf("column=1\n");
	    return 1;
    }
    else if ((PIND & (1<<PIND6))==0) { //check column 2
	    printf("column=2\n");
	    return 2;
    }
    else if ((PIND & (1<<PIND5))==0) { //check column 3
	    printf("column=3\n");
	    return 3;
    }
    else {
	    return 0;
    }
}
