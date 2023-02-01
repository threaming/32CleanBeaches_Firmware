/**
 *--------------------------------------------------------------------\n
 *          HSLU T&A Hochschule Luzern Technik+Architektur            \n
 *--------------------------------------------------------------------\n
 *
 * \brief         wait util (using cycle counter)
 * \file
 * \author        Christian Jost, christian.jost@hslu.ch
 * \date          22.05.2020
 *
 *--------------------------------------------------------------------
 */
#include "platform.h"
#include "wait.h"

// addresses of registers
//volatile uint32_t *DWT_CONTROL = (uint32_t *)0xE0001000;
//volatile uint32_t *DWT_CYCCNT = (uint32_t *)0xE0001004;
//volatile uint32_t *DEMCR = (uint32_t *)0xE000EDFC;

// ToDo Check if time > MAX_TIME/2

#define DWT_CYCCNT      (DWT->CYCCNT)
//#define MAX_TIME        (1<<32 / CORECLOCK)

void waitStartCycleCounter(void)
{
  DWT->CTRL |= 1;   // Enable counter
}

void waitStopCycleCounter(void)
{
  DWT->CTRL &= ~1;   // Enable counter
}



//#define SS_SELECT_SLAVE_()     GPIOC_PCOR = (1<<1)           // PTC1 (low active)
//#define SS_UNSELECT_SLAVE_()   GPIOC_PSOR = (1<<1)           // PTC1 (low active)
//#define SS_SET_MUX_()          PORTC_PCR1 = PORT_PCR_MUX(1)  // PTC1 Mux as GPIO
//#define SS_SET_PDDR_()         GPIOC_PDDR |= (1<<1)          // PTC1 as output
//

void waitInit(void)
{
  DWT->CYCCNT;
  CoreDebug->DEMCR |= 0x01000000;;
  DWT->CYCCNT = 0;  // Reset Counter
  DWT->CTRL |= 1;   // Enable counter

//  SS_SET_MUX_();
//  SS_SET_PDDR_();
//  SS_UNSELECT_SLAVE_();
//
//  uint32_t start = waitStartWatch();
//  waitMs(60000);
//  waitMs(1000);
//  uint32_t stop = waitGetTimeMs(start);
}



uint32_t waitStartWatch(void)
{
  return DWT->CYCCNT;
}


uint32_t waitGetTimeMs(uint32_t startTime)
{
  uint32_t time = DWT_CYCCNT - startTime;
  return time / (CORECLOCK / 1000);
}


uint32_t waitGetTimeUs(uint32_t startTime)
{
  uint32_t time = DWT_CYCCNT - startTime;
  return time / (CORECLOCK / (1000 * 1000));
  //return ((uint64_t)startTime * 1000 * 1000) / CORECLOCK;
}


void waitMs(uint16_t ms)
{
  uint32_t timeout = ((CORECLOCK/1000) * ms) + DWT_CYCCNT;
  elogif(timeout & 0x80000000, "Overflow in waitMs %ld", ms);
  //while ((DWT_CYCCNT - timeout) > 0x80000000) {}
  while (((int32_t)(DWT_CYCCNT - timeout)) < 0) {}
}


void waitUs(uint16_t us)
{
  uint32_t timeout = (((CORECLOCK/1000) * us)/1000) + DWT_CYCCNT;// - 25;
  //while ((DWT_CYCCNT - timeout) > 0x80000000) {}
  while (((int32_t)(DWT_CYCCNT - timeout)) < 0) {}
}

