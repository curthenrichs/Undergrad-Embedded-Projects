/*******************************************************************************
* File Name: Button_Pins.h
* Version 3.50
*
* Description:
*  This file contains API to enable firmware control of a Pins component.
*
* Note:
*  None
*
********************************************************************************
* Copyright 2008-2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CAPSENSE_CSD_Pins_Button_H)
#define CY_CAPSENSE_CSD_Pins_Button_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Button.h"


/***************************************
*        Function Prototypes
***************************************/

void Button_SetAllSensorsDriveMode(uint8 mode) ;
void Button_SetAllCmodsDriveMode(uint8 mode) ;
#if (Button_CURRENT_SOURCE == Button_EXTERNAL_RB)
    void Button_SetAllRbsDriveMode(uint8 mode) ;    
#endif  /* (Button_CURRENT_SOURCE == Button_EXTERNAL_RB) */   


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define Button_DM_ALG_HIZ         (PIN_DM_ALG_HIZ)
#define Button_DM_DIG_HIZ         (PIN_DM_DIG_HIZ)
#define Button_DM_RES_UP          (PIN_DM_RES_UP)
#define Button_DM_RES_DWN         (PIN_DM_RES_DWN)
#define Button_DM_OD_LO           (PIN_DM_OD_LO)
#define Button_DM_OD_HI           (PIN_DM_OD_HI)
#define Button_DM_STRONG          (PIN_DM_STRONG)
#define Button_DM_RES_UPDWN       (PIN_DM_RES_UPDWN)

/* PC registers defines for sensors */
#define Button_PortCH0__Left__BTN  Button_PortCH0__Left__BTN__PC
#define Button_PortCH0__Right__BTN  Button_PortCH0__Right__BTN__PC
/* For Cmods*/
#define Button_CmodCH0_Cmod_CH0       Button_CmodCH0__Cmod_CH0__PC


#endif /* (CY_CAPSENSE_CSD_Pins_Button_H) */


/* [] END OF FILE */
