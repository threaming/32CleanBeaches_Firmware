/**
 *--------------------------------------------------------------------\n
 *          HSLU T&A Hochschule Luzern Technik+Architektur            \n
 *--------------------------------------------------------------------\n
 *
 * \brief         TinyK22 for APROG
 * \file
 * \author        Christian Jost, christian.jost@hslu.ch
 * \date          22.05.2020
 *
 *--------------------------------------------------------------------
 */
#include "platform.h"
#include "ftm0.h"
#include "ftm3.h"
#include "term.h"
#include "i2c.h"
#include "adc.h"
#include "util.h"
#include "uart.h"
#include "led.h"
#include "joystick.h"
#include "sht30.h"

// calulates the nr of TOF count for a given number of milliseconds
#define TOFS_MS(x)   ((uint16_t)(((FTM3_CLOCK / 1000) * x) / (FTM3_MODULO + 1)))


void processUartGatway(bool tof)
{
  static int16_t count = 0;
  static bool dataReceived = false;
  if (lpuart0RxBufCount())
  {
    uart0WriteChar(lpuart0ReadChar());
    GPIOC->PSOR = 1<<2;
    dataReceived = true;
  }
  if (uart0RxBufCount())
  {
    lpuart0WriteChar(uart0ReadChar());
    GPIOC->PSOR = 1<<2;
    dataReceived = true;
  }

  if (tof && dataReceived)   // check for timer overflow
  {
    if (count++ >= TOFS_MS(50))
    {
      count=0;
      dataReceived = false;
      GPIOC->PCOR = 1<<2; // led on
    }
  }
}


void processJoystick(bool tof)
{
  static int16_t count = 0;
  if (tof)                          // check for timer overflow
  {
    count++;                        // count the number of TOF's
    if (count >= TOFS_MS(50))       // check if number of TOF's is equal or greater 250ms
    {
      count = 0;                    // restart counter to wait again 50ms
      joystickDoWork();
    }
  }
}

void processSHT40(bool tof)
{
  static int16_t count = 0;
  if (tof)
  {
    count++;
    if (count >= TOFS_MS(1000))       // check if number of TOF's is equal or greater 250ms
    {
      count=0;
      sht30DoWork();
    }
  }
}


void processRaspyLed(bool tof)
{
  static int16_t count = 0;
  if (tof)
  {
    count++;
    if (count >= TOFS_MS(500))       // check if number of TOF's is equal or greater 250ms
    {
      count=0;
      GPIOC->PTOR = 1<<2;
    }
  }
}


/**
 * The main function
 */
void main(void)
{
  bool tof = FALSE;
  bool sensOn = FALSE;

  // Wake Raspi
  GPIOA->PDDR |= 1<<5;
  PORTA->PCR[5] = PORT_PCR_MUX(1);
  GPIOA->PSOR = 1<<5; // Leitung Low => Wake Raspi

  // I2C-Bus Leitung ---___--- => Wake Raspi
  PORTB->PCR[0] = PORT_PCR_MUX(1);
  GPIOB->PDDR |= 1<<0;
  GPIOB->PCOR = 1<<0;
  for (int i=0; i<5000000; i++) {}
  GPIOA->PDDR &= ~(1<<5);



  joystickInit();
  ftm3Init();                           // init flex timer 3
  ledInit();
  i2cInit();
  sht30Init();
  GPIOC->PDDR |= 1<<2;                  // Set port direction of the blue Led on tinyK22 as output
  PORTC->PCR[2] = PORT_PCR_MUX(1);      // configure port mux of the blue led to GPIO

  // PTA1 as Input
  PORTA->PCR[1] = PORT_PCR_MUX(1) | PORT_PCR_PE(1) | PORT_PCR_PS(0);

  termInit(115200);                     // init terminal with a baudrate of 115200

  lpuart0Init(115200);
  uart0Init(115200);
  lpuart0WriteLine("TinyK22 uart gateway ready.");
  termWriteLine("TinyK22 ready.");
  while(true)
  {
    processUartGatway(tof);

    if ((GPIOA->PDIR & (1<<1)) && !sensOn)
    {
      // Betrieb über TinyK22
      sensOn = TRUE;
      GPIOA->PCOR = 1<<5;
      termWriteLine("TinyK22 sensors ON");
    }
    if (!(GPIOA->PDIR & (1<<1)) && sensOn)
    {
      // Betrieb über Raspberry Pi
      sensOn = FALSE;
      GPIOA->PSOR = 1<<5;
      termWriteLine("TinyK22 sensors OFF");
    }

    if (sensOn)
    {
      processRaspyLed(tof);
      processJoystick(tof);
      processSHT40(tof);
    }

    termDoWork();                      // process commands from the terminal

    tof = FALSE;
    if (FTM3->SC & FTM_SC_TOF_MASK)    // check for timer overflow
    {
      tof = TRUE;
      FTM3->SC &= ~FTM_SC_TOF_MASK;    // overflow occurred => clear TOF flag
    }
  }


/*
  while(TRUE)
  {
    if (raspiTerm)
    {
      if (uart0RxBufCount())
      {
        uart1WriteChar(uart0ReadChar());
        GPIOC->PSOR = 1<<2;
      }
      if (uart1RxBufCount())
      {
        uart0WriteChar(uart1ReadChar());
        GPIOC->PSOR = 1<<2;
      }
      if (!count)
      {
        GPIOC->PCOR = 1<<2; // led on
      }
    }
    else
    {
      termDoWork();                   // process commands from the terminal
      GPIOC->PSOR = 1<<2; // led off
    }

    if (FTM3->SC & FTM_SC_TOF_MASK)    // check for timer overflow
    {
      FTM3->SC &= ~FTM_SC_TOF_MASK;    // overflow occurred => clear TOF flag
      count++;                         // count the number of TOF's
      if (count >= TOFS_MS(50))        // check if number of TOF's is equal or greater 250ms
      {
        count = 0;                     // restart counter to wait again 50ms

        if (!raspiTerm)
        {
          joystickDoWork();
        }

        jb = joystickGetState();
        if (jb == jbUp) joystickCount++;
        else if (jb == jbDown) joystickCount--;
        else joystickCount = 0;
        if (joystickCount == 40) raspiTerm = TRUE;
        if (joystickCount == -40) raspiTerm = FALSE;
      }

      count2++;
      if (count2 >= TOFS_MS(1000))       // check if number of TOF's is equal or greater 250ms
      {
        count2=0;
        if (!raspiTerm)
        {
          sht30DoWork();
        }
      }
    }
  }
  */
}
