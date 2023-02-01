/*
 * led.h
 *
 *  Created on: 30.11.2018
 *      Author: zajost
 */

#ifndef SOURCES_PI_LED_H_
#define SOURCES_PI_LED_H_


typedef enum
{
  ledRed,
  ledGreen,
  ledYellow,
  ledBlue
} tLed;



void ledEnable(tLed led, bool enable);
void ledInit(void);


#endif /* SOURCES_PI_LED_H_ */
