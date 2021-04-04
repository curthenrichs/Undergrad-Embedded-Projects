/*******************************************************************************
* File Name: CM_Test_Timer_PM.c
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

#include "CM_Test_Timer.h"

static CM_Test_Timer_backupStruct CM_Test_Timer_backup;


/*******************************************************************************
* Function Name: CM_Test_Timer_SaveConfig
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
*  CM_Test_Timer_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void CM_Test_Timer_SaveConfig(void) 
{
    #if (!CM_Test_Timer_UsingFixedFunction)
        CM_Test_Timer_backup.TimerUdb = CM_Test_Timer_ReadCounter();
        CM_Test_Timer_backup.InterruptMaskValue = CM_Test_Timer_STATUS_MASK;
        #if (CM_Test_Timer_UsingHWCaptureCounter)
            CM_Test_Timer_backup.TimerCaptureCounter = CM_Test_Timer_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!CM_Test_Timer_UDB_CONTROL_REG_REMOVED)
            CM_Test_Timer_backup.TimerControlRegister = CM_Test_Timer_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: CM_Test_Timer_RestoreConfig
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
*  CM_Test_Timer_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void CM_Test_Timer_RestoreConfig(void) 
{   
    #if (!CM_Test_Timer_UsingFixedFunction)

        CM_Test_Timer_WriteCounter(CM_Test_Timer_backup.TimerUdb);
        CM_Test_Timer_STATUS_MASK =CM_Test_Timer_backup.InterruptMaskValue;
        #if (CM_Test_Timer_UsingHWCaptureCounter)
            CM_Test_Timer_SetCaptureCount(CM_Test_Timer_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!CM_Test_Timer_UDB_CONTROL_REG_REMOVED)
            CM_Test_Timer_WriteControlRegister(CM_Test_Timer_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: CM_Test_Timer_Sleep
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
*  CM_Test_Timer_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void CM_Test_Timer_Sleep(void) 
{
    #if(!CM_Test_Timer_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(CM_Test_Timer_CTRL_ENABLE == (CM_Test_Timer_CONTROL & CM_Test_Timer_CTRL_ENABLE))
        {
            /* Timer is enabled */
            CM_Test_Timer_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            CM_Test_Timer_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    CM_Test_Timer_Stop();
    CM_Test_Timer_SaveConfig();
}


/*******************************************************************************
* Function Name: CM_Test_Timer_Wakeup
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
*  CM_Test_Timer_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void CM_Test_Timer_Wakeup(void) 
{
    CM_Test_Timer_RestoreConfig();
    #if(!CM_Test_Timer_UDB_CONTROL_REG_REMOVED)
        if(CM_Test_Timer_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                CM_Test_Timer_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
