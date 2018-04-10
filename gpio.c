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

#define PIN0    0x01
#define PIN1    0x02
#define PIN2    0x04
#define PIN3    0x08
#define PIN4    0x10
#define PIN5    0x20
#define PIN6    0x40
#define PIN7    0x80



void pinMode(uint8_t num, uint8_t mode){
  
}
void digitalWrite(uint8_t num, uint8_t value);
void digitalRead(uint8_t num);



