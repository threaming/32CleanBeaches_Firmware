/**
 *--------------------------------------------------------------------\n
 *          HSLU T&A Hochschule Luzern Technik+Architektur            \n
 *--------------------------------------------------------------------\n
 *
 * \brief         ADC-Driver
 * \file
 * \author        Christian Jost, christian.jost@hslu.ch
 * \date          22.05.2020
 *
 *--------------------------------------------------------------------
 */

#ifndef SOURCES_UTILS_ADC0_H_
#define SOURCES_UTILS_ADC0_H_

#define ADC_CH_DP0                 0
#define ADC_CH_DM0                19
#define ADC_CH_TEMPERATURE        26


uint16_t adc0Get16BitValue(uint8_t channel);
uint32_t adc0GetVoltage(uint8_t channel);
int16_t adc0GetTemperature(void);
void adc0Init(void);

#endif /* SOURCES_UTILS_ADC0_H_ */
