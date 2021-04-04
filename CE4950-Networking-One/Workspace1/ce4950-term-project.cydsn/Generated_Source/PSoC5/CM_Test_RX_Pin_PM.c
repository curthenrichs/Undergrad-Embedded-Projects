/*******************************************************************************
* File Name: CM_Test_RX_Pin_PM.c
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

#include "CM_Test_RX_Pin.h"

/* Check for removal by optimization */
#if !defined(CM_Test_RX_Pin_Sync_ctrl_reg__REMOVED)

static CM_Test_RX_Pin_BACKUP_STRUCT  CM_Test_RX_Pin_backup = {0u};

    
/*******************************************************************************
* Function Name: CM_Test_RX_Pin_SaveConfig
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
void CM_Test_RX_Pin_SaveConfig(void) 
{
    CM_Test_RX_Pin_backup.controlState = CM_Test_RX_Pin_Control;
}


/*******************************************************************************
* Function Name: CM_Test_RX_Pin_RestoreConfig
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
void CM_Test_RX_Pin_RestoreConfig(void) 
{
     CM_Test_RX_Pin_Control = CM_Test_RX_Pin_backup.controlState;
}


/*******************************************************************************
* Function Name: CM_Test_RX_Pin_Sleep
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
void CM_Test_RX_Pin_Sleep(void) 
{
    CM_Test_RX_Pin_SaveConfig();
}


/*******************************************************************************
* Function Name: CM_Test_RX_Pin_Wakeup
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
void CM_Test_RX_Pin_Wakeup(void)  
{
    CM_Test_RX_Pin_RestoreConfig();
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
