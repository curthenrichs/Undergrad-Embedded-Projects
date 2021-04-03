/*******************************************************************************
* File Name: HB_DAC_PM.c  
* Version 1.90
*
* Description:
*  This file provides the power management source code to API for the
*  VDAC8.  
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

#include "HB_DAC.h"

static HB_DAC_backupStruct HB_DAC_backup;


/*******************************************************************************
* Function Name: HB_DAC_SaveConfig
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
void HB_DAC_SaveConfig(void) 
{
    if (!((HB_DAC_CR1 & HB_DAC_SRC_MASK) == HB_DAC_SRC_UDB))
    {
        HB_DAC_backup.data_value = HB_DAC_Data;
    }
}


/*******************************************************************************
* Function Name: HB_DAC_RestoreConfig
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
void HB_DAC_RestoreConfig(void) 
{
    if (!((HB_DAC_CR1 & HB_DAC_SRC_MASK) == HB_DAC_SRC_UDB))
    {
        if((HB_DAC_Strobe & HB_DAC_STRB_MASK) == HB_DAC_STRB_EN)
        {
            HB_DAC_Strobe &= (uint8)(~HB_DAC_STRB_MASK);
            HB_DAC_Data = HB_DAC_backup.data_value;
            HB_DAC_Strobe |= HB_DAC_STRB_EN;
        }
        else
        {
            HB_DAC_Data = HB_DAC_backup.data_value;
        }
    }
}


/*******************************************************************************
* Function Name: HB_DAC_Sleep
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
*  HB_DAC_backup.enableState:  Is modified depending on the enable 
*  state  of the block before entering sleep mode.
*
*******************************************************************************/
void HB_DAC_Sleep(void) 
{
    /* Save VDAC8's enable state */    
    if(HB_DAC_ACT_PWR_EN == (HB_DAC_PWRMGR & HB_DAC_ACT_PWR_EN))
    {
        /* VDAC8 is enabled */
        HB_DAC_backup.enableState = 1u;
    }
    else
    {
        /* VDAC8 is disabled */
        HB_DAC_backup.enableState = 0u;
    }
    
    HB_DAC_Stop();
    HB_DAC_SaveConfig();
}


/*******************************************************************************
* Function Name: HB_DAC_Wakeup
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
*  HB_DAC_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void HB_DAC_Wakeup(void) 
{
    HB_DAC_RestoreConfig();
    
    if(HB_DAC_backup.enableState == 1u)
    {
        /* Enable VDAC8's operation */
        HB_DAC_Enable();

        /* Restore the data register */
        HB_DAC_SetValue(HB_DAC_Data);
    } /* Do nothing if VDAC8 was disabled before */    
}


/* [] END OF FILE */
