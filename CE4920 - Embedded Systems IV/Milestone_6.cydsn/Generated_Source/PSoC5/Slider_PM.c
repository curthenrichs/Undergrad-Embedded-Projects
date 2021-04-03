/*******************************************************************************
* File Name: Slider_PM.c
* Version 3.50
*
* Description:
*  This file provides Sleep APIs for CapSense CSD Component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "Slider.h"

Slider_BACKUP_STRUCT Slider_backup =
{   
    0x00u, /* enableState; */
    
};


/*******************************************************************************
* Function Name: Slider_SaveConfig
********************************************************************************
*
* Summary:
*  Saves customer configuration of CapSense none-retention registers. Resets 
*  all sensors to an inactive state.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  Function Slider_SaveConfig disconnects all sensors from the
*  Analog MUX Bus and puts them into inactive state. Call this function
*  during the active scan can cause unpredictable component behavior.
*
* Note:
*  This function should be called after completion of all scans.
*
* Global Variables:
*  Slider_backup - used to save component state before enter sleep 
*  mode and none-retention registers.
*
* Reentrant:
*  Yes.
*
*******************************************************************************/
void Slider_SaveConfig(void) 
{    

    /* Set CONTROL_REG */
    Slider_backup.ctrlReg = Slider_CONTROL_REG;

    /* Clear all sensors */
    Slider_ClearSensors();
    
    /* The pins disable is customer concern: Cmod and Rb */
}


/*******************************************************************************
* Function Name: Slider_Sleep
********************************************************************************
*
* Summary:
*  Disables Active mode power template bits for number of component used within 
*  CapSense. Calls Slider_SaveConfig() function to save customer 
*  configuration of CapSense none-retention registers and resets all sensors 
*  to an inactive state.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  Function Slider_Sleep disconnects all sensors from the
*  Analog MUX Bus and puts them into inactive state. Call this function
*  during the active scan can cause unpredictable component behavior.
*
* Note:
*  This function should be called after completion of all scans.
*
* Global Variables:
*  Slider_backup - used to save component state before enter sleep 
*  mode and none-retention registers.
*
* Reentrant:
*  No
*
*******************************************************************************/
void Slider_Sleep(void) 
{
    /* Check and save enable state */
    if(Slider_IS_CAPSENSE_ENABLE(Slider_CONTROL_REG))
    {
        Slider_backup.enableState = 1u;
        Slider_Stop();
    }
    else
    {
        Slider_backup.enableState = 0u;
    }
    
    Slider_SaveConfig();
}


