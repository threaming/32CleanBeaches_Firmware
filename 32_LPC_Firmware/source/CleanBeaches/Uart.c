/*
 * Uart.c
 *
 *  Created on: 28.04.2023
 *      Author: Ming
 */

#include <stdint.h>
#include "CleanBeaches/Uart.h"
#include "McuShellUart.h"
#include "McuShell.h"

void Uart_SendString(const uint8_t *str){
	McuShell_SendStr(str, McuShellUart_stdio.stdOut);
}

void Uart_SendNum32(uint32_t num){
	McuShell_SendNum32u(num, McuShellUart_stdio.stdOut);
}

void Uart_Init(void){
	McuShellUart_Init();
}

void Uart_Deinit(void);
