/*******************************************************************************
* File Name: HB_PGA.c  
* Version 2.0
*
* Description:
*  This file provides the source code to the API for the PGA 
*  User Module.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "HB_PGA.h"

#if (!CY_PSOC5A)
    #if (CYDEV_VARIABLE_VDDA == 1u)
        #include "CyScBoostClk.h"
    #endif /* (CYDEV_VARIABLE_VDDA == 1u) */
#endif /* (!CY_PSOC5A) */

#if (CY_PSOC5A)
    static HB_PGA_BACKUP_STRUCT  HB_PGA_P5backup;
#endif /* (CY_ PSOC5A) */

uint8 HB_PGA_initVar = 0u;


/*******************************************************************************   
* Function Name: HB_PGA_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the 
*  customizer of the component placed onto schematic. Usually called in 
*  HB_PGA_Start().
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void HB_PGA_Init(void) 
{
    /* Set PGA mode */
    HB_PGA_CR0_REG = HB_PGA_MODE_PGA;      
    /* Set non-inverting PGA mode and reference mode */
    HB_PGA_CR1_REG |= HB_PGA_PGA_NINV;  
    /* Set default gain and ref mode */
    HB_PGA_CR2_REG = HB_PGA_VREF_MODE;
    /* Set gain and compensation */
    HB_PGA_SetGain(HB_PGA_DEFAULT_GAIN);
    /* Set power */
    HB_PGA_SetPower(HB_PGA_DEFAULT_POWER);
}


/*******************************************************************************   
* Function Name: HB_PGA_Enable
********************************************************************************
*
* Summary:
*  Enables the PGA block operation.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void HB_PGA_Enable(void) 
{
    /* This is to restore the value of register CR1 and CR2 which is saved 
      in prior to the modifications in stop() API */
    #if (CY_PSOC5A)
        if(HB_PGA_P5backup.enableState == 1u)
        {
            HB_PGA_CR1_REG = HB_PGA_P5backup.scCR1Reg;
            HB_PGA_CR2_REG = HB_PGA_P5backup.scCR2Reg;
            HB_PGA_P5backup.enableState = 0u;
        }
    #endif /* CY_PSOC5A */   

    /* Enable power to the Amp in Active mode*/
    HB_PGA_PM_ACT_CFG_REG |= HB_PGA_ACT_PWR_EN;

    /* Enable power to the Amp in Alternative Active mode*/
    HB_PGA_PM_STBY_CFG_REG |= HB_PGA_STBY_PWR_EN;
    
    HB_PGA_PUMP_CR1_REG |= HB_PGA_PUMP_CR1_SC_CLKSEL;
    
    #if (!CY_PSOC5A)
        #if (CYDEV_VARIABLE_VDDA == 1u)
            if(CyScPumpEnabled == 1u)
            {
                HB_PGA_BSTCLK_REG &= (uint8)(~HB_PGA_BST_CLK_INDEX_MASK);
                HB_PGA_BSTCLK_REG |= HB_PGA_BST_CLK_EN | CyScBoostClk__INDEX;
                HB_PGA_SC_MISC_REG |= HB_PGA_PUMP_FORCE;
                CyScBoostClk_Start();
            }
            else
            {
                HB_PGA_BSTCLK_REG &= (uint8)(~HB_PGA_BST_CLK_EN);
                HB_PGA_SC_MISC_REG &= (uint8)(~HB_PGA_PUMP_FORCE);
            }
        #endif /* (CYDEV_VARIABLE_VDDA == 1u) */
    #endif /* (!CY_PSOC5A) */
}


/*******************************************************************************
* Function Name: HB_PGA_Start
********************************************************************************
*
* Summary:
*  The start function initializes the PGA with the default values and sets
*  the power to the given level. A power level of 0, is same as executing
*  the stop function.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void HB_PGA_Start(void) 
{

    /* This is to restore the value of register CR1 and CR2 which is saved 
      in prior to the modification in stop() API */

    if(HB_PGA_initVar == 0u)
    {
        HB_PGA_Init();
        HB_PGA_initVar = 1u;
    }

    HB_PGA_Enable();
}


/*******************************************************************************
* Function Name: HB_PGA_Stop
********************************************************************************
*
* Summary:
*  Powers down amplifier to lowest power state.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void HB_PGA_Stop(void) 
{ 
    /* Disble power to the Amp in Active mode template */
    HB_PGA_PM_ACT_CFG_REG &= (uint8)(~HB_PGA_ACT_PWR_EN);

    /* Disble power to the Amp in Alternative Active mode template */
    HB_PGA_PM_STBY_CFG_REG &= (uint8)(~HB_PGA_STBY_PWR_EN);

    #if (!CY_PSOC5A)
        #if (CYDEV_VARIABLE_VDDA == 1u)
            HB_PGA_BSTCLK_REG &= (uint8)(~HB_PGA_BST_CLK_EN);
            /* Disable pumps only if there aren't any SC block in use */
            if ((HB_PGA_PM_ACT_CFG_REG & HB_PGA_PM_ACT_CFG_MASK) == 0u)
            {
                HB_PGA_SC_MISC_REG &= (uint8)(~HB_PGA_PUMP_FORCE);
                HB_PGA_PUMP_CR1_REG &= (uint8)(~HB_PGA_PUMP_CR1_SC_CLKSEL);
                CyScBoostClk_Stop();
            }
        #endif /* CYDEV_VARIABLE_VDDA == 1u */
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */

    /* This sets PGA in zero current mode and output routes are valid */
    #if (CY_PSOC5A)
        HB_PGA_P5backup.scCR1Reg = HB_PGA_CR1_REG;
        HB_PGA_P5backup.scCR2Reg = HB_PGA_CR2_REG;
        HB_PGA_CR1_REG = 0x00u;
        HB_PGA_CR2_REG = 0x00u;
        HB_PGA_P5backup.enableState = 1u;
    #endif /* CY_PSOC5A */
}


