/*******************************************************************************
* File Name: Button_IdacCH0.c
* Version 2.0
*
* Description:
*  This file provides the source code to the API for the 8-bit Current 
*  DAC (IDAC8) User Module.
*
* Note:
*  None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "Button_IdacCH0.h"   

#if (CY_PSOC5A)
    #include <CyLib.h>
#endif /* CY_PSOC5A */


uint8 Button_IdacCH0_initVar = 0u;


#if (CY_PSOC5A)
    static Button_IdacCH0_LOWPOWER_BACKUP_STRUCT  Button_IdacCH0_lowPowerBackup;
#endif /* CY_PSOC5A */

/* Variable to decide whether or not to restore control register in Enable()
   API. This valid only for PSoC5A */
#if (CY_PSOC5A)
    static uint8 Button_IdacCH0_restoreReg = 0u;
#endif /* CY_PSOC5A */


/*******************************************************************************
* Function Name: Button_IdacCH0_Init
********************************************************************************
* Summary:
*  Initialize to the schematic state.
* 
* Parameters:
*  void:
*
* Return:
*  (void)
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
void Button_IdacCH0_Init(void) 
{
    Button_IdacCH0_CR0 = (Button_IdacCH0_MODE_I | Button_IdacCH0_DEFAULT_RANGE );

    /* Set default data source */
    #if(Button_IdacCH0_DEFAULT_DATA_SRC != 0u )    
        Button_IdacCH0_CR1 = (Button_IdacCH0_DEFAULT_CNTL | Button_IdacCH0_DACBUS_ENABLE);
    #else
        Button_IdacCH0_CR1 = (Button_IdacCH0_DEFAULT_CNTL | Button_IdacCH0_DACBUS_DISABLE);
    #endif /* (Button_IdacCH0_DEFAULT_DATA_SRC != 0u ) */
    
    /*Controls polarity from UDB Control*/
    #if(Button_IdacCH0_DEFAULT_POLARITY == Button_IdacCH0_HARDWARE_CONTROLLED)
        Button_IdacCH0_CR1 |= Button_IdacCH0_IDIR_SRC_UDB;
    #else
        Button_IdacCH0_CR1 |= Button_IdacCH0_DEFAULT_POLARITY;
    #endif/* (Button_IdacCH0_DEFAULT_POLARITY == Button_IdacCH0_HARDWARE_CONTROLLED) */
    /*Controls Current Source from UDB Control*/
    #if(Button_IdacCH0_HARDWARE_ENABLE != 0u ) 
        Button_IdacCH0_CR1 |= Button_IdacCH0_IDIR_CTL_UDB;
    #endif /* (Button_IdacCH0_HARDWARE_ENABLE != 0u ) */ 
    
    /* Set default strobe mode */
    #if(Button_IdacCH0_DEFAULT_STRB != 0u)
        Button_IdacCH0_Strobe |= Button_IdacCH0_STRB_EN;
    #endif /* (Button_IdacCH0_DEFAULT_STRB != 0u) */
    
    /* Set default speed */
    Button_IdacCH0_SetSpeed(Button_IdacCH0_DEFAULT_SPEED);
    
    /* Set proper DAC trim */
    Button_IdacCH0_DacTrim();
    
}


/*******************************************************************************
* Function Name: Button_IdacCH0_Enable
********************************************************************************
* Summary:
*  Enable the IDAC8
* 
* Parameters:
*  void:
*
* Return:
*  (void)
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
void Button_IdacCH0_Enable(void) 
{
    Button_IdacCH0_PWRMGR |= Button_IdacCH0_ACT_PWR_EN;
    Button_IdacCH0_STBY_PWRMGR |= Button_IdacCH0_STBY_PWR_EN;

    /* This is to restore the value of register CR0 which is saved 
      in prior to the modification in stop() API */
    #if (CY_PSOC5A)
        if(Button_IdacCH0_restoreReg == 1u)
        {
            Button_IdacCH0_CR0 = Button_IdacCH0_lowPowerBackup.DACCR0Reg;

            /* Clear the flag */
            Button_IdacCH0_restoreReg = 0u;
        }
    #endif /* CY_PSOC5A */
}


/*******************************************************************************
* Function Name: Button_IdacCH0_Start
********************************************************************************
* Summary:
*  Set power level then turn on IDAC8.
*
* Parameters:  
*  power: Sets power level between off (0) and (3) high power
*
* Return:
*  (void)
*
* Global variables:
*  Button_IdacCH0_initVar: Is modified when this function is called for 
*   the first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void Button_IdacCH0_Start(void) 
{
    /* Hardware initiazation only needs to occur the first time */
    if ( Button_IdacCH0_initVar == 0u)  
    {
        Button_IdacCH0_Init();
        
        Button_IdacCH0_initVar = 1u;
    }
   
    /* Enable power to DAC */
    Button_IdacCH0_Enable();

    /* Set default value */
    Button_IdacCH0_SetValue(Button_IdacCH0_DEFAULT_DATA);

}


