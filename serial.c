/*
 * CFile2.c
 *
 * Created: 4/12/2018 11:17:45 PM
 *  Author: Owen Lyke
 */ 

#include "serial.h"
#include <avr/interrupt.h>

// SNOWCAT uses USART0
// With the final 4.194304 MHz clock the best setting to use is the normal speed baud rate generator with UBRR0 = 26. However if the internal 
// We want to use 8 data bits, no parity, one stop bit
void SERIAL_init(void)
{
	// Set baud rate
	if(1) // Use this until we switch to the low-speed oscillator
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
	
	SERIAL_read_ind = 0;
	SERIAL_write_ind = 0;
	SERIAL_write_ok = 1;				// Okay to write things to the buffer (from a receive operation)
	
	// Setup interrupts
	SREG |= 0x80;	// Enable global interrupts
	UCSR0B |= (1<<RXCIE0); // Enable receive interrupt
}

void SERIAL_transmit( uint8_t data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) )
	;
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

//uint8_t SERIAL_receive( void )
//{
	///* Wait for data to be received */
	//while ( !(UCSR0A & (1<<RXC0)) ){};
	///* Get and return received data from buffer */
	//return UDR0;
//}

void SERIAL_print(uint8_t * pdata, uint8_t num_bytes)
{
	for(uint8_t indi = 0; indi < num_bytes; indi++)
	{
		SERIAL_transmit(*(pdata + indi));
	}
}


uint8_t SERIAL_available(void )
{
	if(SERIAL_read_ind > SERIAL_write_ind)
	{
		return ((SERIAL_buff_length - SERIAL_read_ind) + (SERIAL_write_ind));
	}
	if(SERIAL_read_ind < SERIAL_write_ind)
	{
		return (SERIAL_write_ind - SERIAL_read_ind);
	}
	else
	{
		return 0;
	}
}

uint8_t SERIAL_read_buff(void)
{
	
	uint8_t ret = 0;
	if(SERIAL_available())
	{
		ret = *(&SERIAL_buff[0] + SERIAL_read_ind);
		SERIAL_read_ind++;
		if(SERIAL_read_ind >= SERIAL_buff_length){ SERIAL_read_ind = 0; }
	}
	else
	{
		ret = 255;
	}	
	SERIAL_write_ok = 1;
	return ret;
}


// Interrupt for the reception of serial data
ISR(USART0_RX_vect)
{	
	PIND |= 0b00010000;	// Toggles the red LED so that we can see the interrupt happening

	uint8_t discard;
	if(SERIAL_write_ok)
	{
		*(&SERIAL_buff[0] + SERIAL_write_ind) = UDR0;								// If it is OK to write to the buffer then do so
		SERIAL_write_ind++;															// Increment the index
		if(SERIAL_write_ind >= SERIAL_buff_length){ SERIAL_write_ind = 0; }			// Check Overflow
		if(SERIAL_write_ind == SERIAL_read_ind){ SERIAL_write_ok = 0; }				// If you arrive at the read index after having just incremented the write index then clearly you aren't allowed to write any more until the read index increases
	}
	else
	{
		discard = UDR0;	// Discrad it becuase the buffer is full
	}
	PIND |= 0b00010000;
}



