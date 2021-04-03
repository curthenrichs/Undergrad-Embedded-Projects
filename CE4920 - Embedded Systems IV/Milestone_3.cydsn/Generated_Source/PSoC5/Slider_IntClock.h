/*******************************************************************************
* File Name: Slider_IntClock.h
* Version 2.10
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

#if !defined(CY_CLOCK_Slider_IntClock_H)
#define CY_CLOCK_Slider_IntClock_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
* Conditional Compilation Parameters
***************************************/

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component cy_clock_v2_10 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */


/***************************************
*        Function Prototypes
***************************************/

void Slider_IntClock_Start(void) ;
void Slider_IntClock_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void Slider_IntClock_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void Slider_IntClock_StandbyPower(uint8 state) ;
void Slider_IntClock_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 Slider_IntClock_GetDividerRegister(void) ;
void Slider_IntClock_SetModeRegister(uint8 modeBitMask) ;
void Slider_IntClock_ClearModeRegister(uint8 modeBitMask) ;
uint8 Slider_IntClock_GetModeRegister(void) ;
void Slider_IntClock_SetSourceRegister(uint8 clkSource) ;
uint8 Slider_IntClock_GetSourceRegister(void) ;
#if defined(Slider_IntClock__CFG3)
void Slider_IntClock_SetPhaseRegister(uint8 clkPhase) ;
uint8 Slider_IntClock_GetPhaseRegister(void) ;
#endif /* defined(Slider_IntClock__CFG3) */

#define Slider_IntClock_Enable()                       Slider_IntClock_Start()
#define Slider_IntClock_Disable()                      Slider_IntClock_Stop()
#define Slider_IntClock_SetDivider(clkDivider)         Slider_IntClock_SetDividerRegister(clkDivider, 1u)
#define Slider_IntClock_SetDividerValue(clkDivider)    Slider_IntClock_SetDividerRegister((clkDivider) - 1u, 1u)
#define Slider_IntClock_SetMode(clkMode)               Slider_IntClock_SetModeRegister(clkMode)
#define Slider_IntClock_SetSource(clkSource)           Slider_IntClock_SetSourceRegister(clkSource)
#if defined(Slider_IntClock__CFG3)
#define Slider_IntClock_SetPhase(clkPhase)             Slider_IntClock_SetPhaseRegister(clkPhase)
#define Slider_IntClock_SetPhaseValue(clkPhase)        Slider_IntClock_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(Slider_IntClock__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define Slider_IntClock_CLKEN              (* (reg8 *) Slider_IntClock__PM_ACT_CFG)
#define Slider_IntClock_CLKEN_PTR          ((reg8 *) Slider_IntClock__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define Slider_IntClock_CLKSTBY            (* (reg8 *) Slider_IntClock__PM_STBY_CFG)
#define Slider_IntClock_CLKSTBY_PTR        ((reg8 *) Slider_IntClock__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define Slider_IntClock_DIV_LSB            (* (reg8 *) Slider_IntClock__CFG0)
#define Slider_IntClock_DIV_LSB_PTR        ((reg8 *) Slider_IntClock__CFG0)
#define Slider_IntClock_DIV_PTR            ((reg16 *) Slider_IntClock__CFG0)

/* Clock MSB divider configuration register. */
#define Slider_IntClock_DIV_MSB            (* (reg8 *) Slider_IntClock__CFG1)
#define Slider_IntClock_DIV_MSB_PTR        ((reg8 *) Slider_IntClock__CFG1)

/* Mode and source configuration register */
#define Slider_IntClock_MOD_SRC            (* (reg8 *) Slider_IntClock__CFG2)
#define Slider_IntClock_MOD_SRC_PTR        ((reg8 *) Slider_IntClock__CFG2)

#if defined(Slider_IntClock__CFG3)
/* Analog clock phase configuration register */
#define Slider_IntClock_PHASE              (* (reg8 *) Slider_IntClock__CFG3)
#define Slider_IntClock_PHASE_PTR          ((reg8 *) Slider_IntClock__CFG3)
#endif /* defined(Slider_IntClock__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define Slider_IntClock_CLKEN_MASK         Slider_IntClock__PM_ACT_MSK
#define Slider_IntClock_CLKSTBY_MASK       Slider_IntClock__PM_STBY_MSK

/* CFG2 field masks */
#define Slider_IntClock_SRC_SEL_MSK        Slider_IntClock__CFG2_SRC_SEL_MASK
#define Slider_IntClock_MODE_MASK          (~(Slider_IntClock_SRC_SEL_MSK))

#if defined(Slider_IntClock__CFG3)
/* CFG3 phase mask */
#define Slider_IntClock_PHASE_MASK         Slider_IntClock__CFG3_PHASE_DLY_MASK
#endif /* defined(Slider_IntClock__CFG3) */

#endif /* CY_CLOCK_Slider_IntClock_H */


/* [] END OF FILE */