/*******************************************************************************
* Function Name: HB_PGA_SetPower
********************************************************************************
*
* Summary:
*  Set the power of the PGA.
*
* Parameters:
*  power: Sets power level between (0) and (3) high power
*
* Return:
*  void
*
*******************************************************************************/
void HB_PGA_SetPower(uint8 power) 
{
    uint8 tmpCR;

    tmpCR = HB_PGA_CR1_REG & (uint8)(~HB_PGA_DRIVE_MASK);
    tmpCR |= (power & HB_PGA_DRIVE_MASK);
    HB_PGA_CR1_REG = tmpCR;  
}


/*******************************************************************************
* Function Name: HB_PGA_SetGain
********************************************************************************
*
* Summary:
*  This function sets values of the input and feedback resistors to set a 
*  specific gain of the amplifier.
*
* Parameters:
*  gain: Gain value of PGA (See header file for gain values.)
*
* Return:
*  void 
*
*******************************************************************************/
void HB_PGA_SetGain(uint8 gain) 
{
    /* Constant array for gain settings */
    const uint8 HB_PGA_GainArray[9] = { 
        (HB_PGA_RVAL_0K   | HB_PGA_R20_40B_40K | HB_PGA_BIAS_LOW), /* G=1  */
        (HB_PGA_RVAL_40K  | HB_PGA_R20_40B_40K | HB_PGA_BIAS_LOW), /* G=2  */
        (HB_PGA_RVAL_120K | HB_PGA_R20_40B_40K | HB_PGA_BIAS_LOW), /* G=4  */
        (HB_PGA_RVAL_280K | HB_PGA_R20_40B_40K | HB_PGA_BIAS_LOW), /* G=8  */
        (HB_PGA_RVAL_600K | HB_PGA_R20_40B_40K | HB_PGA_BIAS_LOW), /* G=16 */
        (HB_PGA_RVAL_460K | HB_PGA_R20_40B_40K | HB_PGA_BIAS_LOW), /* G=24, Sets Rin as 20k */
        (HB_PGA_RVAL_620K | HB_PGA_R20_40B_20K | HB_PGA_BIAS_LOW), /* G=32 */
        (HB_PGA_RVAL_470K | HB_PGA_R20_40B_20K | HB_PGA_BIAS_LOW), /* G=48, Sets Rin as 10k */
        (HB_PGA_RVAL_490K | HB_PGA_R20_40B_20K | HB_PGA_BIAS_LOW)  /* G=50, Sets Rin as 10k */
    };
    
    /* Constant array for gain compenstion settings */
    const uint8 HB_PGA_GainComp[9] = { 
        ( HB_PGA_COMP_4P35PF  | (uint8)( HB_PGA_REDC_00 >> 2 )), /* G=1  */
        ( HB_PGA_COMP_4P35PF  | (uint8)( HB_PGA_REDC_01 >> 2 )), /* G=2  */
        ( HB_PGA_COMP_3P0PF   | (uint8)( HB_PGA_REDC_01 >> 2 )), /* G=4  */
        ( HB_PGA_COMP_3P0PF   | (uint8)( HB_PGA_REDC_01 >> 2 )), /* G=8  */
        ( HB_PGA_COMP_3P6PF   | (uint8)( HB_PGA_REDC_01 >> 2 )), /* G=16 */
        ( HB_PGA_COMP_3P6PF   | (uint8)( HB_PGA_REDC_11 >> 2 )), /* G=24 */
        ( HB_PGA_COMP_3P6PF   | (uint8)( HB_PGA_REDC_11 >> 2 )), /* G=32 */
        ( HB_PGA_COMP_3P6PF   | (uint8)( HB_PGA_REDC_00 >> 2 )), /* G=48 */
        ( HB_PGA_COMP_3P6PF   | (uint8)( HB_PGA_REDC_00 >> 2 ))  /* G=50 */
    };
    
    /* Only set new gain if it is a valid gain */
    if( gain <= HB_PGA_GAIN_MAX)
    {
        /* Clear resistors, redc, and bias */
        HB_PGA_CR2_REG &= (uint8)(~(HB_PGA_RVAL_MASK | HB_PGA_R20_40B_MASK | 
                                HB_PGA_REDC_MASK | HB_PGA_BIAS_MASK ));

        /* Set gain value resistors, redc comp, and bias */
        HB_PGA_CR2_REG |= (HB_PGA_GainArray[gain] |
                                ((uint8)(HB_PGA_GainComp[gain] << 2 ) & HB_PGA_REDC_MASK));

        /* Clear sc_comp  */
        HB_PGA_CR1_REG &= (uint8)(~HB_PGA_COMP_MASK);
        /* Set sc_comp  */
        HB_PGA_CR1_REG |= ( HB_PGA_GainComp[gain] | HB_PGA_COMP_MASK );
    }
}


/* [] END OF FILE */
