/*******************************************************************************
* File Name: Tone_C.c  
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

#include "Tone_C.h"

uint8 Tone_C_initVar = 0u;


/*******************************************************************************
* Function Name: Tone_C_Init
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
void Tone_C_Init(void) 
{
        #if (!Tone_C_UsingFixedFunction && !Tone_C_ControlRegRemoved)
            uint8 ctrl;
        #endif /* (!Tone_C_UsingFixedFunction && !Tone_C_ControlRegRemoved) */
        
        #if(!Tone_C_UsingFixedFunction) 
            /* Interrupt State Backup for Critical Region*/
            uint8 Tone_C_interruptState;
        #endif /* (!Tone_C_UsingFixedFunction) */
        
        #if (Tone_C_UsingFixedFunction)
            /* Clear all bits but the enable bit (if it's already set for Timer operation */
            Tone_C_CONTROL &= Tone_C_CTRL_ENABLE;
            
            /* Clear the mode bits for continuous run mode */
            #if (CY_PSOC5A)
                Tone_C_CONTROL2 &= ((uint8)(~Tone_C_CTRL_MODE_MASK));
            #endif /* (CY_PSOC5A) */
            #if (CY_PSOC3 || CY_PSOC5LP)
                Tone_C_CONTROL3 &= ((uint8)(~Tone_C_CTRL_MODE_MASK));                
            #endif /* (CY_PSOC3 || CY_PSOC5LP) */
            /* Check if One Shot mode is enabled i.e. RunMode !=0*/
            #if (Tone_C_RunModeUsed != 0x0u)
                /* Set 3rd bit of Control register to enable one shot mode */
                Tone_C_CONTROL |= Tone_C_ONESHOT;
            #endif /* (Tone_C_RunModeUsed != 0x0u) */
            
            /* Set the IRQ to use the status register interrupts */
            Tone_C_CONTROL2 |= Tone_C_CTRL2_IRQ_SEL;
            
            /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
            Tone_C_RT1 &= ((uint8)(~Tone_C_RT1_MASK));
            Tone_C_RT1 |= Tone_C_SYNC;     
                    
            /*Enable DSI Sync all all inputs of the Timer*/
            Tone_C_RT1 &= ((uint8)(~Tone_C_SYNCDSI_MASK));
            Tone_C_RT1 |= Tone_C_SYNCDSI_EN;

        #else
            #if(!Tone_C_ControlRegRemoved)
            /* Set the default compare mode defined in the parameter */
            ctrl = Tone_C_CONTROL & ((uint8)(~Tone_C_CTRL_CMPMODE_MASK));
            Tone_C_CONTROL = ctrl | Tone_C_DEFAULT_COMPARE_MODE;
            
            /* Set the default capture mode defined in the parameter */
            ctrl = Tone_C_CONTROL & ((uint8)(~Tone_C_CTRL_CAPMODE_MASK));
            
            #if( 0 != Tone_C_CAPTURE_MODE_CONF)
                Tone_C_CONTROL = ctrl | Tone_C_DEFAULT_CAPTURE_MODE;
            #else
                Tone_C_CONTROL = ctrl;
            #endif /* 0 != Tone_C_CAPTURE_MODE */ 
            
            #endif /* (!Tone_C_ControlRegRemoved) */
        #endif /* (Tone_C_UsingFixedFunction) */
        
        /* Clear all data in the FIFO's */
        #if (!Tone_C_UsingFixedFunction)
            Tone_C_ClearFIFO();
        #endif /* (!Tone_C_UsingFixedFunction) */
        
        /* Set Initial values from Configuration */
        Tone_C_WritePeriod(Tone_C_INIT_PERIOD_VALUE);
        #if (!(Tone_C_UsingFixedFunction && (CY_PSOC5A)))
            Tone_C_WriteCounter(Tone_C_INIT_COUNTER_VALUE);
        #endif /* (!(Tone_C_UsingFixedFunction && (CY_PSOC5A))) */
        Tone_C_SetInterruptMode(Tone_C_INIT_INTERRUPTS_MASK);
        
        #if (!Tone_C_UsingFixedFunction)
            /* Read the status register to clear the unwanted interrupts */
            (void)Tone_C_ReadStatusRegister();
            /* Set the compare value (only available to non-fixed function implementation */
            Tone_C_WriteCompare(Tone_C_INIT_COMPARE_VALUE);
            /* Use the interrupt output of the status register for IRQ output */
            
            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            Tone_C_interruptState = CyEnterCriticalSection();
            
            Tone_C_STATUS_AUX_CTRL |= Tone_C_STATUS_ACTL_INT_EN_MASK;
            
            /* Exit Critical Region*/
            CyExitCriticalSection(Tone_C_interruptState);
            
        #endif /* (!Tone_C_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Tone_C_Enable
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
void Tone_C_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (Tone_C_UsingFixedFunction)
        Tone_C_GLOBAL_ENABLE |= Tone_C_BLOCK_EN_MASK;
        Tone_C_GLOBAL_STBY_ENABLE |= Tone_C_BLOCK_STBY_EN_MASK;
    #endif /* (Tone_C_UsingFixedFunction) */  
        
    /* Enable the counter from the control register  */
    /* If Fixed Function then make sure Mode is set correctly */
    /* else make sure reset is clear */
    #if(!Tone_C_ControlRegRemoved || Tone_C_UsingFixedFunction)
        Tone_C_CONTROL |= Tone_C_CTRL_ENABLE;                
    #endif /* (!Tone_C_ControlRegRemoved || Tone_C_UsingFixedFunction) */
    
}


