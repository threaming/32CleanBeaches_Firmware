/*
 * Servo.c
 *
 *  Created on: 31.03.2023
 *      Author: Ming
 */

#include "CleanBeaches/Servo.h"
#include "fsl_sctimer.h"

/* Timer Defines */
#define SCTIMER_CLK_FREQ CLOCK_GetFreq(kCLOCK_Fro)

/* Servo Defines */
//#define PULSE_TO_PWMPERC(upper, lower, percent) ((uint8_t)(((((upper-lower)*percent)/20000))+(lower/20000)))
#define PULSE_TO_PWMPERC(upper, lower, percent) ((uint8_t)(((upper - lower) * percent) / (100 * 256)))

/*
 * Initialize SCT0 Timer for PWM
 */
void Servo_Init(){
	sctimer_config_t sctimerConfig;

	SCTIMER_GetDefaultConfig(&sctimerConfig);
	SCTIMER_Init(SCT0, &sctimerConfig);
}

/*
 * Setup a Servo on pin servoHandle.output
 *
 * After all Setups Servo_TimerStart(); must be called
 */
void Servo_Setup(Servo_Handle_t *servoHandle, uint32_t *event){
	sctimer_pwm_signal_param_t pwmParam;
	uint32_t periodMatchReg, period;

	pwmParam.output = servoHandle->output;
	pwmParam.level = kSCTIMER_HighTrue;
	pwmParam.dutyCyclePercent = PULSE_TO_PWMPERC(servoHandle->upperPulse_us,
												 servoHandle->lowerPulse_us,
												 servoHandle->pulsePercent);

	// sctimer setup
	SCTIMER_SetupPwm(SCT0, &pwmParam, kSCTIMER_CenterAlignedPwm, 50U, SCTIMER_CLK_FREQ, event);

	// get period
	/* Retrieve the match register number for the PWM period */
	periodMatchReg = SCT0->EV[*event].CTRL & SCT_EV_CTRL_MATCHSEL_MASK;
	period = SCT0->MATCH[periodMatchReg];

	// calculate register limits for pulse
	servoHandle->lowerRegister = (servoHandle->lowerPulse_us*period)/20000;
	servoHandle->upperRegister = (servoHandle->upperPulse_us*period)/20000;
}

/*
 * Must be called after Servo_Setup();
 */
void Servo_TimerStart(void){
	SCTIMER_StartTimer(SCT0, kSCTIMER_Counter_U);
}

void Servo_SetPulse(Servo_Handle_t servoHandle, uint32_t pulsePercent, uint32_t event){
	assert(pulsePercent <= 100U);
	SCT_Type *base = SCT0;
	uint32_t pulseMatchReg;
	uint32_t pulsePeriod = 0;

	/* Retrieve the match register number for the PWM pulse period */
	pulseMatchReg = base->EV[event + 1U].CTRL & SCT_EV_CTRL_MATCHSEL_MASK;

	/* Stop the counter before updating match register */
	SCTIMER_StopTimer(base, (uint32_t)kSCTIMER_Counter_U);

	/* For 100% dutycyle, make pulse period greater than period so the event will never occur */
	pulsePeriod = ((((pulsePercent * (servoHandle.upperRegister - servoHandle.lowerRegister)) / 100U) + servoHandle.lowerRegister));

	/* Update dutycycle */
	base->MATCH[pulseMatchReg]    = pulsePeriod;
	base->MATCHREL[pulseMatchReg] = pulsePeriod;

	/* Restart the counter */
	SCTIMER_StartTimer(base, (uint32_t)kSCTIMER_Counter_U);
}

void Servo_Deinit(void);
