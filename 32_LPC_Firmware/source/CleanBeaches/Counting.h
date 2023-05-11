/*
 * Counting.h
 *
 *  Created on: 11.05.2023
 *      Author: Ming
 */

#ifndef COUNTING_H_
#define COUNTING_H_

#include <stdint.h>

typedef enum {
	ZIGI,
	PET,
	BIER
} objects_t;

void Counter_Init(void);
void Counter_Deinit(void);
uint32_t GetCnt(objects_t object);

#endif /* COUNTING_H_ */
