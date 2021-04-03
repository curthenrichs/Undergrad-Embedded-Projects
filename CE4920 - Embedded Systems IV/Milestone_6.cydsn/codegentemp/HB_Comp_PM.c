/*******************************************************************************
* File Name: HB_Comp.c
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

#include "HB_Comp.h"

static HB_Comp_backupStruct HB_Comp_backup;


/*******************************************************************************
* Function Name: HB_Comp_SaveConfig
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
void HB_Comp_SaveConfig(void) 
{
    /* Empty since all are system reset for retention flops */
}


/*******************************************************************************
* Function Name: HB_Comp_RestoreConfig
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
void HB_Comp_RestoreConfig(void) 
{
    /* Empty since all are system reset for retention flops */    
}


/*******************************************************************************
* Function Name: HB_Comp_Sleep
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
*  HB_Comp_backup.enableState:  Is modified depending on the enable 
*   state of the block before entering sleep mode.
*
*******************************************************************************/
void HB_Comp_Sleep(void) 
{
    /* Save Comp's enable state */    
    if(HB_Comp_ACT_PWR_EN == (HB_Comp_PWRMGR & HB_Comp_ACT_PWR_EN))
    {
        /* Comp is enabled */
        HB_Comp_backup.enableState = 1u;
    }
    else
    {
        /* Comp is disabled */
        HB_Comp_backup.enableState = 0u;
    }    
    
    HB_Comp_Stop();
    HB_Comp_SaveConfig();
}


/*******************************************************************************
* Function Name: HB_Comp_Wakeup
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
*  HB_Comp_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void HB_Comp_Wakeup(void) 
{
    HB_Comp_RestoreConfig();
    
    if(HB_Comp_backup.enableState == 1u)
    {
        /* Enable Comp's operation */
        HB_Comp_Enable();

    } /* Do nothing if Comp was disabled before */ 
}


/* [] END OF FILE */
