/*******************************************************************************
* File Name: I_Heartbeat.c
* Version 2.10
*
* Description:
*  This file provides the source code for the 8-bit Waveform DAC 
*  (WaveDAC8) Component.
*
********************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "I_Heartbeat.h"

uint8  I_Heartbeat_initVar = 0u;

const uint8 CYCODE I_Heartbeat_wave1[I_Heartbeat_WAVE1_LENGTH] = { 0u,1u,0u,0u,0u,1u,1u,2u,3u,3u,3u,0u,0u,0u,0u,2u,11u,11u,11u,12u,12u,12u,12u,12u,12u,12u,12u,12u,12u,12u,12u,12u,11u,11u,0u,2u,1u,1u,0u,0u,0u,0u,0u,2u,2u,2u,2u,2u,2u,2u,2u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u };
const uint8 CYCODE I_Heartbeat_wave2[I_Heartbeat_WAVE2_LENGTH] = { 0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u };

static uint8  I_Heartbeat_Wave1Chan;
static uint8  I_Heartbeat_Wave2Chan;
static uint8  I_Heartbeat_Wave1TD;
static uint8  I_Heartbeat_Wave2TD;


/*******************************************************************************
* Function Name: I_Heartbeat_Init
********************************************************************************
*
* Summary:
*  Initializes component with parameters set in the customizer.
*
* Parameters:  
*  None
*
* Return: 
*  None
*
*******************************************************************************/
void I_Heartbeat_Init(void) 
{
	I_Heartbeat_VDAC8_Init();
	I_Heartbeat_VDAC8_SetSpeed(I_Heartbeat_HIGHSPEED);
	I_Heartbeat_VDAC8_SetRange(I_Heartbeat_DAC_RANGE);

	#if(I_Heartbeat_DAC_MODE == I_Heartbeat_CURRENT_MODE)
		I_Heartbeat_IDAC8_SetPolarity(I_Heartbeat_DAC_POL);
	#endif /* I_Heartbeat_DAC_MODE == I_Heartbeat_CURRENT_MODE */

	#if(I_Heartbeat_OUT_MODE == I_Heartbeat_BUFFER_MODE)
	   I_Heartbeat_BuffAmp_Init();
	#endif /* I_Heartbeat_OUT_MODE == I_Heartbeat_BUFFER_MODE */

	/* Get the TD Number for the DMA channel 1 and 2   */
	I_Heartbeat_Wave1TD = CyDmaTdAllocate();
	I_Heartbeat_Wave2TD = CyDmaTdAllocate();
	
	/* Initialize waveform pointers  */
	I_Heartbeat_Wave1Setup(I_Heartbeat_wave1, I_Heartbeat_WAVE1_LENGTH) ;
	I_Heartbeat_Wave2Setup(I_Heartbeat_wave2, I_Heartbeat_WAVE2_LENGTH) ;
	
	/* Initialize the internal clock if one present  */
	#if defined(I_Heartbeat_DacClk_PHASE)
	   I_Heartbeat_DacClk_SetPhase(I_Heartbeat_CLK_PHASE_0nS);
	#endif /* defined(I_Heartbeat_DacClk_PHASE) */
}


/*******************************************************************************
* Function Name: I_Heartbeat_Enable
********************************************************************************
*  
* Summary: 
*  Enables the DAC block and DMA operation.
*
* Parameters:  
*  None
*
* Return: 
*  None
*
*******************************************************************************/
void I_Heartbeat_Enable(void) 
{
	I_Heartbeat_VDAC8_Enable();

	#if(I_Heartbeat_OUT_MODE == I_Heartbeat_BUFFER_MODE)
	   I_Heartbeat_BuffAmp_Enable();
	#endif /* I_Heartbeat_OUT_MODE == I_Heartbeat_BUFFER_MODE */

	/* 
	* Enable the channel. It is configured to remember the TD value so that
	* it can be restored from the place where it has been stopped.
	*/
	(void)CyDmaChEnable(I_Heartbeat_Wave1Chan, 1u);
	(void)CyDmaChEnable(I_Heartbeat_Wave2Chan, 1u);
	
	/* set the initial value */
	I_Heartbeat_SetValue(0u);
	
	#if(I_Heartbeat_CLOCK_SRC == I_Heartbeat_CLOCK_INT)  	
	   I_Heartbeat_DacClk_Start();
	#endif /* I_Heartbeat_CLOCK_SRC == I_Heartbeat_CLOCK_INT */
}


