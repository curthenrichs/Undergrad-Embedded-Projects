/*******************************************************************************
* File Name: Heartbeat_COP.h  
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

#if !defined(CY_PINS_Heartbeat_COP_H) /* Pins Heartbeat_COP_H */
#define CY_PINS_Heartbeat_COP_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Heartbeat_COP_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Heartbeat_COP__PORT == 15 && ((Heartbeat_COP__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Heartbeat_COP_Write(uint8 value);
void    Heartbeat_COP_SetDriveMode(uint8 mode);
uint8   Heartbeat_COP_ReadDataReg(void);
uint8   Heartbeat_COP_Read(void);
void    Heartbeat_COP_SetInterruptMode(uint16 position, uint16 mode);
uint8   Heartbeat_COP_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Heartbeat_COP_SetDriveMode() function.
     *  @{
     */
        #define Heartbeat_COP_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Heartbeat_COP_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Heartbeat_COP_DM_RES_UP          PIN_DM_RES_UP
        #define Heartbeat_COP_DM_RES_DWN         PIN_DM_RES_DWN
        #define Heartbeat_COP_DM_OD_LO           PIN_DM_OD_LO
        #define Heartbeat_COP_DM_OD_HI           PIN_DM_OD_HI
        #define Heartbeat_COP_DM_STRONG          PIN_DM_STRONG
        #define Heartbeat_COP_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Heartbeat_COP_MASK               Heartbeat_COP__MASK
#define Heartbeat_COP_SHIFT              Heartbeat_COP__SHIFT
#define Heartbeat_COP_WIDTH              1u

/* Interrupt constants */
#if defined(Heartbeat_COP__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Heartbeat_COP_SetInterruptMode() function.
     *  @{
     */
        #define Heartbeat_COP_INTR_NONE      (uint16)(0x0000u)
        #define Heartbeat_COP_INTR_RISING    (uint16)(0x0001u)
        #define Heartbeat_COP_INTR_FALLING   (uint16)(0x0002u)
        #define Heartbeat_COP_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Heartbeat_COP_INTR_MASK      (0x01u) 
#endif /* (Heartbeat_COP__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Heartbeat_COP_PS                     (* (reg8 *) Heartbeat_COP__PS)
/* Data Register */
#define Heartbeat_COP_DR                     (* (reg8 *) Heartbeat_COP__DR)
/* Port Number */
#define Heartbeat_COP_PRT_NUM                (* (reg8 *) Heartbeat_COP__PRT) 
/* Connect to Analog Globals */                                                  
#define Heartbeat_COP_AG                     (* (reg8 *) Heartbeat_COP__AG)                       
/* Analog MUX bux enable */
#define Heartbeat_COP_AMUX                   (* (reg8 *) Heartbeat_COP__AMUX) 
/* Bidirectional Enable */                                                        
#define Heartbeat_COP_BIE                    (* (reg8 *) Heartbeat_COP__BIE)
/* Bit-mask for Aliased Register Access */
#define Heartbeat_COP_BIT_MASK               (* (reg8 *) Heartbeat_COP__BIT_MASK)
/* Bypass Enable */
#define Heartbeat_COP_BYP                    (* (reg8 *) Heartbeat_COP__BYP)
/* Port wide control signals */                                                   
#define Heartbeat_COP_CTL                    (* (reg8 *) Heartbeat_COP__CTL)
/* Drive Modes */
#define Heartbeat_COP_DM0                    (* (reg8 *) Heartbeat_COP__DM0) 
#define Heartbeat_COP_DM1                    (* (reg8 *) Heartbeat_COP__DM1)
#define Heartbeat_COP_DM2                    (* (reg8 *) Heartbeat_COP__DM2) 
/* Input Buffer Disable Override */
#define Heartbeat_COP_INP_DIS                (* (reg8 *) Heartbeat_COP__INP_DIS)
/* LCD Common or Segment Drive */
#define Heartbeat_COP_LCD_COM_SEG            (* (reg8 *) Heartbeat_COP__LCD_COM_SEG)
/* Enable Segment LCD */
#define Heartbeat_COP_LCD_EN                 (* (reg8 *) Heartbeat_COP__LCD_EN)
/* Slew Rate Control */
#define Heartbeat_COP_SLW                    (* (reg8 *) Heartbeat_COP__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Heartbeat_COP_PRTDSI__CAPS_SEL       (* (reg8 *) Heartbeat_COP__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Heartbeat_COP_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Heartbeat_COP__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Heartbeat_COP_PRTDSI__OE_SEL0        (* (reg8 *) Heartbeat_COP__PRTDSI__OE_SEL0) 
#define Heartbeat_COP_PRTDSI__OE_SEL1        (* (reg8 *) Heartbeat_COP__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Heartbeat_COP_PRTDSI__OUT_SEL0       (* (reg8 *) Heartbeat_COP__PRTDSI__OUT_SEL0) 
#define Heartbeat_COP_PRTDSI__OUT_SEL1       (* (reg8 *) Heartbeat_COP__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Heartbeat_COP_PRTDSI__SYNC_OUT       (* (reg8 *) Heartbeat_COP__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Heartbeat_COP__SIO_CFG)
    #define Heartbeat_COP_SIO_HYST_EN        (* (reg8 *) Heartbeat_COP__SIO_HYST_EN)
    #define Heartbeat_COP_SIO_REG_HIFREQ     (* (reg8 *) Heartbeat_COP__SIO_REG_HIFREQ)
    #define Heartbeat_COP_SIO_CFG            (* (reg8 *) Heartbeat_COP__SIO_CFG)
    #define Heartbeat_COP_SIO_DIFF           (* (reg8 *) Heartbeat_COP__SIO_DIFF)
#endif /* (Heartbeat_COP__SIO_CFG) */

/* Interrupt Registers */
#if defined(Heartbeat_COP__INTSTAT)
    #define Heartbeat_COP_INTSTAT            (* (reg8 *) Heartbeat_COP__INTSTAT)
    #define Heartbeat_COP_SNAP               (* (reg8 *) Heartbeat_COP__SNAP)
    
	#define Heartbeat_COP_0_INTTYPE_REG 		(* (reg8 *) Heartbeat_COP__0__INTTYPE)
#endif /* (Heartbeat_COP__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Heartbeat_COP_H */


/* [] END OF FILE */