/*******************************************************************************
* Function Name: Slider_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores CapSense configuration and non-retention register values.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  Must be called only after Slider_SaveConfig() routine. Otherwise 
*  the component configuration will be overwritten with its initial setting.
*  This finction modifies the CONTROL_REG register. 
*
* Note:
*  This function should be called after completion of all scans.
*
* Global Variables:
*  Slider_backup - used to save component state before enter sleep 
*  mode and none-retention registers.
*
*******************************************************************************/
void Slider_RestoreConfig(void) 
{       
    /* Set PRS */
    #if (Slider_PRS_OPTIONS == Slider_PRS_8BITS)        
        /* Write FIFO with seed */
        Slider_SEED_COPY_REG = Slider_MEASURE_FULL_RANGE_LOW;               /* F0 register */
    
    #elif (Slider_PRS_OPTIONS == Slider_PRS_16BITS)
        /* Write FIFO with seed */
        CY_SET_REG16(Slider_SEED_COPY_PTR, Slider_MEASURE_FULL_RANGE);      /* F0 register */
                
    #elif (Slider_PRS_OPTIONS == Slider_PRS_16BITS_4X)
        
        /* Write FIFO with seed */
        Slider_SEED_COPY_A__F1_REG = Slider_MEASURE_FULL_RANGE_LOW;         /* F0 register */
        Slider_SEED_COPY_A__F0_REG =Slider_MEASURE_FULL_RANGE_LOW;          /* F1 register */
        
    #else
        /* Do nothing = config without PRS */
    #endif  /* (Slider_PRS_OPTIONS == Slider_PRS_8BITS) */
    
    /* Set the Measure */
    #if (Slider_IMPLEMENTATION_CH0 == Slider_MEASURE_IMPLEMENTATION_FF)
        /* Window PWM  - FF Timer register are retention */
        /* Raw Counter - FF Timer register are retention */
    #else
        
        /* Window PWM */
        Slider_PWM_CH0_PERIOD_LO_REG    = Slider_MEASURE_FULL_RANGE_LOW;        /* F0 register */
        
        /* Raw Counter */
        Slider_RAW_CH0_PERIOD_HI_REG    = Slider_MEASURE_FULL_RANGE_LOW;        /* F1 register */
        Slider_RAW_CH0_PERIOD_LO_REG    = Slider_MEASURE_FULL_RANGE_LOW;        /* F0 register */
    
    #endif  /* (Slider_IMPLEMENTATION_CH0 == Slider_MEASURE_IMPLEMENTATION_FF) */ 
    
    #if (Slider_DESIGN_TYPE == Slider_TWO_CHANNELS_DESIGN)
        #if (Slider_IMPLEMENTATION_CH1 == Slider_MEASURE_IMPLEMENTATION_FF)
            /* Window PWM  - FF Timer register are retention */
            /* Raw Counter - FF Timer register are retention */
        #else
            
            /* Window PWM */
            Slider_PWM_CH1_PERIOD_LO_REG    = Slider_MEASURE_FULL_RANGE_LOW;       /* F0 register */
            
            /* Raw Counter */
            Slider_RAW_CH1_PERIOD_HI_REG    = Slider_MEASURE_FULL_RANGE_LOW;       /* F1 register */
            Slider_RAW_CH1_PERIOD_LO_REG    = Slider_MEASURE_FULL_RANGE_LOW;       /* F0 register */
            
        #endif  /* (Slider_IMPLEMENTATION_CH1 == Slider_MEASURE_IMPLEMENTATION_FF) */
    
    #endif  /* (Slider_DESIGN_TYPE == TWO_CHANNELS_DESIGN)*/

    /* Set CONTROL_REG */
    Slider_CONTROL_REG = Slider_backup.ctrlReg;

    /* Enable window generation */
    #if (Slider_DESIGN_TYPE == Slider_ONE_CHANNEL_DESIGN)
        Slider_CONTROL_REG |= Slider_CTRL_WINDOW_EN__CH0;
    #elif (Slider_DESIGN_TYPE == Slider_TWO_CHANNELS_DESIGN) 
        Slider_CONTROL_REG |= (Slider_CTRL_WINDOW_EN__CH0 | Slider_CTRL_WINDOW_EN__CH1); 
    #endif  /* Slider_DESIGN_TYPE */
 
    /* The pins enable are customer concern: Cmod and Rb */
 }
 

/*******************************************************************************
* Function Name: Slider_Wakeup
********************************************************************************
*
* Summary:
*  Restores CapSense configuration and non-retention register values. 
*  Restores enabled state of component by setting Active mode power template 
*  bits for number of component used within CapSense.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  Must be called only after Slider_SaveConfig() routine. Otherwise 
*  the component configuration will be overwritten with its initial setting.
*  This finction modifies the CONTROL_REG register. 
*
* Note:
*  This function should be called after completion of all scans.
*
* Global Variables:
*  Slider_backup - used to save component state before enter sleep 
*  mode and none-retention registers.
*
*******************************************************************************/
void Slider_Wakeup(void) 
{
    Slider_RestoreConfig();
    
    /* Restore CapSense Enable state */
    if (Slider_backup.enableState != 0u)
    {
        Slider_Enable();
    }
}


/* [] END OF FILE */
