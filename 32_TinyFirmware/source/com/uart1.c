/**
 *--------------------------------------------------------------------\n
 *          HSLU T&A Hochschule Luzern Technik+Architektur            \n
 *--------------------------------------------------------------------\n
 *
 * \brief         driver for the serial communication (uart1 tinyk22)
 * \file
 * \author        Christian Jost, christian.jost@hslu.ch
 * \date          10.03.2020
 *
 *--------------------------------------------------------------------
 */
#include "platform.h"
#include "uart.h"

#if UART1_EN


/**
 * the receive queue of this driver, implemented as ring buffer
 */
static char rxBuf[UART1_RX_BUF_SIZE];
static volatile uint16_t rxBufCount;
static uint16_t rxBufWritePos;
static uint16_t rxBufReadPos;

/**
 * the transmit queue of this driver, implemented as ring buffer
 */
static char txBuf[UART1_TX_BUF_SIZE];
static volatile uint16_t txBufCount;
static uint16_t txBufWritePos;
static uint16_t txBufReadPos;


/**
 * @brief UART Interrupt Service Routine
 * - Received bytes are stored in the queue rxBuf
 * - Bytes in the queue txBuf are sent
 */
void UART1_RX_TX_IRQHandler(void)
{
  OnEnterUart1RxTxISR();
  uint8_t status = UART1->S1;
  uint8_t data = UART1->D;
  if (status & UART_S1_RDRF_MASK)
  {
    // store the received byte into receiver Queue (rxBuf)
    // but only if the queue isn't full!
    if (rxBufCount < UART1_RX_BUF_SIZE)
    {
      rxBuf[rxBufWritePos++] = data;
      rxBufCount++;
      if (rxBufWritePos == UART1_RX_BUF_SIZE) rxBufWritePos = 0;
    }
  }

  if (status & UART_S1_TDRE_MASK)
  {
    if (txBufCount > 0)
    {
      UART1->D = txBuf[txBufReadPos++];
      txBufCount--;
      if (txBufReadPos == UART1_TX_BUF_SIZE) txBufReadPos = 0;
    }
    else
    {
      UART1->C2 &= ~UART_C2_TIE_MASK;
    }
  }
  OnExitUart1RxTxISR();
}

/**
 * Error Interrupt Service Routine
 * Clears the error flags.
 */
void UART1_ERR_IRQHandler(void)
{
  (void)UART1->S1;
  (void)UART1->D;
}

/**
 * Writes one Byte in the transmit buffer.
 *
 * @details
 *   Switching on the TIE interrupt causes an interrupt to be
 *   triggered immediately. The function blocks until there is
 *   space in the txBuf queue.
 *
 * @param[in] ch
 *   the byte to send
 */
void uart1WriteChar(char ch)
{
  while(txBufCount >= UART1_TX_BUF_SIZE);
  txBuf[txBufWritePos++] = ch;
  if (txBufWritePos == UART1_TX_BUF_SIZE) txBufWritePos = 0;
  NVIC_DisableIRQEx(UART1_RX_TX_IRQn);
  txBufCount++;
  NVIC_EnableIRQ(UART1_RX_TX_IRQn);
  UART1->C2 |= UART_C2_TIE_MASK;
}

/**
 * Writes a null terminated string in the send buffer. If the
 * string is null, the function returns immediately.
 *
 * @param[in] str
 *   the null terminated string to send
 */
void uart1Write(const char *str)
{
  if (str == NULL) return;
  while (*str != '\0') uart1WriteChar(*str++);
}

/**
 * Writes a null terminated string in the send buffer. If the
 * string is null, only a new new line character is sent.
 *
 * @param[in] str
 *   the null terminated string to send
 */
void uart1WriteLine(const char *str)
{
  uart1Write(str);
  uart1WriteChar(NEW_LINE);
}


/**
 * Reads one char out of the rxBuf queue. The function blocks
 * until there is at least one byte in the queue.
 *
 * @return
 *   the received byte
 */
char uart1ReadChar(void)
{
  char ch;
  while (rxBufCount == 0);
  ch = rxBuf[rxBufReadPos++];
  if (rxBufReadPos == UART1_RX_BUF_SIZE) rxBufReadPos = 0;
  NVIC_DisableIRQEx(UART1_RX_TX_IRQn);
  rxBufCount--;
  NVIC_EnableIRQ(UART1_RX_TX_IRQn);
  return ch;
}

/**
 * Reads a null terminated string out of the rxBuf queue. The
 * function blocks until a new Line character has been received
 * or the length has been exceeded.
 *
 * @details
 *   the new line character will be replaced with a '\0' to
 *   terminate the string.
 *
 * @param[out] *str
 *   pointer to a char array to store the received string
 * @param[in] length
 *   the length of the str char array.
 * @returns
 *   the length of the received string.
 */
uint16_t uart1ReadLine(char *str, uint16_t length)
{
  uint16_t i;
  for (i=1; i<length; i++)
  {
    *str = uart1ReadChar();
    if (*str == NEW_LINE)
    {
      *str = '\0';
      break;
    }
    str++;
  }
  return i;
}

