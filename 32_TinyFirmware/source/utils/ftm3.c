/**
 *--------------------------------------------------------------------\n
 *          HSLU T&A Hochschule Luzern Technik+Architektur            \n
 *--------------------------------------------------------------------\n
 *
 * \brief         Common settings of the FTM3
 * \file
 * \author        Christian Jost, christian.jost@hslu.ch
 * \date          15.04.2020
 *
 *--------------------------------------------------------------------
 */

#include "platform.h"
#include "ftm3.h"

/**
 * Default handler is called if there is no handler for the FTM0 channel or tof interrupt
 */
void Default_Handler_FTM3()
{
  __asm("bkpt"); // Still a hacker? ;-)
}


void FTM3CH0_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler_FTM3")));
void FTM3CH1_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler_FTM3")));
void FTM3CH2_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler_FTM3")));
void FTM3CH3_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler_FTM3")));
void FTM3CH4_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler_FTM3")));
void FTM3CH5_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler_FTM3")));
void FTM3CH6_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler_FTM3")));
void FTM3CH7_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler_FTM3")));
void FTM3TOF_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler_FTM3")));


#define CHF_CHIE_MASK       (FTM_CnSC_CHF_MASK | FTM_CnSC_CHIE_MASK)
#define TOF_TOIE_MASK       (FTM_SC_TOF_MASK | FTM_SC_TOIE_MASK)

/**
 * Interrupt handler to distribute the different interrupt sources of the FTM:
 * - channel 0..7
 * - timer overflow
 */
void FTM3_IRQHandler(void)
{
  if ((FTM3->CONTROLS[0].CnSC & CHF_CHIE_MASK) == CHF_CHIE_MASK) FTM3CH0_IRQHandler();
  if ((FTM3->CONTROLS[1].CnSC & CHF_CHIE_MASK) == CHF_CHIE_MASK) FTM3CH1_IRQHandler();
  if ((FTM3->CONTROLS[2].CnSC & CHF_CHIE_MASK) == CHF_CHIE_MASK) FTM3CH2_IRQHandler();
  if ((FTM3->CONTROLS[3].CnSC & CHF_CHIE_MASK) == CHF_CHIE_MASK) FTM3CH3_IRQHandler();
  if ((FTM3->CONTROLS[4].CnSC & CHF_CHIE_MASK) == CHF_CHIE_MASK) FTM3CH4_IRQHandler();
  if ((FTM3->CONTROLS[5].CnSC & CHF_CHIE_MASK) == CHF_CHIE_MASK) FTM3CH5_IRQHandler();
  if ((FTM3->CONTROLS[6].CnSC & CHF_CHIE_MASK) == CHF_CHIE_MASK) FTM3CH6_IRQHandler();
  if ((FTM3->CONTROLS[7].CnSC & CHF_CHIE_MASK) == CHF_CHIE_MASK) FTM3CH7_IRQHandler();
  if ((FTM3->SC & TOF_TOIE_MASK) == TOF_TOIE_MASK) FTM3TOF_IRQHandler();
}

void ftm3Init(void)
{
  // set clockgating for FTM3
  SIM->SCGC6 |= SIM_SCGC6_FTM3(1);

  // sets the modulo
  FTM3->MOD = FTM3_MODULO;

  // configure the timer with "system clock" as clocksource and with a "Prescaler" of 1 => 60 MHz
  FTM3->SC = FTM_SC_CLKS(1) |  FTM_SC_PS(0);

  // Enable FTM3 interrupt on NVIC with Prio: PRIO_FTM3 (defined in platform.h)
  NVIC_SetPriority(FTM3_IRQn, PRIO_FTM3);       // set interrupt priority
  NVIC_EnableIRQ(FTM3_IRQn);                    // enable interrupt
}
