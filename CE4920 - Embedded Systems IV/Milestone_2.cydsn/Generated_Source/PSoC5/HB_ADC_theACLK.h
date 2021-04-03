/*******************************************************************************
* File Name: HB_ADC_theACLK.h
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

#if !defined(CY_CLOCK_HB_ADC_theACLK_H)
#define CY_CLOCK_HB_ADC_theACLK_H

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

void HB_ADC_theACLK_Start(void) ;
void HB_ADC_theACLK_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void HB_ADC_theACLK_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void HB_ADC_theACLK_StandbyPower(uint8 state) ;
void HB_ADC_theACLK_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 HB_ADC_theACLK_GetDividerRegister(void) ;
void HB_ADC_theACLK_SetModeRegister(uint8 modeBitMask) ;
void HB_ADC_theACLK_ClearModeRegister(uint8 modeBitMask) ;
uint8 HB_ADC_theACLK_GetModeRegister(void) ;
void HB_ADC_theACLK_SetSourceRegister(uint8 clkSource) ;
uint8 HB_ADC_theACLK_GetSourceRegister(void) ;
#if defined(HB_ADC_theACLK__CFG3)
void HB_ADC_theACLK_SetPhaseRegister(uint8 clkPhase) ;
uint8 HB_ADC_theACLK_GetPhaseRegister(void) ;
#endif /* defined(HB_ADC_theACLK__CFG3) */

#define HB_ADC_theACLK_Enable()                       HB_ADC_theACLK_Start()
#define HB_ADC_theACLK_Disable()                      HB_ADC_theACLK_Stop()
#define HB_ADC_theACLK_SetDivider(clkDivider)         HB_ADC_theACLK_SetDividerRegister(clkDivider, 1u)
#define HB_ADC_theACLK_SetDividerValue(clkDivider)    HB_ADC_theACLK_SetDividerRegister((clkDivider) - 1u, 1u)
#define HB_ADC_theACLK_SetMode(clkMode)               HB_ADC_theACLK_SetModeRegister(clkMode)
#define HB_ADC_theACLK_SetSource(clkSource)           HB_ADC_theACLK_SetSourceRegister(clkSource)
#if defined(HB_ADC_theACLK__CFG3)
#define HB_ADC_theACLK_SetPhase(clkPhase)             HB_ADC_theACLK_SetPhaseRegister(clkPhase)
#define HB_ADC_theACLK_SetPhaseValue(clkPhase)        HB_ADC_theACLK_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(HB_ADC_theACLK__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define HB_ADC_theACLK_CLKEN              (* (reg8 *) HB_ADC_theACLK__PM_ACT_CFG)
#define HB_ADC_theACLK_CLKEN_PTR          ((reg8 *) HB_ADC_theACLK__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define HB_ADC_theACLK_CLKSTBY            (* (reg8 *) HB_ADC_theACLK__PM_STBY_CFG)
#define HB_ADC_theACLK_CLKSTBY_PTR        ((reg8 *) HB_ADC_theACLK__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define HB_ADC_theACLK_DIV_LSB            (* (reg8 *) HB_ADC_theACLK__CFG0)
#define HB_ADC_theACLK_DIV_LSB_PTR        ((reg8 *) HB_ADC_theACLK__CFG0)
#define HB_ADC_theACLK_DIV_PTR            ((reg16 *) HB_ADC_theACLK__CFG0)

/* Clock MSB divider configuration register. */
#define HB_ADC_theACLK_DIV_MSB            (* (reg8 *) HB_ADC_theACLK__CFG1)
#define HB_ADC_theACLK_DIV_MSB_PTR        ((reg8 *) HB_ADC_theACLK__CFG1)

/* Mode and source configuration register */
#define HB_ADC_theACLK_MOD_SRC            (* (reg8 *) HB_ADC_theACLK__CFG2)
#define HB_ADC_theACLK_MOD_SRC_PTR        ((reg8 *) HB_ADC_theACLK__CFG2)

#if defined(HB_ADC_theACLK__CFG3)
/* Analog clock phase configuration register */
#define HB_ADC_theACLK_PHASE              (* (reg8 *) HB_ADC_theACLK__CFG3)
#define HB_ADC_theACLK_PHASE_PTR          ((reg8 *) HB_ADC_theACLK__CFG3)
#endif /* defined(HB_ADC_theACLK__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define HB_ADC_theACLK_CLKEN_MASK         HB_ADC_theACLK__PM_ACT_MSK
#define HB_ADC_theACLK_CLKSTBY_MASK       HB_ADC_theACLK__PM_STBY_MSK

/* CFG2 field masks */
#define HB_ADC_theACLK_SRC_SEL_MSK        HB_ADC_theACLK__CFG2_SRC_SEL_MASK
#define HB_ADC_theACLK_MODE_MASK          (~(HB_ADC_theACLK_SRC_SEL_MSK))

#if defined(HB_ADC_theACLK__CFG3)
/* CFG3 phase mask */
#define HB_ADC_theACLK_PHASE_MASK         HB_ADC_theACLK__CFG3_PHASE_DLY_MASK
#endif /* defined(HB_ADC_theACLK__CFG3) */

#endif /* CY_CLOCK_HB_ADC_theACLK_H */


/* [] END OF FILE */