/**
 * This functions checks, if there is a new line character
 * in the rxBuf queue.
 *
 * @returns
 *   TRUE, if there is a new line character, otherweise FALSE.
 */
bool uart1HasLineReceived(void)
{
  uint16_t i;
  uint16_t index = rxBufReadPos;

  for (i=0; i<rxBufCount; i++)
  {
    if (rxBuf[index] == NEW_LINE) return TRUE;
    if (rxBuf[index] == CARRIAGE_RETURN) return TRUE;
    index++;
    if (index == UART1_RX_BUF_SIZE) index = 0;
  }
  return FALSE;
}

/**
 * \fn uint16_t uart1RxBufCount(void)
 * Returns the number of bytes in the receiver queue.
 *
 * @returns
 *   the number of bytes in the receiver queue.
 */
uint16_t uart1RxBufCount(void)
{
  return rxBufCount;
}

#if UART1_PRINTF_EN
/**
 * This function redirects the printf output to the uart
 *
 * @params[in] fd
 *
 * @params[in] buf
 *   the buffer with the data bytes to send
 *
 * @params[in] count
 *   the number of bytes to send
 *
 * @returns
 *   the number of bytes sent.
 */
int _write(int fd, const char *buf, int count)
{
  int i = count;
  while (i--) uart1WriteChar(*(uint8_t*)buf++);
  return count;
}
#endif

#if UART1_SCANF_EN
/**
 * This function redirects the scanf input to the uart
 *
 * @params[in] fd
 *
 * @params[in] buf
 *   buffer to write the bytes read from the uart
 *
 * @params[in] count
 *   number of bytes written into the buffer
 *
 * @returns
 *   the number of bytes sent.
 */
int _read(int fd, char *buf, int count)
{
  *buf = uart1ReadChar();
  return 1;
}
#endif

/**
 * initializes the uart with the desired baud rate.
 *
 * @details
 *   The uart connection between the debugger and the target
 *   device is on the MC-Car and the tinyK22 different:
 *
 *   MC-Car:
 *   - PTA1 Mux2:UART0_RX
 *   - PTA2 Mux2:UART0_TX
 *
 *   tinyK22:
 *   - PTC3 Mux3:UART1_RX, (MUX7:LPUART1_RX)
 *   - PTC4 Mux3:UART1_TX, (MUX7:LPUART1_TX)
 */
void uart1Init(uint32_t baudrate)
{
  txBufReadPos = txBufWritePos = txBufCount = 0;
  rxBufReadPos = rxBufWritePos = rxBufCount = 0;

  // configure clock gating (Kinetis Reference Manual p277) KRM277
  SIM->SCGC4 |= SIM_SCGC4_UART1_MASK;

  // configure port multiplexing, enable Pull-Ups and enable OpenDrain (ODE)!
  // OpenDrain is needed to ensure that no current flows from Target-uC to the Debugger-uC
#if UART1_PINS == UART1_PTC3_PTC4
  PORTC->PCR[3] = PORT_PCR_MUX(3) | PORT_PCR_PE(1) | PORT_PCR_PS(1) | PORT_PCR_ODE_MASK;
  PORTC->PCR[4] = PORT_PCR_MUX(3) | PORT_PCR_PE(1) | PORT_PCR_PS(1) | PORT_PCR_ODE_MASK;
#elif UART1_PINS == UART1_PTE0_PTE1
  PORTE->PCR[0] = PORT_PCR_MUX(3) | PORT_PCR_PE(1) | PORT_PCR_PS(1) | PORT_PCR_ODE_MASK;
  PORTEvPCR[1] = PORT_PCR_MUX(3) | PORT_PCR_PE(1) | PORT_PCR_PS(1) | PORT_PCR_ODE_MASK;
#else
#error Invalid UART1 Port Pin configuration!
#endif

  // set the baudrate into the BDH (first) and BDL (second) register. KRM1215ff
  uint32_t bd = (CORECLOCK / (16 * baudrate));
  UART1->BDH = (bd >> 8) & 0x1F;
  UART1->BDL = bd & 0xFF;

  // enable uart receiver, receiver interrupt and transmitter as well as
  // enable and set the rx/tx interrupt in the nested vector interrupt controller (NVIC)
  UART1->C2 = UART_C2_RIE_MASK | UART_C2_RE_MASK | UART_C2_TE_MASK;
  NVIC_SetPriority(UART1_RX_TX_IRQn, PRIO_UART1);
  NVIC_EnableIRQ(UART1_RX_TX_IRQn);

  // enable the error interrupts of the uart and configure the NVIC
  UART1->C3 = UART_C3_ORIE_MASK | UART_C3_NEIE_MASK | UART_C3_FEIE_MASK;
  NVIC_SetPriority(UART1_ERR_IRQn, PRIO_UART1);
  NVIC_EnableIRQ(UART1_ERR_IRQn);
}
#endif
