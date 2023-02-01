/**
 *--------------------------------------------------------------------\n
 *          HSLU T&A Hochschule Luzern Technik+Architektur            \n
 *--------------------------------------------------------------------\n
 *
 * \brief         driver for the serial communication (lpuart0 MC-Car)
 * \file
 * \author        Christian Jost, christian.jost@hslu.ch
 * \date          19.03.2020
 *
 *--------------------------------------------------------------------
 */
#include "platform.h"
#include "uart.h"

#if LPUART0_EN


/**
 * the receive queue of this driver, implemented as ring buffer
 */
static char rxBuf[LPUART0_RX_BUF_SIZE];
static volatile uint16_t rxBufCount;
static uint16_t rxBufWritePos;
static uint16_t rxBufReadPos;

/**
 * the transmit queue of this driver, implemented as ring buffer
 */
static char txBuf[LPUART0_TX_BUF_SIZE];
static volatile uint16_t txBufCount;
static uint16_t txBufWritePos;
static uint16_t txBufReadPos;

#define ENABLE_LPUART0_INTERRUPTS()     NVIC_EnableIRQ(LPUART0_IRQn)
#define DISABLE_LPUART0_INTERRUPTS()    NVIC_DisableIRQEx(LPUART0_IRQn)



/**
 * @brief LPUART Interrupt Service Routine
 * - Received bytes are stored in the queue rxBuf
 * - Bytes in the queue txBuf are sent
 */
void LPUART0_IRQHandler(void)
{
  OnEnterLpUart0ISR();
  uint32_t status = LPUART0->STAT;
  uint8_t data = (uint8_t)LPUART0->DATA;

  if (status & LPUART_STAT_RDRF_MASK)
  {
    if (rxBufCount < LPUART0_RX_BUF_SIZE)
    {
      rxBuf[rxBufWritePos++] = data;
      rxBufCount++;
      if (rxBufWritePos == LPUART0_RX_BUF_SIZE) rxBufWritePos = 0;
    }
  }

  if (status & LPUART_STAT_TDRE_MASK)
  {
    if (txBufCount > 0)
    {
      LPUART0->DATA = txBuf[txBufReadPos++];
      txBufCount--;
      if (txBufReadPos == LPUART0_TX_BUF_SIZE) txBufReadPos = 0;
    }
    else
    {
      LPUART0->CTRL &= ~LPUART_CTRL_TIE_MASK;
    }
  }

  // clear the error flags in the STAT register
  LPUART0->STAT |= (LPUART_STAT_OR_MASK | LPUART_STAT_NF_MASK | LPUART_STAT_FE_MASK);
  OnExitLpUart0ISR();
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
void lpuart0WriteChar(char ch)
{
  while(txBufCount >= LPUART0_TX_BUF_SIZE);
  txBuf[txBufWritePos++] = ch;
  if (txBufWritePos == LPUART0_TX_BUF_SIZE) txBufWritePos = 0;
  DISABLE_LPUART0_INTERRUPTS();
  txBufCount++;
  ENABLE_LPUART0_INTERRUPTS();
  LPUART0->CTRL |= LPUART_CTRL_TIE_MASK;
}

/**
 * Writes a null terminated string in the send buffer. If the
 * string is null, the function returns immediately.
 *
 * @param[in] str
 *   the null terminated string to send
 */
void lpuart0Write(const char *str)
{
  if (str == NULL) return;
  while (*str != '\0') lpuart0WriteChar(*str++);
}

/**
 * Writes a null terminated string in the send buffer. If the
 * string is null, only a new new line character is sent.
 *
 * @param[in] str
 *   the null terminated string to send
 */
void lpuart0WriteLine(const char *str)
{
  lpuart0Write(str);
  lpuart0WriteChar(NEW_LINE);
}


/**
 * Reads one char out of the rxBuf queue. The function blocks
 * until there is at least one byte in the queue.
 *
 * @return
 *   the received byte
 */
char lpuart0ReadChar(void)
{
  char ch;
  while (rxBufCount == 0);
  ch = rxBuf[rxBufReadPos++];
  if (rxBufReadPos == LPUART0_RX_BUF_SIZE) rxBufReadPos = 0;
  DISABLE_LPUART0_INTERRUPTS();
  rxBufCount--;
  ENABLE_LPUART0_INTERRUPTS();
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
uint16_t lpuart0ReadLine(char *str, uint16_t length)
{
  uint16_t i;
  for (i=1; i<length; i++)
  {
    *str = lpuart0ReadChar();
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
bool lpuart0HasLineReceived(void)
{
  uint16_t i;
  uint16_t index = rxBufReadPos;

  for (i=0; i<rxBufCount; i++)
  {
    if (rxBuf[index++] == NEW_LINE) return TRUE;
    if (index == LPUART0_RX_BUF_SIZE) index = 0;
  }
  return FALSE;
}

/**
 * Returns the number of bytes in the receiver queue.
 *
 * @returns
 *   the number of bytes in the receiver queue.
 */
uint16_t lpuart0RxBufCount(void)
{
  return rxBufCount;
}

#if LPUART0_PRINTF_EN
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
  while (i--) lpuart0WriteChar(*(uint8_t*)buf++);
  return count;
}
#endif

#if LPUART0_SCANF_EN
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
  *buf = lpuart0ReadChar();
  return 1;
}
#endif

/**
 * initializes the lpuart with the desired baud rate.
 *
 * @details
 *   The lpuart is different from the uart0 or uart1!
 *
 *   MC-Car:
 *   - PTB10 Mux3:LPUART0_RX
 *   - PTB11 Mux3:LPUART0_TX
 *
 *   APROG:
 *   - PTC3  Mux7:LPUART0_RX
 *   - PTC4  Mux7:LPUART0_TX
 */
void lpuart0Init(uint32_t baudrate)
{
  txBufReadPos = txBufWritePos = txBufCount = 0;
  rxBufReadPos = rxBufWritePos = rxBufCount = 0;

  // configure clock gating and clock source to MCGPLLCLK
  SIM->SCGC6 |= SIM_SCGC6_LPUART0_MASK;
  //SIM->SOPT2 |= SIM_SOPT2_LPUARTSRC(1);

  // configure port multiplexing
#if LPUART0_PINS == LPUART0_PTB10_PTB11
  PORTB->PCR[10] = PORT_PCR_MUX(3);
  PORTB->PCR[11] = PORT_PCR_MUX(3);
#elif LPUART0_PINS == LPUART0_PTC3_PTC4
  PORTC->PCR[3] = PORT_PCR_MUX(7);
  PORTC->PCR[4] = PORT_PCR_MUX(7);
#else
#error Invalid LPUART0 Port Pin configuration!
#endif

  // set the baudrate
  //uint32_t bd = (CORECLOCK / (16 * baudrate));
  uint32_t bd = (((10 * OSCERCLOCK) / (14 * baudrate) + 5) /10);
  LPUART0->BAUD = LPUART_BAUD_SBR(bd) | LPUART_BAUD_OSR(14-1);

  // enable uart receiver, receiver interrupt and transmitter as well as
  // the error interrupts of the lpuart and configure the NVIC
  LPUART0->CTRL |= LPUART_CTRL_RE_MASK | LPUART_CTRL_RIE_MASK | LPUART_CTRL_TE_MASK |
      LPUART_CTRL_ORIE_MASK | LPUART_CTRL_NEIE_MASK | LPUART_CTRL_FEIE_MASK;
  NVIC_SetPriority(LPUART0_IRQn, PRIO_LPUART0);
  NVIC_EnableIRQ(LPUART0_IRQn);
}
#endif
