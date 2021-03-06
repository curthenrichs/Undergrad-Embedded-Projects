/*******************************************************************************
* File Name: Slider_IdacCH0.c
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


#include "Slider_IdacCH0.h"

static Slider_IdacCH0_backupStruct Slider_IdacCH0_backup;


/*******************************************************************************
* Function Name: Slider_IdacCH0_SaveConfig
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
void Slider_IdacCH0_SaveConfig(void) 
{
    if (!((Slider_IdacCH0_CR1 & Slider_IdacCH0_SRC_MASK) == Slider_IdacCH0_SRC_UDB))
    {
        Slider_IdacCH0_backup.data_value = Slider_IdacCH0_Data;
    }
}


/*******************************************************************************
* Function Name: Slider_IdacCH0_RestoreConfig
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
void Slider_IdacCH0_RestoreConfig(void) 
{
    if (!((Slider_IdacCH0_CR1 & Slider_IdacCH0_SRC_MASK) == Slider_IdacCH0_SRC_UDB))
    {
        if((Slider_IdacCH0_Strobe & Slider_IdacCH0_STRB_MASK) == Slider_IdacCH0_STRB_EN)
        {
            Slider_IdacCH0_Strobe &= (uint8)(~Slider_IdacCH0_STRB_MASK);
            Slider_IdacCH0_Data = Slider_IdacCH0_backup.data_value;
            Slider_IdacCH0_Strobe |= Slider_IdacCH0_STRB_EN;
        }
        else
        {
            Slider_IdacCH0_Data = Slider_IdacCH0_backup.data_value;
        }
    }
}


/*******************************************************************************
* Function Name: Slider_IdacCH0_Sleep
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
*  Slider_IdacCH0_backup.enableState: Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Slider_IdacCH0_Sleep(void) 
{
    if(Slider_IdacCH0_ACT_PWR_EN == (Slider_IdacCH0_PWRMGR & Slider_IdacCH0_ACT_PWR_EN))
    {
        /* IDAC8 is enabled */
        Slider_IdacCH0_backup.enableState = 1u;
    }
    else
    {
        /* IDAC8 is disabled */
        Slider_IdacCH0_backup.enableState = 0u;
    }

    Slider_IdacCH0_Stop();
    Slider_IdacCH0_SaveConfig();
}


/*******************************************************************************
* Function Name: Slider_IdacCH0_Wakeup
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
*  Slider_IdacCH0_backup.enableState: Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Slider_IdacCH0_Wakeup(void) 
{
    Slider_IdacCH0_RestoreConfig();
    
    if(Slider_IdacCH0_backup.enableState == 1u)
    {
        /* Enable IDAC8's operation */
        Slider_IdacCH0_Enable();
        
        /* Set the data register */
        Slider_IdacCH0_SetValue(Slider_IdacCH0_Data);
    } /* Do nothing if IDAC8 was disabled before */    
}


/* [] END OF FILE */
