/*
 * IncFile2.h
 *
 * Created: 4/12/2018 11:14:28 PM
 *  Author: Owen Lyke
 */ 

#include <avr/io.h>
#include "SNOWCATHWR1.h"

#ifndef SERIAL_H_
#define SERIAL_H_

#define serial_baud 9600

#define SERIAL_buff_length 64
uint8_t SERIAL_buff[SERIAL_buff_length];

void SERIAL_init(void);
void SERIAL_transmit( uint8_t data );
uint8_t SERIAL_receive( void );




#endif /* INCFILE2_H_ */
