/*
 * Stepper.c
 *
 *  Created on: 24.03.2023
 *      Author: Ming
 */

/* Includes */
#include "McuGPIO.h"
#include "McuWait.h"
#include "fsl_ctimer.h"
#include "fsl_common_arm.h"

#define CTIMER_CLK_FREQ CLOCK_GetFreq(kCLOCK_CoreSysClk)

/* Static Pins for usage throughout the file */
static McuGPIO_Handle_t Step1Pin;
static McuGPIO_Handle_t Dir1Pin;
static McuGPIO_Handle_t Step2Pin;
static McuGPIO_Handle_t Dir2Pin;
static McuGPIO_Handle_t EnPin;
static McuGPIO_Handle_t Ms1Pin;
static McuGPIO_Handle_t Ms2Pin;
static McuGPIO_Handle_t GreenLED;
static McuGPIO_Handle_t BlueLED;

/* Static Match Configuration for Steppers */
static ctimer_match_config_t matchConfig0;
static ctimer_match_config_t matchConfig1;
static ctimer_match_config_t matchConfig2;

/* Function Prototypes */
void Pins_Init(void);
void Stepper_Timer_Init(void);
void ctimer_match0_callback(uint32_t flags);
void ctimer_match1_callback(uint32_t flags);
void ctimer_match2_callback(uint32_t flags);

/* Array of function pointers for callback for each channel for Steppers */
ctimer_callback_t ctimer_callback_table[] = {
    ctimer_match0_callback,
	ctimer_match1_callback,
	ctimer_match2_callback,
	NULL, NULL, NULL, NULL, NULL};


/*
 * Initialize Pins and Timers for Stepper ussage.
 */
void Stepper_Init(void){
	Pins_Init();
	Stepper_Timer_Init();
}

/*
 * match0 callback
 */
void ctimer_match0_callback(uint32_t flags){
	McuGPIO_Toggle(GreenLED);
	matchConfig0.matchValue += MSEC_TO_COUNT(200, CTIMER_CLK_FREQ);
	CTIMER_SetupMatch(CTIMER0, kCTIMER_Match_0, &matchConfig0);
}

/*
 * match1 callback
 */
void ctimer_match1_callback(uint32_t flags){
	McuGPIO_Toggle(BlueLED);
	matchConfig1.matchValue += CTIMER_CLK_FREQ / 30;
	CTIMER_SetupMatch(CTIMER0, kCTIMER_Match_1, &matchConfig1);
}

/*
 * match2 callback
 */
void ctimer_match1_callback(uint32_t flags){

}

/*
 * Testing loop
 */
void Stepper_Dostuff(void){
	/* intialize Stepper Motor */
	    McuGPIO_SetLow(EnPin);
	    McuGPIO_SetHigh(Ms1Pin);
	    McuGPIO_SetHigh(Dir1Pin);
	while(1) {
	    	/* Stepping */
	    	McuGPIO_SetLow(Step1Pin);
	    	McuGPIO_SetLow(Step2Pin);
	    	McuWait_Waitms(3);
	    	McuGPIO_SetHigh(Step1Pin);
	    	McuGPIO_SetHigh(Step2Pin);
			McuWait_Waitms(3);
	    }
}

/*
 * nothing implemented yet
 */
void Stepper_Deinit(void) {}

void Pins_Init(void){
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

/* GPIO LED Green*/
	McuGPIO_GetDefaultConfig(&config);
	config.isInput = false;
	config.isHighOnInit = false;
	config.hw.gpio = GPIO;
	config.hw.port = 1;
	config.hw.pin = 0;
	config.hw.iocon = IOCON_INDEX_PIO1_0;
	GreenLED = McuGPIO_InitGPIO(&config);

/* GPIO LED Blue*/
	McuGPIO_GetDefaultConfig(&config);
	config.isInput = false;
	config.isHighOnInit = false;
	config.hw.gpio = GPIO;
	config.hw.port = 1;
	config.hw.pin = 1;
	config.hw.iocon = IOCON_INDEX_PIO1_1;
	BlueLED = McuGPIO_InitGPIO(&config);
}

void Stepper_Timer_Init(void){
	ctimer_config_t config;

	CTIMER_GetDefaultConfig(&config);
	CTIMER_Init(CTIMER0, &config);

	/* Configuration 0 */
	matchConfig0.enableCounterReset = false;
	matchConfig0.enableCounterStop  = false;
	matchConfig0.matchValue         = MSEC_TO_COUNT(150, CTIMER_CLK_FREQ);
	matchConfig0.outControl         = kCTIMER_Output_NoAction;
	matchConfig0.outPinInitState    = false;
	matchConfig0.enableInterrupt    = true;

	/* Configuration 1 */
	matchConfig1.enableCounterReset = false;
	matchConfig1.enableCounterStop  = false;
	matchConfig1.matchValue         = MSEC_TO_COUNT(100, CTIMER_CLK_FREQ);
	matchConfig1.outControl         = kCTIMER_Output_NoAction;
	matchConfig1.outPinInitState    = false;
	matchConfig1.enableInterrupt    = true;

	/* Configuration 2 */
	matchConfig2.enableCounterReset = false;
	matchConfig2.enableCounterStop  = false;
	matchConfig2.matchValue         = MSEC_TO_COUNT(200, CTIMER_CLK_FREQ);
	matchConfig2.outControl         = kCTIMER_Output_NoAction;
	matchConfig2.outPinInitState    = false;
	matchConfig2.enableInterrupt    = true;

	CTIMER_RegisterCallBack(CTIMER0, &ctimer_callback_table[0], kCTIMER_MultipleCallback);
	CTIMER_SetupMatch(CTIMER0, kCTIMER_Match_0, &matchConfig0);
	CTIMER_SetupMatch(CTIMER0, kCTIMER_Match_1, &matchConfig1);
	CTIMER_SetupMatch(CTIMER0, kCTIMER_Match_2, &matchConfig2);
	CTIMER_StartTimer(CTIMER0);
}
