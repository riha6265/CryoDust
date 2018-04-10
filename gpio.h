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

typedef enum{
	GPIOA = 0,
	GPIOB,
	GPIOC,
	GPIOD
}GPIO_TypeDef;

#define PIN0    0x01
#define PIN1    0x02
#define PIN2    0x04
#define PIN3    0x08
#define PIN4    0x10
#define PIN5    0x20
#define PIN6    0x40
#define PIN7    0x80

void setOutput(GPIO_TypeDef GPIOX, uint8_t pins);
void setInput(GPIO_TypeDef GPIOX, uint8_t pins);
void setAnalog(GPIO_TypeDef GPIOX, uint8_t pins);
void toggle(GPIO_TypeDef GPIOX, uint8_t pins);
void setHigh(GPIO_TypeDef GPIOX, uint8_t pins);
void setLow(GPIO_TypeDef GPIOX, uint8_t pins);
uint8_t read(GPIO_TypeDef GPIOX, uint8_t pin)
uint16_t readADC(GPIO_TypeDef GPIOX, uint8_t pin);
