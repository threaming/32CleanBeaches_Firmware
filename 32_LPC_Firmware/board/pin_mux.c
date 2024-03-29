/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v13.0
processor: LPC845
package_id: LPC845M301JBD48
mcu_data: ksdk2_0
processor_version: 13.0.1
pin_labels:
- {pin_num: '11', pin_signal: PIO1_0/CAPT_X1, label: LEDG, identifier: LEDG}
- {pin_num: '14', pin_signal: PIO1_1/CAPT_X2, label: LEDB, identifier: LEDB}
- {pin_num: '16', pin_signal: PIO1_2/CAPT_X3, label: LEDR, identifier: LEDR}
- {pin_num: '26', pin_signal: PIO0_8/XTALIN, label: STEP_EN, identifier: STEP_EN}
- {pin_num: '25', pin_signal: PIO0_9/XTALOUT, label: STEP_MS1, identifier: STEP_MS1}
- {pin_num: '13', pin_signal: PIO0_10/I2C0_SCL, label: STEP_MS2, identifier: STEP_MS2}
- {pin_num: '12', pin_signal: PIO0_11/I2C0_SDA, label: STEP_STEP, identifier: STEP_STEP}
- {pin_num: '2', pin_signal: PIO0_13/ADC_10, label: STEP_DIR, identifier: STEP_DIR}
- {pin_num: '36', pin_signal: PIO0_0/ACMP_I1, label: LuxServo, identifier: LuxServo}
- {pin_num: '18', pin_signal: PIO0_26, label: LS_Zigi, identifier: LS_Zigi}
- {pin_num: '17', pin_signal: PIO0_27, label: LS_PET, identifier: LS_PET}
- {pin_num: '7', pin_signal: PIO0_28/WKTCLKIN, label: LS_Bier, identifier: LS_Bier}
- {pin_num: '24', pin_signal: PIO0_1/ACMP_I2/CLKIN, label: Fan, identifier: Fan}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

#include "fsl_common.h"
#include "fsl_swm.h"
#include "pin_mux.h"

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitBootPins
 * Description   : Calls initialization functions.
 *
 * END ****************************************************************************************************************/
void BOARD_InitBootPins(void)
{
    BOARD_InitPins();
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '36', peripheral: SCT0, signal: 'OUT, 0', pin_signal: PIO0_0/ACMP_I1}
  - {pin_num: '18', peripheral: PINT, signal: 'PINT, 0', pin_signal: PIO0_26}
  - {pin_num: '17', peripheral: PINT, signal: 'PINT, 1', pin_signal: PIO0_27}
  - {pin_num: '7', peripheral: PINT, signal: 'PINT, 2', pin_signal: PIO0_28/WKTCLKIN}
  - {pin_num: '24', peripheral: CTIMER0, signal: 'MATCH, 1', pin_signal: PIO0_1/ACMP_I2/CLKIN}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
/* Function assigned for the Cortex-M0P */
void BOARD_InitPins(void)
{
    /* Enables clock for switch matrix.: enable */
    CLOCK_EnableClock(kCLOCK_Swm);

    /* TimerMatchChannel1 connect to P0_1 */
    SWM_SetMovablePinSelect(SWM0, kSWM_T0_MAT_CHN1, kSWM_PortPin_P0_1);

    /* SCT_OUT0 connect to P0_0 */
    SWM_SetMovablePinSelect(SWM0, kSWM_SCT_OUT0, kSWM_PortPin_P0_0);

    SYSCON->PINTSEL[0] = ((SYSCON->PINTSEL[0] &
                           /* Mask bits to zero which are setting */
                           (~(SYSCON_PINTSEL_INTPIN_MASK)))

                          /* Pin number select for pin interrupt or pattern match engine input. (PIO0_0 to
                           * PIO0_31correspond to numbers 0 to 31 and PIO1_0 to PIO1_31 correspond to numbers 32 to
                           * 63).: 0x1Au */
                          | SYSCON_PINTSEL_INTPIN(0x1Au));

    SYSCON->PINTSEL[1] = ((SYSCON->PINTSEL[1] &
                           /* Mask bits to zero which are setting */
                           (~(SYSCON_PINTSEL_INTPIN_MASK)))

                          /* Pin number select for pin interrupt or pattern match engine input. (PIO0_0 to
                           * PIO0_31correspond to numbers 0 to 31 and PIO1_0 to PIO1_31 correspond to numbers 32 to
                           * 63).: 0x1Bu */
                          | SYSCON_PINTSEL_INTPIN(0x1Bu));

    SYSCON->PINTSEL[2] = ((SYSCON->PINTSEL[2] &
                           /* Mask bits to zero which are setting */
                           (~(SYSCON_PINTSEL_INTPIN_MASK)))

                          /* Pin number select for pin interrupt or pattern match engine input. (PIO0_0 to
                           * PIO0_31correspond to numbers 0 to 31 and PIO1_0 to PIO1_31 correspond to numbers 32 to
                           * 63).: 0x1Cu */
                          | SYSCON_PINTSEL_INTPIN(0x1Cu));

    /* Disable clock for switch matrix. */
    CLOCK_DisableClock(kCLOCK_Swm);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
