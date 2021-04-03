/*******************************************************************************
* File Name: CM_BUSY_FLAG.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_CM_BUSY_FLAG_H) /* Pins CM_BUSY_FLAG_H */
#define CY_PINS_CM_BUSY_FLAG_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "CM_BUSY_FLAG_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 CM_BUSY_FLAG__PORT == 15 && ((CM_BUSY_FLAG__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    CM_BUSY_FLAG_Write(uint8 value);
void    CM_BUSY_FLAG_SetDriveMode(uint8 mode);
uint8   CM_BUSY_FLAG_ReadDataReg(void);
uint8   CM_BUSY_FLAG_Read(void);
void    CM_BUSY_FLAG_SetInterruptMode(uint16 position, uint16 mode);
uint8   CM_BUSY_FLAG_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the CM_BUSY_FLAG_SetDriveMode() function.
     *  @{
     */
        #define CM_BUSY_FLAG_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define CM_BUSY_FLAG_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define CM_BUSY_FLAG_DM_RES_UP          PIN_DM_RES_UP
        #define CM_BUSY_FLAG_DM_RES_DWN         PIN_DM_RES_DWN
        #define CM_BUSY_FLAG_DM_OD_LO           PIN_DM_OD_LO
        #define CM_BUSY_FLAG_DM_OD_HI           PIN_DM_OD_HI
        #define CM_BUSY_FLAG_DM_STRONG          PIN_DM_STRONG
        #define CM_BUSY_FLAG_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define CM_BUSY_FLAG_MASK               CM_BUSY_FLAG__MASK
#define CM_BUSY_FLAG_SHIFT              CM_BUSY_FLAG__SHIFT
#define CM_BUSY_FLAG_WIDTH              1u

/* Interrupt constants */
#if defined(CM_BUSY_FLAG__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in CM_BUSY_FLAG_SetInterruptMode() function.
     *  @{
     */
        #define CM_BUSY_FLAG_INTR_NONE      (uint16)(0x0000u)
        #define CM_BUSY_FLAG_INTR_RISING    (uint16)(0x0001u)
        #define CM_BUSY_FLAG_INTR_FALLING   (uint16)(0x0002u)
        #define CM_BUSY_FLAG_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define CM_BUSY_FLAG_INTR_MASK      (0x01u) 
#endif /* (CM_BUSY_FLAG__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define CM_BUSY_FLAG_PS                     (* (reg8 *) CM_BUSY_FLAG__PS)
/* Data Register */
#define CM_BUSY_FLAG_DR                     (* (reg8 *) CM_BUSY_FLAG__DR)
/* Port Number */
#define CM_BUSY_FLAG_PRT_NUM                (* (reg8 *) CM_BUSY_FLAG__PRT) 
/* Connect to Analog Globals */                                                  
#define CM_BUSY_FLAG_AG                     (* (reg8 *) CM_BUSY_FLAG__AG)                       
/* Analog MUX bux enable */
#define CM_BUSY_FLAG_AMUX                   (* (reg8 *) CM_BUSY_FLAG__AMUX) 
/* Bidirectional Enable */                                                        
#define CM_BUSY_FLAG_BIE                    (* (reg8 *) CM_BUSY_FLAG__BIE)
/* Bit-mask for Aliased Register Access */
#define CM_BUSY_FLAG_BIT_MASK               (* (reg8 *) CM_BUSY_FLAG__BIT_MASK)
/* Bypass Enable */
#define CM_BUSY_FLAG_BYP                    (* (reg8 *) CM_BUSY_FLAG__BYP)
/* Port wide control signals */                                                   
#define CM_BUSY_FLAG_CTL                    (* (reg8 *) CM_BUSY_FLAG__CTL)
/* Drive Modes */
#define CM_BUSY_FLAG_DM0                    (* (reg8 *) CM_BUSY_FLAG__DM0) 
#define CM_BUSY_FLAG_DM1                    (* (reg8 *) CM_BUSY_FLAG__DM1)
#define CM_BUSY_FLAG_DM2                    (* (reg8 *) CM_BUSY_FLAG__DM2) 
/* Input Buffer Disable Override */
#define CM_BUSY_FLAG_INP_DIS                (* (reg8 *) CM_BUSY_FLAG__INP_DIS)
/* LCD Common or Segment Drive */
#define CM_BUSY_FLAG_LCD_COM_SEG            (* (reg8 *) CM_BUSY_FLAG__LCD_COM_SEG)
/* Enable Segment LCD */
#define CM_BUSY_FLAG_LCD_EN                 (* (reg8 *) CM_BUSY_FLAG__LCD_EN)
/* Slew Rate Control */
#define CM_BUSY_FLAG_SLW                    (* (reg8 *) CM_BUSY_FLAG__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define CM_BUSY_FLAG_PRTDSI__CAPS_SEL       (* (reg8 *) CM_BUSY_FLAG__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define CM_BUSY_FLAG_PRTDSI__DBL_SYNC_IN    (* (reg8 *) CM_BUSY_FLAG__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define CM_BUSY_FLAG_PRTDSI__OE_SEL0        (* (reg8 *) CM_BUSY_FLAG__PRTDSI__OE_SEL0) 
#define CM_BUSY_FLAG_PRTDSI__OE_SEL1        (* (reg8 *) CM_BUSY_FLAG__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define CM_BUSY_FLAG_PRTDSI__OUT_SEL0       (* (reg8 *) CM_BUSY_FLAG__PRTDSI__OUT_SEL0) 
#define CM_BUSY_FLAG_PRTDSI__OUT_SEL1       (* (reg8 *) CM_BUSY_FLAG__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define CM_BUSY_FLAG_PRTDSI__SYNC_OUT       (* (reg8 *) CM_BUSY_FLAG__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(CM_BUSY_FLAG__SIO_CFG)
    #define CM_BUSY_FLAG_SIO_HYST_EN        (* (reg8 *) CM_BUSY_FLAG__SIO_HYST_EN)
    #define CM_BUSY_FLAG_SIO_REG_HIFREQ     (* (reg8 *) CM_BUSY_FLAG__SIO_REG_HIFREQ)
    #define CM_BUSY_FLAG_SIO_CFG            (* (reg8 *) CM_BUSY_FLAG__SIO_CFG)
    #define CM_BUSY_FLAG_SIO_DIFF           (* (reg8 *) CM_BUSY_FLAG__SIO_DIFF)
#endif /* (CM_BUSY_FLAG__SIO_CFG) */

/* Interrupt Registers */
#if defined(CM_BUSY_FLAG__INTSTAT)
    #define CM_BUSY_FLAG_INTSTAT            (* (reg8 *) CM_BUSY_FLAG__INTSTAT)
    #define CM_BUSY_FLAG_SNAP               (* (reg8 *) CM_BUSY_FLAG__SNAP)
    
	#define CM_BUSY_FLAG_0_INTTYPE_REG 		(* (reg8 *) CM_BUSY_FLAG__0__INTTYPE)
#endif /* (CM_BUSY_FLAG__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_CM_BUSY_FLAG_H */


/* [] END OF FILE */
