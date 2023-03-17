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
#include "McuWait.h"
#include "McuGPIO.h"
/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */
int main(void) {

    McuWait_Init();
    McuGPIO_Init();
    /* GPIO Configuration */
		McuGPIO_Config_t config;
		McuGPIO_Handle_t Step1Pin;
		McuGPIO_Handle_t Dir1Pin;
		McuGPIO_Handle_t Step2Pin;
		McuGPIO_Handle_t Dir2Pin;
		McuGPIO_Handle_t EnPin;
		McuGPIO_Handle_t Ms1Pin;
		McuGPIO_Handle_t Ms2Pin;

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


    /* intialize Stepper Motor */
    McuGPIO_SetLow(EnPin);
    McuGPIO_SetHigh(Ms1Pin);

    /* Enter an infinite loop, just incrementing a counter. */
    while(1) {
    	/* Stepping */
    	McuGPIO_SetLow(Step1Pin);
    	McuGPIO_SetLow(Step2Pin);
    	McuWait_Waitms(10);
    	McuGPIO_SetHigh(Step1Pin);
    	McuGPIO_SetHigh(Step2Pin);
		McuWait_Waitms(10);
    }
    return 0 ;
}
