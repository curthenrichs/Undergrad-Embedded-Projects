/*******************************************************************************
* File Name: Tone_D.c  
* Version 3.0
*
*  Description:
*     The Counter component consists of a 8, 16, 24 or 32-bit counter with
*     a selectable period between 2 and 2^Width - 1.  
*
*   Note:
*     None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "Tone_D.h"

uint8 Tone_D_initVar = 0u;


/*******************************************************************************
* Function Name: Tone_D_Init
********************************************************************************
* Summary:
*     Initialize to the schematic state
* 
* Parameters:  
*  void  
*
* Return: 
*  void
*
*******************************************************************************/
void Tone_D_Init(void) 
{
        #if (!Tone_D_UsingFixedFunction && !Tone_D_ControlRegRemoved)
            uint8 ctrl;
        #endif /* (!Tone_D_UsingFixedFunction && !Tone_D_ControlRegRemoved) */
        
        #if(!Tone_D_UsingFixedFunction) 
            /* Interrupt State Backup for Critical Region*/
            uint8 Tone_D_interruptState;
        #endif /* (!Tone_D_UsingFixedFunction) */
        
        #if (Tone_D_UsingFixedFunction)
            /* Clear all bits but the enable bit (if it's already set for Timer operation */
            Tone_D_CONTROL &= Tone_D_CTRL_ENABLE;
            
            /* Clear the mode bits for continuous run mode */
            #if (CY_PSOC5A)
                Tone_D_CONTROL2 &= ((uint8)(~Tone_D_CTRL_MODE_MASK));
            #endif /* (CY_PSOC5A) */
            #if (CY_PSOC3 || CY_PSOC5LP)
                Tone_D_CONTROL3 &= ((uint8)(~Tone_D_CTRL_MODE_MASK));                
            #endif /* (CY_PSOC3 || CY_PSOC5LP) */
            /* Check if One Shot mode is enabled i.e. RunMode !=0*/
            #if (Tone_D_RunModeUsed != 0x0u)
                /* Set 3rd bit of Control register to enable one shot mode */
                Tone_D_CONTROL |= Tone_D_ONESHOT;
            #endif /* (Tone_D_RunModeUsed != 0x0u) */
            
            /* Set the IRQ to use the status register interrupts */
            Tone_D_CONTROL2 |= Tone_D_CTRL2_IRQ_SEL;
            
            /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
            Tone_D_RT1 &= ((uint8)(~Tone_D_RT1_MASK));
            Tone_D_RT1 |= Tone_D_SYNC;     
                    
            /*Enable DSI Sync all all inputs of the Timer*/
            Tone_D_RT1 &= ((uint8)(~Tone_D_SYNCDSI_MASK));
            Tone_D_RT1 |= Tone_D_SYNCDSI_EN;

        #else
            #if(!Tone_D_ControlRegRemoved)
            /* Set the default compare mode defined in the parameter */
            ctrl = Tone_D_CONTROL & ((uint8)(~Tone_D_CTRL_CMPMODE_MASK));
            Tone_D_CONTROL = ctrl | Tone_D_DEFAULT_COMPARE_MODE;
            
            /* Set the default capture mode defined in the parameter */
            ctrl = Tone_D_CONTROL & ((uint8)(~Tone_D_CTRL_CAPMODE_MASK));
            
            #if( 0 != Tone_D_CAPTURE_MODE_CONF)
                Tone_D_CONTROL = ctrl | Tone_D_DEFAULT_CAPTURE_MODE;
            #else
                Tone_D_CONTROL = ctrl;
            #endif /* 0 != Tone_D_CAPTURE_MODE */ 
            
            #endif /* (!Tone_D_ControlRegRemoved) */
        #endif /* (Tone_D_UsingFixedFunction) */
        
        /* Clear all data in the FIFO's */
        #if (!Tone_D_UsingFixedFunction)
            Tone_D_ClearFIFO();
        #endif /* (!Tone_D_UsingFixedFunction) */
        
        /* Set Initial values from Configuration */
        Tone_D_WritePeriod(Tone_D_INIT_PERIOD_VALUE);
        #if (!(Tone_D_UsingFixedFunction && (CY_PSOC5A)))
            Tone_D_WriteCounter(Tone_D_INIT_COUNTER_VALUE);
        #endif /* (!(Tone_D_UsingFixedFunction && (CY_PSOC5A))) */
        Tone_D_SetInterruptMode(Tone_D_INIT_INTERRUPTS_MASK);
        
        #if (!Tone_D_UsingFixedFunction)
            /* Read the status register to clear the unwanted interrupts */
            (void)Tone_D_ReadStatusRegister();
            /* Set the compare value (only available to non-fixed function implementation */
            Tone_D_WriteCompare(Tone_D_INIT_COMPARE_VALUE);
            /* Use the interrupt output of the status register for IRQ output */
            
            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            Tone_D_interruptState = CyEnterCriticalSection();
            
            Tone_D_STATUS_AUX_CTRL |= Tone_D_STATUS_ACTL_INT_EN_MASK;
            
            /* Exit Critical Region*/
            CyExitCriticalSection(Tone_D_interruptState);
            
        #endif /* (!Tone_D_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Tone_D_Enable
********************************************************************************
* Summary:
*     Enable the Counter
* 
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Side Effects: 
*   If the Enable mode is set to Hardware only then this function has no effect 
*   on the operation of the counter.
*
*******************************************************************************/
void Tone_D_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (Tone_D_UsingFixedFunction)
        Tone_D_GLOBAL_ENABLE |= Tone_D_BLOCK_EN_MASK;
        Tone_D_GLOBAL_STBY_ENABLE |= Tone_D_BLOCK_STBY_EN_MASK;
    #endif /* (Tone_D_UsingFixedFunction) */  
        
    /* Enable the counter from the control register  */
    /* If Fixed Function then make sure Mode is set correctly */
    /* else make sure reset is clear */
    #if(!Tone_D_ControlRegRemoved || Tone_D_UsingFixedFunction)
        Tone_D_CONTROL |= Tone_D_CTRL_ENABLE;                
    #endif /* (!Tone_D_ControlRegRemoved || Tone_D_UsingFixedFunction) */
    
}


