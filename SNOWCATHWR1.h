/*
 * IncFile2.h
 *
 * Created: 4/12/2018 11:14:28 PM
 *  Author: Owen Lyke
 */ 


#ifndef SNOWCATHWR1_H_
#define SNOWCATHWR1_H_

// Color named LED output
#define RLED_GPIO GPIOD
#define RLED_PIN PIN4
#define GLED_GPIO GPIOD
#define GLED_PIN PIN5

// General output
#define OUT1_GPIO GPIOD
#define OUT1_PIN PIN4
#define OUT2_GPIO GPIOD
#define OUT2_PIN PIN5

// Limit switches
#define LS_ALL_GPIO GPIOC
#define LS1_GPIO GPIOC
#define LS1_PIN PIN0
#define LS2_GPIO GPIOC
#define LS2_PIN PIN1
#define LS3_GPIO GPIOC
#define LS3_PIN PIN2
#define LS4_GPIO GPIOC
#define LS4_PIN PIN3
#define LS5_GPIO GPIOC
#define LS5_PIN PIN4

// Analog inputs
#define VBATTH_GPIO GPIOA
#define VBATTH_PIN	PIN6

// Clock Info
#define FOSC	8000000




#endif /* SNOWCATHWR1_H_ */
