/**
 *--------------------------------------------------------------------\n
 *          HSLU T&A Hochschule Luzern Technik+Architektur            \n
 *--------------------------------------------------------------------\n
 *
 * \brief         System initialization and utils.
 * \file
 * \author        Christian Jost, christian.jost@hslu.ch
 * \date          16.02.2020
 *
 *--------------------------------------------------------------------
 */
#ifndef SYSTEM_H_
#define SYSTEM_H_

typedef struct
{
  uint16_t stackFree;
  uint16_t stackUsed;
} tStackData;

tStackData utilCheckStackSize(void);

#endif /* SYSTEM_H_ */
