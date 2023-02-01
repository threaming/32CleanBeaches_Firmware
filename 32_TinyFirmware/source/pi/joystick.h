/*
 * joystick.h
 *
 *  Created on: 03.12.2018
 *      Author: zajost
 */

#ifndef SOURCES_PI_JOYSTICK_H_
#define SOURCES_PI_JOYSTICK_H_


typedef enum
{
  jbNone = 0,
  jbLeft = 1,
  jbRight = 2,
  jbUp = 4,
  jbDown = 8,
  jbCenter = 16
} tJoystickButton;

tJoystickButton joystickGetState(void);
void joystickDoWork(void);
void joystickInit(void);



#endif /* SOURCES_PI_JOYSTICK_H_ */
