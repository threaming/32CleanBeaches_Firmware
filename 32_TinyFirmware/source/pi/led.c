/*
 * led.c
 *
 *  Created on: 30.11.2018
 *      Author: zajost
 */


#include <string.h>
#include "platform.h"
#include "led.h"
#include "term.h"


#define LED_PIN_RED     (1<<11)
#define LED_PIN_YELLOW  (1<<10)
#define LED_PIN_GREEN   (1<<9)
#define LED_PIN_BLUE    (1<<8)


static tCommandLineHandler clh;


void ledEnable(tLed led, bool enable)
{
  if (enable)
  {
    switch (led)
    {
      case ledRed: GPIOC->PSOR = LED_PIN_RED; break;
      case ledYellow : GPIOC->PSOR = LED_PIN_YELLOW; break;
      case ledGreen: GPIOC->PSOR = LED_PIN_GREEN; break;
      case ledBlue: GPIOC->PSOR = LED_PIN_BLUE; break;
    }
  }
  else
  {
    switch (led)
    {
      case ledRed: GPIOC->PCOR = LED_PIN_RED; break;
      case ledYellow : GPIOC->PCOR = LED_PIN_YELLOW; break;
      case ledGreen: GPIOC->PCOR = LED_PIN_GREEN; break;
      case ledBlue: GPIOC->PCOR = LED_PIN_BLUE; break;
    }
  }
}


#define CheckCommand(buf, command)   (strncmp(buf, command, sizeof(command)-1) == 0)
/**
 * This function parses one command line, executes the command and returns the status
 *
 * @param[in] cmd
 *   the null terminated string to parse
 * @returns
 *   EC_SUCCESS if there was a valid command
 *   EC_INVALID_CMD if the command was unknown or invalid
 */
tError ledParseCommand(const char *cmd)
{
  tError result = EC_INVALID_ARG;
  if (strcmp(cmd, "help") == 0)
  {
    termWriteLine("led commands:");
    termWriteLine("  help");
    termWriteLine("  red on");
    termWriteLine("  red off");
    termWriteLine("  green on");
    termWriteLine("  green off");
    termWriteLine("  blue on");
    termWriteLine("  blue off");
    termWriteLine("  yellow on");
    termWriteLine("  yellow off");
    result = EC_SUCCESS;
  }
  else if (CheckCommand(cmd, "red on"))
  {
    ledEnable(ledRed, TRUE);
    result = EC_SUCCESS;
  }
  else if (CheckCommand(cmd, "red off"))
  {
    ledEnable(ledRed, FALSE);
    result = EC_SUCCESS;
  }
  else if (CheckCommand(cmd, "yellow on"))
  {
    ledEnable(ledYellow, TRUE);
    result = EC_SUCCESS;
  }
  else if (CheckCommand(cmd, "yellow off"))
  {
    ledEnable(ledYellow, FALSE);
    result = EC_SUCCESS;
  }
  else if (CheckCommand(cmd, "green on"))
  {
    ledEnable(ledGreen, TRUE);
    result = EC_SUCCESS;
  }
  else if (CheckCommand(cmd, "green off"))
  {
    ledEnable(ledGreen, FALSE);
    result = EC_SUCCESS;
  }
  else if (CheckCommand(cmd, "blue on"))
  {
    ledEnable(ledBlue, TRUE);
    result = EC_SUCCESS;
  }
  else if (CheckCommand(cmd, "blue off"))
  {
    ledEnable(ledBlue, FALSE);
    result = EC_SUCCESS;
  }
  return result;
}

void ledInit(void)
{
  PORTC->PCR[8] = PORT_PCR_MUX(1);
  PORTC->PCR[9] = PORT_PCR_MUX(1);
  PORTC->PCR[10] = PORT_PCR_MUX(1);
  PORTC->PCR[11] = PORT_PCR_MUX(1);

  GPIOC->PDDR = LED_PIN_RED | LED_PIN_YELLOW | LED_PIN_GREEN | LED_PIN_BLUE;

  // register terminal command line handler
  termRegisterCommandLineHandler(&clh, "led", "led red, yellow, green, blue", ledParseCommand);
}
