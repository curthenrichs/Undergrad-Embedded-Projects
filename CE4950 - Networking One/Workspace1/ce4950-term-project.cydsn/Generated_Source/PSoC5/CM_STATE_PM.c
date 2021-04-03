/*******************************************************************************
* File Name: CM_STATE_PM.c
* Version 1.80
*
* Description:
*  This file contains the setup, control, and status commands to support 
*  the component operation in the low power mode. 
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "CM_STATE.h"

/* Check for removal by optimization */
#if !defined(CM_STATE_Sync_ctrl_reg__REMOVED)

static CM_STATE_BACKUP_STRUCT  CM_STATE_backup = {0u};

    
/*******************************************************************************
* Function Name: CM_STATE_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void CM_STATE_SaveConfig(void) 
{
    CM_STATE_backup.controlState = CM_STATE_Control;
}


/*******************************************************************************
* Function Name: CM_STATE_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*
*******************************************************************************/
void CM_STATE_RestoreConfig(void) 
{
     CM_STATE_Control = CM_STATE_backup.controlState;
}


/*******************************************************************************
* Function Name: CM_STATE_Sleep
********************************************************************************
*
* Summary:
*  Prepares the component for entering the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void CM_STATE_Sleep(void) 
{
    CM_STATE_SaveConfig();
}


/*******************************************************************************
* Function Name: CM_STATE_Wakeup
********************************************************************************
*
* Summary:
*  Restores the component after waking up from the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void CM_STATE_Wakeup(void)  
{
    CM_STATE_RestoreConfig();
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
