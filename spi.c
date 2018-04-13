#include "spi.h"

void initSpi(void){
	//PRR0 = (1<<PRSPI);
	 //Set MOSI and SCK output, all others input 
	DDRB = (1<<DDB5)|(1<<DDB7)|(1<<DDB4);
	//Enable SPI, Master, set clock rate fck/16 
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}

uint8_t transmitByte(char data){
	//Start transmission
	SPDR = data;
	//Wait for transmission complete
	while(!(SPSR & (1<<SPIF)))
	;
	return SPDR;
}

uint8_t * transmit(uint8_t * sentence, uint8_t size){
	volatile int i = 0;
	uint8_t read[size];
	for(i; i < size; i++){
		read[i] = transmitByte(sentence[i]);
	}
	return read;
}

char receive(){
	return SPDR;
}