/*
 * Stepper.h
 *
 *  Created on: 24.03.2023
 *      Author: Ming
 */

#ifndef STEPPER_H_
#define STEPPER_H_

#include <stdint.h>

/* Type for identifying the steppers */
typedef enum {
	STEPPER_INOUT,
	STEPPER_LEFTRIGHT,
	STEPPER_UPDOWN
} stepper_t;

/* Enum for the directions (adjust here) */
typedef enum {
	IN = 1, OUT = 0,
	RIGHT = 1, LEFT = 0,
	UP = 1, DOWN = 0
} direction_t;

void Stepper_Init(void);

void Stepper_Dostuff(stepper_t stepper, direction_t dir, uint32_t steps);

void Stepper_Home(stepper_t stepper);

void Stepper_Halt(stepper_t stepper);

bool Stepper_Isdone(stepper_t stepper);

void Stepper_Deinit(void);

#endif /* STEPPER_H_ */
