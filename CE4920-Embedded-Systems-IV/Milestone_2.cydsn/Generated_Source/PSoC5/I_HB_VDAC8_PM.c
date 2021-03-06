/*******************************************************************************
* File Name: I_HB_VDAC8_PM.c  
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

#include "I_HB_VDAC8.h"

static I_HB_VDAC8_backupStruct I_HB_VDAC8_backup;


/*******************************************************************************
* Function Name: I_HB_VDAC8_SaveConfig
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
void I_HB_VDAC8_SaveConfig(void) 
{
    if (!((I_HB_VDAC8_CR1 & I_HB_VDAC8_SRC_MASK) == I_HB_VDAC8_SRC_UDB))
    {
        I_HB_VDAC8_backup.data_value = I_HB_VDAC8_Data;
    }
}


/*******************************************************************************
* Function Name: I_HB_VDAC8_RestoreConfig
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
void I_HB_VDAC8_RestoreConfig(void) 
{
    if (!((I_HB_VDAC8_CR1 & I_HB_VDAC8_SRC_MASK) == I_HB_VDAC8_SRC_UDB))
    {
        if((I_HB_VDAC8_Strobe & I_HB_VDAC8_STRB_MASK) == I_HB_VDAC8_STRB_EN)
        {
            I_HB_VDAC8_Strobe &= (uint8)(~I_HB_VDAC8_STRB_MASK);
            I_HB_VDAC8_Data = I_HB_VDAC8_backup.data_value;
            I_HB_VDAC8_Strobe |= I_HB_VDAC8_STRB_EN;
        }
        else
        {
            I_HB_VDAC8_Data = I_HB_VDAC8_backup.data_value;
        }
    }
}


/*******************************************************************************
* Function Name: I_HB_VDAC8_Sleep
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
*  I_HB_VDAC8_backup.enableState:  Is modified depending on the enable 
*  state  of the block before entering sleep mode.
*
*******************************************************************************/
void I_HB_VDAC8_Sleep(void) 
{
    /* Save VDAC8's enable state */    
    if(I_HB_VDAC8_ACT_PWR_EN == (I_HB_VDAC8_PWRMGR & I_HB_VDAC8_ACT_PWR_EN))
    {
        /* VDAC8 is enabled */
        I_HB_VDAC8_backup.enableState = 1u;
    }
    else
    {
        /* VDAC8 is disabled */
        I_HB_VDAC8_backup.enableState = 0u;
    }
    
    I_HB_VDAC8_Stop();
    I_HB_VDAC8_SaveConfig();
}


/*******************************************************************************
* Function Name: I_HB_VDAC8_Wakeup
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
*  I_HB_VDAC8_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void I_HB_VDAC8_Wakeup(void) 
{
    I_HB_VDAC8_RestoreConfig();
    
    if(I_HB_VDAC8_backup.enableState == 1u)
    {
        /* Enable VDAC8's operation */
        I_HB_VDAC8_Enable();

        /* Restore the data register */
        I_HB_VDAC8_SetValue(I_HB_VDAC8_Data);
    } /* Do nothing if VDAC8 was disabled before */    
}


/* [] END OF FILE */
