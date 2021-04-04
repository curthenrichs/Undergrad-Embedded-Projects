/*******************************************************************************
* File Name: Piezo.h  
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

#if !defined(CY_PINS_Piezo_H) /* Pins Piezo_H */
#define CY_PINS_Piezo_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Piezo_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Piezo__PORT == 15 && ((Piezo__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Piezo_Write(uint8 value);
void    Piezo_SetDriveMode(uint8 mode);
uint8   Piezo_ReadDataReg(void);
uint8   Piezo_Read(void);
void    Piezo_SetInterruptMode(uint16 position, uint16 mode);
uint8   Piezo_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Piezo_SetDriveMode() function.
     *  @{
     */
        #define Piezo_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Piezo_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Piezo_DM_RES_UP          PIN_DM_RES_UP
        #define Piezo_DM_RES_DWN         PIN_DM_RES_DWN
        #define Piezo_DM_OD_LO           PIN_DM_OD_LO
        #define Piezo_DM_OD_HI           PIN_DM_OD_HI
        #define Piezo_DM_STRONG          PIN_DM_STRONG
        #define Piezo_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Piezo_MASK               Piezo__MASK
#define Piezo_SHIFT              Piezo__SHIFT
#define Piezo_WIDTH              1u

/* Interrupt constants */
#if defined(Piezo__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Piezo_SetInterruptMode() function.
     *  @{
     */
        #define Piezo_INTR_NONE      (uint16)(0x0000u)
        #define Piezo_INTR_RISING    (uint16)(0x0001u)
        #define Piezo_INTR_FALLING   (uint16)(0x0002u)
        #define Piezo_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Piezo_INTR_MASK      (0x01u) 
#endif /* (Piezo__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Piezo_PS                     (* (reg8 *) Piezo__PS)
/* Data Register */
#define Piezo_DR                     (* (reg8 *) Piezo__DR)
/* Port Number */
#define Piezo_PRT_NUM                (* (reg8 *) Piezo__PRT) 
/* Connect to Analog Globals */                                                  
#define Piezo_AG                     (* (reg8 *) Piezo__AG)                       
/* Analog MUX bux enable */
#define Piezo_AMUX                   (* (reg8 *) Piezo__AMUX) 
/* Bidirectional Enable */                                                        
#define Piezo_BIE                    (* (reg8 *) Piezo__BIE)
/* Bit-mask for Aliased Register Access */
#define Piezo_BIT_MASK               (* (reg8 *) Piezo__BIT_MASK)
/* Bypass Enable */
#define Piezo_BYP                    (* (reg8 *) Piezo__BYP)
/* Port wide control signals */                                                   
#define Piezo_CTL                    (* (reg8 *) Piezo__CTL)
/* Drive Modes */
#define Piezo_DM0                    (* (reg8 *) Piezo__DM0) 
#define Piezo_DM1                    (* (reg8 *) Piezo__DM1)
#define Piezo_DM2                    (* (reg8 *) Piezo__DM2) 
/* Input Buffer Disable Override */
#define Piezo_INP_DIS                (* (reg8 *) Piezo__INP_DIS)
/* LCD Common or Segment Drive */
#define Piezo_LCD_COM_SEG            (* (reg8 *) Piezo__LCD_COM_SEG)
/* Enable Segment LCD */
#define Piezo_LCD_EN                 (* (reg8 *) Piezo__LCD_EN)
/* Slew Rate Control */
#define Piezo_SLW                    (* (reg8 *) Piezo__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Piezo_PRTDSI__CAPS_SEL       (* (reg8 *) Piezo__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Piezo_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Piezo__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Piezo_PRTDSI__OE_SEL0        (* (reg8 *) Piezo__PRTDSI__OE_SEL0) 
#define Piezo_PRTDSI__OE_SEL1        (* (reg8 *) Piezo__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Piezo_PRTDSI__OUT_SEL0       (* (reg8 *) Piezo__PRTDSI__OUT_SEL0) 
#define Piezo_PRTDSI__OUT_SEL1       (* (reg8 *) Piezo__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Piezo_PRTDSI__SYNC_OUT       (* (reg8 *) Piezo__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Piezo__SIO_CFG)
    #define Piezo_SIO_HYST_EN        (* (reg8 *) Piezo__SIO_HYST_EN)
    #define Piezo_SIO_REG_HIFREQ     (* (reg8 *) Piezo__SIO_REG_HIFREQ)
    #define Piezo_SIO_CFG            (* (reg8 *) Piezo__SIO_CFG)
    #define Piezo_SIO_DIFF           (* (reg8 *) Piezo__SIO_DIFF)
#endif /* (Piezo__SIO_CFG) */

/* Interrupt Registers */
#if defined(Piezo__INTSTAT)
    #define Piezo_INTSTAT            (* (reg8 *) Piezo__INTSTAT)
    #define Piezo_SNAP               (* (reg8 *) Piezo__SNAP)
    
	#define Piezo_0_INTTYPE_REG 		(* (reg8 *) Piezo__0__INTTYPE)
#endif /* (Piezo__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Piezo_H */


/* [] END OF FILE */
