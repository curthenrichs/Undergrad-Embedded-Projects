/*******************************************************************************
* File Name: Button_IdacCH0.c
* Version 2.0
*
* Description:
*  This file provides the power management source code to API for the
*  IDAC8.
*
* Note:
*  None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/


#include "Button_IdacCH0.h"

static Button_IdacCH0_backupStruct Button_IdacCH0_backup;


/*******************************************************************************
* Function Name: Button_IdacCH0_SaveConfig
********************************************************************************
* Summary:
*  Save the current user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void Button_IdacCH0_SaveConfig(void) 
{
    if (!((Button_IdacCH0_CR1 & Button_IdacCH0_SRC_MASK) == Button_IdacCH0_SRC_UDB))
    {
        Button_IdacCH0_backup.data_value = Button_IdacCH0_Data;
    }
}


/*******************************************************************************
* Function Name: Button_IdacCH0_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void Button_IdacCH0_RestoreConfig(void) 
{
    if (!((Button_IdacCH0_CR1 & Button_IdacCH0_SRC_MASK) == Button_IdacCH0_SRC_UDB))
    {
        if((Button_IdacCH0_Strobe & Button_IdacCH0_STRB_MASK) == Button_IdacCH0_STRB_EN)
        {
            Button_IdacCH0_Strobe &= (uint8)(~Button_IdacCH0_STRB_MASK);
            Button_IdacCH0_Data = Button_IdacCH0_backup.data_value;
            Button_IdacCH0_Strobe |= Button_IdacCH0_STRB_EN;
        }
        else
        {
            Button_IdacCH0_Data = Button_IdacCH0_backup.data_value;
        }
    }
}


/*******************************************************************************
* Function Name: Button_IdacCH0_Sleep
********************************************************************************
* Summary:
*  Stop and Save the user configuration
*
* Parameters:
*  void:
*
* Return:
*  void
*
* Global variables:
*  Button_IdacCH0_backup.enableState: Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Button_IdacCH0_Sleep(void) 
{
    if(Button_IdacCH0_ACT_PWR_EN == (Button_IdacCH0_PWRMGR & Button_IdacCH0_ACT_PWR_EN))
    {
        /* IDAC8 is enabled */
        Button_IdacCH0_backup.enableState = 1u;
    }
    else
    {
        /* IDAC8 is disabled */
        Button_IdacCH0_backup.enableState = 0u;
    }

    Button_IdacCH0_Stop();
    Button_IdacCH0_SaveConfig();
}


/*******************************************************************************
* Function Name: Button_IdacCH0_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*  
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Button_IdacCH0_backup.enableState: Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Button_IdacCH0_Wakeup(void) 
{
    Button_IdacCH0_RestoreConfig();
    
    if(Button_IdacCH0_backup.enableState == 1u)
    {
        /* Enable IDAC8's operation */
        Button_IdacCH0_Enable();
        
        /* Set the data register */
        Button_IdacCH0_SetValue(Button_IdacCH0_Data);
    } /* Do nothing if IDAC8 was disabled before */    
}


/* [] END OF FILE */
