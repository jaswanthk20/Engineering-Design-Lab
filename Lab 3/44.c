/*
 * UART_Test.c
 *
 * Created: 2023-01-22 10:41:48 AM
 * Author : Dr. Sara.stout-grandy
 */ 
 
#define F_CPU 16000000UL //CPU frequency  16MHz
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

int main(void)
{
	char name [32];
	char key;
	
	init_uart();//need to have uart_utility_function.c added to the project
	printf("System Booted, built %s on %s \n", __TIME__, __DATE__);
	
	printf("Hello world!\n What is your name?\n\r");
	scanf("%s", name);//wait for your name (from host)
	printf("\n Hi %s! Glad you got your code working!", name);
	
    while (1) 
    {
		printf("\n\r");
		printf("What Port do you want to read (B, C, or D)? Enter X to quit.\n\r");
		key = uart_getchar(); //waits for a character
		switch(key)
		{
			case 'B':
			printf("=%d", PINB);
			break;
			case 'C':
			printf("=%d", PINC);
			break;
			case 'D':
			printf("=%d", PIND);
			break;
			case 'X':
			printf("=Good bye!\n\r");
			while(1);
		}
    }
	return 0;
}
