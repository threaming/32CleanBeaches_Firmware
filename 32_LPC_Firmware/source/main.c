/*
 * Copyright 2016-2023 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file    LPC845_Project.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include <stdint.h>
/* insert other include files here. */
#include "pin_mux.h"
#include "clock_config.h"
#include "McuWait.h"
#include "McuGPIO.h"
#include "CleanBeaches/Stepper.h"
#include "CleanBeaches/Servo.h"
#include "CleanBeaches/Uart.h"
#include "CleanBeaches/Counting.h"
/* TODO: insert other definitions and declarations here. */
#define SCTIMER_SERVO_BUCKET	kSCTIMER_Out_0
#define SCTIMER_PWM_FAN			kSCTIMER_Out_1

/* Global Handles */
McuGPIO_Handle_t MainSwitch = NULL;

/* States */
typedef enum {
	SETUP,
	GET_STONES,
	PUT_STONES,
	WAIT
} states_t;
typedef struct{
	states_t state;
	states_t next_state;
} states_s;

/*
 * @brief   Application entry point.
 */
int main(void) {
	/* Initialisations */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();

    McuWait_Init();
    McuGPIO_Init();
    Uart_Init();
    Stepper_Init();
    Servo_Init();
    Counter_Init();

    /* Main switch */
	McuGPIO_Config_t config;
	McuGPIO_GetDefaultConfig(&config);
	config.isInput = true;
	config.isHighOnInit = false;
	config.hw.gpio = GPIO;
	config.hw.port = 0;
	config.hw.pin = 4;
	config.hw.iocon = IOCON_INDEX_PIO0_4;
	MainSwitch = McuGPIO_InitGPIO(&config);

    /* PWM Setup servo */
    uint32_t eventServoBucket;
    Servo_Handle_t servoBucket = {SCTIMER_SERVO_BUCKET, 900, 0, 2100, 0, 0}; // SCTIMER, lower-pulse, -register, upper-pulse, -register, percentage
    Servo_Setup(&servoBucket, &eventServoBucket);
    Servo_TimerStart();
    Servo_SetPulse(servoBucket, 100, eventServoBucket);

    /* PWM Setup Fan */
	uint32_t eventFan;
	Servo_Handle_t servoFan = {SCTIMER_PWM_FAN, 900, 0, 2100, 0, 0}; // SCTIMER, lower-pulse, -register, upper-pulse, -register, percentage
	Servo_Setup(&servoFan, &eventFan);
	Servo_TimerStart();
	Servo_SetPulse(servoFan, 100, eventFan);

    /* FSM setup */
    states_s fsm;
    fsm.state = fsm.next_state = SETUP;

    /* Enter an infinite loop, just incrementing a counter. */
    while(1) {
    	/* Finite State Machine */
    	switch(fsm.state){
    	case SETUP:
    		Stepper_Home(STEPPER_BACKFORTH);
    		Stepper_Home(STEPPER_INOUT);
    		Stepper_Home(STEPPER_LEFTRIGHT);
    		Stepper_Home(STEPPER_UPDOWN);
    		while(!(Stepper_Isdone(STEPPER_BACKFORTH) 	&& 	// Wait for steppers to arrive
    				Stepper_Isdone(STEPPER_INOUT) 		&&
					Stepper_Isdone(STEPPER_LEFTRIGHT)	&&
					Stepper_Isdone(STEPPER_UPDOWN))){
    			McuWait_Waitms(10);
    		}
    		while(McuGPIO_IsLow(MainSwitch)){				// Wait for user activation
    			McuWait_Waitms(10);
    		}
    		Stepper_Dostuff(STEPPER_BACKFORTH, FORTH, 4500);
    		fsm.state = GET_STONES;
    		break;

    	case GET_STONES:
    		fsm.next_state = PUT_STONES;
    		Stepper_Dostuff(STEPPER_UPDOWN, DOWN, 1000);
    		Stepper_Dostuff(STEPPER_INOUT, OUT, 5000);
    		Stepper_Dostuff(STEPPER_LEFTRIGHT, LEFT, 250);
    		fsm.state = WAIT;
    		break;

    	case PUT_STONES:
    		fsm.next_state = GET_STONES;
    		Stepper_Dostuff(STEPPER_INOUT, IN, 1000);
			Stepper_Home(STEPPER_LEFTRIGHT);
			Stepper_Home(STEPPER_UPDOWN);
			fsm.state = WAIT;
    		break;

    	case WAIT:
    		if(	Stepper_Isdone(STEPPER_INOUT) 		&& 		// Wait for steppers to arrive
				Stepper_Isdone(STEPPER_LEFTRIGHT)	&&
				Stepper_Isdone(STEPPER_UPDOWN)){
				fsm.state = fsm.next_state;
			}
    		break;

    	default:
    		fsm.state = SETUP;
    		break;
    	}

    	/* FSM external control sequence */
    	if (McuGPIO_IsLow(MainSwitch)){
    		fsm.state = SETUP;
    	}

	//    	Servo_SetPulse(servoBucket, 20, eventServoBucket);
	//    	Servo_SetPulse(servoBucket, 80, eventServoBucket);
    }
    return 0 ;
}
