/*
 * IncFile1.h
 *
 * Created: 4/20/2018 11:16:00 PM
 *  Author: Owen Lyke
 */ 


#ifndef SERVO_H_
#define SERVO_H_

#ifdef __cplusplus
extern "C" {
	#endif

#include <avr/io.h>

volatile uint8_t fin;

void servo_goto_chamber(uint8_t chamber);

void servo_initialize();
void servo_power_on();
void servo_power_off();
void servo_write( int16_t value );

typedef struct{
	uint16_t	center_value;
	uint16_t	current_value;
	uint16_t	frame_length;
}servo_HandleTypeDef;

servo_HandleTypeDef		SNWCT_SERVO;

#ifdef __cplusplus
}
#endif

#endif /* SERVO_H_ */
