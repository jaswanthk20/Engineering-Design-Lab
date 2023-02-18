/*
 * uart_utility_functions.h
 *
 * Created: 2022-11-02 10:27:53 AM
 * Author : JF
 */ 

#ifndef UART_UTILITY_FUNCTIONS_H_
#define UART_UTILITY_FUNCTIONS_H_

#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

int uart_putchar(char c, FILE *stream);
int uart_getchar(FILE *stream);
void init_uart(void);

#endif /* UART_UTILITY_FUNCTIONS_H_ */
