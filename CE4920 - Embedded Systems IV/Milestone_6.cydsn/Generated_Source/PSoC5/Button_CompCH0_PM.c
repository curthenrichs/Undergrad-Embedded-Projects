/*******************************************************************************
* File Name: Button_CompCH0.c
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

#include "Button_CompCH0.h"

static Button_CompCH0_backupStruct Button_CompCH0_backup;


/*******************************************************************************
* Function Name: Button_CompCH0_SaveConfig
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
void Button_CompCH0_SaveConfig(void) 
{
    /* Empty since all are system reset for retention flops */
}


/*******************************************************************************
* Function Name: Button_CompCH0_RestoreConfig
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
void Button_CompCH0_RestoreConfig(void) 
{
    /* Empty since all are system reset for retention flops */    
}


/*******************************************************************************
* Function Name: Button_CompCH0_Sleep
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
*  Button_CompCH0_backup.enableState:  Is modified depending on the enable 
*   state of the block before entering sleep mode.
*
*******************************************************************************/
void Button_CompCH0_Sleep(void) 
{
    /* Save Comp's enable state */    
    if(Button_CompCH0_ACT_PWR_EN == (Button_CompCH0_PWRMGR & Button_CompCH0_ACT_PWR_EN))
    {
        /* Comp is enabled */
        Button_CompCH0_backup.enableState = 1u;
    }
    else
    {
        /* Comp is disabled */
        Button_CompCH0_backup.enableState = 0u;
    }    
    
    Button_CompCH0_Stop();
    Button_CompCH0_SaveConfig();
}


/*******************************************************************************
* Function Name: Button_CompCH0_Wakeup
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
*  Button_CompCH0_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Button_CompCH0_Wakeup(void) 
{
    Button_CompCH0_RestoreConfig();
    
    if(Button_CompCH0_backup.enableState == 1u)
    {
        /* Enable Comp's operation */
        Button_CompCH0_Enable();

    } /* Do nothing if Comp was disabled before */ 
}


/* [] END OF FILE */
