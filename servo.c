/*
 * servo.c
 *
 * Created: 4/21/2018 3:23:53 AM
 *  Author: Owen Lyke
 */ 

/*
 * CFile1.c
 *
 * Created: 4/20/2018 11:16:21 PM
 *  Author: Owen Lyke
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "servo.h"
#include "gpio.h"
#include "SNWCTHWR1.h"

void servo_initialize()
{
	// Set pins as outputs, set center value, set current value
	GPIO_setOutput(SERVO_PWM_GPIO, SERVO_PWM_PIN);
	GPIO_setOutput(PWRSW_SERVO_GPIO, PWRSW_SERVO_PIN);
	
	GPIO_setInput(LS_ALL_GPIO, (LS1_PIN | LS2_PIN | LS3_PIN | LS4_PIN | LS5_PIN), TRISTATE);
	
	SNWCT_SERVO.center_value = SERVO_CENTER_VAL;
	SNWCT_SERVO.current_value =  SERVO_CENTER_VAL;
	SNWCT_SERVO.frame_length = SERVO_FRAME_LENGTH;
}

void servo_power_on()
{
	// This is used to start the PWM generation and turn on the power to the servo

	
	//TCCR1A	// Already configured to normal mode
	TCCR1B |= ( (1<<WGM12) | (1<<CS11) ); // Setup the clear on compare match mode, using no prescaler
	
	TCNT1 = 0x00;					
	OCR1A = SNWCT_SERVO.frame_length - SNWCT_SERVO.current_value;	// Choose the low period as the first state
	GPIO_setLow(SERVO_PWM_GPIO, SERVO_PWM_PIN);
	
	TIMSK1|=(1<<OCIE1A);											// Enable interrupts
	sei();
	
	// Turn on the servo
	GPIO_setHigh(PWRSW_SERVO_GPIO, PWRSW_SERVO_PIN);
}

void servo_power_off()
{
	// Turn off the servo then stop interrupts on the timer
	GPIO_setLow(PWRSW_SERVO_GPIO, PWRSW_SERVO_PIN);
	TIMSK1|=(0<<OCIE1A);
}

void servo_write( int16_t value )
{
	SNWCT_SERVO.current_value = (SNWCT_SERVO.center_value + value);
}

void servo_goto_chamber(uint8_t chamber)
{
	// Try one direction for a while to see if you can get a hit, otherwise go the other direction
	uint8_t hit = 0;
	servo_write(-20);
	while(~hit)
	{
		
		if(GPIO_read(LS_ALL_GPIO, (LS1_PIN)))
		{
			GPIO_toggle(RLED_GPIO,RLED_PIN);
			hit = 1;
		}
		if(GPIO_read(LS_ALL_GPIO, (LS2_PIN)))
		{
			hit = 2;
		}
		if(GPIO_read(LS_ALL_GPIO, (LS3_PIN)))
		{
			hit = 3;
		}
		if(GPIO_read(LS_ALL_GPIO, (LS4_PIN)))
		{
			hit = 4;
		}
		if(GPIO_read(LS_ALL_GPIO, (LS5_PIN)))
		{
			hit = 5;
		}
	}
	servo_write(0);
}


ISR(TIMER1_COMPA_vect)
{
	// Insert my code here, all the hard stuff is already taken care of
	GPIO_toggle(SERVO_PWM_GPIO, SERVO_PWM_PIN);
	
	if(GPIO_read(SERVO_PWM_GPIO, SERVO_PWM_PIN))
	{
		OCR1A = SNWCT_SERVO.current_value;
	}
	else
	{
		OCR1A = SNWCT_SERVO.frame_length - SNWCT_SERVO.current_value;
	}
}
