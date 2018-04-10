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
#define DDRD	0x2A



// Digital Output
void setOutput(GPIO_TypeDef GPIOX, uint8_t pins)
{ 
	// GPIOX must be the name of the GPIO port that you want to use (i.e. GPIOX as defined in the header file)
	// pins is a byte containing a '1' in the position of any pins that you want to set as outputs. Any bits with a zero will have no effect on the direction of those pins
	// - I suggest ORing any pin values together to create multi-pin sets that are easily read
	if((GPIOX >= GPIOA) && (GPIOX <= GPIOD)){
		*(DDRA + 3*GPIOX) |= pins;					// Offset the address by 3 times the GPIO number, then OR equals with pins to set new pins to output while leaving old pins intact
	}
}

// Digital Input
void setInput(GPIO_TypeDef GPIOX, uint8_t pins)
{ 
	// GPIOX must be the name of the GPIO port that you want to use (i.e. GPIOX as defined in the header file)
	// pins is a byte containing a '1' in the position of any pins that you want to set as inputs. Any bits with a zero will have no effect on the direction of those pins
	// - I suggest ORing any pin values together to create multi-pin sets that are easily read			
	if((GPIOX >= GPIOA) && (GPIOX <= GPIOD)){
		*(DDRA + 3*GPIOX) &= (~pins);					// Offset the address by 3 times the GPIO number, then AND equals with NOT pins to set new pins to input while leaving old pins intact
	}
}

// Analog Input
void setAnalog(GPIO_TypeDef GPIOX, uint8_t pins)
{

}

void toggle(GPIO_TypeDef GPIOX, uint8_t pins)
{
	*(PINA + 3*GPIOX) |= pins;
}

void setHigh(GPIO_TypeDef GPIOX, uint8_t pins)
{
	uint8_t res = (pins ^ *(PORTA + 3*GPIOX));
	*(PINA + 3*GPIOX) |= (pins & res);
}

void setLow(GPIO_TypeDef GPIOX, uint8_t pins)
{
	*(PINA + 3*GPIOX) |= (pins & (PortA + 3*GPIOX));
}

uint8_t read(GPIO_TypeDef GPIOX, uint8_t pin)

uint8_t readADC(GPIO_TypeDef GPIOX, uint8_t pin)
{

}


