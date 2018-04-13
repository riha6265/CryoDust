/*
 * IncFile1.h
 *
 * Created: 4/12/2018 9:10:17 AM
 *  Author: Owen Lyke
 */ 



#ifndef GPIO_H_
#define GPIO_H_

#define OUTPUT  1
#define INPUT   0

#define PULLUP		1
#define TRISTATE	0

#define HIGH    1
#define LOW     0

#define ADC_VREF_256	0xC0
#define ADC_RADJ		0x00
#define ADC_LADJ		0x20
#define ADC_MUX_0		0x00
#define ADC_MUX_1		0x01
#define ADC_MUX_2		0x02
#define ADC_MUX_3		0x03
#define ADC_MUX_4		0x04
#define ADC_MUX_5		0x05
#define ADC_MUX_6		0x06
#define ADC_MUX_7		0x07

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

void GPIO_setOutput(GPIO_TypeDef GPIOX, uint8_t pins);
void GPIO_setInput(GPIO_TypeDef GPIOX, uint8_t pins,uint8_t pupd);
void GPIO_setAnalog(GPIO_TypeDef GPIOX, uint8_t pins);
void GPIO_toggle(GPIO_TypeDef GPIOX, uint8_t pins);
void GPIO_setHigh(GPIO_TypeDef GPIOX, uint8_t pins);
void GPIO_setLow(GPIO_TypeDef GPIOX, uint8_t pins);
uint8_t GPIO_read(GPIO_TypeDef GPIOX, uint8_t pin);
uint16_t GPIO_readADC(GPIO_TypeDef GPIOX, uint8_t pin);

#endif /* GPIO_H_ */
