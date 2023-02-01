/*
 * sht30.c
 *
 *  Created on: 10.12.2018
 *      Author: zajost
 */

#include "platform.h"
#include "sht30.h"
#include "i2c.h"
#include "term.h"
#include "util.h"


int16_t temp, humidity;

void sht30Read(void)
{
  uint8_t wData[1] = {0xFD};
  uint8_t rData[6];
  int16_t v;

  tError result = i2cRead(0x44, rData, sizeof(rData));

  if (result == EC_SUCCESS)
  {
    v = rData[0] << 8 | rData[1];
    temp = ((int32_t)1750 * v) / 0xffff - 450;

    v = rData[3] << 8 | rData[4];
    humidity = ((int32_t)1250 * v) / 0xffff - 60;
  }

  i2cWrite(0x44, wData, sizeof(wData));



//  uint8_t wData[2] = {0x2C, 0x06};
//  uint8_t rData[6];
//  int16_t v;
//  tError result = i2cRead(0x44, rData, sizeof(rData));
//
//  if (result == EC_SUCCESS)
//  {
//    v = rData[0] << 8 | rData[1];
//    temp = ((int32_t)1750 * v) / 0xffff - 450;
//
//    v = rData[3] << 8 | rData[4];
//    humidity = ((int32_t)1000 * v) / 0xffff;
//  }
//
//  i2cWrite(0x44, wData, sizeof(wData));
}


void sht30DoWork(void)
{
  static int16_t oldTemp = -9999;
  static int16_t oldHumidity = -9999;
  char str[16];

  sht30Read();

  if (oldTemp != temp)
  {
    oldTemp = temp;
    termWrite("Temp ");
    utilNum16uToStr(str, sizeof(str), temp);
    termWriteLine(str);
  }

  if (oldHumidity != humidity)
  {
    oldHumidity = humidity;
    termWrite("Humidity ");
    utilNum16uToStr(str, sizeof(str), humidity);
    termWriteLine(str);
  }
}



void sht30Init(void)
{

}


