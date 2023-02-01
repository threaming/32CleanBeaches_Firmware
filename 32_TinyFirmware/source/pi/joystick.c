/*
 * joystick.c
 *
 *  Created on: 03.12.2018
 *      Author: zajost
 */
#include "platform.h"
#include "joystick.h"
#include "term.h"
#include "util.h"


#define JOYSTICK_PIN_UP       (1<<3)
#define JOYSTICK_PIN_DN       (1<<2)
#define JOYSTICK_PIN_LEFT     (1<<13)
#define JOYSTICK_PIN_RIGHT    (1<<12)
#define JOYSTICK_PIN_CENTER   (1<<16)


tJoystickButton joystickGetState(void)
{
  tJoystickButton state = jbNone;
  if (!(GPIOB->PDIR & JOYSTICK_PIN_UP)) state |= jbUp;
  if (!(GPIOB->PDIR & JOYSTICK_PIN_DN)) state |= jbDown;
  if (!(GPIOA->PDIR & JOYSTICK_PIN_LEFT)) state |= jbLeft;
  if (!(GPIOA->PDIR & JOYSTICK_PIN_RIGHT)) state |= jbRight;
  if (!(GPIOB->PDIR & JOYSTICK_PIN_CENTER)) state |= jbCenter;
  return state;
}

void joystickDoWork(void)
{
  static tJoystickButton oldState = jbNone;
  char str[16];

  tJoystickButton newState = joystickGetState();
  if (oldState != newState)
  {
    oldState = newState;
    utilNum16uToStr(str, sizeof(str), newState);
    termWrite("joystick ");
    termWriteLine(str);
  }
}


void joystickInit(void)
{
  PORTA->PCR[12] = PORT_PCR_MUX(1) | PORT_PCR_PS(1) | PORT_PCR_PE(1);
  PORTA->PCR[13] = PORT_PCR_MUX(1) | PORT_PCR_PS(1) | PORT_PCR_PE(1);
  PORTB->PCR[2] = PORT_PCR_MUX(1) | PORT_PCR_PS(1) | PORT_PCR_PE(1);
  PORTB->PCR[3] = PORT_PCR_MUX(1) | PORT_PCR_PS(1) | PORT_PCR_PE(1);
  PORTB->PCR[16] = PORT_PCR_MUX(1) | PORT_PCR_PS(1) | PORT_PCR_PE(1);

  //GPIOB_PDDR = JOYSTICK_PIN_UP | JOYSTICK_PIN_DN | JOYSTICK_PIN_LEFT | JOYSTICK_PIN_RIGHT | JOYSTICK_PIN_CENTER;
}
