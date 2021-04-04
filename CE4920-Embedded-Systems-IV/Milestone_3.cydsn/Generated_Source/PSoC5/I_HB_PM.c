/*******************************************************************************
* File Name: I_HB_PM.c  
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

#include "I_HB.h"

static I_HB_BACKUP_STRUCT  I_HB_backup;


/*******************************************************************************
* Function Name: I_HB_Sleep
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
*  I_HB_backup:  The structure field 'enableState' is modified 
*  depending on the enable state of the block before entering to sleep mode.
*
* Reentrant:
*  No
*
*******************************************************************************/
void I_HB_Sleep(void) 
{
	/* Save DAC8's enable state */

	I_HB_backup.enableState = (I_HB_VDAC8_ACT_PWR_EN == 
		(I_HB_VDAC8_PWRMGR_REG & I_HB_VDAC8_ACT_PWR_EN)) ? 1u : 0u ;
	
	I_HB_Stop();
	I_HB_SaveConfig();
}


/*******************************************************************************
* Function Name: I_HB_Wakeup
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
*  I_HB_backup:  The structure field 'enableState' is used to 
*  restore the enable state of block after wakeup from sleep mode.
*
* Reentrant:
*  No
*
*******************************************************************************/
void I_HB_Wakeup(void) 
{
	I_HB_RestoreConfig();

	if(I_HB_backup.enableState == 1u)
	{
		I_HB_Enable();
	}
}


/* [] END OF FILE */
