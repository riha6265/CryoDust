//#include "sensors.h"

#include <avr/io.h>
#include "timeout.h"
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sd/ff.h"
#include "sd/integer.h"

#include "sensors.h"
#include "spi.h"
#include "SNWCTHWR1.h"
#include "servo.h"
#include <avr/interrupt.h>

ADT7320_HandleTypeDef hTEMP;
LIS3MDL_HandleTypeDef hMAG;
HSC_HandleTypeDef hPRESS;
HIH_HandleTypeDef hHUM;

extern volatile uint8_t fin;

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

int main(void)
{
	servo_power_off();
	GPIO_setOutput(RLED_GPIO,RLED_PIN);
	GPIO_setLow(RLED_GPIO,RLED_PIN);
	
	servo_goto_chamber(3);
	while(!fin){}
	//GPIO_setLow(RLED_GPIO,RLED_PIN);
	
	/*// reboot delay
	_delay_ms(200);
	UINT bw;
	f_mount(0, &FatFs);		// Give a work area to the FatFs module
	// open file
	fp = (FIL *)malloc(sizeof (FIL));
	if (f_open(fp, "test.txt", FA_WRITE | FA_CREATE_ALWAYS) == FR_OK) {	// Create a file
		char *text = "Hello World! SDCard support up and running!\r\n";
		f_write(fp, text, strlen(text), &bw);	// Write data to the file
		f_close(fp);// Close the file
	}
	
	
	while(1){
		
		hMAG.Init.CTRL_REG1_VAL = (LIS3MDL_OMXY_UHPM | LIS3MDL_ODR_80 );
		hMAG.Init.CTRL_REG2_VAL = (LIS3MDL_FS_4);
		hMAG.Init.CTRL_REG3_VAL = (LIS3MDL_OM_CONT);
		hMAG.Init.CTRL_REG4_VAL = (LIS3MDL_OMZ_UHPM);
		hMAG.Init.CTRL_REG5_VAL = (LIS3MDL_BDUen);
		hMAG.Init.INT_CFG_VAL = LIS3MDL_INT_CFG_DEFAULT;
		hMAG.CS_GPIO = CS_MAG_GPIO;
		hMAG.CS_GPIO_Pin = CS_MAG_GPIO_PIN;
		LIS3MDL_init(&hMAG);
		
		LIS3MDL_update_vals(&hMAG);
		
		// Temperature Sensor
		hTEMP.CS_GPIO = CS_TEMP_GPIO;
		hTEMP.CS_GPIO_Pin = CS_TEMP_GPIO_PIN;
		hTEMP.Init.ADT7320_CONFIG_VAL = ADT7320_CONFIG_DEFAULT;
		ADT7320_init(&hTEMP);
		
		ADT7320_update_temp(&hTEMP);
		
		// Pressure sensor
		hPRESS.CS_GPIO = CS_PRESS_GPIO;
		hPRESS.CS_GPIO_Pin = CS_PRESS_GPIO_PIN;
		HSC_init(&hPRESS);
		

		HSC_read(&hPRESS);
		
		// Humidity sensor
		hHUM.CS_GPIO = CS_HUM_GPIO;
		hHUM.CS_GPIO_Pin = CS_HUM_GPIO_PIN;
		HIH_init(&hHUM);
		
		HIH_read(&hHUM);
		
		uint16_t temp[] = {hTEMP.T}; //FIXME
		
		uint16_t mag[] = {hMAG.X,hMAG.Y,hMAG.Z};
			
		uint16_t hum[] = {hHUM.RH};
		
		uint16_t press[] = {hPRESS.P};
		
		// Allocates storage
		char * magData = (char*)malloc(40 * sizeof(char));
		// Prints formats the string and store to magData
		sprintf(magData, "M: %d, %d, %d\r\n", mag[0],mag[1],mag[2]);
		
		char * tempData = (char*)malloc(20 * sizeof(char));
		sprintf(tempData, "T: %d\r\n", temp[0]);
		
		char * pressData = (char*)malloc(20 * sizeof(char));
		sprintf(pressData, "P: %d\r\n", press[0]);
				
		char * humData = (char*)malloc(20 * sizeof(char));
		sprintf(humData, "H: %d\r\n", hum[0]);
								

		SPI_init(0,0,0);
		SPCR = 0; ///////////////////////// Must have after SPI sensor read
		// test append
		f_mount(0, &FatFs);
		if (f_open(fp, "test.txt", FA_WRITE | FA_OPEN_ALWAYS) == FR_OK) {	// Open existing or create new file
			if (f_lseek(fp, f_size(fp)) == FR_OK)
			{
				f_write(fp,magData,strlen(magData),&bw);	// Write data to the file
				f_write(fp,tempData,strlen(tempData),&bw);
				f_write(fp,pressData,strlen(pressData),&bw);
				f_write(fp,humData,strlen(humData),&bw);
			}
			f_close(fp);// Close the file
		
			_delay_ms(200); //Always delay to allow SPI lines to settle.
			
		}
	}*/
}