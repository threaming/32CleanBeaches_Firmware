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
/* insert other include files here. */
#include "pin_mux.h"
#include "clock_config.h"
#include "McuWait.h"
#include "McuGPIO.h"
#include "CleanBeaches/Stepper.h"
#include "CleanBeaches/Servo.h"
#include "fsl_sctimer.h"
#include "fsl_iocon.h"
/* TODO: insert other definitions and declarations here. */
#define SCTIMER_SERVO_BUCKET	kSCTIMER_Out_0

/*
 * @brief   Application entry point.
 */
int main(void) {
	/* Initialisations */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();

    McuWait_Init();
    McuGPIO_Init();
    Stepper_Init();
    Servo_Init();
    Stepper_Dostuff();

    /* PWM Setup */
    uint32_t eventServoBucket;
    Servo_Handle_t servoBucket = {SCTIMER_SERVO_BUCKET, 800, 0, 2000, 0, 0}; // SCTIMER, lower-pulse, -register, upper-pulse, -register, percentage
    Servo_Setup(&servoBucket, &eventServoBucket);
    Servo_TimerStart();
    Servo_SetPulse(servoBucket, 100, eventServoBucket);

    /* Enter an infinite loop, just incrementing a counter. */
    while(1) {
//    	McuWait_Waitms(5000);
//    	Servo_SetPulse(servoBucket, 20, eventServoBucket);
//    	McuWait_Waitms(5000);
//    	Servo_SetPulse(servoBucket, 80, eventServoBucket);


    }
    return 0 ;
}
