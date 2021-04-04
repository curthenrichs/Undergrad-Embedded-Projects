/*******************************************************************************
* File Name: Button_Pins.c
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

#include "Button_Pins.h"


/*******************************************************************************
* Function Name: Button_SetAllSensorsDriveMode
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
void Button_SetAllSensorsDriveMode(uint8 mode) 
{
    /* Set pins drive mode */
    CyPins_SetPinDriveMode(Button_PortCH0__Left__BTN, mode);
    CyPins_SetPinDriveMode(Button_PortCH0__Right__BTN, mode);

}



/*******************************************************************************
* Function Name: Button_SetAllCmodsDriveMode
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
void Button_SetAllCmodsDriveMode(uint8 mode) 
{
   /* Set pins drive mode */
    CyPins_SetPinDriveMode(Button_CmodCH0_Cmod_CH0, mode);

}


#if (Button_CURRENT_SOURCE == Button_EXTERNAL_RB)
    /*******************************************************************************
    * Function Name: Button_SetAllRbsDriveMode
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
    void Button_SetAllRbsDriveMode(uint8 mode) 
    {
        /* Set pins drive mode */
      
    }
#endif  /* (Button_CURRENT_SOURCE == Button_EXTERNAL_RB) */


/* [] END OF FILE */
