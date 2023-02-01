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

#include "platform.h"
#include "color.h"



/**
 * Converts a RGB color into the hsv model
 * http://de.wikipedia.org/wiki/RGB-Farbraum
 * http://de.wikipedia.org/wiki/HSV-Farbraum
 *
 * @param [in] rgb
 *   the source color (red, green, blue)
 *
 * @param [out] hsv
 *   the hsv model of the rgb color
 *   color->saturation [0..1000]
 *   color->value [0..1000]
 *   color->hue [0..360]
 */
void colorRGBtoHSV(tColorRGB *rgb, tColorHSV *hsv)
{
  int32_t h, min, max;
  h = 0;

  if (rgb->red == rgb->blue && rgb->red == rgb->green)
  {
    h = 0;
    max = min = rgb->red;
  }
  else if ((rgb->red >= rgb->blue) && (rgb->red >= rgb->green))
  {
    max = rgb->red;
    min = ((rgb->blue > rgb->green) ? rgb->green : rgb->blue);

    h = (60l * ((int32_t)rgb->green - rgb->blue)) / (max - min);
  }
  else if ((rgb->green >= rgb->blue) && (rgb->green >= rgb->red))
  {
    max = rgb->green;
    min = ((rgb->blue > rgb->red) ? rgb->red : rgb->blue);
    h = 120l + (60 * ((int32_t)rgb->blue - rgb->red)) / (max - min);
  }
  else if ((rgb->blue >= rgb->green) && (rgb->blue >= rgb->red))
  {
    max = rgb->blue;
    min = ((rgb->green > rgb->red) ? rgb->red : rgb->green);
    h = 240l + (60 * ((int32_t)rgb->red - rgb->green)) / (max - min);
  }
  if (h < 0) h += 360;

  if (max == 0) hsv->saturation = 0;
  else hsv->saturation = (uint16_t)( (1000*(max-min))/max);

  hsv->hue = (uint16_t)h;
  hsv->value = (uint16_t)((1000 * max)/255);
}

void colorHueToRGB(uint16_t hue, tColorRGB *pColorRGB)
{
  if (hue > 360) hue = 360;

  if (hue >= 300)
  {
    pColorRGB->red = 255;
    pColorRGB->green = 0;
    pColorRGB->blue = (uint8_t)((uint32_t)255 * (359 - hue) / 60);
  }
  else if (hue >= 240)
  {
    pColorRGB->blue = 255;
    pColorRGB->green = 0;
    pColorRGB->red = (uint8_t)(255 - ((uint32_t)255 * (299 - hue) / 60));
  }
  else if (hue >= 180)
  {
    pColorRGB->blue = 255;
    pColorRGB->red = 0;
    pColorRGB->green = (uint8_t)((uint32_t)255 * (239 - hue) / 60);
  }
  else if (hue >= 120)
  {
    pColorRGB->green = 255;
    pColorRGB->red = 0;
    pColorRGB->blue = (uint8_t)(255 - ((uint32_t)255 * (179 - hue) / 60));
  }
  else if (hue >= 60)
  {
    pColorRGB->green = 255;
    pColorRGB->blue = 0;
    pColorRGB->red = (uint8_t)((uint32_t)255 * (119 - hue) / 60);
  }
  else
  {
    pColorRGB->red = 255;
    pColorRGB->blue = 0;
    pColorRGB->green = (uint8_t)(255 - ((uint32_t)255 * (59 - hue) / 60));
  }
}
