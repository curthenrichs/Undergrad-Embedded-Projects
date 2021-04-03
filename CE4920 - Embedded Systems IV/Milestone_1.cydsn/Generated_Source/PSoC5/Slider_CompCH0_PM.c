/*******************************************************************************
* File Name: Slider_CompCH0.c
* Version 2.0
*
* Description:
*  This file provides the power management source code APIs for the
*  Comparator.
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

#include "Slider_CompCH0.h"

static Slider_CompCH0_backupStruct Slider_CompCH0_backup;


/*******************************************************************************
* Function Name: Slider_CompCH0_SaveConfig
********************************************************************************
*
* Summary:
*  Save the current user configuration
*
* Parameters:
*  void:
*
* Return:
*  void
*
*******************************************************************************/
void Slider_CompCH0_SaveConfig(void) 
{
    /* Empty since all are system reset for retention flops */
}


/*******************************************************************************
* Function Name: Slider_CompCH0_RestoreConfig
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
********************************************************************************/
void Slider_CompCH0_RestoreConfig(void) 
{
    /* Empty since all are system reset for retention flops */    
}


/*******************************************************************************
* Function Name: Slider_CompCH0_Sleep
********************************************************************************
*
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
*  Slider_CompCH0_backup.enableState:  Is modified depending on the enable 
*   state of the block before entering sleep mode.
*
*******************************************************************************/
void Slider_CompCH0_Sleep(void) 
{
    /* Save Comp's enable state */    
    if(Slider_CompCH0_ACT_PWR_EN == (Slider_CompCH0_PWRMGR & Slider_CompCH0_ACT_PWR_EN))
    {
        /* Comp is enabled */
        Slider_CompCH0_backup.enableState = 1u;
    }
    else
    {
        /* Comp is disabled */
        Slider_CompCH0_backup.enableState = 0u;
    }    
    
    Slider_CompCH0_Stop();
    Slider_CompCH0_SaveConfig();
}


/*******************************************************************************
* Function Name: Slider_CompCH0_Wakeup
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
*  Slider_CompCH0_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Slider_CompCH0_Wakeup(void) 
{
    Slider_CompCH0_RestoreConfig();
    
    if(Slider_CompCH0_backup.enableState == 1u)
    {
        /* Enable Comp's operation */
        Slider_CompCH0_Enable();

    } /* Do nothing if Comp was disabled before */ 
}


/* [] END OF FILE */