/*******************************************************************************
* Function Name: Button_IdacCH0_Stop
********************************************************************************
* Summary:
*  Powers down IDAC8 to lowest power state.
*
* Parameters:
*  (void)
*
* Return:
*  (void)
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
void Button_IdacCH0_Stop(void) 
{
    /* Disble power to DAC */
    Button_IdacCH0_PWRMGR &= (uint8)(~Button_IdacCH0_ACT_PWR_EN);
    Button_IdacCH0_STBY_PWRMGR &= (uint8)(~Button_IdacCH0_STBY_PWR_EN);
    
    #if (CY_PSOC5A)
    
        /* Set the global variable  */
        Button_IdacCH0_restoreReg = 1u;

        /* Save the control register and then Clear it. */
        Button_IdacCH0_lowPowerBackup.DACCR0Reg = Button_IdacCH0_CR0;
        Button_IdacCH0_CR0 = (Button_IdacCH0_MODE_I | Button_IdacCH0_RANGE_3 | Button_IdacCH0_HS_HIGHSPEED);
    #endif /* CY_PSOC5A */
}


/*******************************************************************************
* Function Name: Button_IdacCH0_SetSpeed
********************************************************************************
* Summary:
*  Set DAC speed
*
* Parameters:
*  power: Sets speed value
*
* Return:
*  (void)
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
void Button_IdacCH0_SetSpeed(uint8 speed) 
{
    /* Clear power mask then write in new value */
    Button_IdacCH0_CR0 &= (uint8)(~Button_IdacCH0_HS_MASK);
    Button_IdacCH0_CR0 |=  ( speed & Button_IdacCH0_HS_MASK);
}


/*******************************************************************************
* Function Name: Button_IdacCH0_SetPolarity
********************************************************************************
* Summary:
*  Sets IDAC to Sink or Source current.
*  
* Parameters:
*  Polarity: Sets the IDAC to Sink or Source 
*  0x00 - Source
*  0x04 - Sink
*
* Return:
*  (void)
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
#if(Button_IdacCH0_DEFAULT_POLARITY != Button_IdacCH0_HARDWARE_CONTROLLED)
    void Button_IdacCH0_SetPolarity(uint8 polarity) 
    {
        Button_IdacCH0_CR1 &= (uint8)(~Button_IdacCH0_IDIR_MASK);                /* clear polarity bit */
        Button_IdacCH0_CR1 |= (polarity & Button_IdacCH0_IDIR_MASK);             /* set new value */
    
        Button_IdacCH0_DacTrim();
    }
#endif/*(Button_IdacCH0_DEFAULT_POLARITY != Button_IdacCH0_HARDWARE_CONTROLLED)*/


/*******************************************************************************
* Function Name: Button_IdacCH0_SetRange
********************************************************************************
* Summary:
*  Set current range
*
* Parameters:
*  Range: Sets on of four valid ranges.
*
* Return:
*  (void)
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
void Button_IdacCH0_SetRange(uint8 range) 
{
    Button_IdacCH0_CR0 &= (uint8)(~Button_IdacCH0_RANGE_MASK);       /* Clear existing mode */
    Button_IdacCH0_CR0 |= ( range & Button_IdacCH0_RANGE_MASK );     /*  Set Range  */
    Button_IdacCH0_DacTrim();
}


/*******************************************************************************
* Function Name: Button_IdacCH0_SetValue
********************************************************************************
* Summary:
*  Set DAC value
*
* Parameters:
*  value: Sets DAC value between 0 and 255.
*
* Return:
*  (void)
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
void Button_IdacCH0_SetValue(uint8 value) 
{

    #if (CY_PSOC5A)
        uint8 Button_IdacCH0_intrStatus = CyEnterCriticalSection();
    #endif /* CY_PSOC5A */

    Button_IdacCH0_Data = value;         /*  Set Value  */
    
    /* PSOC5A silicons require a double write */
    #if (CY_PSOC5A)
        Button_IdacCH0_Data = value;
        CyExitCriticalSection(Button_IdacCH0_intrStatus);
    #endif /* CY_PSOC5A */
}


/*******************************************************************************
* Function Name: Button_IdacCH0_DacTrim
********************************************************************************
* Summary:
*  Set the trim value for the given range.
*
* Parameters:
*  None
*
* Return:
*  (void) 
*
* Theory:
*  Trim values for the IDAC blocks are stored in the "Customer Table" area in 
*  Row 1 of the Hidden Flash.  There are 8 bytes of trim data for each 
*  IDAC block.
*  The values are:
*       I Gain offset, min range, Sourcing
*       I Gain offset, min range, Sinking
*       I Gain offset, med range, Sourcing
*       I Gain offset, med range, Sinking
*       I Gain offset, max range, Sourcing
*       I Gain offset, max range, Sinking
*       V Gain offset, 1V range
*       V Gain offset, 4V range
*
* Side Effects:
*
*******************************************************************************/
void Button_IdacCH0_DacTrim(void) 
{
    uint8 mode;

    mode = ((Button_IdacCH0_CR0 & Button_IdacCH0_RANGE_MASK) >> 1u);
    
    if((Button_IdacCH0_IDIR_MASK & Button_IdacCH0_CR1) == Button_IdacCH0_IDIR_SINK)
    {
        mode++;
    }

    Button_IdacCH0_TR = CY_GET_XTND_REG8((uint8 *)(Button_IdacCH0_DAC_TRIM_BASE + mode));
}


/* [] END OF FILE */
