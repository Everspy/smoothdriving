/*
 * USARTFunc.h
 *
 * Created: 11/18/2017 4:16:50 PM
 *  Author: Matt
 */ 

#ifndef USARTFUNCS_H_
#define USARTFUNCS_H_

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#ifndef BAUD
#define BAUD 9600
#endif

#include <avr/interrupt.h>
#include <avr/io.h>

/**
    The InitUART() function sets up the RX and TX pins for UART
 */
void InitUART();
/**
    The SendUARTString() sends the passed string through UART. String must be null-terminated
 */
void SendUARTString(char* str);
/**
    The SendUARTChar() function sends the passed char through UART
 */
void SendUARTChar(char letter);
/**
    The PrintUARTChars function sends the 'length' number of chars from the passed char array
 */
void PrintUARTChars(char* str, uint8_t length);

#endif