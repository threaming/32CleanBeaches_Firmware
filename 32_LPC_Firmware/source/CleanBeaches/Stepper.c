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

/* default settings */
#define STEPPER_INOUT_DELAYMS (6)
#define STEPPER_LEFTRIGHT_DELAYMS (15)
#define STEPPER_UPDOWN_DELAYMS (10)
#define STEPPER_BACKFORTH_DELAYMS (4)		// 4ms

/* Static Pins for usage throughout the file */
static McuGPIO_Handle_t EnPin;
static McuGPIO_Handle_t Ms1Pin;
static McuGPIO_Handle_t Ms2Pin;
static McuGPIO_Handle_t GreenLED;
static McuGPIO_Handle_t BlueLED;

/* Static Match Configuration for Steppers */
static ctimer_match_config_t matchConfig0;
static ctimer_match_config_t matchConfig1;
static ctimer_match_config_t matchConfig2;
static ctimer_match_config_t matchConfig3;

/* Static data for stepper control */
typedef struct{
	direction_t dir;
	direction_t homeDir;
	uint32_t stepsTodo;
	int32_t currentPos;
	uint32_t delayms;
	uint32_t delayMatch;
	bool homing;
	McuGPIO_Handle_t StepPin;
	McuGPIO_Handle_t DirPin;
	McuGPIO_Handle_t StopPin;
} stepControl_t;

static stepControl_t stepCtrl[4];

/* Function Prototypes */
void Pins_Init(void);
void Stepper_Timer_Init(void);
void ctimer_match0_callback(uint32_t flags);
void ctimer_match1_callback(uint32_t flags);
void ctimer_match2_callback(uint32_t flags);
void ctimer_match3_callback(uint32_t flags);

/* Array of function pointers for callback for each channel for Steppers */
ctimer_callback_t ctimer_callback_table[] = {
    ctimer_match0_callback,
	ctimer_match1_callback,
	ctimer_match2_callback,
	ctimer_match3_callback,
	NULL, NULL, NULL, NULL};

/*
 * Stepper Control function to be called every couple of milliseconds, defined
 * by the timer (and ultimately by the stepCtrl[].delayMatch)
 */
void StepCtrl(stepper_t stepper){
	/* set direction */
	McuGPIO_SetValue(stepCtrl[stepper].DirPin, stepCtrl[stepper].dir);
	McuGPIO_SetValue(BlueLED, stepCtrl[stepper].dir);

	/* at the endstop */
	if(McuGPIO_IsHigh(stepCtrl[stepper].StopPin) && (stepCtrl[stepper].dir == stepCtrl[stepper].homeDir)){
		stepCtrl[stepper].homing = false;
		stepCtrl[stepper].currentPos = 0;
		stepCtrl[stepper].stepsTodo = 0;
	}

	/* Do steps if needed */
	if(stepCtrl[stepper].stepsTodo){	// check for steps to do
		McuGPIO_SetHigh(stepCtrl[stepper].StepPin);	// single impulse
		McuGPIO_SetLow(GreenLED);	// debug
		McuGPIO_SetLow(stepCtrl[stepper].StepPin);
		McuGPIO_SetHigh(GreenLED);	// debug
		stepCtrl[stepper].stepsTodo--;
		if(stepCtrl[stepper].dir){
			stepCtrl[stepper].currentPos++;
		}
		else{
			stepCtrl[stepper].currentPos--;
		}
	}

	/* Homing sequence */
	if(stepCtrl[stepper].homing){
		McuGPIO_SetHigh(stepCtrl[stepper].StepPin);	// single impulse
		McuGPIO_SetLow(GreenLED);	// debug
		McuGPIO_SetLow(stepCtrl[stepper].StepPin);
		McuGPIO_SetHigh(GreenLED);	// debug
	}
}

/*
 * match0 callback for driving the arm in and out
 */
void ctimer_match0_callback(uint32_t flags){
	/* set next match with corresponding delay */
	matchConfig0.matchValue += stepCtrl[STEPPER_INOUT].delayMatch;
	CTIMER_SetupMatch(CTIMER0, kCTIMER_Match_0, &matchConfig0);
	/* Control the stepper */
	StepCtrl(STEPPER_INOUT);
}

/*
 * match1 callback for turning the arm
 */
