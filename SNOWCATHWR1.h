/*
 * IncFile2.h
 *
 * Created: 4/12/2018 11:14:28 PM
 *  Author: Owen Lyke
 */ 


#ifndef SNOWCATHWR1_H_
#define SNOWCATHWR1_H_

#include "gpio.h"

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

// Power Switching Pins
#define PWRSW_LQC_GPIO GPIOB
#define PWRSW_LQC_PIN PIN0
#define PWRSW_HQC_GPIO GPIOB
#define PWRSW_HQC_PIN PIN1
#define PWRSW_SERVO_GPIO GPIOC
#define PWRSW_SERVO_PIN PIN6

// SPI
#define SPI_ALL_GPIO GPIOB
#define MOSI_GPIO GPIOB
#define MOSI_PIN PIN5
#define MISO_GPIO GPIOB
#define MISO_PIN PIN6
#define SCLK_GPIO GPIOB
#define SCLK_PIN PIN7

// CS Pins
#define CS_MAG_GPIO GPIOA
#define CS_MAG_GPIO_PIN PIN1
#define CS_PRESS_GPIO GPIOA
#define CS_PRESS_GPIO_PIN PIN3
#define CS_HUM_GPIO GPIOA
#define CS_HUM_GPIO_PIN PIN2
#define CS_TEMP_GPIO GPIOA
#define CS_TEMP_GPIO_PIN PIN0

// Servo 
#define SERVO_PWM_GPIO GPIOC
#define SERVO_PWM_GPIO_PIN PIN5
#define SERVO_CENTER_VAL 4149			// Use this to define the "center" value of the servo... i.e. 
#define SERVO_FRAME_LENGTH 82988		// Value should be FOSC/F_frames, and should be greater than the center value


// Clock Info
#define FOSC	8000000




#endif /* SNOWCATHWR1_H_ */