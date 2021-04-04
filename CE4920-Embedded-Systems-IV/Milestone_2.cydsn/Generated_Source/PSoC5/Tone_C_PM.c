/*******************************************************************************
* File Name: Tone_C_PM.c  
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

#include "Tone_C.h"

static Tone_C_backupStruct Tone_C_backup;


/*******************************************************************************
* Function Name: Tone_C_SaveConfig
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
*  Tone_C_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void Tone_C_SaveConfig(void) 
{
    #if (!Tone_C_UsingFixedFunction)

        Tone_C_backup.CounterUdb = Tone_C_ReadCounter();

        #if(!Tone_C_ControlRegRemoved)
            Tone_C_backup.CounterControlRegister = Tone_C_ReadControlRegister();
        #endif /* (!Tone_C_ControlRegRemoved) */

    #endif /* (!Tone_C_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Tone_C_RestoreConfig
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
*  Tone_C_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Tone_C_RestoreConfig(void) 
{      
    #if (!Tone_C_UsingFixedFunction)

       Tone_C_WriteCounter(Tone_C_backup.CounterUdb);

        #if(!Tone_C_ControlRegRemoved)
            Tone_C_WriteControlRegister(Tone_C_backup.CounterControlRegister);
        #endif /* (!Tone_C_ControlRegRemoved) */

    #endif /* (!Tone_C_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Tone_C_Sleep
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
*  Tone_C_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Tone_C_Sleep(void) 
{
    #if(!Tone_C_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Tone_C_CTRL_ENABLE == (Tone_C_CONTROL & Tone_C_CTRL_ENABLE))
        {
            /* Counter is enabled */
            Tone_C_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            Tone_C_backup.CounterEnableState = 0u;
        }
    #else
        Tone_C_backup.CounterEnableState = 1u;
        if(Tone_C_backup.CounterEnableState != 0u)
        {
            Tone_C_backup.CounterEnableState = 0u;
        }
    #endif /* (!Tone_C_ControlRegRemoved) */
    
    Tone_C_Stop();
    Tone_C_SaveConfig();
}


/*******************************************************************************
* Function Name: Tone_C_Wakeup
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
*  Tone_C_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Tone_C_Wakeup(void) 
{
    Tone_C_RestoreConfig();
    #if(!Tone_C_ControlRegRemoved)
        if(Tone_C_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            Tone_C_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!Tone_C_ControlRegRemoved) */
    
}


/* [] END OF FILE */
