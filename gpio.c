/*
gpio.c

Owen Lyke
April 2018

GPIO functionality for ATmega 1248p
*/

#include <avr/io.h>
#include "gpio.h"


// Digital Output
void GPIO_setOutput(GPIO_TypeDef GPIOX, uint8_t pins)
{
	// GPIOX must be the name of the GPIO port that you want to use (i.e. GPIOX as defined in the header file)
	// pins is a byte containing a '1' in the position of any pins that you want to set as outputs. Any bits with a zero will have no effect on the direction of those pins
	// - I suggest ORing any pin values together to create multi-pin sets that are easily read
	if((GPIOX >= GPIOA) && (GPIOX <= GPIOD)){
		*(&DDRA + 3*GPIOX) |= pins;					// Offset the address by 3 times the GPIO number, then OR equals with pins to set new pins to output while leaving old pins intact
	}
}

// Digital Input
void GPIO_setInput(GPIO_TypeDef GPIOX, uint8_t pins)
{
	// GPIOX must be the name of the GPIO port that you want to use (i.e. GPIOX as defined in the header file)
	// pins is a byte containing a '1' in the position of any pins that you want to set as inputs. Any bits with a zero will have no effect on the direction of those pins
	// - I suggest ORing any pin values together to create multi-pin sets that are easily read
	if((GPIOX >= GPIOA) && (GPIOX <= GPIOD)){
		*(&DDRA + 3*GPIOX) &= (~pins);					// Offset the address by 3 times the GPIO number, then AND equals with NOT pins to set new pins to input while leaving old pins intact
	}
}

// Analog Input
void GPIO_setAnalog(GPIO_TypeDef GPIOX, uint8_t pins)
{

}

void GPIO_toggle(GPIO_TypeDef GPIOX, uint8_t pins)
{
	*(&PINA + 3*GPIOX) = pins;
}

void GPIO_setHigh(GPIO_TypeDef GPIOX, uint8_t pins)
{
	uint8_t res = (pins ^ *(&PORTA + 3*GPIOX));
	*(&PINA +3*GPIOX) = (pins & res);
}

void GPIO_setLow(GPIO_TypeDef GPIOX, uint8_t pins)
{
	*(&PINA + 3*GPIOX) = (pins & *(&PORTA + 3*GPIOX));
}

uint8_t GPIO_read(GPIO_TypeDef GPIOX, uint8_t pin)
{
	return ( *(&PORTA + 3*GPIOX) & pin );
}

uint16_t GPIO_readADC(GPIO_TypeDef GPIOX, uint8_t pin)
{

}
