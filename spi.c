#include "spi.h"

void SPI_init(uint8_t polarity, uint8_t phase){
	//PRR0 = (1<<PRSPI);
	//Set MOSI and SCK output, all others input
	DDRB = (1<<DDB5)|(1<<DDB7)|(1<<DDB4);
	//Enable SPI, Master, set clock rate fck/16
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0)|(polarity<<CPOL)|(phase<<CPHA);
	
}

uint8_t SPI_transmitByte(char data){
	//Start transmission
	SPDR = data;
	//Wait for transmission complete
	while(!(SPSR & (1<<SPIF)))
	;
	return SPDR;
}

uint8_t * SPI_transmit(uint8_t * sentence, uint8_t size){
	volatile int i = 0;
	uint8_t read[size];
	for(i; i < size; i++){
		read[i] = SPI_transmitByte(sentence[i]);
	}
	return read;
}

uint8_t SPI_receive(){
	return SPDR;
}
