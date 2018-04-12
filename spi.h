/*
 * spi.h
 *
 * Created: 4/12/2018 5:27:48 AM
 *  Author: Riley
 */ 

#ifndef SPI_H
#define SPI_H

#include <avr/io.h>

void initSpi(void);
//Initializes the SPI interface

void transmit(uint8_t * sentence, uint8_t size);
//Transmits an array of chars of size

char receive(void);
//Return the SPI data buffer

#endif