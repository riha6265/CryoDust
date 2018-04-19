

#include <avr/io.h>
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



int main(void)
{
	// reboot delay
	_delay_ms(200);
	
	UINT bw;
	f_mount(0, &FatFs);		// Give a work area to the FatFs module 
	// open file
	fp = (FIL *)malloc(sizeof (FIL));
	if (f_open(fp, "file.txt", FA_WRITE | FA_CREATE_ALWAYS) == FR_OK) {	// Create a file 
		char *text = "Hello World! SDCard support up and running!\r\n";
		f_write(fp, text, strlen(text), &bw);	// Write data to the file 
		f_close(fp);// Close the file 
	}
	// test append
	if (f_open(fp, "file.txt", FA_WRITE | FA_OPEN_ALWAYS) == FR_OK) {	// Open existing or create new file
		if (f_lseek(fp, f_size(fp)) == FR_OK) 
			{
			char *text2 = "This is a new line, appended to existing file!\r\n";
			f_write(fp, text2, strlen(text2), &bw);	// Write data to the file
		}	
		f_close(fp);// Close the file		
	}	
}