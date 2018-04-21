/*
 * IncFile2.h
 *
 * Created: 4/12/2018 11:14:28 PM
 *  Author: Owen Lyke
 */ 


#ifndef MOTOR_H_
#define MOTOR_H_

void servo_initialize( void );
void servo_power_off( void );
void servo_power_on( void );
void servo_write( int32_t value );

typedef struct{
	uint16_t	center_value;
	uint16_t	current_value;
	uint16_t	frame_length;
	uint8_t		active;
}servo_HandleTypeDef;

servo_HandleTypeDef SNWCT_SERVO;





#endif /* MOTOR_H_ */