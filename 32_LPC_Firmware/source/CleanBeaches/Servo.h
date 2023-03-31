/*
 * Servo.h
 *
 *  Created on: 31.03.2023
 *      Author: Ming
 */

#include <stdint.h>
#include "fsl_sctimer.h"

#ifndef SERVO_H_
#define SERVO_H_

typedef struct {
	sctimer_out_t output;           	// The output pin to use to generate the PWM signal
	const uint64_t lowerPulse_us;		// pulse length for smallest angle (in us)
	uint32_t lowerRegister;				// min register entry for smallest angle (Set in Servo_Setup();)
	const uint64_t upperPulse_us;		// pulse length for greatest angle (in us)
	uint32_t upperRegister;				// max register entry for greatest angle (Set in Servo_Setup();)
	uint32_t pulsePercent;				// 0% -> lowerPulse, 100% upperPulse
} Servo_Handle_t;

void Servo_Init();

void Servo_Setup(Servo_Handle_t *servoHandle, uint32_t *event);

void Servo_SetPulse(Servo_Handle_t servoHandle, uint32_t pulsePercent, uint32_t event);

void Servo_TimerStart(void);

void Servo_Deinit(void);

#endif /* SERVO_H_ */
