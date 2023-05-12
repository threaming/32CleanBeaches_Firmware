/*
 * Counting.c
 *
 *  Created on: 11.05.2023
 *      Author: Ming
 */

#include "CleanBeaches/Counting.h"
#include "fsl_pint.h"
#include "fsl_syscon.h"
#include "McuWait.h"
#include "CleanBeaches/Uart.h"

#define PINT_ZIGI 	kSYSCON_GpioPort0Pin26ToPintsel
#define PINT_PET 	kSYSCON_GpioPort0Pin27ToPintsel
#define PINT_BIER 	kSYSCON_GpioPort0Pin28ToPintsel

#define kPINT_ZIGI kPINT_PinInt0
#define kPINT_PET kPINT_PinInt1
#define kPINT_BIER kPINT_PinInt2

#define DBNC_DELAYMS 50

/* Global Variables */
static uint32_t cnt[3] = {0, 0, 0};

void pint_zigi_callback(pint_pin_int_t pintr, uint32_t pmatch_status)
{
	PINT_DisableCallbackByIndex(PINT, kPINT_ZIGI);
    cnt[ZIGI]++;
    Uart_SendString("ZIGI ");
	Uart_SendNum32(cnt[ZIGI]);
	Uart_SendString("\n");
    McuWait_Waitms(DBNC_DELAYMS);
    PINT_EnableCallbackByIndex(PINT, kPINT_ZIGI);
}

void pint_pet_callback(pint_pin_int_t pintr, uint32_t pmatch_status)
{
	PINT_DisableCallbackByIndex(PINT, kPINT_PET);
    cnt[PET]++;
    Uart_SendString("PET ");
	Uart_SendNum32(cnt[PET]);
	Uart_SendString("\n");
    McuWait_Waitms(DBNC_DELAYMS);
    PINT_EnableCallbackByIndex(PINT, kPINT_PET);
}

void pint_bier_callback(pint_pin_int_t pintr, uint32_t pmatch_status)
{
	PINT_DisableCallbackByIndex(PINT, kPINT_BIER);
    cnt[BIER]++;
    Uart_SendString("BIER ");
	Uart_SendNum32(cnt[BIER]);
	Uart_SendString("\n");
    McuWait_Waitms(DBNC_DELAYMS);
    PINT_EnableCallbackByIndex(PINT, kPINT_BIER);
}

uint32_t GetCnt(objects_t object){
	return cnt[object];
}

void Counter_Init(void){
	/* Connect trigger sources to PINT */
	SYSCON_AttachSignal(SYSCON, kPINT_ZIGI, PINT_ZIGI);
	SYSCON_AttachSignal(SYSCON, kPINT_PET, PINT_PET);
	SYSCON_AttachSignal(SYSCON, kPINT_BIER, PINT_BIER);

	/* Initialize PINT */
	PINT_Init(PINT);

	/* Setup Pin Interrupt 0 for rising edge */
	PINT_PinInterruptConfig(PINT, kPINT_ZIGI, kPINT_PinIntEnableRiseEdge, pint_zigi_callback);
	/* Enable callbacks for PINT0 by Index */
	PINT_EnableCallbackByIndex(PINT, kPINT_ZIGI);

	/* Setup Pin Interrupt 1 for rising edge */
	PINT_PinInterruptConfig(PINT, kPINT_PET, kPINT_PinIntEnableRiseEdge, pint_pet_callback);
	/* Enable callbacks for PINT0 by Index */
	PINT_EnableCallbackByIndex(PINT, kPINT_PET);

	/* Setup Pin Interrupt 2 for rising edge */
	PINT_PinInterruptConfig(PINT, kPINT_BIER, kPINT_PinIntEnableRiseEdge, pint_bier_callback);
	/* Enable callbacks for PINT0 by Index */
	PINT_EnableCallbackByIndex(PINT, kPINT_BIER);

	Uart_SendString("ZIGI 0\nPET 0\nBIER 0\n");		// initialize count ended
}

void Counter_Deinit(void){
	/* nothing to do */
}

