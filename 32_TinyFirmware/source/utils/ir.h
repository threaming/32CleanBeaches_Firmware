/**
 *--------------------------------------------------------------------\n
 *          HSLU T&A Hochschule Luzern Technik+Architektur            \n
 *--------------------------------------------------------------------\n
 *
 * \brief         Infrared Remote Control driver
 * \file
 * \author        Christian Jost, christian.jost@hslu.ch
 * \date          14.04.2020
 *
 *--------------------------------------------------------------------
 */

#ifndef SOURCES_UTILS_IR_H_
#define SOURCES_UTILS_IR_H_


#define Key0        ('0')
#define Key1        ('1')
#define Key2        ('2')
#define Key3        ('3')
#define Key4        ('4')
#define Key5        ('5')
#define Key6        ('6')
#define Key7        ('7')
#define Key8        ('8')
#define Key9        ('9')
#define KeyStar     ('*')
#define KeyHash     ('#')
#define KeyUp       ('U')
#define KeyDown     ('D')
#define KeyLeft     ('L')
#define KeyRight    ('R')
#define KeyOK       ('\r')
#define KeyUnknown  ('\0')



uint8_t irGetKey(void);
void irInit(void);


#define START_BIT_PULSE_TIME        9000.0e-6           // 9000 us pulse
#define START_BIT_PAUSE_TIME        4500.0e-6           // 4500 us pause
#define RSTART_BIT_PAUSE_TIME       2250.0e-6           // 2250 us pause
#define PULSE_TIME                   560.0e-6           //  560 us pulse
#define PAUSE_1_TIME                1690.0e-6           // 1690 us pause
#define PAUSE_0_TIME                 560.0e-6           //  560 us pause

#define MIN_TOLERANCE                 0.7       // -30%
#define MAX_TOLERANCE                 1.3       // +30%

// Number of timer ticks for each symbol
#define START_BIT_PULSE_LEN_MIN     ((uint16_t)(FTM0_CLOCK * START_BIT_PULSE_TIME * MIN_TOLERANCE + 0.5)-1)
#define START_BIT_PULSE_LEN_MAX     ((uint16_t)(FTM0_CLOCK * START_BIT_PULSE_TIME * MAX_TOLERANCE + 0.5)-1)
#define START_BIT_PAUSE_LEN_MIN     ((uint16_t)(FTM0_CLOCK * START_BIT_PAUSE_TIME * MIN_TOLERANCE + 0.5)-1)
#define START_BIT_PAUSE_LEN_MAX     ((uint16_t)(FTM0_CLOCK * START_BIT_PAUSE_TIME * MAX_TOLERANCE + 0.5)-1)
#define RSTART_BIT_PAUSE_LEN_MIN    ((uint16_t)(FTM0_CLOCK * RSTART_BIT_PAUSE_TIME * MIN_TOLERANCE + 0.5)-1)
#define RSTART_BIT_PAUSE_LEN_MAX    ((uint16_t)(FTM0_CLOCK * RSTART_BIT_PAUSE_TIME * MAX_TOLERANCE + 0.5)-1)
#define PULSE_LEN_MIN               ((uint16_t)(FTM0_CLOCK * PULSE_TIME * MIN_TOLERANCE + 0.5)-1)
#define PULSE_LEN_MAX               ((uint16_t)(FTM0_CLOCK * PULSE_TIME * MAX_TOLERANCE + 0.5)-1)
#define PAUSE_1_LEN_MIN             ((uint16_t)(FTM0_CLOCK * PAUSE_1_TIME * MIN_TOLERANCE + 0.5)-1)
#define PAUSE_1_LEN_MAX             ((uint16_t)(FTM0_CLOCK * PAUSE_1_TIME * MAX_TOLERANCE + 0.5)-1)
#define PAUSE_0_LEN_MIN             ((uint16_t)(FTM0_CLOCK * PAUSE_0_TIME * MIN_TOLERANCE + 0.5)-1)
#define PAUSE_0_LEN_MAX             ((uint16_t)(FTM0_CLOCK * PAUSE_0_TIME * MAX_TOLERANCE + 0.5)-1)


#if (FTM0_CLOCK > 5500000)
// Calculation of 5400000:
// x = (1/START_BIT_PULSE_TIME * MAX_TOLERANCE) * 2^16)
// x = (1/9000e-6 * 1.3) * 2^16
// x = 5601400 => ~5500000 with some timer ticks reserve
#error WARNING: FTM0 Frequency to high to detect a start bit!!!
#endif


#endif /* SOURCES_UTILS_IR_H_ */
