/*******************************************************************************
* File Name: Slider_Pins.c
* Version 3.50
*
* Description:
*  This file contains API to enable firmware control of a Pins component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "Slider_Pins.h"


/*******************************************************************************
* Function Name: Slider_SetAllSensorsDriveMode
********************************************************************************
*
* Summary:
*  Sets the drive mode for the all pins used by capacitive sensors within 
*  CapSense component.
* 
* Parameters:  
*  mode: Desired drive mode.
*
* Return: 
*  None
*
*******************************************************************************/
void Slider_SetAllSensorsDriveMode(uint8 mode) 
{
    /* Set pins drive mode */
    CyPins_SetPinDriveMode(Slider_PortCH0__LinearSlider_e0__LS, mode);
    CyPins_SetPinDriveMode(Slider_PortCH0__LinearSlider_e1__LS, mode);
    CyPins_SetPinDriveMode(Slider_PortCH0__LinearSlider_e2__LS, mode);
    CyPins_SetPinDriveMode(Slider_PortCH0__LinearSlider_e3__LS, mode);
    CyPins_SetPinDriveMode(Slider_PortCH0__LinearSlider_e4__LS, mode);

}



/*******************************************************************************
* Function Name: Slider_SetAllCmodsDriveMode
********************************************************************************
*
* Summary:
*  Sets the drive mode for the all pins used by Cmod capacitors within CapSense 
*  component.
* 
* Parameters:  
*  mode: Desired drive mode.
*
* Return: 
*  None
*
*******************************************************************************/
void Slider_SetAllCmodsDriveMode(uint8 mode) 
{
   /* Set pins drive mode */
    CyPins_SetPinDriveMode(Slider_CmodCH0_Cmod_CH0, mode);

}


#if (Slider_CURRENT_SOURCE == Slider_EXTERNAL_RB)
    /*******************************************************************************
    * Function Name: Slider_SetAllRbsDriveMode
    ********************************************************************************
    *
    * Summary:
    *  Sets the drive mode for the all pins used by bleed resistors (Rb) within 
    *  CapSense component. Only available when Current Source is external resistor.
    * 
    * Parameters:  
    *  mode: Desired drive mode.
    *
    * Return: 
    *  None
    *
    *******************************************************************************/
    void Slider_SetAllRbsDriveMode(uint8 mode) 
    {
        /* Set pins drive mode */
      
    }
#endif  /* (Slider_CURRENT_SOURCE == Slider_EXTERNAL_RB) */


/* [] END OF FILE */
