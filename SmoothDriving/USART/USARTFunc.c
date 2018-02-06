/*
 * USARTFunc.c
 *
 * Created: 11/18/2017 4:16:14 PM
 *  Author: Matt
 */ 
 #include "USARTFunc.h"

 void InitUART()
 {
	 unsigned int baudrate;

	 // Set baud rate:  UBRR = [F_CPU/(16*BAUD)] -1
	 baudrate = ((F_CPU/16)/BAUD) - 1;
	 UBRR0H = (unsigned char) (baudrate >> 8);
	 UBRR0L = (unsigned char) baudrate;

	 UCSR0B |= (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);		// Enable receive, transmit and interrupt
	 UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);		// Set data frame: 8 data bits, 1 stop bit, no parity
 }


 void SendUARTString(char* str)
 {
	 unsigned int i = 0;
	 while(str[i] != 0 && i != 65535)
	 {
		 SendUARTChar(str[i]);
		 i++;
	 }
 }
 void SendUARTChar(char letter)
 {
	 UDR0 = letter;
	 while (!(UCSR0A & (1<<UDRE0)));
	 //_delay_us(1100);
 }

 void PrintUARTChars(char* str, uint8_t length)
{
	for(uint8_t i = 0; i < length; i++)
		SendUARTChar(str[i]);
}