/*******************************************************************************
* Function Name: I_Heartbeat_Start
********************************************************************************
*
* Summary:
*  The start function initializes the voltage DAC with the default values, 
*  and sets the power to the given level.  A power level of 0, is the same as 
*  executing the stop function.
*
* Parameters:  
*  None
*
* Return: 
*  None
*
* Reentrant:
*  No
*
*******************************************************************************/
void I_Heartbeat_Start(void) 
{
	/* If not Initialized then initialize all required hardware and software */
	if(I_Heartbeat_initVar == 0u)
	{
		I_Heartbeat_Init();
		I_Heartbeat_initVar = 1u;
	}
	
	I_Heartbeat_Enable();
}


/*******************************************************************************
* Function Name: I_Heartbeat_StartEx
********************************************************************************
*
* Summary:
*  The StartEx function sets pointers and sizes for both waveforms
*  and then starts the component.
*
* Parameters:  
*   uint8 * wavePtr1:     Pointer to the waveform 1 array.
*   uint16  sampleSize1:  The amount of samples in the waveform 1.
*   uint8 * wavePtr2:     Pointer to the waveform 2 array.
*   uint16  sampleSize2:  The amount of samples in the waveform 2.
*
* Return: 
*  None
*
* Reentrant:
*  No
*
*******************************************************************************/
void I_Heartbeat_StartEx(const uint8 * wavePtr1, uint16 sampleSize1, const uint8 * wavePtr2, uint16 sampleSize2)

{
	I_Heartbeat_Wave1Setup(wavePtr1, sampleSize1);
	I_Heartbeat_Wave2Setup(wavePtr2, sampleSize2);
	I_Heartbeat_Start();
}


/*******************************************************************************
* Function Name: I_Heartbeat_Stop
********************************************************************************
*
* Summary:
*  Stops the clock (if internal), disables the DMA channels
*  and powers down the DAC.
*
* Parameters:  
*  None  
*
* Return: 
*  None
*
*******************************************************************************/
void I_Heartbeat_Stop(void) 
{
	/* Turn off internal clock, if one present */
	#if(I_Heartbeat_CLOCK_SRC == I_Heartbeat_CLOCK_INT)  	
	   I_Heartbeat_DacClk_Stop();
	#endif /* I_Heartbeat_CLOCK_SRC == I_Heartbeat_CLOCK_INT */
	
	/* Disble DMA channels */
	(void)CyDmaChDisable(I_Heartbeat_Wave1Chan);
	(void)CyDmaChDisable(I_Heartbeat_Wave2Chan);

	/* Disable power to DAC */
	I_Heartbeat_VDAC8_Stop();
}


/*******************************************************************************
* Function Name: I_Heartbeat_Wave1Setup
********************************************************************************
*
* Summary:
*  Sets pointer and size for waveform 1.                                    
*
* Parameters:  
*  uint8 * WavePtr:     Pointer to the waveform array.
*  uint16  SampleSize:  The amount of samples in the waveform.
*
* Return: 
*  None 
*
*******************************************************************************/
void I_Heartbeat_Wave1Setup(const uint8 * wavePtr, uint16 sampleSize)

