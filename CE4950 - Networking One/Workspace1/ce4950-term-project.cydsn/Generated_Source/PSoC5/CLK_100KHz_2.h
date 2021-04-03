/*******************************************************************************
* File Name: CLK_100KHz_2.h
* Version 2.20
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CLOCK_CLK_100KHz_2_H)
#define CY_CLOCK_CLK_100KHz_2_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
* Conditional Compilation Parameters
***************************************/

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component cy_clock_v2_20 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */


/***************************************
*        Function Prototypes
***************************************/

void CLK_100KHz_2_Start(void) ;
void CLK_100KHz_2_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void CLK_100KHz_2_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void CLK_100KHz_2_StandbyPower(uint8 state) ;
void CLK_100KHz_2_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 CLK_100KHz_2_GetDividerRegister(void) ;
void CLK_100KHz_2_SetModeRegister(uint8 modeBitMask) ;
void CLK_100KHz_2_ClearModeRegister(uint8 modeBitMask) ;
uint8 CLK_100KHz_2_GetModeRegister(void) ;
void CLK_100KHz_2_SetSourceRegister(uint8 clkSource) ;
uint8 CLK_100KHz_2_GetSourceRegister(void) ;
#if defined(CLK_100KHz_2__CFG3)
void CLK_100KHz_2_SetPhaseRegister(uint8 clkPhase) ;
uint8 CLK_100KHz_2_GetPhaseRegister(void) ;
#endif /* defined(CLK_100KHz_2__CFG3) */

#define CLK_100KHz_2_Enable()                       CLK_100KHz_2_Start()
#define CLK_100KHz_2_Disable()                      CLK_100KHz_2_Stop()
#define CLK_100KHz_2_SetDivider(clkDivider)         CLK_100KHz_2_SetDividerRegister(clkDivider, 1u)
#define CLK_100KHz_2_SetDividerValue(clkDivider)    CLK_100KHz_2_SetDividerRegister((clkDivider) - 1u, 1u)
#define CLK_100KHz_2_SetMode(clkMode)               CLK_100KHz_2_SetModeRegister(clkMode)
#define CLK_100KHz_2_SetSource(clkSource)           CLK_100KHz_2_SetSourceRegister(clkSource)
#if defined(CLK_100KHz_2__CFG3)
#define CLK_100KHz_2_SetPhase(clkPhase)             CLK_100KHz_2_SetPhaseRegister(clkPhase)
#define CLK_100KHz_2_SetPhaseValue(clkPhase)        CLK_100KHz_2_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(CLK_100KHz_2__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define CLK_100KHz_2_CLKEN              (* (reg8 *) CLK_100KHz_2__PM_ACT_CFG)
#define CLK_100KHz_2_CLKEN_PTR          ((reg8 *) CLK_100KHz_2__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define CLK_100KHz_2_CLKSTBY            (* (reg8 *) CLK_100KHz_2__PM_STBY_CFG)
#define CLK_100KHz_2_CLKSTBY_PTR        ((reg8 *) CLK_100KHz_2__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define CLK_100KHz_2_DIV_LSB            (* (reg8 *) CLK_100KHz_2__CFG0)
#define CLK_100KHz_2_DIV_LSB_PTR        ((reg8 *) CLK_100KHz_2__CFG0)
#define CLK_100KHz_2_DIV_PTR            ((reg16 *) CLK_100KHz_2__CFG0)

/* Clock MSB divider configuration register. */
#define CLK_100KHz_2_DIV_MSB            (* (reg8 *) CLK_100KHz_2__CFG1)
#define CLK_100KHz_2_DIV_MSB_PTR        ((reg8 *) CLK_100KHz_2__CFG1)

/* Mode and source configuration register */
#define CLK_100KHz_2_MOD_SRC            (* (reg8 *) CLK_100KHz_2__CFG2)
#define CLK_100KHz_2_MOD_SRC_PTR        ((reg8 *) CLK_100KHz_2__CFG2)

#if defined(CLK_100KHz_2__CFG3)
/* Analog clock phase configuration register */
#define CLK_100KHz_2_PHASE              (* (reg8 *) CLK_100KHz_2__CFG3)
#define CLK_100KHz_2_PHASE_PTR          ((reg8 *) CLK_100KHz_2__CFG3)
#endif /* defined(CLK_100KHz_2__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define CLK_100KHz_2_CLKEN_MASK         CLK_100KHz_2__PM_ACT_MSK
#define CLK_100KHz_2_CLKSTBY_MASK       CLK_100KHz_2__PM_STBY_MSK

/* CFG2 field masks */
#define CLK_100KHz_2_SRC_SEL_MSK        CLK_100KHz_2__CFG2_SRC_SEL_MASK
#define CLK_100KHz_2_MODE_MASK          (~(CLK_100KHz_2_SRC_SEL_MSK))

#if defined(CLK_100KHz_2__CFG3)
/* CFG3 phase mask */
#define CLK_100KHz_2_PHASE_MASK         CLK_100KHz_2__CFG3_PHASE_DLY_MASK
#endif /* defined(CLK_100KHz_2__CFG3) */

#endif /* CY_CLOCK_CLK_100KHz_2_H */


/* [] END OF FILE */