void ctimer_match1_callback(uint32_t flags){
	/* set next match with corresponding delay */
	matchConfig1.matchValue += stepCtrl[STEPPER_LEFTRIGHT].delayMatch;
	CTIMER_SetupMatch(CTIMER0, kCTIMER_Match_1, &matchConfig1);
	/* Control the stepper */
	StepCtrl(STEPPER_LEFTRIGHT);
}

/*
 * match2 callback for raising and lowering the arm
 */
void ctimer_match2_callback(uint32_t flags){
	/* set next match within corresponding delay */
	matchConfig2.matchValue += stepCtrl[STEPPER_UPDOWN].delayMatch;
	CTIMER_SetupMatch(CTIMER0, kCTIMER_Match_2, &matchConfig2);
	/* Control the stepper */
	StepCtrl(STEPPER_UPDOWN);
}

/*
 * match3 callback for moving the band back and forth
 */
void ctimer_match3_callback(uint32_t flags){
	/* set next match within corresponding delay */
	matchConfig3.matchValue += stepCtrl[STEPPER_BACKFORTH].delayMatch;
	CTIMER_SetupMatch(CTIMER0, kCTIMER_Match_3, &matchConfig3);
	/* Control the stepper */
	StepCtrl(STEPPER_BACKFORTH);
}

/*
 * Testing loop
 */
void Stepper_Dostuff(stepper_t stepper, direction_t dir, uint32_t steps){
	stepCtrl[stepper].dir = dir;
	stepCtrl[stepper].stepsTodo = steps;
	stepCtrl[stepper].homing = false;
}

/*
 * Homing a Stepper motor
 */
void Stepper_Home(stepper_t stepper){
	stepCtrl[stepper].homing = true;
	stepCtrl[stepper].stepsTodo = 0;
	stepCtrl[stepper].dir = stepCtrl[stepper].homeDir;
}

/*
 * Stop the current action
 */
void Stepper_Halt(stepper_t stepper){
	stepCtrl[stepper].stepsTodo = 0;
	stepCtrl[stepper].homing = false;
}

/*
 * Check if Stepper is done
 */
bool Stepper_Isdone(stepper_t stepper){
	if((stepCtrl[stepper].stepsTodo == 0) && stepCtrl[stepper].homing == false){
		return true;
	}
	return false;
}

/*
 * Change delay time of a stepper
 */
void Stepper_NewDelayms(stepper_t stepper, uint32_t delayms){
	stepCtrl[stepper].delayms = delayms;
	stepCtrl[stepper].delayMatch = MS_TICKS(delayms);
}

/*
 * Initialize Pins and Timers for Stepper ussage.
 */
void Stepper_Init(void){
	Pins_Init();
	// default stepper setting
	stepCtrl[STEPPER_INOUT].delayMatch = MS_TICKS(STEPPER_INOUT_DELAYMS);
	stepCtrl[STEPPER_INOUT].delayms = STEPPER_INOUT_DELAYMS;
	stepCtrl[STEPPER_INOUT].dir = IN;
	stepCtrl[STEPPER_INOUT].homeDir = IN;
	stepCtrl[STEPPER_INOUT].stepsTodo = 0;
	stepCtrl[STEPPER_INOUT].homing = false;
	stepCtrl[STEPPER_LEFTRIGHT].delayMatch = MS_TICKS(STEPPER_LEFTRIGHT_DELAYMS);
	stepCtrl[STEPPER_LEFTRIGHT].delayms = STEPPER_LEFTRIGHT_DELAYMS;
	stepCtrl[STEPPER_LEFTRIGHT].dir = LEFT;
	stepCtrl[STEPPER_LEFTRIGHT].dir = LEFT;
	stepCtrl[STEPPER_LEFTRIGHT].stepsTodo = 0;
	stepCtrl[STEPPER_LEFTRIGHT].homing = false;
	stepCtrl[STEPPER_UPDOWN].delayMatch = MS_TICKS(STEPPER_UPDOWN_DELAYMS);
	stepCtrl[STEPPER_UPDOWN].delayms = STEPPER_UPDOWN_DELAYMS;
	stepCtrl[STEPPER_UPDOWN].dir = UP;
	stepCtrl[STEPPER_UPDOWN].homeDir = UP;
	stepCtrl[STEPPER_UPDOWN].stepsTodo = 0;
	stepCtrl[STEPPER_UPDOWN].homing = false;
	stepCtrl[STEPPER_BACKFORTH].delayMatch = MS_TICKS(STEPPER_BACKFORTH_DELAYMS);
	stepCtrl[STEPPER_BACKFORTH].delayms = STEPPER_BACKFORTH_DELAYMS;
	stepCtrl[STEPPER_BACKFORTH].dir = FORTH;
	stepCtrl[STEPPER_BACKFORTH].homeDir = BACK;
	stepCtrl[STEPPER_BACKFORTH].stepsTodo = 0;
	stepCtrl[STEPPER_BACKFORTH].homing = false;
	Stepper_Timer_Init();
}


