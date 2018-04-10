/*
gpio.c

Owen Lyke
April 2018

GPIO functionality for ATmega 1248p
*/

// Register definitions
#define MCUCR 0x55

#define PORTA 	0x22
#define PORTB 	0x25
#define PORTC 	0x28
#define PORTD 	0x2B

#define PINA 	0x20
#define PINB 	0x23
#define PINC	0x26
#define PIND	0x29

#define DDRA	0x21
#define DDRB	0x24
#define DDRC	0x27
#define DDRD	0x28




