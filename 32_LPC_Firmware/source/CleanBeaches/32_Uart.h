/*
 * 32_Uart.h
 *
 *  Created on: 27.04.2023
 *      Author: Ming
 */

#include <stdint.h>

#ifndef _UART_H_
#define _UART_H_

void Uart_Init(void);
void Uart_Write(const uint8_t *data);

#endif /* 32_UART_H_ */