void Pins_Init(void){
/* GPIO Configuration */
	McuGPIO_Config_t config;
	McuGPIO_GetDefaultConfig(&config);

/* GPIO Stepper 1 Step */
	config.isInput = false;
	config.isHighOnInit = false;
	config.hw.gpio = GPIO;
	config.hw.port = 0;
	config.hw.pin = 11;
	config.hw.iocon = IOCON_INDEX_PIO0_11;
	stepCtrl[STEPPER_INOUT].StepPin = McuGPIO_InitGPIO(&config);

/* GPIO Stepper 1 Direction */
	config.isInput = false;
	config.isHighOnInit = false;
	config.hw.gpio = GPIO;
	config.hw.port = 0;
	config.hw.pin = 13;
	config.hw.iocon = IOCON_INDEX_PIO0_13;
	stepCtrl[STEPPER_INOUT].DirPin = McuGPIO_InitGPIO(&config);

/* GPIO Stepper 1 Limitswitch */
	config.isInput = true;
	config.isHighOnInit = false;
	config.hw.gpio = GPIO;
	config.hw.port = 0;
	config.hw.pin = 18;
	config.hw.iocon = IOCON_INDEX_PIO0_18;
	stepCtrl[STEPPER_INOUT].StopPin = McuGPIO_InitGPIO(&config);

/* GPIO Stepper 2 Step */
	config.isInput = false;
	config.isHighOnInit = false;
	config.hw.gpio = GPIO;
	config.hw.port = 0;
	config.hw.pin = 14;
	config.hw.iocon = IOCON_INDEX_PIO0_14;
	stepCtrl[STEPPER_LEFTRIGHT].StepPin = McuGPIO_InitGPIO(&config);

/* GPIO Stepper 2 Direction */
	config.isInput = false;
	config.isHighOnInit = false;
	config.hw.gpio = GPIO;
	config.hw.port = 0;
	config.hw.pin = 15;
	config.hw.iocon = IOCON_INDEX_PIO0_15;
	stepCtrl[STEPPER_LEFTRIGHT].DirPin = McuGPIO_InitGPIO(&config);

/* GPIO Stepper 2 Limitswitch */
	config.isInput = false;
	config.isHighOnInit = false;
	config.hw.gpio = GPIO;
	config.hw.port = 0;
	config.hw.pin = 19;
	config.hw.iocon = IOCON_INDEX_PIO0_19;
	stepCtrl[STEPPER_LEFTRIGHT].StopPin = McuGPIO_InitGPIO(&config);

/* GPIO Stepper 3 Step */
	config.isInput = false;
	config.isHighOnInit = false;
	config.hw.gpio = GPIO;
	config.hw.port = 0;
	config.hw.pin = 16;
	config.hw.iocon = IOCON_INDEX_PIO0_16;
	stepCtrl[STEPPER_UPDOWN].StepPin = McuGPIO_InitGPIO(&config);

/* GPIO Stepper 3 Direction */
	config.isInput = false;
	config.isHighOnInit = false;
	config.hw.gpio = GPIO;
	config.hw.port = 0;
	config.hw.pin = 17;
	config.hw.iocon = IOCON_INDEX_PIO0_17;
	stepCtrl[STEPPER_UPDOWN].DirPin = McuGPIO_InitGPIO(&config);

/* GPIO Stepper 3 Limitswitch */
	config.isInput = false;
	config.isHighOnInit = false;
	config.hw.gpio = GPIO;
	config.hw.port = 0;
	config.hw.pin = 20;
	config.hw.iocon = IOCON_INDEX_PIO0_20;
	stepCtrl[STEPPER_UPDOWN].StopPin = McuGPIO_InitGPIO(&config);

/* GPIO Stepper 4 Step */
	config.isInput = false;
	config.isHighOnInit = false;
	config.hw.gpio = GPIO;
	config.hw.port = 0;
	config.hw.pin = 22;
	config.hw.iocon = IOCON_INDEX_PIO0_22;
	stepCtrl[STEPPER_BACKFORTH].StepPin = McuGPIO_InitGPIO(&config);

/* GPIO Stepper 4 Direction */
	config.isInput = false;
	config.isHighOnInit = false;
	config.hw.gpio = GPIO;
	config.hw.port = 0;
	config.hw.pin = 21;
	config.hw.iocon = IOCON_INDEX_PIO0_21;
	stepCtrl[STEPPER_BACKFORTH].DirPin = McuGPIO_InitGPIO(&config);

/* GPIO Stepper 4 Limitswitch */
	config.isInput = false;
	config.isHighOnInit = false;
	config.hw.gpio = GPIO;
	config.hw.port = 0;
	config.hw.pin = 23;
	config.hw.iocon = IOCON_INDEX_PIO0_23;
	stepCtrl[STEPPER_BACKFORTH].StopPin = McuGPIO_InitGPIO(&config);


/* GPIO Stepper Enable */
	config.isInput = false;
	config.isHighOnInit = true;
	config.hw.gpio = GPIO;
	config.hw.port = 0;
	config.hw.pin = 8;
	config.hw.iocon = IOCON_INDEX_PIO0_8;
	EnPin = McuGPIO_InitGPIO(&config);

/* GPIO Stepper MS1 */
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
	config.isInput = false;
	config.isHighOnInit = false;
	config.hw.gpio = GPIO;
	config.hw.port = 1;
	config.hw.pin = 0;
	config.hw.iocon = IOCON_INDEX_PIO1_0;
	GreenLED = McuGPIO_InitGPIO(&config);

/* GPIO LED Blue*/
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
	matchConfig0.matchValue         = stepCtrl[STEPPER_INOUT].delayMatch;
	matchConfig0.outControl         = kCTIMER_Output_NoAction;
	matchConfig0.outPinInitState    = false;
	matchConfig0.enableInterrupt    = true;

	/* Configuration 1 Stepper left/right */
	matchConfig1.enableCounterReset = false;
	matchConfig1.enableCounterStop  = false;
	matchConfig1.matchValue         = stepCtrl[STEPPER_LEFTRIGHT].delayMatch;
	matchConfig1.outControl         = kCTIMER_Output_NoAction;
	matchConfig1.outPinInitState    = false;
	matchConfig1.enableInterrupt    = true;

	/* Configuration 2 Stepper up/down */
	matchConfig2.enableCounterReset = false;
	matchConfig2.enableCounterStop  = false;
	matchConfig2.matchValue         = stepCtrl[STEPPER_UPDOWN].delayMatch;
	matchConfig2.outControl         = kCTIMER_Output_NoAction;
	matchConfig2.outPinInitState    = false;
	matchConfig2.enableInterrupt    = true;

	/* Configuration 3 Stepper up/down */
	matchConfig3.enableCounterReset = false;
	matchConfig3.enableCounterStop  = false;
	matchConfig3.matchValue         = stepCtrl[STEPPER_BACKFORTH].delayMatch;
	matchConfig3.outControl         = kCTIMER_Output_NoAction;
	matchConfig3.outPinInitState    = false;
	matchConfig3.enableInterrupt    = true;

	CTIMER_RegisterCallBack(CTIMER0, &ctimer_callback_table[0], kCTIMER_MultipleCallback);
	CTIMER_SetupMatch(CTIMER0, kCTIMER_Match_0, &matchConfig0);
	CTIMER_SetupMatch(CTIMER0, kCTIMER_Match_1, &matchConfig1);
	CTIMER_SetupMatch(CTIMER0, kCTIMER_Match_2, &matchConfig2);
	CTIMER_SetupMatch(CTIMER0, kCTIMER_Match_3, &matchConfig3);

	NVIC_SetPriority(CTIMER0_IRQn, 1);	// Set Timer priority to highest

	CTIMER_StartTimer(CTIMER0);

	McuGPIO_SetLow(EnPin);
	McuGPIO_SetHigh(Ms1Pin);
}

/*
 * nothing implemented yet
 */
void Stepper_Deinit(void) {}
