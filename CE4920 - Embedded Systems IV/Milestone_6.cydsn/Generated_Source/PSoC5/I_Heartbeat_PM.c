/*******************************************************************************
* File Name: I_Heartbeat_PM.c  
* Version 2.10
*
* Description:
*  This file provides the power manager source code to the API for 
*  the WaveDAC8 component.
*
********************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "I_Heartbeat.h"

static I_Heartbeat_BACKUP_STRUCT  I_Heartbeat_backup;


/*******************************************************************************
* Function Name: I_Heartbeat_Sleep
********************************************************************************
*
* Summary:
*  Stops the component and saves its configuration. Should be called 
*  just prior to entering sleep.
*  
* Parameters:  
*  None
*
* Return: 
*  None
*
* Global variables:
*  I_Heartbeat_backup:  The structure field 'enableState' is modified 
*  depending on the enable state of the block before entering to sleep mode.
*
* Reentrant:
*  No
*
*******************************************************************************/
void I_Heartbeat_Sleep(void) 
{
	/* Save DAC8's enable state */

	I_Heartbeat_backup.enableState = (I_Heartbeat_VDAC8_ACT_PWR_EN == 
		(I_Heartbeat_VDAC8_PWRMGR_REG & I_Heartbeat_VDAC8_ACT_PWR_EN)) ? 1u : 0u ;
	
	I_Heartbeat_Stop();
	I_Heartbeat_SaveConfig();
}


/*******************************************************************************
* Function Name: I_Heartbeat_Wakeup
********************************************************************************
*
* Summary:
*  Restores the component configuration. Should be called
*  just after awaking from sleep.
*  
* Parameters:  
*  None
*
* Return: 
*  void
*
* Global variables:
*  I_Heartbeat_backup:  The structure field 'enableState' is used to 
*  restore the enable state of block after wakeup from sleep mode.
*
* Reentrant:
*  No
*
*******************************************************************************/
void I_Heartbeat_Wakeup(void) 
{
	I_Heartbeat_RestoreConfig();

	if(I_Heartbeat_backup.enableState == 1u)
	{
		I_Heartbeat_Enable();
	}
}


/* [] END OF FILE */
