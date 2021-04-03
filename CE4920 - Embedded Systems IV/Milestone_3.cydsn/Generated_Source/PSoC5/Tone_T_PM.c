/*******************************************************************************
* File Name: Tone_T_PM.c
* Version 2.70
*
*  Description:
*     This file provides the power management source code to API for the
*     Timer.
*
*   Note:
*     None
*
*******************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "Tone_T.h"

static Tone_T_backupStruct Tone_T_backup;


/*******************************************************************************
* Function Name: Tone_T_SaveConfig
********************************************************************************
*
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
*  Tone_T_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Tone_T_SaveConfig(void) 
{
    #if (!Tone_T_UsingFixedFunction)
        Tone_T_backup.TimerUdb = Tone_T_ReadCounter();
        Tone_T_backup.InterruptMaskValue = Tone_T_STATUS_MASK;
        #if (Tone_T_UsingHWCaptureCounter)
            Tone_T_backup.TimerCaptureCounter = Tone_T_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!Tone_T_UDB_CONTROL_REG_REMOVED)
            Tone_T_backup.TimerControlRegister = Tone_T_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: Tone_T_RestoreConfig
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
*  Tone_T_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Tone_T_RestoreConfig(void) 
{   
    #if (!Tone_T_UsingFixedFunction)

        Tone_T_WriteCounter(Tone_T_backup.TimerUdb);
        Tone_T_STATUS_MASK =Tone_T_backup.InterruptMaskValue;
        #if (Tone_T_UsingHWCaptureCounter)
            Tone_T_SetCaptureCount(Tone_T_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!Tone_T_UDB_CONTROL_REG_REMOVED)
            Tone_T_WriteControlRegister(Tone_T_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: Tone_T_Sleep
********************************************************************************
*
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
*  Tone_T_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void Tone_T_Sleep(void) 
{
    #if(!Tone_T_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(Tone_T_CTRL_ENABLE == (Tone_T_CONTROL & Tone_T_CTRL_ENABLE))
        {
            /* Timer is enabled */
            Tone_T_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            Tone_T_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    Tone_T_Stop();
    Tone_T_SaveConfig();
}


/*******************************************************************************
* Function Name: Tone_T_Wakeup
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
*  Tone_T_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Tone_T_Wakeup(void) 
{
    Tone_T_RestoreConfig();
    #if(!Tone_T_UDB_CONTROL_REG_REMOVED)
        if(Tone_T_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                Tone_T_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
