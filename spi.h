/*
 * spi.h
 *
 * Created: 4/12/2018 5:27:48 AM
 *  Author: Riley
 */ 

#ifndef SPI_H
#define SPI_H

#include <avr/io.h>

void SPI_init(uint8_t polarity, uint8_t phase);
//Initializes the SPI interface

uint8_t * SPI_transmit(uint8_t * sentence, uint8_t size);
//Transmits an array of chars of size

uint8_t SPI_receive(void);
//Return the SPI data buffer

#endif
