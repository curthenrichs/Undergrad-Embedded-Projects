/*******************************************************************************
* File Name: Slider_Pins.h
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

#if !defined(CY_CAPSENSE_CSD_Pins_Slider_H)
#define CY_CAPSENSE_CSD_Pins_Slider_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Slider.h"


/***************************************
*        Function Prototypes
***************************************/

void Slider_SetAllSensorsDriveMode(uint8 mode) ;
void Slider_SetAllCmodsDriveMode(uint8 mode) ;
#if (Slider_CURRENT_SOURCE == Slider_EXTERNAL_RB)
    void Slider_SetAllRbsDriveMode(uint8 mode) ;    
#endif  /* (Slider_CURRENT_SOURCE == Slider_EXTERNAL_RB) */   


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define Slider_DM_ALG_HIZ         (PIN_DM_ALG_HIZ)
#define Slider_DM_DIG_HIZ         (PIN_DM_DIG_HIZ)
#define Slider_DM_RES_UP          (PIN_DM_RES_UP)
#define Slider_DM_RES_DWN         (PIN_DM_RES_DWN)
#define Slider_DM_OD_LO           (PIN_DM_OD_LO)
#define Slider_DM_OD_HI           (PIN_DM_OD_HI)
#define Slider_DM_STRONG          (PIN_DM_STRONG)
#define Slider_DM_RES_UPDWN       (PIN_DM_RES_UPDWN)

/* PC registers defines for sensors */
#define Slider_PortCH0__LinearSlider_e0__LS  Slider_PortCH0__LinearSlider_e0__LS__PC
#define Slider_PortCH0__LinearSlider_e1__LS  Slider_PortCH0__LinearSlider_e1__LS__PC
#define Slider_PortCH0__LinearSlider_e2__LS  Slider_PortCH0__LinearSlider_e2__LS__PC
#define Slider_PortCH0__LinearSlider_e3__LS  Slider_PortCH0__LinearSlider_e3__LS__PC
#define Slider_PortCH0__LinearSlider_e4__LS  Slider_PortCH0__LinearSlider_e4__LS__PC
/* For Cmods*/
#define Slider_CmodCH0_Cmod_CH0       Slider_CmodCH0__Cmod_CH0__PC


#endif /* (CY_CAPSENSE_CSD_Pins_Slider_H) */


/* [] END OF FILE */
