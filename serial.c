/*
 * CFile2.c
 *
 * Created: 4/12/2018 11:17:45 PM
 *  Author: Owen Lyke
 */ 

#include "serial.h"


// SNOWCAT uses USART0
// With the final 4.194304 MHz clock the best setting to use is the normal speed baud rate generator with UBRR0 = 26. However if the internal 
// We want to use 8 data bits, no parity, one stop bit
void SERIAL_init(void)
{
	// Set baud rate
	if(FOSC == 8000000)
	{
		// Use UBRR0 = 51, normal speed mode
		UBRR0H = 0x00;
		UBRR0L = 12;
	}
	else
	{
		// Assume the 4.194304 MHz clock is used, then use UBRR0 = 26, normal speed mode
		UBRR0H = 0x00;
		UBRR0L = 6;
	}
	UCSR0A = 0b11000010;
	UCSR0B = 0b00011000; // Tx/Rx complete interrupts disabled, Data Reg Empty interrupt disables, Tx/Rx enabled, 8bit word
	UCSR0C = 0b00000110; // Sets asynchronous mode, no parity, 1 stop bit, 8bit word (in combo with UCSZ02 of UCSR0B)
	
	// Setup interrupts
	

}

void SERIAL_transmit( uint8_t data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) )
	;
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

uint8_t SERIAL_receive( void )
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) ){};
	/* Get and return received data from buffer */
	return UDR0;
}
