/*
 * GccApplication5.c
 *
 * Created: 4/12/2018 9:09:54 AM
 * Author : Owen Lyke
 */ 

#include <avr/io.h>
#include "gpio.h"
#include "SNOWCATHWR1.h"
#include "sensors.h"
#include "rtc.h"
#include "serial.h"
#include "spi.h"
#include "motor.h"

#include <avr/interrupt.h>


volatile uint8_t dont_optimize_me = 0;
uint8_t mag_str[] = {'M',' ','[',0x00,0x00,',',0x00,0x00,',',0x00,0x00,']',0x00,'\n'};
uint8_t temp_str[] = {'T',' ','[',0x00,0x00,']',0x00,'\n'};
uint8_t press_str[] = {'P',' ','[',0x00,0x00,']',0x00,'\n'};
uint8_t hum_str[] = {'H',' ','[',0x00,0x00,']',0x00,'\n'};
	

LIS3MDL_HandleTypeDef hMAG;
ADT7320_HandleTypeDef hTEMP;
HSC_HandleTypeDef hPRESS;
HIH_HandleTypeDef hHUM;


int main(void)
{
	GPIO_setOutput(GPIOD, (RLED_PIN | GLED_PIN));	// Set LED pins as outputs
	GPIO_setInput(LS_ALL_GPIO, (LS1_PIN | LS2_PIN | LS3_PIN | LS4_PIN | LS5_PIN ), TRISTATE);
	
	GPIO_setOutput(SPI_ALL_GPIO,(MOSI_PIN |SCLK_PIN));
	GPIO_setInput(SPI_ALL_GPIO,MISO_PIN, TRISTATE);
	
	SERIAL_init();
	SPI_init(1, 1);
	
	
	// Initialize sensors
	//GPIO_setOutput(CS_MAG_GPIO, CS_MAG_PIN);
	
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
	
	// Initialize the servo, then turn it on
	servo_initialize();
	servo_power_on();
	
	GPIO_setLow(SERVO_PWM_GPIO, SERVO_PWM_GPIO_PIN);
	
	while(1)
	{
		
		
	}
	
    /* Replace with your application code */
    while (1) 
    {	
		// Echo limit switch states in the LED
		if(GPIO_read(LS_ALL_GPIO, LS1_PIN) == 0x01){ GPIO_setHigh(RLED_GPIO, RLED_PIN); }
		else{ GPIO_setLow(RLED_GPIO, RLED_PIN); }
		if(GPIO_read(LS_ALL_GPIO, LS2_PIN) == 0x01){ GPIO_setHigh(GLED_GPIO, GLED_PIN); }
		else{ GPIO_setLow(GLED_GPIO, GLED_PIN); }
			
		// Try out the serial buffer
		if(SERIAL_available() > 10)
		{
			uint8_t num = SERIAL_available();
			for(uint8_t indi = 0; indi < num; indi++)
			{
				SERIAL_transmit(SERIAL_read_buff());
			}	
			
			//SPI_transmit(&hello[0], 7);
			//SPI_transmitByte(0xAA);
		}
		
		//void servo_power_on( void );
		uint16_t del = 5000;
		for(uint16_t indi = 0; indi < del; indi++)
		{
			dont_optimize_me++;
		}
		void servo_power_off( void );
		for(uint16_t indi = 0; indi < del; indi++)
		{
			dont_optimize_me++;
		}
		//GPIO_toggle(PWRSW_SERVO_GPIO, PWRSW_SERVO_PIN);
		
		
		
		// // Update the sensors:
		HIH_read(&hHUM);
		HSC_read(&hPRESS);
		ADT7320_update_temp(&hTEMP);
		LIS3MDL_update_vals(&hMAG);
		
		
		mag_str[3] = ((hMAG.X & 0xFF00) >> 8);
		mag_str[4] = (hMAG.X & 0x00FF);
		mag_str[6] = ((hMAG.Y & 0xFF00) >> 8);
		mag_str[7] = (hMAG.Y & 0x00FF);
		mag_str[9] = ((hMAG.Z & 0xFF00) >> 8);
		mag_str[10] = (hMAG.Z & 0x00FF);
		
		temp_str[3] = ((hTEMP.T & 0xFF00 ) >> 8);
		temp_str[4] = ((hTEMP.T & 0x00FF ) >> 0);
		
		hum_str[3] = ((hHUM.RH & 0xFF00 ) >> 8);
		hum_str[4] = ((hHUM.RH & 0x00FF ) >> 0);
		
		press_str[3] = ((hPRESS.P & 0xFF00 ) >> 8);
		press_str[4] = ((hPRESS.P & 0x00FF ) >> 0);
		
		
		for(uint8_t indi = 0; indi < 14; indi++)
		{
			SERIAL_transmit(mag_str[indi]);
		}
		
		for(uint8_t indi = 0; indi < 8; indi++)
		{
			SERIAL_transmit(temp_str[indi]);
		}
		
		for(uint8_t indi = 0; indi < 8; indi++)
		{
			SERIAL_transmit(press_str[indi]);
		}
		
		for(uint8_t indi = 0; indi < 8; indi++)
		{
			SERIAL_transmit(hum_str[indi]);
		}
		
		
		
		
		
		
		
    }
}

