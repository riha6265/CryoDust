/*
 * IncFile2.h
 *
 * Created: 4/12/2018 11:14:28 PM
 *  Author: Owen Lyke
 */ 

#include <avr/io.h>

#ifndef SERIAL_H_
#define SERIAL_H_

#define serial_baud 9600

#define SERIAL_buff_length 64
uint8_t SERIAL_buff[SERIAL_buff_length];
uint8_t SERIAL_read_ind;
uint8_t SERIAL_write_ind;
uint8_t SERIAL_write_ok;

void SERIAL_init(void);
void SERIAL_transmit( uint8_t data );
//uint8_t SERIAL_receive( void );
void SERIAL_print(uint8_t * pdata, uint8_t num_bytes);
uint8_t SERIAL_available( void );
uint8_t SERIAL_read_buff( void );




#endif /* INCFILE2_H_ */
