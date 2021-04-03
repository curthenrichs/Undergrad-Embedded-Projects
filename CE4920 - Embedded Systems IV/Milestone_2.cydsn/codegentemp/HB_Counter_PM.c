/*******************************************************************************
* File Name: HB_Counter_PM.c  
* Version 3.0
*
*  Description:
*    This file provides the power management source code to API for the
*    Counter.  
*
*   Note:
*     None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "HB_Counter.h"

static HB_Counter_backupStruct HB_Counter_backup;


/*******************************************************************************
* Function Name: HB_Counter_SaveConfig
********************************************************************************
* Summary:
*     Save the current user configuration
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  HB_Counter_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void HB_Counter_SaveConfig(void) 
{
    #if (!HB_Counter_UsingFixedFunction)

        HB_Counter_backup.CounterUdb = HB_Counter_ReadCounter();

        #if(!HB_Counter_ControlRegRemoved)
            HB_Counter_backup.CounterControlRegister = HB_Counter_ReadControlRegister();
        #endif /* (!HB_Counter_ControlRegRemoved) */

    #endif /* (!HB_Counter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: HB_Counter_RestoreConfig
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
* Global variables:
*  HB_Counter_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void HB_Counter_RestoreConfig(void) 
{      
    #if (!HB_Counter_UsingFixedFunction)

       HB_Counter_WriteCounter(HB_Counter_backup.CounterUdb);

        #if(!HB_Counter_ControlRegRemoved)
            HB_Counter_WriteControlRegister(HB_Counter_backup.CounterControlRegister);
        #endif /* (!HB_Counter_ControlRegRemoved) */

    #endif /* (!HB_Counter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: HB_Counter_Sleep
********************************************************************************
* Summary:
*     Stop and Save the user configuration
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  HB_Counter_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void HB_Counter_Sleep(void) 
{
    #if(!HB_Counter_ControlRegRemoved)
        /* Save Counter's enable state */
        if(HB_Counter_CTRL_ENABLE == (HB_Counter_CONTROL & HB_Counter_CTRL_ENABLE))
        {
            /* Counter is enabled */
            HB_Counter_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            HB_Counter_backup.CounterEnableState = 0u;
        }
    #else
        HB_Counter_backup.CounterEnableState = 1u;
        if(HB_Counter_backup.CounterEnableState != 0u)
        {
            HB_Counter_backup.CounterEnableState = 0u;
        }
    #endif /* (!HB_Counter_ControlRegRemoved) */
    
    HB_Counter_Stop();
    HB_Counter_SaveConfig();
}


/*******************************************************************************
* Function Name: HB_Counter_Wakeup
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
*  HB_Counter_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void HB_Counter_Wakeup(void) 
{
    HB_Counter_RestoreConfig();
    #if(!HB_Counter_ControlRegRemoved)
        if(HB_Counter_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            HB_Counter_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!HB_Counter_ControlRegRemoved) */
    
}


/* [] END OF FILE */
