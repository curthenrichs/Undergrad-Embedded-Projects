/*******************************************************************************
* File Name: Tone_D_CLK.h
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

#if !defined(CY_CLOCK_Tone_D_CLK_H)
#define CY_CLOCK_Tone_D_CLK_H

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

void Tone_D_CLK_Start(void) ;
void Tone_D_CLK_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void Tone_D_CLK_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void Tone_D_CLK_StandbyPower(uint8 state) ;
void Tone_D_CLK_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 Tone_D_CLK_GetDividerRegister(void) ;
void Tone_D_CLK_SetModeRegister(uint8 modeBitMask) ;
void Tone_D_CLK_ClearModeRegister(uint8 modeBitMask) ;
uint8 Tone_D_CLK_GetModeRegister(void) ;
void Tone_D_CLK_SetSourceRegister(uint8 clkSource) ;
uint8 Tone_D_CLK_GetSourceRegister(void) ;
#if defined(Tone_D_CLK__CFG3)
void Tone_D_CLK_SetPhaseRegister(uint8 clkPhase) ;
uint8 Tone_D_CLK_GetPhaseRegister(void) ;
#endif /* defined(Tone_D_CLK__CFG3) */

#define Tone_D_CLK_Enable()                       Tone_D_CLK_Start()
#define Tone_D_CLK_Disable()                      Tone_D_CLK_Stop()
#define Tone_D_CLK_SetDivider(clkDivider)         Tone_D_CLK_SetDividerRegister(clkDivider, 1u)
#define Tone_D_CLK_SetDividerValue(clkDivider)    Tone_D_CLK_SetDividerRegister((clkDivider) - 1u, 1u)
#define Tone_D_CLK_SetMode(clkMode)               Tone_D_CLK_SetModeRegister(clkMode)
#define Tone_D_CLK_SetSource(clkSource)           Tone_D_CLK_SetSourceRegister(clkSource)
#if defined(Tone_D_CLK__CFG3)
#define Tone_D_CLK_SetPhase(clkPhase)             Tone_D_CLK_SetPhaseRegister(clkPhase)
#define Tone_D_CLK_SetPhaseValue(clkPhase)        Tone_D_CLK_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(Tone_D_CLK__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define Tone_D_CLK_CLKEN              (* (reg8 *) Tone_D_CLK__PM_ACT_CFG)
#define Tone_D_CLK_CLKEN_PTR          ((reg8 *) Tone_D_CLK__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define Tone_D_CLK_CLKSTBY            (* (reg8 *) Tone_D_CLK__PM_STBY_CFG)
#define Tone_D_CLK_CLKSTBY_PTR        ((reg8 *) Tone_D_CLK__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define Tone_D_CLK_DIV_LSB            (* (reg8 *) Tone_D_CLK__CFG0)
#define Tone_D_CLK_DIV_LSB_PTR        ((reg8 *) Tone_D_CLK__CFG0)
#define Tone_D_CLK_DIV_PTR            ((reg16 *) Tone_D_CLK__CFG0)

/* Clock MSB divider configuration register. */
#define Tone_D_CLK_DIV_MSB            (* (reg8 *) Tone_D_CLK__CFG1)
#define Tone_D_CLK_DIV_MSB_PTR        ((reg8 *) Tone_D_CLK__CFG1)

/* Mode and source configuration register */
#define Tone_D_CLK_MOD_SRC            (* (reg8 *) Tone_D_CLK__CFG2)
#define Tone_D_CLK_MOD_SRC_PTR        ((reg8 *) Tone_D_CLK__CFG2)

#if defined(Tone_D_CLK__CFG3)
/* Analog clock phase configuration register */
#define Tone_D_CLK_PHASE              (* (reg8 *) Tone_D_CLK__CFG3)
#define Tone_D_CLK_PHASE_PTR          ((reg8 *) Tone_D_CLK__CFG3)
#endif /* defined(Tone_D_CLK__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define Tone_D_CLK_CLKEN_MASK         Tone_D_CLK__PM_ACT_MSK
#define Tone_D_CLK_CLKSTBY_MASK       Tone_D_CLK__PM_STBY_MSK

/* CFG2 field masks */
#define Tone_D_CLK_SRC_SEL_MSK        Tone_D_CLK__CFG2_SRC_SEL_MASK
#define Tone_D_CLK_MODE_MASK          (~(Tone_D_CLK_SRC_SEL_MSK))

#if defined(Tone_D_CLK__CFG3)
/* CFG3 phase mask */
#define Tone_D_CLK_PHASE_MASK         Tone_D_CLK__CFG3_PHASE_DLY_MASK
#endif /* defined(Tone_D_CLK__CFG3) */

#endif /* CY_CLOCK_Tone_D_CLK_H */


/* [] END OF FILE */
