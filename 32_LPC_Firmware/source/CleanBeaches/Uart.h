/*
 * Uart.h
 *
 *  Created on: 28.04.2023
 *      Author: Ming
 */
#include <stdint.h>

#ifndef UART_H_
#define UART_H_

void Uart_Init(void);

void Uart_Deinit(void);

void Uart_SendString(const uint8_t *str);

void Uart_SendNum32(uint32_t num);

#endif /* UART_H_ */