/*******************************************************************************
* Function Name: Tone_D_Start
********************************************************************************
* Summary:
*  Enables the counter for operation 
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Global variables:
*  Tone_D_initVar: Is modified when this function is called for the  
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void Tone_D_Start(void) 
{
    if(Tone_D_initVar == 0u)
    {
        Tone_D_Init();
        
        Tone_D_initVar = 1u; /* Clear this bit for Initialization */        
    }
    
    /* Enable the Counter */
    Tone_D_Enable();        
}


/*******************************************************************************
* Function Name: Tone_D_Stop
********************************************************************************
* Summary:
* Halts the counter, but does not change any modes or disable interrupts.
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Side Effects: If the Enable mode is set to Hardware only then this function
*               has no effect on the operation of the counter.
*
*******************************************************************************/
void Tone_D_Stop(void) 
{
    /* Disable Counter */
    #if(!Tone_D_ControlRegRemoved || Tone_D_UsingFixedFunction)
        Tone_D_CONTROL &= ((uint8)(~Tone_D_CTRL_ENABLE));        
    #endif /* (!Tone_D_ControlRegRemoved || Tone_D_UsingFixedFunction) */
    
    /* Globally disable the Fixed Function Block chosen */
    #if (Tone_D_UsingFixedFunction)
        Tone_D_GLOBAL_ENABLE &= ((uint8)(~Tone_D_BLOCK_EN_MASK));
        Tone_D_GLOBAL_STBY_ENABLE &= ((uint8)(~Tone_D_BLOCK_STBY_EN_MASK));
    #endif /* (Tone_D_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Tone_D_SetInterruptMode
********************************************************************************
* Summary:
* Configures which interrupt sources are enabled to generate the final interrupt
*
* Parameters:  
*  InterruptsMask: This parameter is an or'd collection of the status bits
*                   which will be allowed to generate the counters interrupt.   
*
* Return: 
*  void
*
*******************************************************************************/
void Tone_D_SetInterruptMode(uint8 interruptsMask) 
{
    Tone_D_STATUS_MASK = interruptsMask;
}


/*******************************************************************************
* Function Name: Tone_D_ReadStatusRegister
********************************************************************************
* Summary:
*   Reads the status register and returns it's state. This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the status register
*
* Side Effects:
*   Status register bits may be clear on read. 
*
*******************************************************************************/
uint8   Tone_D_ReadStatusRegister(void) 
{
    return Tone_D_STATUS;
}


#if(!Tone_D_ControlRegRemoved)
/*******************************************************************************
* Function Name: Tone_D_ReadControlRegister
********************************************************************************
* Summary:
*   Reads the control register and returns it's state. This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the control register
*
*******************************************************************************/
uint8   Tone_D_ReadControlRegister(void) 
{
    return Tone_D_CONTROL;
}


/*******************************************************************************
* Function Name: Tone_D_WriteControlRegister
********************************************************************************
* Summary:
*   Sets the bit-field of the control register.  This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the control register
*
*******************************************************************************/
void    Tone_D_WriteControlRegister(uint8 control) 
{
    Tone_D_CONTROL = control;
}

#endif  /* (!Tone_D_ControlRegRemoved) */


#if (!(Tone_D_UsingFixedFunction && (CY_PSOC5A)))
/*******************************************************************************
* Function Name: Tone_D_WriteCounter
********************************************************************************
* Summary:
*   This funtion is used to set the counter to a specific value
*
* Parameters:  
*  counter:  New counter value. 
*
* Return: 
*  void 
*
*******************************************************************************/
void Tone_D_WriteCounter(uint16 counter) \
                                   
{
    #if(Tone_D_UsingFixedFunction)
        /* assert if block is already enabled */
        CYASSERT (0u == (Tone_D_GLOBAL_ENABLE & Tone_D_BLOCK_EN_MASK));
        /* If block is disabled, enable it and then write the counter */
        Tone_D_GLOBAL_ENABLE |= Tone_D_BLOCK_EN_MASK;
        CY_SET_REG16(Tone_D_COUNTER_LSB_PTR, (uint16)counter);
        Tone_D_GLOBAL_ENABLE &= ((uint8)(~Tone_D_BLOCK_EN_MASK));
    #else
        CY_SET_REG16(Tone_D_COUNTER_LSB_PTR, counter);
    #endif /* (Tone_D_UsingFixedFunction) */
}
#endif /* (!(Tone_D_UsingFixedFunction && (CY_PSOC5A))) */


/*******************************************************************************
* Function Name: Tone_D_ReadCounter
********************************************************************************
* Summary:
* Returns the current value of the counter.  It doesn't matter
* if the counter is enabled or running.
*
* Parameters:  
*  void:  
*
* Return: 
*  (uint16) The present value of the counter.
*
*******************************************************************************/
uint16 Tone_D_ReadCounter(void) 
{
    /* Force capture by reading Accumulator */
    /* Must first do a software capture to be able to read the counter */
    /* It is up to the user code to make sure there isn't already captured data in the FIFO */
    #if(Tone_D_UsingFixedFunction)
		(void)CY_GET_REG16(Tone_D_COUNTER_LSB_PTR);
	#else
		(void)CY_GET_REG8(Tone_D_COUNTER_LSB_PTR_8BIT);
	#endif/* (Tone_D_UsingFixedFunction) */
    
    /* Read the data from the FIFO (or capture register for Fixed Function)*/
    #if(Tone_D_UsingFixedFunction)
        return ((uint16)CY_GET_REG16(Tone_D_STATICCOUNT_LSB_PTR));
    #else
        return (CY_GET_REG16(Tone_D_STATICCOUNT_LSB_PTR));
    #endif /* (Tone_D_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Tone_D_ReadCapture
********************************************************************************
* Summary:
*   This function returns the last value captured.
*
* Parameters:  
*  void
*
* Return: 
*  (uint16) Present Capture value.
*
*******************************************************************************/
uint16 Tone_D_ReadCapture(void) 
{
    #if(Tone_D_UsingFixedFunction)
        return ((uint16)CY_GET_REG16(Tone_D_STATICCOUNT_LSB_PTR));
    #else
        return (CY_GET_REG16(Tone_D_STATICCOUNT_LSB_PTR));
    #endif /* (Tone_D_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Tone_D_WritePeriod
********************************************************************************
* Summary:
* Changes the period of the counter.  The new period 
* will be loaded the next time terminal count is detected.
*
* Parameters:  
*  period: (uint16) A value of 0 will result in
*         the counter remaining at zero.  
*
* Return: 
*  void
*
*******************************************************************************/
void Tone_D_WritePeriod(uint16 period) 
{
    #if(Tone_D_UsingFixedFunction)
        CY_SET_REG16(Tone_D_PERIOD_LSB_PTR,(uint16)period);
    #else
        CY_SET_REG16(Tone_D_PERIOD_LSB_PTR, period);
    #endif /* (Tone_D_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Tone_D_ReadPeriod
********************************************************************************
* Summary:
* Reads the current period value without affecting counter operation.
*
* Parameters:  
*  void:  
*
* Return: 
*  (uint16) Present period value.
*
*******************************************************************************/
uint16 Tone_D_ReadPeriod(void) 
{
    #if(Tone_D_UsingFixedFunction)
        return ((uint16)CY_GET_REG16(Tone_D_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG16(Tone_D_PERIOD_LSB_PTR));
    #endif /* (Tone_D_UsingFixedFunction) */
}


#if (!Tone_D_UsingFixedFunction)
/*******************************************************************************
* Function Name: Tone_D_WriteCompare
********************************************************************************
* Summary:
* Changes the compare value.  The compare output will 
* reflect the new value on the next UDB clock.  The compare output will be 
* driven high when the present counter value compares true based on the 
* configured compare mode setting. 
*
* Parameters:  
*  Compare:  New compare value. 
*
* Return: 
*  void
*
*******************************************************************************/
void Tone_D_WriteCompare(uint16 compare) \
                                   
{
    #if(Tone_D_UsingFixedFunction)
        CY_SET_REG16(Tone_D_COMPARE_LSB_PTR, (uint16)compare);
    #else
        CY_SET_REG16(Tone_D_COMPARE_LSB_PTR, compare);
    #endif /* (Tone_D_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Tone_D_ReadCompare
********************************************************************************
* Summary:
* Returns the compare value.
*
* Parameters:  
*  void:
*
* Return: 
*  (uint16) Present compare value.
*
*******************************************************************************/
uint16 Tone_D_ReadCompare(void) 
{
    return (CY_GET_REG16(Tone_D_COMPARE_LSB_PTR));
}


#if (Tone_D_COMPARE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: Tone_D_SetCompareMode
********************************************************************************
* Summary:
*  Sets the software controlled Compare Mode.
*
* Parameters:
*  compareMode:  Compare Mode Enumerated Type.
*
* Return:
*  void
*
*******************************************************************************/
void Tone_D_SetCompareMode(uint8 compareMode) 
{
    /* Clear the compare mode bits in the control register */
    Tone_D_CONTROL &= ((uint8)(~Tone_D_CTRL_CMPMODE_MASK));
    
    /* Write the new setting */
    Tone_D_CONTROL |= compareMode;
}
#endif  /* (Tone_D_COMPARE_MODE_SOFTWARE) */


#if (Tone_D_CAPTURE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: Tone_D_SetCaptureMode
********************************************************************************
* Summary:
*  Sets the software controlled Capture Mode.
*
* Parameters:
*  captureMode:  Capture Mode Enumerated Type.
*
* Return:
*  void
*
*******************************************************************************/
void Tone_D_SetCaptureMode(uint8 captureMode) 
{
    /* Clear the capture mode bits in the control register */
    Tone_D_CONTROL &= ((uint8)(~Tone_D_CTRL_CAPMODE_MASK));
    
    /* Write the new setting */
    Tone_D_CONTROL |= ((uint8)((uint8)captureMode << Tone_D_CTRL_CAPMODE0_SHIFT));
}
#endif  /* (Tone_D_CAPTURE_MODE_SOFTWARE) */


/*******************************************************************************
* Function Name: Tone_D_ClearFIFO
********************************************************************************
* Summary:
*   This function clears all capture data from the capture FIFO
*
* Parameters:  
*  void:
*
* Return: 
*  None
*
*******************************************************************************/
void Tone_D_ClearFIFO(void) 
{

    while(0u != (Tone_D_ReadStatusRegister() & Tone_D_STATUS_FIFONEMP))
    {
        (void)Tone_D_ReadCapture();
    }

}
#endif  /* (!Tone_D_UsingFixedFunction) */


/* [] END OF FILE */

