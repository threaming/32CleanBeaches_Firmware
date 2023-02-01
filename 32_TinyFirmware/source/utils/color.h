/**
 *--------------------------------------------------------------------\n
 *          HSLU T&A Hochschule Luzern Technik+Architektur            \n
 *--------------------------------------------------------------------\n
 *
 * \brief         color utils
 * \file
 * \author        Christian Jost, christian.jost@hslu.ch
 * \date          25.04.2020
 *
 *--------------------------------------------------------------------
 */

#ifndef SOURCES_UTILS_COLOR_H_
#define SOURCES_UTILS_COLOR_H_


typedef struct
{
  uint8_t green;
  uint8_t red;
  uint8_t blue;
} tColorRGB;

typedef struct
{
  uint16_t hue;
  uint16_t saturation;
  uint16_t value;
} tColorHSV;

void colorRGBtoHSV(tColorRGB *rgb, tColorHSV *hsv);
void colorHueToRGB(uint16_t hue, tColorRGB *pColorRGB);


#endif /* SOURCES_UTILS_COLOR_H_ */
