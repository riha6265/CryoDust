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
void GPIO_setInput(GPIO_TypeDef GPIOX, uint8_t pins, uint8_t pupd)
{
	// GPIOX must be the name of the GPIO port that you want to use (i.e. GPIOX as defined in the header file)
	// pins is a byte containing a '1' in the position of any pins that you want to set as inputs. Any bits with a zero will have no effect on the direction of those pins
	// - I suggest ORing any pin values together to create multi-pin sets that are easily read
	if((GPIOX >= GPIOA) && (GPIOX <= GPIOD)){
		*(&DDRA + 3*GPIOX) &= (~pins);					// Offset the address by 3 times the GPIO number, then AND equals with NOT pins to set new pins to input while leaving old pins intact
	}
	if(pupd == PULLUP)
	{
		*(&PORTA + 3*GPIOX) |= pins;
	}
	else
	{
		*(&PORTA + 3*GPIOX) &= (~pins);
	}
}

// Analog Input
void GPIO_setAnalog(GPIO_TypeDef GPIOX, uint8_t pins)
{
	GPIO_setInput(GPIOX, pins, TRISTATE);
}

void GPIO_toggle(GPIO_TypeDef GPIOX, uint8_t pins)
{
	*(&PINA + 3*GPIOX) = pins;
}

void GPIO_setHigh(GPIO_TypeDef GPIOX, uint8_t pins)
{
	//uint8_t res = (pins ^ *(&PORTA + 3*GPIOX));
	//*(&PINA +3*GPIOX) = (pins & res);
	*(&PORTA + 3*GPIOX) |= pins;
}

void GPIO_setLow(GPIO_TypeDef GPIOX, uint8_t pins)
{
	//*(&PINA + 3*GPIOX) = (pins & *(&PORTA + 3*GPIOX));
	*(&PORTA + 3*GPIOX) &= (~pins);
}

uint8_t GPIO_read(GPIO_TypeDef GPIOX, uint8_t pin)
{
	uint8_t shift = 0;
	switch(pin){
		case PIN0 : shift = 0; break;
		case PIN1 : shift = 1; break;
		case PIN2 : shift = 2; break;
		case PIN3 : shift = 3; break;
		case PIN4 : shift = 4; break;
		case PIN5 : shift = 5; break;
		case PIN6 : shift = 6; break;
		case PIN7 : shift = 7; break;
	}
	return (uint8_t)( (*(&PINA + 3*GPIOX) & pin) >> shift );
}

uint16_t GPIO_readADC(GPIO_TypeDef GPIOX, uint8_t pin)
{

	while(ADCSRA & (1<<ADSC)){}	// Wait for any current conversions to finish
		
	switch(pin)				// Select the appropriate pin
	{
		case PIN0 : ADMUX = (ADC_VREF_256 | ADC_RADJ | ADC_MUX_0);
		case PIN1 : ADMUX = (ADC_VREF_256 | ADC_RADJ | ADC_MUX_1);
		case PIN2 : ADMUX = (ADC_VREF_256 | ADC_RADJ | ADC_MUX_2);
		case PIN3 : ADMUX = (ADC_VREF_256 | ADC_RADJ | ADC_MUX_3);
		case PIN4 : ADMUX = (ADC_VREF_256 | ADC_RADJ | ADC_MUX_4);
		case PIN5 : ADMUX = (ADC_VREF_256 | ADC_RADJ | ADC_MUX_5);
		case PIN6 : ADMUX = (ADC_VREF_256 | ADC_RADJ | ADC_MUX_6);
		case PIN7 : ADMUX = (ADC_VREF_256 | ADC_RADJ | ADC_MUX_7);
	}
	
	ADCSRA = ( (1<<ADEN) |  (1<<ADSC) | 0x07);			// Enable the ADC, start convertion set prescaler to highest
	while(ADCSRA &  (1<<ADSC)){}					// Wait for any current conversions to finish
	ADCSRA |=  (1<<ADSC);							// Start another conversion
	while(ADCSRA &  (1<<ADSC)){}					// Wait for any current conversions to finish
	
	uint8_t low = ADCL;
	return ((ADCH << 8) | low);
}
