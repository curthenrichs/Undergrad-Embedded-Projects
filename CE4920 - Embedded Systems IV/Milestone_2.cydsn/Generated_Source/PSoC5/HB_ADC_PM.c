/*******************************************************************************
* File Name: HB_ADC_PM.c
* Version 3.0
*
* Description:
*  This file provides Sleep/WakeUp APIs functionality.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "HB_ADC.h"


/***************************************
* Local data allocation
***************************************/

static HB_ADC_BACKUP_STRUCT  HB_ADC_backup =
{
    HB_ADC_DISABLED
};


/*******************************************************************************
* Function Name: HB_ADC_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void HB_ADC_SaveConfig(void)
{
    /* All configuration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: HB_ADC_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void HB_ADC_RestoreConfig(void)
{
    /* All congiguration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: HB_ADC_Sleep
********************************************************************************
*
* Summary:
*  This is the preferred routine to prepare the component for sleep.
*  The HB_ADC_Sleep() routine saves the current component state,
*  then it calls the ADC_Stop() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  HB_ADC_backup - The structure field 'enableState' is modified
*  depending on the enable state of the block before entering to sleep mode.
*
*******************************************************************************/
void HB_ADC_Sleep(void)
{
    if((HB_ADC_PWRMGR_SAR_REG  & HB_ADC_ACT_PWR_SAR_EN) != 0u)
    {
        if((HB_ADC_SAR_CSR0_REG & HB_ADC_SAR_SOF_START_CONV) != 0u)
        {
            HB_ADC_backup.enableState = HB_ADC_ENABLED | HB_ADC_STARTED;
        }
        else
        {
            HB_ADC_backup.enableState = HB_ADC_ENABLED;
        }
        HB_ADC_Stop();
    }
    else
    {
        HB_ADC_backup.enableState = HB_ADC_DISABLED;
    }
}


/*******************************************************************************
* Function Name: HB_ADC_Wakeup
********************************************************************************
*
* Summary:
*  This is the preferred routine to restore the component to the state when
*  HB_ADC_Sleep() was called. If the component was enabled before the
*  HB_ADC_Sleep() function was called, the
*  HB_ADC_Wakeup() function also re-enables the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  HB_ADC_backup - The structure field 'enableState' is used to
*  restore the enable state of block after wakeup from sleep mode.
*
*******************************************************************************/
void HB_ADC_Wakeup(void)
{
    if(HB_ADC_backup.enableState != HB_ADC_DISABLED)
    {
        HB_ADC_Enable();
        #if(HB_ADC_DEFAULT_CONV_MODE != HB_ADC__HARDWARE_TRIGGER)
            if((HB_ADC_backup.enableState & HB_ADC_STARTED) != 0u)
            {
                HB_ADC_StartConvert();
            }
        #endif /* End HB_ADC_DEFAULT_CONV_MODE != HB_ADC__HARDWARE_TRIGGER */
    }
}


/* [] END OF FILE */