{
	#if (CY_PSOC3)
		uint16 memoryType; /* determining the source memory type */
		memoryType = (I_Heartbeat_HI16FLASHPTR == HI16(wavePtr)) ? HI16(CYDEV_FLS_BASE) : HI16(CYDEV_SRAM_BASE);
		
		I_Heartbeat_Wave1Chan = I_Heartbeat_Wave1_DMA_DmaInitialize(
		I_Heartbeat_Wave1_DMA_BYTES_PER_BURST, I_Heartbeat_Wave1_DMA_REQUEST_PER_BURST,
		memoryType, HI16(CYDEV_PERIPH_BASE));
	#else /* PSoC 5 */
		I_Heartbeat_Wave1Chan = I_Heartbeat_Wave1_DMA_DmaInitialize(
		I_Heartbeat_Wave1_DMA_BYTES_PER_BURST, I_Heartbeat_Wave1_DMA_REQUEST_PER_BURST,
		HI16(wavePtr), HI16(I_Heartbeat_DAC8__D));
	#endif /* CY_PSOC3 */
	
	/*
	* TD is looping on itself. 
    * Increment the source address, but not the destination address. 
	*/
	(void)CyDmaTdSetConfiguration(I_Heartbeat_Wave1TD, sampleSize, I_Heartbeat_Wave1TD, 
                                    (uint8)CY_DMA_TD_INC_SRC_ADR | (uint8)I_Heartbeat_Wave1_DMA__TD_TERMOUT_EN); 
	
	/* Set the TD source and destination address */
	(void)CyDmaTdSetAddress(I_Heartbeat_Wave1TD, LO16((uint32)wavePtr), LO16(I_Heartbeat_DAC8__D));
	
	/* Associate the TD with the channel */
	(void)CyDmaChSetInitialTd(I_Heartbeat_Wave1Chan, I_Heartbeat_Wave1TD);
}


/*******************************************************************************
* Function Name: I_Heartbeat_Wave2Setup
********************************************************************************
*
* Summary:
*  Sets pointer and size for waveform 2.                                    
*
* Parameters:  
*  uint8 * WavePtr:     Pointer to the waveform array.
*  uint16  SampleSize:  The amount of samples in the waveform.
*
* Return: 
*  None
*
*******************************************************************************/
void I_Heartbeat_Wave2Setup(const uint8 * wavePtr, uint16 sampleSize)
 
{
	#if (CY_PSOC3)
		uint16 memoryType; /* determining the source memory type */
		memoryType = (I_Heartbeat_HI16FLASHPTR == HI16(wavePtr)) ? HI16(CYDEV_FLS_BASE) : HI16(CYDEV_SRAM_BASE);
			
		I_Heartbeat_Wave2Chan = I_Heartbeat_Wave2_DMA_DmaInitialize(
		I_Heartbeat_Wave2_DMA_BYTES_PER_BURST, I_Heartbeat_Wave2_DMA_REQUEST_PER_BURST,
		memoryType, HI16(CYDEV_PERIPH_BASE));
	#else /* PSoC 5 */
		I_Heartbeat_Wave2Chan = I_Heartbeat_Wave2_DMA_DmaInitialize(
		I_Heartbeat_Wave2_DMA_BYTES_PER_BURST, I_Heartbeat_Wave2_DMA_REQUEST_PER_BURST,
		HI16(wavePtr), HI16(I_Heartbeat_DAC8__D));
	#endif /* CY_PSOC3 */
	
	/*
	* TD is looping on itself. 
	* Increment the source address, but not the destination address. 
	*/
	(void)CyDmaTdSetConfiguration(I_Heartbeat_Wave2TD, sampleSize, I_Heartbeat_Wave2TD, 
                                    (uint8)CY_DMA_TD_INC_SRC_ADR | (uint8)I_Heartbeat_Wave2_DMA__TD_TERMOUT_EN); 
	
	/* Set the TD source and destination address */
	(void)CyDmaTdSetAddress(I_Heartbeat_Wave2TD, LO16((uint32)wavePtr), LO16(I_Heartbeat_DAC8__D));
	
	/* Associate the TD with the channel */
	(void)CyDmaChSetInitialTd(I_Heartbeat_Wave2Chan, I_Heartbeat_Wave2TD);
}


/* [] END OF FILE */
