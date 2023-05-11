/*
 * Counting.c
 *
 *  Created on: 11.05.2023
 *      Author: Ming
 */

#include "CleanBeaches/Counting.h"
#include "fsl_pint.h"
#include "fsl_syscon.h"

#define PINT_ZIGI 	kSYSCON_GpioPort0Pin26ToPintsel
#define PINT_PET 	kSYSCON_GpioPort0Pin27ToPintsel
#define PINT_BIER 	kSYSCON_GpioPort0Pin28ToPintsel

/* Global Variables */
static uint32_t cnt[3] = {0, 0, 0};

void pint_zigi_callback(pint_pin_int_t pintr, uint32_t pmatch_status)
{
    cnt[ZIGI]++;
}

void pint_pet_callback(pint_pin_int_t pintr, uint32_t pmatch_status)
{
    cnt[PET]++;
}

void pint_bier_callback(pint_pin_int_t pintr, uint32_t pmatch_status)
{
    cnt[BIER]++;
}

uint32_t GetCnt(objects_t object){
	return cnt[object];
}

void Counter_Init(void){
	/* Connect trigger sources to PINT */
	SYSCON_AttachSignal(SYSCON, kPINT_PinInt0, PINT_ZIGI);
	SYSCON_AttachSignal(SYSCON, kPINT_PinInt1, PINT_PET);
	SYSCON_AttachSignal(SYSCON, kPINT_PinInt2, PINT_BIER);

	/* Initialize PINT */
	PINT_Init(PINT);

	/* Setup Pin Interrupt 0 for rising edge */
	PINT_PinInterruptConfig(PINT, kPINT_PinInt0, kPINT_PinIntEnableRiseEdge, pint_zigi_callback);
	/* Enable callbacks for PINT0 by Index */
	PINT_EnableCallbackByIndex(PINT, kPINT_PinInt0);

	/* Setup Pin Interrupt 1 for rising edge */
	PINT_PinInterruptConfig(PINT, kPINT_PinInt1, kPINT_PinIntEnableRiseEdge, pint_pet_callback);
	/* Enable callbacks for PINT0 by Index */
	PINT_EnableCallbackByIndex(PINT, kPINT_PinInt1);

	/* Setup Pin Interrupt 2 for rising edge */
	PINT_PinInterruptConfig(PINT, kPINT_PinInt2, kPINT_PinIntEnableRiseEdge, pint_bier_callback);
	/* Enable callbacks for PINT0 by Index */
	PINT_EnableCallbackByIndex(PINT, kPINT_PinInt2);
}

void Counter_Deinit(void){
	/* nothing to do */
}

