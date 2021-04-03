/*******************************************************************************
* File Name: Tone_F_PM.c
* Version 3.30
*
* Description:
*  This file provides the power management source code to API for the
*  PWM.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "Tone_F.h"

static Tone_F_backupStruct Tone_F_backup;


/*******************************************************************************
* Function Name: Tone_F_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  Tone_F_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Tone_F_SaveConfig(void) 
{

    #if(!Tone_F_UsingFixedFunction)
        #if(!Tone_F_PWMModeIsCenterAligned)
            Tone_F_backup.PWMPeriod = Tone_F_ReadPeriod();
        #endif /* (!Tone_F_PWMModeIsCenterAligned) */
        Tone_F_backup.PWMUdb = Tone_F_ReadCounter();
        #if (Tone_F_UseStatus)
            Tone_F_backup.InterruptMaskValue = Tone_F_STATUS_MASK;
        #endif /* (Tone_F_UseStatus) */

        #if(Tone_F_DeadBandMode == Tone_F__B_PWM__DBM_256_CLOCKS || \
            Tone_F_DeadBandMode == Tone_F__B_PWM__DBM_2_4_CLOCKS)
            Tone_F_backup.PWMdeadBandValue = Tone_F_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(Tone_F_KillModeMinTime)
             Tone_F_backup.PWMKillCounterPeriod = Tone_F_ReadKillTime();
        #endif /* (Tone_F_KillModeMinTime) */

        #if(Tone_F_UseControl)
            Tone_F_backup.PWMControlRegister = Tone_F_ReadControlRegister();
        #endif /* (Tone_F_UseControl) */
    #endif  /* (!Tone_F_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Tone_F_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  Tone_F_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Tone_F_RestoreConfig(void) 
{
        #if(!Tone_F_UsingFixedFunction)
            #if(!Tone_F_PWMModeIsCenterAligned)
                Tone_F_WritePeriod(Tone_F_backup.PWMPeriod);
            #endif /* (!Tone_F_PWMModeIsCenterAligned) */

            Tone_F_WriteCounter(Tone_F_backup.PWMUdb);

            #if (Tone_F_UseStatus)
                Tone_F_STATUS_MASK = Tone_F_backup.InterruptMaskValue;
            #endif /* (Tone_F_UseStatus) */

            #if(Tone_F_DeadBandMode == Tone_F__B_PWM__DBM_256_CLOCKS || \
                Tone_F_DeadBandMode == Tone_F__B_PWM__DBM_2_4_CLOCKS)
                Tone_F_WriteDeadTime(Tone_F_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(Tone_F_KillModeMinTime)
                Tone_F_WriteKillTime(Tone_F_backup.PWMKillCounterPeriod);
            #endif /* (Tone_F_KillModeMinTime) */

            #if(Tone_F_UseControl)
                Tone_F_WriteControlRegister(Tone_F_backup.PWMControlRegister);
            #endif /* (Tone_F_UseControl) */
        #endif  /* (!Tone_F_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: Tone_F_Sleep
********************************************************************************
*
* Summary:
*  Disables block's operation and saves the user configuration. Should be called
*  just prior to entering sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  Tone_F_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Tone_F_Sleep(void) 
{
    #if(Tone_F_UseControl)
        if(Tone_F_CTRL_ENABLE == (Tone_F_CONTROL & Tone_F_CTRL_ENABLE))
        {
            /*Component is enabled */
            Tone_F_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            Tone_F_backup.PWMEnableState = 0u;
        }
    #endif /* (Tone_F_UseControl) */

    /* Stop component */
    Tone_F_Stop();

    /* Save registers configuration */
    Tone_F_SaveConfig();
}


/*******************************************************************************
* Function Name: Tone_F_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration. Should be called just after
*  awaking from sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  Tone_F_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Tone_F_Wakeup(void) 
{
     /* Restore registers values */
    Tone_F_RestoreConfig();

    if(Tone_F_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        Tone_F_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