/*******************************************************************************
* Function Name: Tone_C_Start
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
*  Tone_C_initVar: Is modified when this function is called for the  
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void Tone_C_Start(void) 
{
    if(Tone_C_initVar == 0u)
    {
        Tone_C_Init();
        
        Tone_C_initVar = 1u; /* Clear this bit for Initialization */        
    }
    
    /* Enable the Counter */
    Tone_C_Enable();        
}


/*******************************************************************************
* Function Name: Tone_C_Stop
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
void Tone_C_Stop(void) 
{
    /* Disable Counter */
    #if(!Tone_C_ControlRegRemoved || Tone_C_UsingFixedFunction)
        Tone_C_CONTROL &= ((uint8)(~Tone_C_CTRL_ENABLE));        
    #endif /* (!Tone_C_ControlRegRemoved || Tone_C_UsingFixedFunction) */
    
    /* Globally disable the Fixed Function Block chosen */
    #if (Tone_C_UsingFixedFunction)
        Tone_C_GLOBAL_ENABLE &= ((uint8)(~Tone_C_BLOCK_EN_MASK));
        Tone_C_GLOBAL_STBY_ENABLE &= ((uint8)(~Tone_C_BLOCK_STBY_EN_MASK));
    #endif /* (Tone_C_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Tone_C_SetInterruptMode
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
void Tone_C_SetInterruptMode(uint8 interruptsMask) 
{
    Tone_C_STATUS_MASK = interruptsMask;
}


/*******************************************************************************
* Function Name: Tone_C_ReadStatusRegister
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
uint8   Tone_C_ReadStatusRegister(void) 
{
    return Tone_C_STATUS;
}


#if(!Tone_C_ControlRegRemoved)
/*******************************************************************************
* Function Name: Tone_C_ReadControlRegister
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
uint8   Tone_C_ReadControlRegister(void) 
{
    return Tone_C_CONTROL;
}


/*******************************************************************************
* Function Name: Tone_C_WriteControlRegister
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
void    Tone_C_WriteControlRegister(uint8 control) 
{
    Tone_C_CONTROL = control;
}

#endif  /* (!Tone_C_ControlRegRemoved) */


#if (!(Tone_C_UsingFixedFunction && (CY_PSOC5A)))
/*******************************************************************************
* Function Name: Tone_C_WriteCounter
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
void Tone_C_WriteCounter(uint16 counter) \
                                   
{
    #if(Tone_C_UsingFixedFunction)
        /* assert if block is already enabled */
        CYASSERT (0u == (Tone_C_GLOBAL_ENABLE & Tone_C_BLOCK_EN_MASK));
        /* If block is disabled, enable it and then write the counter */
        Tone_C_GLOBAL_ENABLE |= Tone_C_BLOCK_EN_MASK;
        CY_SET_REG16(Tone_C_COUNTER_LSB_PTR, (uint16)counter);
        Tone_C_GLOBAL_ENABLE &= ((uint8)(~Tone_C_BLOCK_EN_MASK));
    #else
        CY_SET_REG16(Tone_C_COUNTER_LSB_PTR, counter);
    #endif /* (Tone_C_UsingFixedFunction) */
}
#endif /* (!(Tone_C_UsingFixedFunction && (CY_PSOC5A))) */


