/*******************************************************************************
* File Name: Tx_Timer_PM.c
* Version 2.80
*
*  Description:
*     This file provides the power management source code to API for the
*     Timer.
*
*   Note:
*     None
*
*******************************************************************************
* Copyright 2008-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "Tx_Timer.h"

static Tx_Timer_backupStruct Tx_Timer_backup;


/*******************************************************************************
* Function Name: Tx_Timer_SaveConfig
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
*  Tx_Timer_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Tx_Timer_SaveConfig(void) 
{
    #if (!Tx_Timer_UsingFixedFunction)
        Tx_Timer_backup.TimerUdb = Tx_Timer_ReadCounter();
        Tx_Timer_backup.InterruptMaskValue = Tx_Timer_STATUS_MASK;
        #if (Tx_Timer_UsingHWCaptureCounter)
            Tx_Timer_backup.TimerCaptureCounter = Tx_Timer_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!Tx_Timer_UDB_CONTROL_REG_REMOVED)
            Tx_Timer_backup.TimerControlRegister = Tx_Timer_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: Tx_Timer_RestoreConfig
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
*  Tx_Timer_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Tx_Timer_RestoreConfig(void) 
{   
    #if (!Tx_Timer_UsingFixedFunction)

        Tx_Timer_WriteCounter(Tx_Timer_backup.TimerUdb);
        Tx_Timer_STATUS_MASK =Tx_Timer_backup.InterruptMaskValue;
        #if (Tx_Timer_UsingHWCaptureCounter)
            Tx_Timer_SetCaptureCount(Tx_Timer_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!Tx_Timer_UDB_CONTROL_REG_REMOVED)
            Tx_Timer_WriteControlRegister(Tx_Timer_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: Tx_Timer_Sleep
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
*  Tx_Timer_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void Tx_Timer_Sleep(void) 
{
    #if(!Tx_Timer_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(Tx_Timer_CTRL_ENABLE == (Tx_Timer_CONTROL & Tx_Timer_CTRL_ENABLE))
        {
            /* Timer is enabled */
            Tx_Timer_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            Tx_Timer_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    Tx_Timer_Stop();
    Tx_Timer_SaveConfig();
}


/*******************************************************************************
* Function Name: Tx_Timer_Wakeup
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
*  Tx_Timer_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Tx_Timer_Wakeup(void) 
{
    Tx_Timer_RestoreConfig();
    #if(!Tx_Timer_UDB_CONTROL_REG_REMOVED)
        if(Tx_Timer_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                Tx_Timer_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
