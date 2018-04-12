#include "spi.h"

void initSpi(void){
	 //Set MOSI and SCK output, all others input 
	DDRB = (1<<DDB5)|(1<<DDB7);
	//Enable SPI, Master, set clock rate fck/16 
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}

void transmitByte(char data){
	//Start transmission
	SPDR = data;
	//Wait for transmission complete
	while(!(SPSR & (1<<SPIF)))
	;
}

void transmit(uint8_t * sentence, uint8_t size){
	volatile int i = 0;
	for(i; i < size; i++){
		transmitByte(*sentence);
	}
}

char receive(){
	return SPDR;
}