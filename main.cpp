/*
 * GccApplication2.cpp
 *
 * Created: 4/21/2018 2:59:36 AM
 * Author : Owen Lyke
 */ 

#include <avr/io.h>
#include "gpio.h"
#include "sensors.h"
#include "SNWCTHWR1.h"
#include "servo.h"
#include "serial.h"
#include "spi.h"

#include "timeout.h"
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include "sd/ff.h"
#include "sd/integer.h"

FATFS FatFs;	// FatFs work area
FIL *fp;		// fpe object

/*---------------------------------------------------------*/
/* User Provided RTC Function called by FatFs module       */
/* Used to provide a Timestamp for SDCard files and folders*/
DWORD get_fattime (void)
{
	// Returns current time packed into a DWORD variable
	return	  ((DWORD)(2018 - 1980) << 25)	// Year 2013
	| ((DWORD)4 << 21)				// Month 7
	| ((DWORD)20 << 16)				// Mday 28
	| ((DWORD)20 << 11)				// Hour 0..24
	| ((DWORD)30 << 5)				// Min 0
	| ((DWORD)0 >> 1);				// Sec 0
}


LIS3MDL_HandleTypeDef hMAG;
ADT7320_HandleTypeDef hTEMP;
HSC_HandleTypeDef hPRESS;
HIH_HandleTypeDef hHUM;


int main(void)
{
	_delay_ms(200); // reboot delay
	
	// ///////////////////// //
	//        Initial SD     //
	// ///////////////////// //
	UINT bw;
	f_mount(0, &FatFs);		// Give a work area to the FatFs module
	// open file
	fp = (FIL *)malloc(sizeof (FIL));
	if (f_open(fp, "test.txt", FA_WRITE | FA_CREATE_ALWAYS) == FR_OK) {	// Create a file
		char *text = "Hello World! SDCard support up and running!\r\n";
		f_write(fp, text, strlen(text), &bw);	// Write data to the file
		
		text = "Owen has to have his say too...\r\n";
		f_write(fp, text, strlen(text), &bw);	// Write data to the file
		f_close(fp);// Close the file
	}
	


	// ///////////////////// //
	//     Gen GPIO Setup    //
	// ///////////////////// //
	GPIO_setOutput(GPIOD, (RLED_PIN | GLED_PIN));													// Set LED pins as outputs
	GPIO_setInput(LS_ALL_GPIO, (LS1_PIN | LS2_PIN | LS3_PIN | LS4_PIN | LS5_PIN ), TRISTATE);		// Set limit switches as inputs
	
	GPIO_setOutput(SPI_ALL_GPIO,(MOSI_PIN |SCLK_PIN));												// Setup SPI pins
	GPIO_setInput(SPI_ALL_GPIO,MISO_PIN, TRISTATE);													//
	
	
	// Initialize serial
	SERIAL_init();
	
	// ///////////////////// //
	//  Sensor/peri initial  //
	// ///////////////////// //
	
	// Magnetometer
	hMAG.Init.CTRL_REG1_VAL = (LIS3MDL_OMXY_UHPM | LIS3MDL_ODR_80 );
	hMAG.Init.CTRL_REG2_VAL = (LIS3MDL_FS_4);
	hMAG.Init.CTRL_REG3_VAL = (LIS3MDL_OM_CONT);
	hMAG.Init.CTRL_REG4_VAL = (LIS3MDL_OMZ_UHPM);
	hMAG.Init.CTRL_REG5_VAL = (LIS3MDL_BDUen);
	hMAG.Init.INT_CFG_VAL = LIS3MDL_INT_CFG_DEFAULT;
	hMAG.CS_GPIO = CS_MAG_GPIO;
	hMAG.CS_GPIO_Pin = CS_MAG_GPIO_PIN;
	LIS3MDL_init(&hMAG);
	
	// Temperature Sensor
	hTEMP.CS_GPIO = CS_TEMP_GPIO;
	hTEMP.CS_GPIO_Pin = CS_TEMP_GPIO_PIN;
	hTEMP.Init.ADT7320_CONFIG_VAL = ADT7320_CONFIG_DEFAULT;
	ADT7320_init(&hTEMP);
	
	
	// Pressure sensor
	hPRESS.CS_GPIO = CS_PRESS_GPIO;
	hPRESS.CS_GPIO_Pin = CS_PRESS_GPIO_PIN;
	HSC_init(&hPRESS);
	
	// Humidity sensor
	hHUM.CS_GPIO = CS_HUM_GPIO;
	hHUM.CS_GPIO_Pin = CS_HUM_GPIO_PIN;
	HIH_init(&hHUM);
	
	// Servo
	servo_initialize();
	servo_power_on();											// Also a little demo
	servo_write(-20);
	for(volatile uint16_t indi = 0; indi < 65535; indi++){};
	servo_power_off();
	
	
	
	uint32_t reading_counts = 0;
	uint8_t mag_str[] = {'M',' ','[',0x00,0x00,',',0x00,0x00,',',0x00,0x00,']',' ',0x00,'\n'};
	uint8_t temp_str[] = {'T',' ','[',0x00,0x00,']',' ',0x00,'\n'};
	uint8_t press_str[] = {'P',' ','[',0x00,0x00,']',' ',0x00,'\n'};
	uint8_t hum_str[] = {'H',' ','[',0x00,0x00,']',' ',0x00,'\n'};
		
		GPIO_toggle(GLED_GPIO, GLED_PIN);
	
    /* Replace with your application code */
    while (1) 
    {
		// Echo limit switch states in the LED
		if(GPIO_read(LS_ALL_GPIO, LS1_PIN) == 0x01){ GPIO_setHigh(RLED_GPIO, RLED_PIN); }
		else{ GPIO_setLow(RLED_GPIO, RLED_PIN); }
		if(GPIO_read(LS_ALL_GPIO, LS2_PIN) == 0x01){ GPIO_setHigh(GLED_GPIO, GLED_PIN); }
		else{ GPIO_setLow(GLED_GPIO, GLED_PIN); }
			
		// Update the sensors:
		HIH_read(&hHUM);
		HSC_read(&hPRESS);
		ADT7320_update_temp(&hTEMP);
		LIS3MDL_update_vals(&hMAG);
		
		// Don't forget to set the SD back to required SPI settings!
		SPI_init(0,0,0);
		SPCR = 0;
		
		
		
		
		// Code seems to hang up after this bit...
		
		
		// Write to the SD card
		f_mount(0, &FatFs);
		if (f_open(fp, "test.txt", FA_WRITE | FA_OPEN_ALWAYS) == FR_OK) {	// Open existing or create new file
			if (f_lseek(fp, f_size(fp)) == FR_OK)
			{
				char *text2 = "This is a new line, appended to existing file!\r\n";
				f_write(fp,text2,2, &bw);	// Write data to the file
			}
			//f_close(fp);// Close the file
		}
		
		GPIO_toggle(GLED_GPIO, GLED_PIN);
		
		reading_counts++;
		
		
		for(volatile uint32_t indi = 0; indi < 65535; indi++){};	// Delay till next reading
    }
}

