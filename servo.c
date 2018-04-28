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

volatile uint8_t dest;
volatile int16_t dir;

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
	//sei();
	
	// Turn on the servo
	GPIO_setHigh(PWRSW_SERVO_GPIO, PWRSW_SERVO_PIN);
}

void servo_power_off()
{
	// Turn off the servo then stop interrupts on the timer
	GPIO_setLow(PWRSW_SERVO_GPIO, PWRSW_SERVO_PIN);
	TIMSK1|=(0<<OCIE1A);
	cli();
}

void servo_write( int16_t value )
{
	SNWCT_SERVO.current_value = (SNWCT_SERVO.center_value + value);
}

void servo_goto_chamber(uint8_t chamber)
{
	dest = chamber;
	dir = -40;
	fin = 0;
	GPIO_setInput(GPIOC,(GPIN0|GPIN1|GPIN2|GPIN3|GPIN4|GPIN5),TRISTATE);
	PORTC = 0;
	//JTAGEN 
	MCUCR |= (1<<PUD) |(1<<JTD);
	MCUCR |= (1<<JTD);
	MCUCR |= (1<<JTD);
	PCICR = (1<<PCIE2);
	PCMSK2 = (1<<PCINT16)|(1<<PCINT17)|(1<<PCINT18)|(1<<PCINT19)|(1<<PCINT20);

	servo_initialize();
	servo_power_on();
	servo_write(dir);
	sei();
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


ISR(PCINT2_vect)
{
	GPIO_setHigh(RLED_GPIO,RLED_PIN);
	uint8_t read = PINC&0x1F;
	switch(read)
	{
		case (1<<PORTC0):
			if(dest>0){
				dir=40;
				servo_write(dir);
			}else{
				servo_write(0);
				servo_power_off();
				fin =1;
			}
			break;
		case (1<<PORTC1):
			if(dest<1){
				dir=-40;
				servo_write(dir);
			}else if(dest>1){
				dir=40;
				servo_write(dir);
			}else {
				servo_write(0);
				servo_power_off();
				fin =1;
			}
			break;
		case (1<<PORTC2):
			if(dest<2){
				dir=-40;
				servo_write(dir);
			}else if(dest>2){
				dir=40;
				servo_write(dir);
			}else {
				servo_write(0);
				servo_power_off();
				fin =1;
			}
			break;
		case (1<<PORTC3):
			if(dest<3){
				dir=-40;
				servo_write(dir);
			}else if(dest>3){
				dir=40;
				servo_write(dir);
			}else {
				servo_write(0);
				servo_power_off();
				fin =1;
			}
			break;
		case (1<<PORTC4):
			if(dest<4){
				dir=-40;
				servo_write(dir);
			}else {
				servo_write(0);
				servo_power_off();
				fin =1;
			}
			break;
		default:
			servo_write(dir);
			break;					
	}
	GPIO_setLow(RLED_GPIO,RLED_PIN);
}
