/*******************************************************************************
* File Name: Button_PM.c
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

#include "Button.h"

Button_BACKUP_STRUCT Button_backup =
{   
    0x00u, /* enableState; */
    
};


/*******************************************************************************
* Function Name: Button_SaveConfig
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
*  Function Button_SaveConfig disconnects all sensors from the
*  Analog MUX Bus and puts them into inactive state. Call this function
*  during the active scan can cause unpredictable component behavior.
*
* Note:
*  This function should be called after completion of all scans.
*
* Global Variables:
*  Button_backup - used to save component state before enter sleep 
*  mode and none-retention registers.
*
* Reentrant:
*  Yes.
*
*******************************************************************************/
void Button_SaveConfig(void) 
{    

    /* Set CONTROL_REG */
    Button_backup.ctrlReg = Button_CONTROL_REG;

    /* Clear all sensors */
    Button_ClearSensors();
    
    /* The pins disable is customer concern: Cmod and Rb */
}


/*******************************************************************************
* Function Name: Button_Sleep
********************************************************************************
*
* Summary:
*  Disables Active mode power template bits for number of component used within 
*  CapSense. Calls Button_SaveConfig() function to save customer 
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
*  Function Button_Sleep disconnects all sensors from the
*  Analog MUX Bus and puts them into inactive state. Call this function
*  during the active scan can cause unpredictable component behavior.
*
* Note:
*  This function should be called after completion of all scans.
*
* Global Variables:
*  Button_backup - used to save component state before enter sleep 
*  mode and none-retention registers.
*
* Reentrant:
*  No
*
*******************************************************************************/
void Button_Sleep(void) 
{
    /* Check and save enable state */
    if(Button_IS_CAPSENSE_ENABLE(Button_CONTROL_REG))
    {
        Button_backup.enableState = 1u;
        Button_Stop();
    }
    else
    {
        Button_backup.enableState = 0u;
    }
    
    Button_SaveConfig();
}


/*******************************************************************************
* Function Name: Button_RestoreConfig
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
*  Must be called only after Button_SaveConfig() routine. Otherwise 
*  the component configuration will be overwritten with its initial setting.
*  This finction modifies the CONTROL_REG register. 
*
* Note:
*  This function should be called after completion of all scans.
*
* Global Variables:
*  Button_backup - used to save component state before enter sleep 
*  mode and none-retention registers.
*
*******************************************************************************/
void Button_RestoreConfig(void) 
{       
    /* Set PRS */
    #if (Button_PRS_OPTIONS == Button_PRS_8BITS)        
        /* Write FIFO with seed */
        Button_SEED_COPY_REG = Button_MEASURE_FULL_RANGE_LOW;               /* F0 register */
    
    #elif (Button_PRS_OPTIONS == Button_PRS_16BITS)
        /* Write FIFO with seed */
        CY_SET_REG16(Button_SEED_COPY_PTR, Button_MEASURE_FULL_RANGE);      /* F0 register */
                
    #elif (Button_PRS_OPTIONS == Button_PRS_16BITS_4X)
        
        /* Write FIFO with seed */
        Button_SEED_COPY_A__F1_REG = Button_MEASURE_FULL_RANGE_LOW;         /* F0 register */
        Button_SEED_COPY_A__F0_REG =Button_MEASURE_FULL_RANGE_LOW;          /* F1 register */
        
    #else
        /* Do nothing = config without PRS */
    #endif  /* (Button_PRS_OPTIONS == Button_PRS_8BITS) */
    
    /* Set the Measure */
    #if (Button_IMPLEMENTATION_CH0 == Button_MEASURE_IMPLEMENTATION_FF)
        /* Window PWM  - FF Timer register are retention */
        /* Raw Counter - FF Timer register are retention */
    #else
        
        /* Window PWM */
        Button_PWM_CH0_PERIOD_LO_REG    = Button_MEASURE_FULL_RANGE_LOW;        /* F0 register */
        
        /* Raw Counter */
        Button_RAW_CH0_PERIOD_HI_REG    = Button_MEASURE_FULL_RANGE_LOW;        /* F1 register */
        Button_RAW_CH0_PERIOD_LO_REG    = Button_MEASURE_FULL_RANGE_LOW;        /* F0 register */
    
    #endif  /* (Button_IMPLEMENTATION_CH0 == Button_MEASURE_IMPLEMENTATION_FF) */ 
    
    #if (Button_DESIGN_TYPE == Button_TWO_CHANNELS_DESIGN)
        #if (Button_IMPLEMENTATION_CH1 == Button_MEASURE_IMPLEMENTATION_FF)
            /* Window PWM  - FF Timer register are retention */
            /* Raw Counter - FF Timer register are retention */
        #else
            
            /* Window PWM */
            Button_PWM_CH1_PERIOD_LO_REG    = Button_MEASURE_FULL_RANGE_LOW;       /* F0 register */
            
            /* Raw Counter */
            Button_RAW_CH1_PERIOD_HI_REG    = Button_MEASURE_FULL_RANGE_LOW;       /* F1 register */
            Button_RAW_CH1_PERIOD_LO_REG    = Button_MEASURE_FULL_RANGE_LOW;       /* F0 register */
            
        #endif  /* (Button_IMPLEMENTATION_CH1 == Button_MEASURE_IMPLEMENTATION_FF) */
    
    #endif  /* (Button_DESIGN_TYPE == TWO_CHANNELS_DESIGN)*/

    /* Set CONTROL_REG */
    Button_CONTROL_REG = Button_backup.ctrlReg;

    /* Enable window generation */
    #if (Button_DESIGN_TYPE == Button_ONE_CHANNEL_DESIGN)
        Button_CONTROL_REG |= Button_CTRL_WINDOW_EN__CH0;
    #elif (Button_DESIGN_TYPE == Button_TWO_CHANNELS_DESIGN) 
        Button_CONTROL_REG |= (Button_CTRL_WINDOW_EN__CH0 | Button_CTRL_WINDOW_EN__CH1); 
    #endif  /* Button_DESIGN_TYPE */
 
    /* The pins enable are customer concern: Cmod and Rb */
 }
 

/*******************************************************************************
* Function Name: Button_Wakeup
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
*  Must be called only after Button_SaveConfig() routine. Otherwise 
*  the component configuration will be overwritten with its initial setting.
*  This finction modifies the CONTROL_REG register. 
*
* Note:
*  This function should be called after completion of all scans.
*
* Global Variables:
*  Button_backup - used to save component state before enter sleep 
*  mode and none-retention registers.
*
*******************************************************************************/
void Button_Wakeup(void) 
{
    Button_RestoreConfig();
    
    /* Restore CapSense Enable state */
    if (Button_backup.enableState != 0u)
    {
        Button_Enable();
    }
}


/* [] END OF FILE */
