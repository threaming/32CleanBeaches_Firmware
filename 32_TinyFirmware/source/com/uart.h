/**
 *--------------------------------------------------------------------\n
 *          HSLU T&A Hochschule Luzern Technik+Architektur            \n
 *--------------------------------------------------------------------\n
 *
 * \brief         driver for the serial communication (uart)
 * \file
 * \author        Christian Jost, christian.jost@hslu.ch
 * \date          10.03.2020
 *
 *--------------------------------------------------------------------
 */
#ifndef SOURCES_UART_H_
#define SOURCES_UART_H_

#define NEW_LINE                '\n'
#define CARRIAGE_RETURN         '\r'

#define UART0_PTA2_PTA4           0
#define UART0_PTB16_PTB17         1
#define UART0_PTD6_PTD7           2

#define UART1_PTC3_PTC4          10
#define UART1_PTE0_PTE1          11

#define LPUART0_PTB10_PTB11      20
#define LPUART0_PTD2_PTD3        21

#define UART0_EN                  1     // [0|1] 1=>enable, 0=>disable
#define UART0_PRINTF_EN           0     // [0|1] redirect printf to uart 0
#define UART0_SCANF_EN            0     // [0|1] redirect scanf to uart 0
#define UART0_RX_BUF_SIZE       512     // size of the receive buffer in bytes
#define UART0_TX_BUF_SIZE       512     // size of the transmit buffer in bytes
#define UART0_PINS              UART0_PTD6_PTD7

#define UART1_EN                  1     // [0|1] 1=>enable, 0=>disable
#define UART1_PRINTF_EN           0     // [0|1] redirect printf to uart 0
#define UART1_SCANF_EN            0     // [0|1] redirect scanf to uart 0
#define UART1_RX_BUF_SIZE       512     // size of the receive buffer in bytes
#define UART1_TX_BUF_SIZE       512     // size of the transmit buffer in bytes
#define UART1_PINS              UART1_PTC3_PTC4

#define UART2_EN                  1     // [0|1] 1=>enable, 0=>disable
#define UART2_PRINTF_EN           0     // [0|1] redirect printf to uart 0
#define UART2_SCANF_EN            0     // redirect scanf to uart 0
#define UART2_RX_BUF_SIZE       512     // size of the receive buffer in bytes
#define UART2_TX_BUF_SIZE       512     // size of the transmit buffer in bytes
#define UART2_PINS              UART2_PTD2_PTD3


#define LPUART0_EN                1     // [0|1] 1=>enable, 0=>disable
#define LPUART0_PRINTF_EN         0     // [0|1] redirect printf to uart 0
#define LPUART0_SCANF_EN          0     // [0|1] redirect scanf to uart 0
#define LPUART0_RX_BUF_SIZE     512     // size of the receive buffer in bytes
#define LPUART0_TX_BUF_SIZE     512     // size of the transmit buffer in bytes
#define LPUART0_PINS            LPUART0_PTC3_PTC4

#if ((PLATFORM & TINYK22) == 0)
#undef UART1_EN
#endif

#if ((PLATFORM & MC_CAR) == 0)
#undef UART0_EN
#undef LPUART0_EN
#endif


void uart0WriteChar(char ch);
void uart0Write(const char *str);
void uart0WriteLine(const char *str);
char uart0ReadChar(void);
uint16_t uart0ReadLine(char *str, uint16_t length);
bool uart0HasLineReceived(void);
uint16_t uart0RxBufCount(void);
void uart0Init(uint32_t baudrate);

void uart1WriteChar(char ch);
void uart1Write(const char *str);
void uart1WriteLine(const char *str);
char uart1ReadChar(void);
uint16_t uart1ReadLine(char *str, uint16_t length);
bool uart1HasLineReceived(void);
uint16_t uart1RxBufCount(void);
void uart1Init(uint32_t baudrate);

void uart2WriteChar(char ch);
void uart2Write(const char *str);
void uart2WriteLine(const char *str);
char uart2ReadChar(void);
uint16_t uart2ReadLine(char *str, uint16_t length);
bool uart2HasLineReceived(void);
uint16_t uart2RxBufCount(void);
void uart2Init(uint32_t baudrate);

void lpuart0WriteChar(char ch);
void lpuart0Write(const char *str);
void lpuart0WriteLine(const char *str);
char lpuart0ReadChar(void);
uint16_t lpuart0ReadLine(char *str, uint16_t length);
bool lpuart0HasLineReceived(void);
uint16_t lpuart0RxBufCount(void);
void lpuart0Init(uint32_t baudrate);

#endif /* SOURCES_UART_H_ */
