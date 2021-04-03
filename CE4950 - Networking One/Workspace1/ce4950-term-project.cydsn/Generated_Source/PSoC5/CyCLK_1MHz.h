/*******************************************************************************
* File Name: CyCLK_1MHz.h
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

#if !defined(CY_CLOCK_CyCLK_1MHz_H)
#define CY_CLOCK_CyCLK_1MHz_H

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

void CyCLK_1MHz_Start(void) ;
void CyCLK_1MHz_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void CyCLK_1MHz_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void CyCLK_1MHz_StandbyPower(uint8 state) ;
void CyCLK_1MHz_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 CyCLK_1MHz_GetDividerRegister(void) ;
void CyCLK_1MHz_SetModeRegister(uint8 modeBitMask) ;
void CyCLK_1MHz_ClearModeRegister(uint8 modeBitMask) ;
uint8 CyCLK_1MHz_GetModeRegister(void) ;
void CyCLK_1MHz_SetSourceRegister(uint8 clkSource) ;
uint8 CyCLK_1MHz_GetSourceRegister(void) ;
#if defined(CyCLK_1MHz__CFG3)
void CyCLK_1MHz_SetPhaseRegister(uint8 clkPhase) ;
uint8 CyCLK_1MHz_GetPhaseRegister(void) ;
#endif /* defined(CyCLK_1MHz__CFG3) */

#define CyCLK_1MHz_Enable()                       CyCLK_1MHz_Start()
#define CyCLK_1MHz_Disable()                      CyCLK_1MHz_Stop()
#define CyCLK_1MHz_SetDivider(clkDivider)         CyCLK_1MHz_SetDividerRegister(clkDivider, 1u)
#define CyCLK_1MHz_SetDividerValue(clkDivider)    CyCLK_1MHz_SetDividerRegister((clkDivider) - 1u, 1u)
#define CyCLK_1MHz_SetMode(clkMode)               CyCLK_1MHz_SetModeRegister(clkMode)
#define CyCLK_1MHz_SetSource(clkSource)           CyCLK_1MHz_SetSourceRegister(clkSource)
#if defined(CyCLK_1MHz__CFG3)
#define CyCLK_1MHz_SetPhase(clkPhase)             CyCLK_1MHz_SetPhaseRegister(clkPhase)
#define CyCLK_1MHz_SetPhaseValue(clkPhase)        CyCLK_1MHz_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(CyCLK_1MHz__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define CyCLK_1MHz_CLKEN              (* (reg8 *) CyCLK_1MHz__PM_ACT_CFG)
#define CyCLK_1MHz_CLKEN_PTR          ((reg8 *) CyCLK_1MHz__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define CyCLK_1MHz_CLKSTBY            (* (reg8 *) CyCLK_1MHz__PM_STBY_CFG)
#define CyCLK_1MHz_CLKSTBY_PTR        ((reg8 *) CyCLK_1MHz__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define CyCLK_1MHz_DIV_LSB            (* (reg8 *) CyCLK_1MHz__CFG0)
#define CyCLK_1MHz_DIV_LSB_PTR        ((reg8 *) CyCLK_1MHz__CFG0)
#define CyCLK_1MHz_DIV_PTR            ((reg16 *) CyCLK_1MHz__CFG0)

/* Clock MSB divider configuration register. */
#define CyCLK_1MHz_DIV_MSB            (* (reg8 *) CyCLK_1MHz__CFG1)
#define CyCLK_1MHz_DIV_MSB_PTR        ((reg8 *) CyCLK_1MHz__CFG1)

/* Mode and source configuration register */
#define CyCLK_1MHz_MOD_SRC            (* (reg8 *) CyCLK_1MHz__CFG2)
#define CyCLK_1MHz_MOD_SRC_PTR        ((reg8 *) CyCLK_1MHz__CFG2)

#if defined(CyCLK_1MHz__CFG3)
/* Analog clock phase configuration register */
#define CyCLK_1MHz_PHASE              (* (reg8 *) CyCLK_1MHz__CFG3)
#define CyCLK_1MHz_PHASE_PTR          ((reg8 *) CyCLK_1MHz__CFG3)
#endif /* defined(CyCLK_1MHz__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define CyCLK_1MHz_CLKEN_MASK         CyCLK_1MHz__PM_ACT_MSK
#define CyCLK_1MHz_CLKSTBY_MASK       CyCLK_1MHz__PM_STBY_MSK

/* CFG2 field masks */
#define CyCLK_1MHz_SRC_SEL_MSK        CyCLK_1MHz__CFG2_SRC_SEL_MASK
#define CyCLK_1MHz_MODE_MASK          (~(CyCLK_1MHz_SRC_SEL_MSK))

#if defined(CyCLK_1MHz__CFG3)
/* CFG3 phase mask */
#define CyCLK_1MHz_PHASE_MASK         CyCLK_1MHz__CFG3_PHASE_DLY_MASK
#endif /* defined(CyCLK_1MHz__CFG3) */

#endif /* CY_CLOCK_CyCLK_1MHz_H */


/* [] END OF FILE */
