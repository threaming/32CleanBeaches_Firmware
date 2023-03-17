/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

#ifndef _CLOCK_CONFIG_H_
#define _CLOCK_CONFIG_H_

#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOARD_XTAL0_CLK_HZ                         12000000U  /*!< Board xtal0 frequency in Hz */
#define BOARD_XTAL32K_CLK_HZ                          32768U  /*!< Board xtal32K frequency in Hz */

/*******************************************************************************
 ************************ BOARD_InitBootClocks function ************************
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*!
 * @brief This function executes default configuration of clocks.
 *
 */
void BOARD_InitBootClocks(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

/*******************************************************************************
 ********************** Configuration BOARD_BootClockRUN ***********************
 ******************************************************************************/
/*******************************************************************************
 * Definitions for BOARD_BootClockRUN configuration
 ******************************************************************************/
#define BOARD_BOOTCLOCKRUN_CORE_CLOCK              15000000U  /*!< Core clock frequency: 15000000Hz */


/* Clock outputs (values are in Hz): */
#define BOARD_BOOTCLOCKRUN_ADC_CLOCK                  0UL
#define BOARD_BOOTCLOCKRUN_CAPT_CLOCK                 0UL
#define BOARD_BOOTCLOCKRUN_CLKOUT_CLOCK               0UL
#define BOARD_BOOTCLOCKRUN_FROHF_CLOCK                30000000UL
#define BOARD_BOOTCLOCKRUN_I2C0_CLOCK                 0UL
#define BOARD_BOOTCLOCKRUN_I2C1_CLOCK                 0UL
#define BOARD_BOOTCLOCKRUN_I2C2_CLOCK                 0UL
#define BOARD_BOOTCLOCKRUN_I2C3_CLOCK                 0UL
#define BOARD_BOOTCLOCKRUN_IOCON0_CLOCK               0UL
#define BOARD_BOOTCLOCKRUN_IOCON1_CLOCK               0UL
#define BOARD_BOOTCLOCKRUN_IOCON2_CLOCK               0UL
#define BOARD_BOOTCLOCKRUN_IOCON3_CLOCK               0UL
#define BOARD_BOOTCLOCKRUN_IOCON4_CLOCK               0UL
#define BOARD_BOOTCLOCKRUN_IOCON5_CLOCK               0UL
#define BOARD_BOOTCLOCKRUN_IOCON6_CLOCK               0UL
#define BOARD_BOOTCLOCKRUN_LOWPOWER_CLOCK             10000UL
#define BOARD_BOOTCLOCKRUN_SCT_CLOCK                  0UL
#define BOARD_BOOTCLOCKRUN_SPI0_CLOCK                 0UL
#define BOARD_BOOTCLOCKRUN_SPI1_CLOCK                 0UL
#define BOARD_BOOTCLOCKRUN_SYSPLL_CLOCK               15000000UL
#define BOARD_BOOTCLOCKRUN_SYSTEM_CLOCK               15000000UL
#define BOARD_BOOTCLOCKRUN_UART0_CLOCK                0UL
#define BOARD_BOOTCLOCKRUN_UART1_CLOCK                0UL
#define BOARD_BOOTCLOCKRUN_UART2_CLOCK                0UL
#define BOARD_BOOTCLOCKRUN_UART3_CLOCK                0UL
#define BOARD_BOOTCLOCKRUN_UART4_CLOCK                0UL
#define BOARD_BOOTCLOCKRUN_WWDT_CLOCK                 0UL
#define BOARD_BOOTCLOCKRUN_DIVTO750K_CLOCK            750000UL

/*******************************************************************************
 * API for BOARD_BootClockRUN configuration
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*!
 * @brief This function executes configuration of clocks.
 *
 */
void BOARD_BootClockRUN(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* _CLOCK_CONFIG_H_ */
