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
#include "CleanBeaches/Stepper.h"

#define CTIMER_CLK_FREQ CLOCK_GetFreq(kCLOCK_CoreSysClk)
#define MS_TICKS(x) (MSEC_TO_COUNT(x, CTIMER_CLK_FREQ))

/* Static Pins for usage throughout the file */
static McuGPIO_Handle_t StepInoutPin;
static McuGPIO_Handle_t DirInoutPin;
static McuGPIO_Handle_t StepLeftrightPin;
static McuGPIO_Handle_t DirLeftrightPin;
static McuGPIO_Handle_t EnPin;
static McuGPIO_Handle_t Ms1Pin;
static McuGPIO_Handle_t Ms2Pin;
static McuGPIO_Handle_t GreenLED;
static McuGPIO_Handle_t BlueLED;

/* Static Match Configuration for Steppers */
static ctimer_match_config_t matchConfig0;
static ctimer_match_config_t matchConfig1;
static ctimer_match_config_t matchConfig2;

/* Static data for stepper control */
typedef struct{
	direction_t dir;
	uint32_t stepsTodo;
	uint32_t delayms;
	uint32_t delayMatch;
} stepControl_t;

static stepControl_t stepControl[3];

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
	// default stepper setting
	stepControl[STEPPER_INOUT].delayMatch = MS_TICKS(6);
	stepControl[STEPPER_INOUT].delayms = 6;
	stepControl[STEPPER_INOUT].dir = IN;
	stepControl[STEPPER_INOUT].stepsTodo = 0;
	stepControl[STEPPER_LEFTRIGHT].delayMatch = MS_TICKS(6);
	stepControl[STEPPER_LEFTRIGHT].delayms = 6;
	stepControl[STEPPER_LEFTRIGHT].dir = LEFT;
	stepControl[STEPPER_LEFTRIGHT].stepsTodo = 0;
	stepControl[STEPPER_UPDOWN].delayMatch = MS_TICKS(20);
	stepControl[STEPPER_UPDOWN].delayms = 20;
	stepControl[STEPPER_UPDOWN].dir = UP;
	stepControl[STEPPER_UPDOWN].stepsTodo = 0;
	Stepper_Timer_Init();
}

/*
 * match0 callback for driving the arm in and out
 */
void ctimer_match0_callback(uint32_t flags){
	/* set next match with corresponding delay */
	matchConfig0.matchValue += stepControl[STEPPER_INOUT].delayMatch;
	CTIMER_SetupMatch(CTIMER0, kCTIMER_Match_0, &matchConfig0);
	/* set direction */
	McuGPIO_SetValue(DirInoutPin, stepControl[STEPPER_INOUT].dir);
	McuGPIO_SetValue(BlueLED, stepControl[STEPPER_INOUT].dir);
	/* Do steps if needed */
	if(stepControl[STEPPER_INOUT].stepsTodo){	// check for steps to do
		McuGPIO_SetHigh(StepInoutPin);	// single impulse
		McuGPIO_SetLow(GreenLED);
		McuGPIO_SetLow(StepInoutPin);
		McuGPIO_SetHigh(GreenLED);
		stepControl[STEPPER_INOUT].stepsTodo--;
	}
}

/*
 * match1 callback for turning the arm
 */
void ctimer_match1_callback(uint32_t flags){
//	McuGPIO_Toggle(BlueLED);
	matchConfig1.matchValue += CTIMER_CLK_FREQ / 30;
	CTIMER_SetupMatch(CTIMER0, kCTIMER_Match_1, &matchConfig1);
}

/*
 * match2 callback for raising and lowering the arm
 */
void ctimer_match2_callback(uint32_t flags){

}

/*
 * Testing loop
 */
void Stepper_Dostuff(stepper_t stepper, direction_t dir, uint32_t steps){
	stepControl[stepper].dir = dir;
	stepControl[stepper].stepsTodo = steps;
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
	StepInoutPin = McuGPIO_InitGPIO(&config);

/* GPIO Stepper 1 Direction */
	McuGPIO_GetDefaultConfig(&config);
	config.isInput = false;
	config.isHighOnInit = false;
	config.hw.gpio = GPIO;
	config.hw.port = 0;
	config.hw.pin = 13;
	config.hw.iocon = IOCON_INDEX_PIO0_13;
	DirInoutPin = McuGPIO_InitGPIO(&config);

/* GPIO Stepper 2 Step */
	McuGPIO_GetDefaultConfig(&config);
	config.isInput = false;
	config.isHighOnInit = false;
	config.hw.gpio = GPIO;
	config.hw.port = 0;
	config.hw.pin = 14;
	config.hw.iocon = IOCON_INDEX_PIO0_14;
	StepLeftrightPin = McuGPIO_InitGPIO(&config);

/* GPIO Stepper 2 Direction */
	McuGPIO_GetDefaultConfig(&config);
	config.isInput = false;
	config.isHighOnInit = false;
	config.hw.gpio = GPIO;
	config.hw.port = 0;
	config.hw.pin = 15;
	config.hw.iocon = IOCON_INDEX_PIO0_15;
	DirLeftrightPin = McuGPIO_InitGPIO(&config);

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

	/* Configuration 0 Stepper in/out */
	matchConfig0.enableCounterReset = false;
	matchConfig0.enableCounterStop  = false;
	matchConfig0.matchValue         = stepControl[STEPPER_INOUT].delayMatch;
	matchConfig0.outControl         = kCTIMER_Output_NoAction;
	matchConfig0.outPinInitState    = false;
	matchConfig0.enableInterrupt    = true;

	/* Configuration 1 Stepper left/right */
	matchConfig1.enableCounterReset = false;
	matchConfig1.enableCounterStop  = false;
	matchConfig1.matchValue         = stepControl[STEPPER_LEFTRIGHT].delayMatch;
	matchConfig1.outControl         = kCTIMER_Output_NoAction;
	matchConfig1.outPinInitState    = false;
	matchConfig1.enableInterrupt    = true;

	/* Configuration 2 Stepper up/down */
	matchConfig2.enableCounterReset = false;
	matchConfig2.enableCounterStop  = false;
	matchConfig2.matchValue         = stepControl[STEPPER_UPDOWN].delayMatch;
	matchConfig2.outControl         = kCTIMER_Output_NoAction;
	matchConfig2.outPinInitState    = false;
	matchConfig2.enableInterrupt    = true;

	CTIMER_RegisterCallBack(CTIMER0, &ctimer_callback_table[0], kCTIMER_MultipleCallback);
	CTIMER_SetupMatch(CTIMER0, kCTIMER_Match_0, &matchConfig0);
	CTIMER_SetupMatch(CTIMER0, kCTIMER_Match_1, &matchConfig1);
	CTIMER_SetupMatch(CTIMER0, kCTIMER_Match_2, &matchConfig2);
	CTIMER_StartTimer(CTIMER0);

	McuGPIO_SetLow(EnPin);
	McuGPIO_SetHigh(Ms1Pin);
}
