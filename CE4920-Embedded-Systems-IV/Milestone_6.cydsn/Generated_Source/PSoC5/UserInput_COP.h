/*******************************************************************************
* File Name: UserInput_COP.h  
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

#if !defined(CY_PINS_UserInput_COP_H) /* Pins UserInput_COP_H */
#define CY_PINS_UserInput_COP_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "UserInput_COP_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 UserInput_COP__PORT == 15 && ((UserInput_COP__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    UserInput_COP_Write(uint8 value);
void    UserInput_COP_SetDriveMode(uint8 mode);
uint8   UserInput_COP_ReadDataReg(void);
uint8   UserInput_COP_Read(void);
void    UserInput_COP_SetInterruptMode(uint16 position, uint16 mode);
uint8   UserInput_COP_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the UserInput_COP_SetDriveMode() function.
     *  @{
     */
        #define UserInput_COP_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define UserInput_COP_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define UserInput_COP_DM_RES_UP          PIN_DM_RES_UP
        #define UserInput_COP_DM_RES_DWN         PIN_DM_RES_DWN
        #define UserInput_COP_DM_OD_LO           PIN_DM_OD_LO
        #define UserInput_COP_DM_OD_HI           PIN_DM_OD_HI
        #define UserInput_COP_DM_STRONG          PIN_DM_STRONG
        #define UserInput_COP_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define UserInput_COP_MASK               UserInput_COP__MASK
#define UserInput_COP_SHIFT              UserInput_COP__SHIFT
#define UserInput_COP_WIDTH              1u

/* Interrupt constants */
#if defined(UserInput_COP__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in UserInput_COP_SetInterruptMode() function.
     *  @{
     */
        #define UserInput_COP_INTR_NONE      (uint16)(0x0000u)
        #define UserInput_COP_INTR_RISING    (uint16)(0x0001u)
        #define UserInput_COP_INTR_FALLING   (uint16)(0x0002u)
        #define UserInput_COP_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define UserInput_COP_INTR_MASK      (0x01u) 
#endif /* (UserInput_COP__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define UserInput_COP_PS                     (* (reg8 *) UserInput_COP__PS)
/* Data Register */
#define UserInput_COP_DR                     (* (reg8 *) UserInput_COP__DR)
/* Port Number */
#define UserInput_COP_PRT_NUM                (* (reg8 *) UserInput_COP__PRT) 
/* Connect to Analog Globals */                                                  
#define UserInput_COP_AG                     (* (reg8 *) UserInput_COP__AG)                       
/* Analog MUX bux enable */
#define UserInput_COP_AMUX                   (* (reg8 *) UserInput_COP__AMUX) 
/* Bidirectional Enable */                                                        
#define UserInput_COP_BIE                    (* (reg8 *) UserInput_COP__BIE)
/* Bit-mask for Aliased Register Access */
#define UserInput_COP_BIT_MASK               (* (reg8 *) UserInput_COP__BIT_MASK)
/* Bypass Enable */
#define UserInput_COP_BYP                    (* (reg8 *) UserInput_COP__BYP)
/* Port wide control signals */                                                   
#define UserInput_COP_CTL                    (* (reg8 *) UserInput_COP__CTL)
/* Drive Modes */
#define UserInput_COP_DM0                    (* (reg8 *) UserInput_COP__DM0) 
#define UserInput_COP_DM1                    (* (reg8 *) UserInput_COP__DM1)
#define UserInput_COP_DM2                    (* (reg8 *) UserInput_COP__DM2) 
/* Input Buffer Disable Override */
#define UserInput_COP_INP_DIS                (* (reg8 *) UserInput_COP__INP_DIS)
/* LCD Common or Segment Drive */
#define UserInput_COP_LCD_COM_SEG            (* (reg8 *) UserInput_COP__LCD_COM_SEG)
/* Enable Segment LCD */
#define UserInput_COP_LCD_EN                 (* (reg8 *) UserInput_COP__LCD_EN)
/* Slew Rate Control */
#define UserInput_COP_SLW                    (* (reg8 *) UserInput_COP__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define UserInput_COP_PRTDSI__CAPS_SEL       (* (reg8 *) UserInput_COP__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define UserInput_COP_PRTDSI__DBL_SYNC_IN    (* (reg8 *) UserInput_COP__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define UserInput_COP_PRTDSI__OE_SEL0        (* (reg8 *) UserInput_COP__PRTDSI__OE_SEL0) 
#define UserInput_COP_PRTDSI__OE_SEL1        (* (reg8 *) UserInput_COP__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define UserInput_COP_PRTDSI__OUT_SEL0       (* (reg8 *) UserInput_COP__PRTDSI__OUT_SEL0) 
#define UserInput_COP_PRTDSI__OUT_SEL1       (* (reg8 *) UserInput_COP__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define UserInput_COP_PRTDSI__SYNC_OUT       (* (reg8 *) UserInput_COP__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(UserInput_COP__SIO_CFG)
    #define UserInput_COP_SIO_HYST_EN        (* (reg8 *) UserInput_COP__SIO_HYST_EN)
    #define UserInput_COP_SIO_REG_HIFREQ     (* (reg8 *) UserInput_COP__SIO_REG_HIFREQ)
    #define UserInput_COP_SIO_CFG            (* (reg8 *) UserInput_COP__SIO_CFG)
    #define UserInput_COP_SIO_DIFF           (* (reg8 *) UserInput_COP__SIO_DIFF)
#endif /* (UserInput_COP__SIO_CFG) */

/* Interrupt Registers */
#if defined(UserInput_COP__INTSTAT)
    #define UserInput_COP_INTSTAT            (* (reg8 *) UserInput_COP__INTSTAT)
    #define UserInput_COP_SNAP               (* (reg8 *) UserInput_COP__SNAP)
    
	#define UserInput_COP_0_INTTYPE_REG 		(* (reg8 *) UserInput_COP__0__INTTYPE)
#endif /* (UserInput_COP__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_UserInput_COP_H */


/* [] END OF FILE */
