/*
 * Stepper.c
 *
 *  Created on: 24.03.2023
 *      Author: Ming
 */

/* Includes */
#include "McuGPIO.h"
#include "McuWait.h"

/* Static Pins for usage throughout the file */
static McuGPIO_Handle_t Step1Pin;
static McuGPIO_Handle_t Dir1Pin;
static McuGPIO_Handle_t Step2Pin;
static McuGPIO_Handle_t Dir2Pin;
static McuGPIO_Handle_t EnPin;
static McuGPIO_Handle_t Ms1Pin;
static McuGPIO_Handle_t Ms2Pin;


/*
 * Initialize Pins and Timers for Stepper ussage.
 */
void Stepper_Init(void){
/* GPIO Configuration */
	McuGPIO_Config_t config;

/* GPIO Stepper 1 Step */
	McuGPIO_GetDefaultConfig(&config);
	config.isInput = false;
	config.isHighOnInit = false;
	config.hw.gpio = GPIO;
	config.hw.port = 0;
	config.hw.pin = 11;
	config.hw.iocon = IOCON_INDEX_PIO0_11;
	Step1Pin = McuGPIO_InitGPIO(&config);

/* GPIO Stepper 1 Direction */
	McuGPIO_GetDefaultConfig(&config);
	config.isInput = false;
	config.isHighOnInit = false;
	config.hw.gpio = GPIO;
	config.hw.port = 0;
	config.hw.pin = 13;
	config.hw.iocon = IOCON_INDEX_PIO0_13;
	Dir1Pin = McuGPIO_InitGPIO(&config);

/* GPIO Stepper 2 Step */
	McuGPIO_GetDefaultConfig(&config);
	config.isInput = false;
	config.isHighOnInit = false;
	config.hw.gpio = GPIO;
	config.hw.port = 0;
	config.hw.pin = 14;
	config.hw.iocon = IOCON_INDEX_PIO0_14;
	Step2Pin = McuGPIO_InitGPIO(&config);

/* GPIO Stepper 2 Direction */
	McuGPIO_GetDefaultConfig(&config);
	config.isInput = false;
	config.isHighOnInit = false;
	config.hw.gpio = GPIO;
	config.hw.port = 0;
	config.hw.pin = 15;
	config.hw.iocon = IOCON_INDEX_PIO0_15;
	Dir2Pin = McuGPIO_InitGPIO(&config);

/* GPIO Stepper Enable */
	McuGPIO_GetDefaultConfig(&config);
	config.isInput = false;
	config.isHighOnInit = true;
	config.hw.gpio = GPIO;
	config.hw.port = 0;
	config.hw.pin = 8;
	config.hw.iocon = IOCON_INDEX_PIO0_8;
	EnPin = McuGPIO_InitGPIO(&config);

/* GPIO Stepper MS1 */
	McuGPIO_GetDefaultConfig(&config);
	config.isInput = false;
	config.isHighOnInit = false;
	config.hw.gpio = GPIO;
	config.hw.port = 0;
	config.hw.pin = 9;
	config.hw.iocon = IOCON_INDEX_PIO0_9;
	Ms1Pin = McuGPIO_InitGPIO(&config);

/* GPIO Stepper MS2 */
	McuGPIO_GetDefaultConfig(&config);
	config.isInput = false;
	config.isHighOnInit = false;
	config.hw.gpio = GPIO;
	config.hw.port = 0;
	config.hw.pin = 10;
	config.hw.iocon = IOCON_INDEX_PIO0_10;
	Ms2Pin = McuGPIO_InitGPIO(&config);
}

void Stepper_Dostuff(void){
	/* intialize Stepper Motor */
	    McuGPIO_SetLow(EnPin);
	    McuGPIO_SetHigh(Ms1Pin);
	while(1) {
	    	/* Stepping */
	    	McuGPIO_SetLow(Step1Pin);
	    	McuGPIO_SetLow(Step2Pin);
	    	McuWait_Waitms(10);
	    	McuGPIO_SetHigh(Step1Pin);
	    	McuGPIO_SetHigh(Step2Pin);
			McuWait_Waitms(10);
	    }
}

/*
 * nothing implemented yet
 */
void Stepper_Deinit(void) {}
