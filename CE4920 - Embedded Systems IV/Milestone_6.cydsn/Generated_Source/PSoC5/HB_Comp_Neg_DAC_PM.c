/*******************************************************************************
* File Name: HB_Comp_Neg_DAC_PM.c  
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

#include "HB_Comp_Neg_DAC.h"

static HB_Comp_Neg_DAC_backupStruct HB_Comp_Neg_DAC_backup;


/*******************************************************************************
* Function Name: HB_Comp_Neg_DAC_SaveConfig
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
void HB_Comp_Neg_DAC_SaveConfig(void) 
{
    if (!((HB_Comp_Neg_DAC_CR1 & HB_Comp_Neg_DAC_SRC_MASK) == HB_Comp_Neg_DAC_SRC_UDB))
    {
        HB_Comp_Neg_DAC_backup.data_value = HB_Comp_Neg_DAC_Data;
    }
}


/*******************************************************************************
* Function Name: HB_Comp_Neg_DAC_RestoreConfig
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
void HB_Comp_Neg_DAC_RestoreConfig(void) 
{
    if (!((HB_Comp_Neg_DAC_CR1 & HB_Comp_Neg_DAC_SRC_MASK) == HB_Comp_Neg_DAC_SRC_UDB))
    {
        if((HB_Comp_Neg_DAC_Strobe & HB_Comp_Neg_DAC_STRB_MASK) == HB_Comp_Neg_DAC_STRB_EN)
        {
            HB_Comp_Neg_DAC_Strobe &= (uint8)(~HB_Comp_Neg_DAC_STRB_MASK);
            HB_Comp_Neg_DAC_Data = HB_Comp_Neg_DAC_backup.data_value;
            HB_Comp_Neg_DAC_Strobe |= HB_Comp_Neg_DAC_STRB_EN;
        }
        else
        {
            HB_Comp_Neg_DAC_Data = HB_Comp_Neg_DAC_backup.data_value;
        }
    }
}


/*******************************************************************************
* Function Name: HB_Comp_Neg_DAC_Sleep
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
*  HB_Comp_Neg_DAC_backup.enableState:  Is modified depending on the enable 
*  state  of the block before entering sleep mode.
*
*******************************************************************************/
void HB_Comp_Neg_DAC_Sleep(void) 
{
    /* Save VDAC8's enable state */    
    if(HB_Comp_Neg_DAC_ACT_PWR_EN == (HB_Comp_Neg_DAC_PWRMGR & HB_Comp_Neg_DAC_ACT_PWR_EN))
    {
        /* VDAC8 is enabled */
        HB_Comp_Neg_DAC_backup.enableState = 1u;
    }
    else
    {
        /* VDAC8 is disabled */
        HB_Comp_Neg_DAC_backup.enableState = 0u;
    }
    
    HB_Comp_Neg_DAC_Stop();
    HB_Comp_Neg_DAC_SaveConfig();
}


/*******************************************************************************
* Function Name: HB_Comp_Neg_DAC_Wakeup
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
*  HB_Comp_Neg_DAC_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void HB_Comp_Neg_DAC_Wakeup(void) 
{
    HB_Comp_Neg_DAC_RestoreConfig();
    
    if(HB_Comp_Neg_DAC_backup.enableState == 1u)
    {
        /* Enable VDAC8's operation */
        HB_Comp_Neg_DAC_Enable();

        /* Restore the data register */
        HB_Comp_Neg_DAC_SetValue(HB_Comp_Neg_DAC_Data);
    } /* Do nothing if VDAC8 was disabled before */    
}


/* [] END OF FILE */
