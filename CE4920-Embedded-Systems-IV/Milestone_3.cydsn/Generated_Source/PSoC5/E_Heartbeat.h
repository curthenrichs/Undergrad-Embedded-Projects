/*******************************************************************************
* File Name: E_Heartbeat.h  
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

#if !defined(CY_PINS_E_Heartbeat_H) /* Pins E_Heartbeat_H */
#define CY_PINS_E_Heartbeat_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "E_Heartbeat_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 E_Heartbeat__PORT == 15 && ((E_Heartbeat__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    E_Heartbeat_Write(uint8 value);
void    E_Heartbeat_SetDriveMode(uint8 mode);
uint8   E_Heartbeat_ReadDataReg(void);
uint8   E_Heartbeat_Read(void);
void    E_Heartbeat_SetInterruptMode(uint16 position, uint16 mode);
uint8   E_Heartbeat_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the E_Heartbeat_SetDriveMode() function.
     *  @{
     */
        #define E_Heartbeat_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define E_Heartbeat_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define E_Heartbeat_DM_RES_UP          PIN_DM_RES_UP
        #define E_Heartbeat_DM_RES_DWN         PIN_DM_RES_DWN
        #define E_Heartbeat_DM_OD_LO           PIN_DM_OD_LO
        #define E_Heartbeat_DM_OD_HI           PIN_DM_OD_HI
        #define E_Heartbeat_DM_STRONG          PIN_DM_STRONG
        #define E_Heartbeat_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define E_Heartbeat_MASK               E_Heartbeat__MASK
#define E_Heartbeat_SHIFT              E_Heartbeat__SHIFT
#define E_Heartbeat_WIDTH              1u

/* Interrupt constants */
#if defined(E_Heartbeat__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in E_Heartbeat_SetInterruptMode() function.
     *  @{
     */
        #define E_Heartbeat_INTR_NONE      (uint16)(0x0000u)
        #define E_Heartbeat_INTR_RISING    (uint16)(0x0001u)
        #define E_Heartbeat_INTR_FALLING   (uint16)(0x0002u)
        #define E_Heartbeat_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define E_Heartbeat_INTR_MASK      (0x01u) 
#endif /* (E_Heartbeat__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define E_Heartbeat_PS                     (* (reg8 *) E_Heartbeat__PS)
/* Data Register */
#define E_Heartbeat_DR                     (* (reg8 *) E_Heartbeat__DR)
/* Port Number */
#define E_Heartbeat_PRT_NUM                (* (reg8 *) E_Heartbeat__PRT) 
/* Connect to Analog Globals */                                                  
#define E_Heartbeat_AG                     (* (reg8 *) E_Heartbeat__AG)                       
/* Analog MUX bux enable */
#define E_Heartbeat_AMUX                   (* (reg8 *) E_Heartbeat__AMUX) 
/* Bidirectional Enable */                                                        
#define E_Heartbeat_BIE                    (* (reg8 *) E_Heartbeat__BIE)
/* Bit-mask for Aliased Register Access */
#define E_Heartbeat_BIT_MASK               (* (reg8 *) E_Heartbeat__BIT_MASK)
/* Bypass Enable */
#define E_Heartbeat_BYP                    (* (reg8 *) E_Heartbeat__BYP)
/* Port wide control signals */                                                   
#define E_Heartbeat_CTL                    (* (reg8 *) E_Heartbeat__CTL)
/* Drive Modes */
#define E_Heartbeat_DM0                    (* (reg8 *) E_Heartbeat__DM0) 
#define E_Heartbeat_DM1                    (* (reg8 *) E_Heartbeat__DM1)
#define E_Heartbeat_DM2                    (* (reg8 *) E_Heartbeat__DM2) 
/* Input Buffer Disable Override */
#define E_Heartbeat_INP_DIS                (* (reg8 *) E_Heartbeat__INP_DIS)
/* LCD Common or Segment Drive */
#define E_Heartbeat_LCD_COM_SEG            (* (reg8 *) E_Heartbeat__LCD_COM_SEG)
/* Enable Segment LCD */
#define E_Heartbeat_LCD_EN                 (* (reg8 *) E_Heartbeat__LCD_EN)
/* Slew Rate Control */
#define E_Heartbeat_SLW                    (* (reg8 *) E_Heartbeat__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define E_Heartbeat_PRTDSI__CAPS_SEL       (* (reg8 *) E_Heartbeat__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define E_Heartbeat_PRTDSI__DBL_SYNC_IN    (* (reg8 *) E_Heartbeat__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define E_Heartbeat_PRTDSI__OE_SEL0        (* (reg8 *) E_Heartbeat__PRTDSI__OE_SEL0) 
#define E_Heartbeat_PRTDSI__OE_SEL1        (* (reg8 *) E_Heartbeat__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define E_Heartbeat_PRTDSI__OUT_SEL0       (* (reg8 *) E_Heartbeat__PRTDSI__OUT_SEL0) 
#define E_Heartbeat_PRTDSI__OUT_SEL1       (* (reg8 *) E_Heartbeat__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define E_Heartbeat_PRTDSI__SYNC_OUT       (* (reg8 *) E_Heartbeat__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(E_Heartbeat__SIO_CFG)
    #define E_Heartbeat_SIO_HYST_EN        (* (reg8 *) E_Heartbeat__SIO_HYST_EN)
    #define E_Heartbeat_SIO_REG_HIFREQ     (* (reg8 *) E_Heartbeat__SIO_REG_HIFREQ)
    #define E_Heartbeat_SIO_CFG            (* (reg8 *) E_Heartbeat__SIO_CFG)
    #define E_Heartbeat_SIO_DIFF           (* (reg8 *) E_Heartbeat__SIO_DIFF)
#endif /* (E_Heartbeat__SIO_CFG) */

/* Interrupt Registers */
#if defined(E_Heartbeat__INTSTAT)
    #define E_Heartbeat_INTSTAT            (* (reg8 *) E_Heartbeat__INTSTAT)
    #define E_Heartbeat_SNAP               (* (reg8 *) E_Heartbeat__SNAP)
    
	#define E_Heartbeat_0_INTTYPE_REG 		(* (reg8 *) E_Heartbeat__0__INTTYPE)
#endif /* (E_Heartbeat__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_E_Heartbeat_H */


/* [] END OF FILE */