/*******************************************************************************
* Function Name: Tone_C_ReadCounter
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
uint16 Tone_C_ReadCounter(void) 
{
    /* Force capture by reading Accumulator */
    /* Must first do a software capture to be able to read the counter */
    /* It is up to the user code to make sure there isn't already captured data in the FIFO */
    #if(Tone_C_UsingFixedFunction)
		(void)CY_GET_REG16(Tone_C_COUNTER_LSB_PTR);
	#else
		(void)CY_GET_REG8(Tone_C_COUNTER_LSB_PTR_8BIT);
	#endif/* (Tone_C_UsingFixedFunction) */
    
    /* Read the data from the FIFO (or capture register for Fixed Function)*/
    #if(Tone_C_UsingFixedFunction)
        return ((uint16)CY_GET_REG16(Tone_C_STATICCOUNT_LSB_PTR));
    #else
        return (CY_GET_REG16(Tone_C_STATICCOUNT_LSB_PTR));
    #endif /* (Tone_C_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Tone_C_ReadCapture
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
uint16 Tone_C_ReadCapture(void) 
{
    #if(Tone_C_UsingFixedFunction)
        return ((uint16)CY_GET_REG16(Tone_C_STATICCOUNT_LSB_PTR));
    #else
        return (CY_GET_REG16(Tone_C_STATICCOUNT_LSB_PTR));
    #endif /* (Tone_C_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Tone_C_WritePeriod
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
void Tone_C_WritePeriod(uint16 period) 
{
    #if(Tone_C_UsingFixedFunction)
        CY_SET_REG16(Tone_C_PERIOD_LSB_PTR,(uint16)period);
    #else
        CY_SET_REG16(Tone_C_PERIOD_LSB_PTR, period);
    #endif /* (Tone_C_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Tone_C_ReadPeriod
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
uint16 Tone_C_ReadPeriod(void) 
{
    #if(Tone_C_UsingFixedFunction)
        return ((uint16)CY_GET_REG16(Tone_C_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG16(Tone_C_PERIOD_LSB_PTR));
    #endif /* (Tone_C_UsingFixedFunction) */
}


#if (!Tone_C_UsingFixedFunction)
/*******************************************************************************
* Function Name: Tone_C_WriteCompare
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
void Tone_C_WriteCompare(uint16 compare) \
                                   
{
    #if(Tone_C_UsingFixedFunction)
        CY_SET_REG16(Tone_C_COMPARE_LSB_PTR, (uint16)compare);
    #else
        CY_SET_REG16(Tone_C_COMPARE_LSB_PTR, compare);
    #endif /* (Tone_C_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Tone_C_ReadCompare
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
uint16 Tone_C_ReadCompare(void) 
{
    return (CY_GET_REG16(Tone_C_COMPARE_LSB_PTR));
}


#if (Tone_C_COMPARE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: Tone_C_SetCompareMode
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
void Tone_C_SetCompareMode(uint8 compareMode) 
{
    /* Clear the compare mode bits in the control register */
    Tone_C_CONTROL &= ((uint8)(~Tone_C_CTRL_CMPMODE_MASK));
    
    /* Write the new setting */
    Tone_C_CONTROL |= compareMode;
}
#endif  /* (Tone_C_COMPARE_MODE_SOFTWARE) */


#if (Tone_C_CAPTURE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: Tone_C_SetCaptureMode
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
void Tone_C_SetCaptureMode(uint8 captureMode) 
{
    /* Clear the capture mode bits in the control register */
    Tone_C_CONTROL &= ((uint8)(~Tone_C_CTRL_CAPMODE_MASK));
    
    /* Write the new setting */
    Tone_C_CONTROL |= ((uint8)((uint8)captureMode << Tone_C_CTRL_CAPMODE0_SHIFT));
}
#endif  /* (Tone_C_CAPTURE_MODE_SOFTWARE) */


/*******************************************************************************
* Function Name: Tone_C_ClearFIFO
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
void Tone_C_ClearFIFO(void) 
{

    while(0u != (Tone_C_ReadStatusRegister() & Tone_C_STATUS_FIFONEMP))
    {
        (void)Tone_C_ReadCapture();
    }

}
#endif  /* (!Tone_C_UsingFixedFunction) */


/* [] END OF FILE */

