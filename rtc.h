#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "spi.h"
#include "gpio.h"
#include "SNWCTHWR1.h"

#ifndef RTC_H_
#define RTC_H_

#ifdef __cplusplus
extern "C" {
	#endif
	typedef struct{
		uint8_t min;
		uint8_t hour;
		uint8_t day;
		uint8_t month;
		uint16_t year; //represent from 2018 on to 2099 
	}timeStamp;
	
	void rtc_init();
	void rtc_update_time(timeStamp *);
	void rtc_set_time(timeStamp *);
	void rtc_timedWake(uint8_t mins); //pass in minutes till wake 0-256
	void powerdown(); 
	#ifdef __cplusplus
}
#endif

#endif
