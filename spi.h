/*
 * spi.h
 *
 * Created: 4/12/2018 5:27:48 AM
 *  Author: Riley
 */ 

#ifndef SPI_H
#define SPI_H

#ifdef __cplusplus
extern "C" {
	#endif

#include <avr/io.h>

void SPI_init(uint8_t polarity, uint8_t phase, uint8_t divider);
//Initializes the SPI interface
//polarity=0 -> Clock idle low, polarity=1 -> Clock idle high
//phase=0 -> Data sampled at first (leading) edge of a clock pulse, phase=1 -> Data sampled at last (trailing) edge of a clock pulse  
//SPI_frequency = System_frequency/(4^(divider + 1)) 

uint8_t * SPI_transmit(uint8_t * sentence, uint8_t size);
//Transmits an array of chars of size
//Full duplex transfer ie data received is returned
//To send a byte set size to 1, to only receive let sentence be 0's 

uint8_t SPI_transmitByte(char data);

	#ifdef __cplusplus
}
#endif

#endif