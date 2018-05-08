#include "rtc.h"

void rtc_init(){
	GPIO_setOutput(CS_RTC_GPIO,CS_RTC_GPIO_PIN);
	SPI_init(1,1,3);
}

void rtc_set_time(timeStamp * snap){
	rtc_init();
	
	//Stop clock
	GPIO_setLow(CS_RTC_GPIO,CS_RTC_GPIO_PIN);
	SPI_transmitByte(0x80|0x10);
	SPI_transmitByte(0x81);
	GPIO_setHigh(CS_RTC_GPIO,CS_RTC_GPIO_PIN);
	
	//Format data to be stored
	uint8_t formatMin = ((uint8_t)((snap->min)%10)) | (((uint8_t)((snap->min)/10))<<4);
	uint8_t formatHr = ((uint8_t)((snap->hour)%10)) | (((uint8_t)((snap->hour)/10))<<4);
	uint8_t formatDay = ((uint8_t)((snap->day)%10)) | (((uint8_t)((snap->day)/10))<<4);
	uint8_t formatMth = ((uint8_t)((snap->month)%10)) | (((uint8_t)((snap->month)/10))<<4);
	uint8_t formatYr = ((uint8_t)((snap->year - 2000)%10)) | (((uint8_t)((snap->year - 2000)/10))<<4);
	
	//Store data
	GPIO_setLow(CS_RTC_GPIO,CS_RTC_GPIO_PIN);
	SPI_transmitByte(0x80|0x02);
	SPI_transmitByte(formatMin);
	SPI_transmitByte(formatHr);
	SPI_transmitByte(formatDay);
	SPI_transmitByte(formatMth);
	SPI_transmitByte(formatYr);
	GPIO_setHigh(CS_RTC_GPIO, CS_RTC_GPIO_PIN);
	
	//start counting again
	GPIO_setLow(CS_RTC_GPIO,CS_RTC_GPIO_PIN);
	SPI_transmitByte(0x80|0x10);
	SPI_transmitByte(0x01);
	GPIO_setHigh(CS_RTC_GPIO,CS_RTC_GPIO_PIN);
}

void rtc_update_time(timeStamp * snap){
	rtc_init();

	//stop timer
	GPIO_setLow(CS_RTC_GPIO,CS_RTC_GPIO_PIN);
	SPI_transmitByte(0x80|0x10);
	SPI_transmitByte(0x81);
	GPIO_setHigh(CS_RTC_GPIO,CS_RTC_GPIO_PIN);
	
	//Store data
	GPIO_setLow(CS_RTC_GPIO,CS_RTC_GPIO_PIN);
	SPI_transmitByte(0x02);
	uint8_t formatMin = SPI_transmitByte(0x00);
	uint8_t formatHr = SPI_transmitByte(0x00);
	uint8_t formatDay = SPI_transmitByte(0x00);
	uint8_t formatMth = SPI_transmitByte(0x00);
	uint8_t formatYr = SPI_transmitByte(0x00);
	GPIO_setHigh(CS_RTC_GPIO, CS_RTC_GPIO_PIN);
	
	//start counting again
	GPIO_setLow(CS_RTC_GPIO,CS_RTC_GPIO_PIN);
	SPI_transmitByte(0x80|0x10);
	SPI_transmitByte(0x01);
	GPIO_setHigh(CS_RTC_GPIO,CS_RTC_GPIO_PIN);
	
	//load the data	
	snap->min = ((uint8_t)(formatMin & 0x0F)) + (((uint8_t)((formatMin & 0xF0)>>4))*10);
	snap->hour = ((uint8_t)(formatHr & 0x0F)) + (((uint8_t)((formatHr & 0xF0)>>4))*10);
	snap->day = ((uint8_t)(formatDay & 0x0F)) + (((uint8_t)((formatDay & 0xF0)>>4))*10);
	snap->month = ((uint8_t)(formatMth & 0x0F)) + (((uint8_t)((formatMth & 0xF0)>>4))*10);
	snap->year = ((uint8_t)(formatYr & 0x0F)) + (((uint8_t)((formatYr & 0xF0)>>4))*10) + 2000;
	
	SPI_transmitByte(0x00);
}

void rtc_timedWake(timeStamp * snap){
	//stop timer
	GPIO_setLow(CS_RTC_GPIO,CS_RTC_GPIO_PIN);
	SPI_transmitByte(0x80|0x0F);
	SPI_transmitByte(0x00); //clear status bits
	SPI_transmitByte(0x95); //stop timer
	SPI_transmitByte(0x0F); //enable IRQ out on FOUT
	SPI_transmitByte(0xE4);	//enable alram interupt
	GPIO_setHigh(CS_RTC_GPIO,CS_RTC_GPIO_PIN);
	
	//Format data to be stored
	uint8_t formatMin = ((uint8_t)((snap->min)%10)) | (((uint8_t)((snap->min)/10))<<4);
	uint8_t formatHr = ((uint8_t)((snap->hour)%10)) | (((uint8_t)((snap->hour)/10))<<4);
	uint8_t formatDay = ((uint8_t)((snap->day)%10)) | (((uint8_t)((snap->day)/10))<<4);
	uint8_t formatMth = ((uint8_t)((snap->month)%10)) | (((uint8_t)((snap->month)/10))<<4);
	uint8_t formatYr = ((uint8_t)((snap->year - 2000)%10)) | (((uint8_t)((snap->year - 2000)/10))<<4);
		
	//Store data
	GPIO_setLow(CS_RTC_GPIO,CS_RTC_GPIO_PIN);
	SPI_transmitByte(0x80|0x0A);
	SPI_transmitByte(formatMin);
	SPI_transmitByte(formatHr);
	SPI_transmitByte(formatDay);
	SPI_transmitByte(formatMth);
	SPI_transmitByte(formatYr);
	GPIO_setHigh(CS_RTC_GPIO, CS_RTC_GPIO_PIN);
	
	//start counting again
	GPIO_setLow(CS_RTC_GPIO,CS_RTC_GPIO_PIN);
	SPI_transmitByte(0x80|0x10);
	SPI_transmitByte(0x15);
	GPIO_setHigh(CS_RTC_GPIO,CS_RTC_GPIO_PIN);
	
}

void powerdown(void){
	EICRA = 0; //enable the low level
	EIMSK = (1<<INT0);  //enable the interrupt
	
	set_sleep_mode(SLEEP_MODE_IDLE);
	cli();
	sleep_enable();
	sei();
	sleep_cpu();
	sleep_disable();
	sei();
	
	rtc_init();
	cli();
	GPIO_setLow(CS_RTC_GPIO,CS_RTC_GPIO_PIN);
	SPI_transmitByte(0x80|0x0F);
	SPI_transmitByte(0x00); //clear status bits
	GPIO_setHigh(CS_RTC_GPIO,CS_RTC_GPIO_PIN);
}

ISR(INT0_vect){
	rtc_init();
	cli();
	GPIO_setLow(CS_RTC_GPIO,CS_RTC_GPIO_PIN);
	SPI_transmitByte(0x80|0x0F);
	SPI_transmitByte(0x00); //clear status bits
	GPIO_setHigh(CS_RTC_GPIO,CS_RTC_GPIO_PIN);
}