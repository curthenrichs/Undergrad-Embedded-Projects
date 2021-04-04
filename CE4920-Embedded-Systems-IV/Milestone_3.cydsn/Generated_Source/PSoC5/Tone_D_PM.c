/*******************************************************************************
* File Name: Tone_D_PM.c  
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

#include "Tone_D.h"

static Tone_D_backupStruct Tone_D_backup;


/*******************************************************************************
* Function Name: Tone_D_SaveConfig
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
*  Tone_D_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void Tone_D_SaveConfig(void) 
{
    #if (!Tone_D_UsingFixedFunction)

        Tone_D_backup.CounterUdb = Tone_D_ReadCounter();

        #if(!Tone_D_ControlRegRemoved)
            Tone_D_backup.CounterControlRegister = Tone_D_ReadControlRegister();
        #endif /* (!Tone_D_ControlRegRemoved) */

    #endif /* (!Tone_D_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Tone_D_RestoreConfig
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
*  Tone_D_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Tone_D_RestoreConfig(void) 
{      
    #if (!Tone_D_UsingFixedFunction)

       Tone_D_WriteCounter(Tone_D_backup.CounterUdb);

        #if(!Tone_D_ControlRegRemoved)
            Tone_D_WriteControlRegister(Tone_D_backup.CounterControlRegister);
        #endif /* (!Tone_D_ControlRegRemoved) */

    #endif /* (!Tone_D_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Tone_D_Sleep
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
*  Tone_D_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Tone_D_Sleep(void) 
{
    #if(!Tone_D_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Tone_D_CTRL_ENABLE == (Tone_D_CONTROL & Tone_D_CTRL_ENABLE))
        {
            /* Counter is enabled */
            Tone_D_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            Tone_D_backup.CounterEnableState = 0u;
        }
    #else
        Tone_D_backup.CounterEnableState = 1u;
        if(Tone_D_backup.CounterEnableState != 0u)
        {
            Tone_D_backup.CounterEnableState = 0u;
        }
    #endif /* (!Tone_D_ControlRegRemoved) */
    
    Tone_D_Stop();
    Tone_D_SaveConfig();
}


/*******************************************************************************
* Function Name: Tone_D_Wakeup
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
*  Tone_D_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Tone_D_Wakeup(void) 
{
    Tone_D_RestoreConfig();
    #if(!Tone_D_ControlRegRemoved)
        if(Tone_D_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            Tone_D_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!Tone_D_ControlRegRemoved) */
    
}


/* [] END OF FILE */
