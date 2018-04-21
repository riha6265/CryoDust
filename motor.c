/*
 * CFile2.c
 *
 * Created: 4/12/2018 11:17:45 PM
 *  Author: Owen Lyke
 */ 

#include <avr/io.h>
#include "SNOWCATHWR1.h"
#include "motor.h"


void servo_initialize( void )
{
	// Setup GPIO for PWM, power switchng
	GPIO_setOutput(SERVO_PWM_GPIO, SERVO_PWM_GPIO_PIN);		
	GPIO_setOutput(PWRSW_SERVO_GPIO, PWRSW_SERVO_PIN);
	GPIO_setLow(SERVO_PWM_GPIO, SERVO_PWM_GPIO_PIN);		// Begins in low configuration
	GPIO_setLow(PWRSW_SERVO_GPIO, PWRSW_SERVO_PIN);			// Keeps the servo powered off
	
	SNWCT_SERVO.center_value = SERVO_CENTER_VAL;
	SNWCT_SERVO.current_value = SERVO_CENTER_VAL;
	SNWCT_SERVO.frame_length = SERVO_FRAME_LENGTH;			// Start in inactive state
	
	SNWCT_SERVO.active = 0;
	SNWCT_SERVO.current_value = SERVO_CENTER_VAL;
}

void servo_power_off( void )
{
	SNWCT_SERVO.active = 0;
	GPIO_setLow(PWRSW_SERVO_GPIO, PWRSW_SERVO_PIN);			// Removes power from the servo
	
	TIMSK1 = 0b00000000;	// Disables output compare interrupt A on timer 1
	TCNT1 = 0x00;			// Reset the timer to zero (This one is actually justified so that other functions can use the timer)
}

void servo_power_on( void )
{
	SNWCT_SERVO.active = 1;
	GPIO_setHigh(PWRSW_SERVO_GPIO, PWRSW_SERVO_PIN);			// Feeds power to the servo
	
	// Setup the timer to generate interrupts based on the PWM that I want
	TCCR1A = 0b00000000;  // 0000: sets the output compare pins to be disconnected - I will use software interrupts only, 00: don't care, 00: normal mode (no waveform generation)
	TCCR1B = 0b00000001;	// 0: input noise cancel disabled, 00: don't care, 00: normal mode (no waveform), 001: no prescaling (allows for a 50Hz output with still good resolution) (also starts the timer I think);
	TCCR1C = 0b00000000;	// No force compare
	
	// Set output compare registers (only need channel A on timer 1)
	OCR1A = SNWCT_SERVO.frame_length - SNWCT_SERVO.center_value; // Prepares the interrupt to occur just in time to
	
	SREG |= 0b10000000;
	TCNT1 = 0x00;			// Reset the timer to zero (I caved and am just reseting the timer to zero to avoid having to do basic maths)
	TIMSK1 = 0x02;	// Enables output compare interrupt A on timer 1
}

void servo_write(int32_t value)
{
	SNWCT_SERVO.current_value = SERVO_CENTER_VAL + value;
}


ISR(TIMER1_COMPA_vect){
	//// Execute my code
	//if(SNWCT_SERVO.active)
	//{
		////if(GPIO_read(SERVO_PWM_GPIO, SERVO_PWM_GPIO_PIN))
		//if(0)
		//{
			//// Pin is high, so the next period should be low
			////GPIO_setLow(SERVO_PWM_GPIO, SERVO_PWM_GPIO_PIN);
			//OCR1A = SNWCT_SERVO.frame_length - SNWCT_SERVO.current_value;
			//TCNT1 = 0x00;			// Reset the timer to zero (I caved and am just reseting the timer to zero to avoid having to do basic maths)
		//}
		//else
		//{
			//// Pin is low, so next period should be high
			////GPIO_setHigh(SERVO_PWM_GPIO, SERVO_PWM_GPIO_PIN);
			//OCR1A = SNWCT_SERVO.current_value;
			//TCNT1 = 0x00;			// Reset the timer to zero (I caved and am just reseting the timer to zero to avoid having to do basic maths)
		//}
//
		//TIMSK1 = 0b00000010;	// Enables output compare interrupt A on timer 1
	//}
	//else
	//{
		//TIMSK1 = 0b00000000;	// Disables the interrupt for efficiency
	//}
	//
	//TIFR1 |= 0b00000010;	// Clear the flag
	
	
	// We've narrowed the possibilities down to between the ISR occurring and these leds toggling. The system reboots
	
	GPIO_setLow(GPIOD, RLED_PIN);
	for(volatile long long int i = 0; i < 5000; i++);
	GPIO_setHigh(GPIOD, RLED_PIN);
	for(volatile long long int i = 0; i < 5000; i++);
}