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
#ifndef SOURCES_FTM3_H_
#define SOURCES_FTM3_H_

#define FTM3_CLOCK              60000000  // 60 MHz
#define FTM3_MODULO               0x0FFF  // 4095


// calulate nr of TOF count for a given number of milliseconds
#define FTM3_TOFS_MS(x)         ((uint16_t)(((FTM3_CLOCK / 1000) * x) / (FTM3_MODULO + 1)))

void ftm3Init(void);



#endif /* SOURCES_FTM3_H_ */
