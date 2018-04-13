/*
LIS3MDL.c

Header: LIS3MDL.h

Written by Owen Lyke, April 2018
Updated April 2018

ToDo:

*/


#include <avr/io.h>
#include "gpio.h"
//#include "spi.h"
#include "sensors.h"


//////////////////////////////////////////////////////
//				LIS3MDL Magnetometer				//
//////////////////////////////////////////////////////

// Functions
// ---------
void LIS3MDL_init(LIS3MDL_HandleTypeDef *hLIS3MDL)
{
	// SET SPI BUS SETTINGS!!!!!!!

	// CTRLx Registers
	LIS3MDL_write(hLIS3MDL, LIS3MDL_REG_CTRL_REG1, 	&(hLIS3MDL->Init.CTRL_REG1_VAL), 	1); 				// Set CTRL1 from user values
	LIS3MDL_write(hLIS3MDL, LIS3MDL_REG_CTRL_REG2, 	&(hLIS3MDL->Init.CTRL_REG2_VAL), 	1); 				// Set CTRL2 from user values
	LIS3MDL_write(hLIS3MDL, LIS3MDL_REG_CTRL_REG3, 	&(hLIS3MDL->Init.CTRL_REG3_VAL), 	1); 				// Set CTRL3 from user values
	LIS3MDL_write(hLIS3MDL, LIS3MDL_REG_CTRL_REG4, 	&(hLIS3MDL->Init.CTRL_REG4_VAL), 	1); 				// Set CTRL4 from user values
	LIS3MDL_write(hLIS3MDL, LIS3MDL_REG_CTRL_REG5, 	&(hLIS3MDL->Init.CTRL_REG5_VAL), 	1); 				// Set CTRL5 from user values
	LIS3MDL_write(hLIS3MDL, LIS3MDL_REG_INT_CFG, 	&(hLIS3MDL->Init.INT_CFG_VAL), 		1); 				// Set INT_CFG from user values
}

void LIS3MDL_read(LIS3MDL_HandleTypeDef *hLIS3MDL, uint8_t reg_add, uint8_t *data_out_ptr, uint8_t num_reads)
{
	GPIO_setLow(hLIS3MDL->CS_GPIO, hLIS3MDL->CS_GPIO_Pin);
	// SPI transfer initial byte ((reg_add & 0x3F) | 0xC0);																			// Set the CS line low to begin transmission
	for(uint8_t indi = 0; indi < num_reads; indi++)
	{
		// SPI transfer stuff (write)
	}
	GPIO_setHigh(hLIS3MDL->CS_GPIO, hLIS3MDL->CS_GPIO_Pin);																			// Set the CS line high to end transmission
}

void LIS3MDL_write(LIS3MDL_HandleTypeDef *hLIS3MDL, uint8_t reg_add, uint8_t *data_in_ptr, uint8_t num_writes)
{
	// This function uses blocking SPI because the transmissions will be relatively short, and it simplifies interrupt handling
	GPIO_setLow(hLIS3MDL->CS_GPIO, hLIS3MDL->CS_GPIO_Pin); 																			// Set the CS line low to begin transmission
	// SPI transfer initial byte (address and write code) setup byte = ((reg_add & 0x3F) | 0x40);
	for(uint8_t indi = 0; indi < num_writes; indi++)
	{
		// SPI transfer stuff (write)
	}
	GPIO_setHigh(hLIS3MDL->CS_GPIO, hLIS3MDL->CS_GPIO_Pin); 																		// Set the CS line high to end transmission
}


void	LIS3MDL_update_vals(LIS3MDL_HandleTypeDef *hLIS3MDL)
{
	const uint8_t num_reads = 9;
	uint8_t vals[num_reads];

	// SET SPI BUS SETTINGS!!!!!!!

	LIS3MDL_read(hLIS3MDL, LIS3MDL_REG_STATUS_REG, &vals[0], num_reads);

	hLIS3MDL->STATUS = vals[0];
	hLIS3MDL->X = ((vals[2] << 8) | vals[1]);
	hLIS3MDL->Y = ((vals[4] << 8) | vals[3]);
	hLIS3MDL->Z = ((vals[6] << 8) | vals[5]);
	hLIS3MDL->T = ((vals[8] << 8) | vals[7]);
}

void	LIS3MDL_get_guass(LIS3MDL_HandleTypeDef *hLIS3MDL, double * pdata)
{
	// This function is intended to provide the acceleration value in Gs (1g = 9.81 m/s2)
	// pdata should point to an array of three double types, used to represent {X,Y,Z} respectively
	uint8_t CR2 = LIS3MDL_CTRL_REG2_DEFAULT;
	uint8_t CR4 = LIS3MDL_CTRL_REG4_DEFAULT;

	// SET SPI BUS SETTINGS!!!!!!!

	LIS3MDL_read(hLIS3MDL, LIS3MDL_REG_CTRL_REG2, &CR2, 1);
	LIS3MDL_read(hLIS3MDL, LIS3MDL_REG_CTRL_REG4, &CR4, 1);

	LIS3MDL_update_vals(hLIS3MDL);

	double scale = 0.000122070312; // Scale for +/-4guass assuming full 16-bit
	switch( (CR2 & 0x60) >> 5 ){
		case 0x00 : scale *= 1; break;					// Unity scale for +/-4guass
		case 0x01 : scale *= 2; break;					// Double scale for +/-8guass
		case 0x02 : scale *= 3; break;					// Triple scale for +/-12guass
		case 0x03 : scale *= 4; break;					// Quadruple scale for +/-16guass
	}
	if( CR4 & 0x02 )
	{
		hLIS3MDL->X = (((hLIS3MDL->X & 0x00FF) << 8) | ((hLIS3MDL->X & 0xFF00) >> 8) );			// Flip the endianness of the variables because of this bit being set
		hLIS3MDL->Y = (((hLIS3MDL->Y & 0x00FF) << 8) | ((hLIS3MDL->Y & 0xFF00) >> 8) );			// Flip the endianness of the variables because of this bit being set
		hLIS3MDL->Z = (((hLIS3MDL->Z & 0x00FF) << 8) | ((hLIS3MDL->Z & 0xFF00) >> 8) );			// Flip the endianness of the variables because of this bit being set
	}

	*(pdata + 0) = (double)(hLIS3MDL->X * scale);												// Now actually scale the data
	*(pdata + 1) = (double)(hLIS3MDL->Y * scale);
	*(pdata + 2) = (double)(hLIS3MDL->Z * scale);
}







