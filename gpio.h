/*
gpio.h

Owen Lyke
April 2018

Just a simple GPIO library for the ATmega1248p, for use in the CryoDust project
*/




#define OUTPUT  1
#define INPUT   0

#define HIGH    1
#define LOW     0

void setOutput(uint8_t DDRX, uint8_t pins);


// void pinMode(uint8_t num, uint8_t mode);
// void digitalWrite(uint8_t num, uint8_t value);
// void digitalRead(uint8_t num);










