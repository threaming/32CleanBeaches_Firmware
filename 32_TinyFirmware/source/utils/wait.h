/**
 *--------------------------------------------------------------------\n
 *          HSLU T&A Hochschule Luzern Technik+Architektur            \n
 *--------------------------------------------------------------------\n
 *
 * \brief         wait util (using cycle counter)
 * \file
 * \author        Christian Jost, christian.jost@hslu.ch
 * \date          22.05.2020
 *
 *--------------------------------------------------------------------
 */
#ifndef SOURCES_UTILS_WAIT_H_
#define SOURCES_UTILS_WAIT_H_

#define waitTimeoutOccuredMs(start, timeoutMs)      (waitGetTimeMs(start) > timeoutMs)
#define waitTimeoutOccuredUs(start, timeoutUs)      (waitGetTimeMs(start) > timeoutUs)

uint32_t waitStartWatch(void);
uint32_t waitGetTimeMs(uint32_t startTime);
uint32_t waitGetTimeUs(uint32_t startTime);

void waitInit(void);
void waitMs(uint16_t ms);
void waitUs(uint16_t us);

#endif /* SOURCES_UTILS_WAIT_H_ */
