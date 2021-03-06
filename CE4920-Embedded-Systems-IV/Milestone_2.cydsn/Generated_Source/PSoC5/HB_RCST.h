/*******************************************************************************
* File Name: HB_RCST.h  
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

#if !defined(CY_PINS_HB_RCST_H) /* Pins HB_RCST_H */
#define CY_PINS_HB_RCST_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "HB_RCST_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 HB_RCST__PORT == 15 && ((HB_RCST__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    HB_RCST_Write(uint8 value);
void    HB_RCST_SetDriveMode(uint8 mode);
uint8   HB_RCST_ReadDataReg(void);
uint8   HB_RCST_Read(void);
void    HB_RCST_SetInterruptMode(uint16 position, uint16 mode);
uint8   HB_RCST_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the HB_RCST_SetDriveMode() function.
     *  @{
     */
        #define HB_RCST_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define HB_RCST_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define HB_RCST_DM_RES_UP          PIN_DM_RES_UP
        #define HB_RCST_DM_RES_DWN         PIN_DM_RES_DWN
        #define HB_RCST_DM_OD_LO           PIN_DM_OD_LO
        #define HB_RCST_DM_OD_HI           PIN_DM_OD_HI
        #define HB_RCST_DM_STRONG          PIN_DM_STRONG
        #define HB_RCST_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define HB_RCST_MASK               HB_RCST__MASK
#define HB_RCST_SHIFT              HB_RCST__SHIFT
#define HB_RCST_WIDTH              1u

/* Interrupt constants */
#if defined(HB_RCST__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in HB_RCST_SetInterruptMode() function.
     *  @{
     */
        #define HB_RCST_INTR_NONE      (uint16)(0x0000u)
        #define HB_RCST_INTR_RISING    (uint16)(0x0001u)
        #define HB_RCST_INTR_FALLING   (uint16)(0x0002u)
        #define HB_RCST_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define HB_RCST_INTR_MASK      (0x01u) 
#endif /* (HB_RCST__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define HB_RCST_PS                     (* (reg8 *) HB_RCST__PS)
/* Data Register */
#define HB_RCST_DR                     (* (reg8 *) HB_RCST__DR)
/* Port Number */
#define HB_RCST_PRT_NUM                (* (reg8 *) HB_RCST__PRT) 
/* Connect to Analog Globals */                                                  
#define HB_RCST_AG                     (* (reg8 *) HB_RCST__AG)                       
/* Analog MUX bux enable */
#define HB_RCST_AMUX                   (* (reg8 *) HB_RCST__AMUX) 
/* Bidirectional Enable */                                                        
#define HB_RCST_BIE                    (* (reg8 *) HB_RCST__BIE)
/* Bit-mask for Aliased Register Access */
#define HB_RCST_BIT_MASK               (* (reg8 *) HB_RCST__BIT_MASK)
/* Bypass Enable */
#define HB_RCST_BYP                    (* (reg8 *) HB_RCST__BYP)
/* Port wide control signals */                                                   
#define HB_RCST_CTL                    (* (reg8 *) HB_RCST__CTL)
/* Drive Modes */
#define HB_RCST_DM0                    (* (reg8 *) HB_RCST__DM0) 
#define HB_RCST_DM1                    (* (reg8 *) HB_RCST__DM1)
#define HB_RCST_DM2                    (* (reg8 *) HB_RCST__DM2) 
/* Input Buffer Disable Override */
#define HB_RCST_INP_DIS                (* (reg8 *) HB_RCST__INP_DIS)
/* LCD Common or Segment Drive */
#define HB_RCST_LCD_COM_SEG            (* (reg8 *) HB_RCST__LCD_COM_SEG)
/* Enable Segment LCD */
#define HB_RCST_LCD_EN                 (* (reg8 *) HB_RCST__LCD_EN)
/* Slew Rate Control */
#define HB_RCST_SLW                    (* (reg8 *) HB_RCST__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define HB_RCST_PRTDSI__CAPS_SEL       (* (reg8 *) HB_RCST__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define HB_RCST_PRTDSI__DBL_SYNC_IN    (* (reg8 *) HB_RCST__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define HB_RCST_PRTDSI__OE_SEL0        (* (reg8 *) HB_RCST__PRTDSI__OE_SEL0) 
#define HB_RCST_PRTDSI__OE_SEL1        (* (reg8 *) HB_RCST__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define HB_RCST_PRTDSI__OUT_SEL0       (* (reg8 *) HB_RCST__PRTDSI__OUT_SEL0) 
#define HB_RCST_PRTDSI__OUT_SEL1       (* (reg8 *) HB_RCST__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define HB_RCST_PRTDSI__SYNC_OUT       (* (reg8 *) HB_RCST__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(HB_RCST__SIO_CFG)
    #define HB_RCST_SIO_HYST_EN        (* (reg8 *) HB_RCST__SIO_HYST_EN)
    #define HB_RCST_SIO_REG_HIFREQ     (* (reg8 *) HB_RCST__SIO_REG_HIFREQ)
    #define HB_RCST_SIO_CFG            (* (reg8 *) HB_RCST__SIO_CFG)
    #define HB_RCST_SIO_DIFF           (* (reg8 *) HB_RCST__SIO_DIFF)
#endif /* (HB_RCST__SIO_CFG) */

/* Interrupt Registers */
#if defined(HB_RCST__INTSTAT)
    #define HB_RCST_INTSTAT            (* (reg8 *) HB_RCST__INTSTAT)
    #define HB_RCST_SNAP               (* (reg8 *) HB_RCST__SNAP)
    
	#define HB_RCST_0_INTTYPE_REG 		(* (reg8 *) HB_RCST__0__INTTYPE)
#endif /* (HB_RCST__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_HB_RCST_H */


/* [] END OF FILE */
