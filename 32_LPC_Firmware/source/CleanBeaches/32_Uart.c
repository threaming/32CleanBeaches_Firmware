/*
 * 32_Uart.c
 *
 *  Created on: 27.04.2023
 *      Author: Ming
 */

#include "CleanBeaches/32_Uart.h"

#include "pin_mux.h"
#include "fsl_usart.h"
#include <stdint.h>

/* Settings */
#define BOARD_DEBUG_USART_BAUDRATE 9600

void Uart_Init(void){
	usart_config_t config;
	/* Select the main clock as source clock of USART0. */
	CLOCK_Select(kUART0_Clk_From_MainClk);

	USART_GetDefaultConfig(&config);
	config.enableRx     = true;
	config.enableTx     = true;
	config.baudRate_Bps = BOARD_DEBUG_USART_BAUDRATE;

    /* Initialize the USART with configuration. */
    USART_Init(USART0, &config, CLOCK_GetFreq(kCLOCK_MainClk));
}

void Uart_Write(const uint8_t *data){
	USART_WriteBlocking(USART0, data, sizeof(data) - 1);
}