//////////////////////////////////////////////////////
//				ADT7320 Temperature Sensor			//
//////////////////////////////////////////////////////
void ADT7320_init(ADT7320_HandleTypeDef *hADT7320)
{
	
}

void ADT7320_read(ADT7320_HandleTypeDef *hADT7320, uint8_t reg_add, uint8_t *data_out_ptr, uint8_t num_reads)
{
	GPIO_setLow(hADT7320->CS_GPIO, hADT7320->CS_GPIO_Pin);
	// SPI transfer setup byte ((reg_addr & 0x7F) | 0x80)
	for(uint8_t indi = 0; indi < num_reads; indi++)
	{
		// SPI transfer data, put the result in the buffer
	}
	GPIO_setHigh(hADT7320->CS_GPIO, hADT7320->CS_GPIO_Pin);
}

void ATD7320_write(ADT7320_HandleTypeDef *hADT7320, uint8_t reg_add, uint8_t *data_in_ptr, uint8_t num_writes)
{
	GPIO_setLow(hADT7320->CS_GPIO, hADT7320->CS_GPIO_Pin);
	// SPI transfer setup byte ((reg_addr & 0x7F) | 0x00)
	for(uint8_t indi = 0; indi < num_writes; indi++)
	{
		// SPI transfer data, send data from the buffer
	}
	GPIO_setHigh(hADT7320->CS_GPIO, hADT7320->CS_GPIO_Pin);
}

void ADT7320_update_temp(ADT7320_HandleTypeDef *hADT7320)
{
	uint8_t buff[2];
	ADT7320_read(hADT7320, ADT7320_REG_TEMP_VAL, &buff[0], 2);
	hADT7320->T = ((buff[0] << 8) | buff[1]);
}

void ADT7320_get_degc(ADT7320_HandleTypeDef *hADT7320, double * pdata)
{
	uint8_t CFG;
	ADT7320_read(hADT7320, ADT7320_REG_CONFIG, &CFG, 1);
	ADT7320_update_temp(hADT7320);
	if( !(CFG & 0x80) )
	{
		if((hADT7320->T) & 0x8000)
		{
			*(pdata) = (double) 0.0625*(((hADT7320->T) >> 3) | (0xFFFF << (13))); // 13 bit mode, negative value
		}
		else
		{
			*(pdata) = (double) 0.0625*((hADT7320->T) >> 3); // 13 bit mode, positive value
		}	
	}
	else
	{
		*(pdata) = (double) 0.0078125 * (hADT7320->T);
	}
}







//////////////////////////////////////////////////////
//				HSCDRRN010MDSA3 Pressure Sensor		//
//////////////////////////////////////////////////////

void HSC_read(HSC_HandleTypeDef *hHSC)
{
	uint8_t buff[2];
	GPIO_setLow(hHSC->CS_GPIO, hHSC->CS_GPIO_Pin);
	// SPI transfer first byte and place contents in buffer	
	// SPI transfer second byte and place contents in buffer	
	GPIO_setHigh(hHSC->CS_GPIO, hHSC->CS_GPIO_Pin);
	hHSC->P = (((buff[0] << 8) | buff[1]) & 0x3FFF);
}

void HSC_get_pa(HSC_HandleTypeDef *hHSC, double *pdata)
{
	HSC_read(hHSC);
	*(pdata) = (double) (hHSC->P - 1638.4) * (2000 / 13107.2) - 1000; // Using the range of our sensor from -1000 pA to 1000 pA
}







//////////////////////////////////////////////////////
//				HIH7131-000-001 Humidity Sensor		//
//////////////////////////////////////////////////////

void HIH_read(HIH_HandleTypeDef *hHIH)
{	
	uint8_t buff[2];
	GPIO_setLow(hHIH->CS_GPIO, hHIH->CS_GPIO_Pin);
	// SPI transfer first byte and place contents in buffer
	// SPI transfer second byte and place contents in buffer
	GPIO_setHigh(hHIH->CS_GPIO, hHIH->CS_GPIO_Pin);
	hHIH->RH = (((buff[0] << 8) | buff[1]) & 0x3FFF);
}

void HIH_get_rh(HIH_HandleTypeDef *hHIH, double *pdata)
{
	HSC_read(hHIH);
	*(pdata) = (double) (hHIH->RH *100)/(16382) ; //
}






