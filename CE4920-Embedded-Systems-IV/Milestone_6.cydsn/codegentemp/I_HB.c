/*******************************************************************************
* File Name: I_HB.c
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

#include "I_HB.h"

uint8  I_HB_initVar = 0u;

const uint8 CYCODE I_HB_wave1[I_HB_WAVE1_LENGTH] = { 12u,13u,13u,13u,13u,13u,13u,13u,13u,13u,13u,13u,13u,13u,13u,13u,14u,14u,14u,14u,14u,14u,14u,14u,14u,14u,14u,14u,14u,14u,14u,15u,15u,15u,15u,15u,15u,15u,15u,15u,15u,15u,15u,15u,15u,15u,15u,16u,16u,16u,16u,16u,16u,16u,16u,16u,16u,16u,16u,16u,16u,16u,16u,17u,17u,17u,17u,17u,17u,17u,17u,17u,17u,17u,17u,17u,17u,17u,17u,18u,18u,18u,18u,18u,18u,18u,18u,18u,18u,18u,18u,18u,18u,18u,18u,18u,19u,19u,19u,19u,19u,19u,19u,19u,19u,19u,19u,19u,19u,19u,19u,19u,19u,19u,20u,20u,20u,20u,20u,20u,20u,20u,20u,20u,20u,20u,20u,20u,20u,20u,20u,20u,20u,21u,21u,21u,21u,21u,21u,21u,21u,21u,21u,21u,21u,21u,21u,21u,21u,21u,21u,21u,21u,21u,22u,22u,22u,22u,22u,22u,22u,22u,22u,22u,22u,22u,22u,22u,22u,22u,22u,22u,22u,22u,22u,22u,22u,22u,23u,23u,23u,23u,23u,23u,23u,23u,23u,23u,23u,23u,23u,23u,23u,23u,23u,23u,23u,23u,23u,23u,23u,23u,23u,23u,23u,23u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,25u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,24u,23u,23u,23u,23u,23u,23u,23u,23u,23u,23u,23u,23u,23u,23u,23u,23u,23u,23u,23u,23u,23u,23u,23u,23u,23u,23u,23u,23u,22u,22u,22u,22u,22u,22u,22u,22u,22u,22u,22u,22u,22u,22u,22u,22u,22u,22u,22u,22u,22u,22u,22u,22u,21u,21u,21u,21u,21u,21u,21u,21u,21u,21u,21u,21u,21u,21u,21u,21u,21u,21u,21u,21u,21u,20u,20u,20u,20u,20u,20u,20u,20u,20u,20u,20u,20u,20u,20u,20u,20u,20u,20u,20u,19u,19u,19u,19u,19u,19u,19u,19u,19u,19u,19u,19u,19u,19u,19u,19u,19u,19u,18u,18u,18u,18u,18u,18u,18u,18u,18u,18u,18u,18u,18u,18u,18u,18u,18u,17u,17u,17u,17u,17u,17u,17u,17u,17u,17u,17u,17u,17u,17u,17u,17u,16u,16u,16u,16u,16u,16u,16u,16u,16u,16u,16u,16u,16u,16u,16u,16u,15u,15u,15u,15u,15u,15u,15u,15u,15u,15u,15u,15u,15u,15u,15u,15u,14u,14u,14u,14u,14u,14u,14u,14u,14u,14u,14u,14u,14u,14u,14u,13u,13u,13u,13u,13u,13u,13u,13u,13u,13u,13u,13u,13u,13u,13u,13u,12u,12u,12u,12u,12u,12u,12u,12u,12u,12u,12u,12u,12u,12u,12u,11u,11u,11u,11u,11u,11u,11u,11u,11u,11u,11u,11u,11u,11u,11u,10u,10u,10u,10u,10u,10u,10u,10u,10u,10u,10u,10u,10u,10u,10u,10u,9u,9u,9u,9u,9u,9u,9u,9u,9u,9u,9u,9u,9u,9u,9u,9u,8u,8u,8u,8u,8u,8u,8u,8u,8u,8u,8u,8u,8u,8u,8u,8u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,5u,5u,5u,5u,5u,5u,5u,5u,5u,5u,5u,5u,5u,5u,5u,5u,5u,5u,5u,4u,4u,4u,4u,4u,4u,4u,4u,4u,4u,4u,4u,4u,4u,4u,4u,4u,4u,4u,4u,4u,3u,3u,3u,3u,3u,3u,3u,3u,3u,3u,3u,3u,3u,3u,3u,3u,3u,3u,3u,3u,3u,3u,3u,3u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,3u,3u,3u,3u,3u,3u,3u,3u,3u,3u,3u,3u,3u,3u,3u,3u,3u,3u,3u,3u,3u,3u,3u,3u,4u,4u,4u,4u,4u,4u,4u,4u,4u,4u,4u,4u,4u,4u,4u,4u,4u,4u,4u,4u,4u,5u,5u,5u,5u,5u,5u,5u,5u,5u,5u,5u,5u,5u,5u,5u,5u,5u,5u,5u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,8u,8u,8u,8u,8u,8u,8u,8u,8u,8u,8u,8u,8u,8u,8u,8u,9u,9u,9u,9u,9u,9u,9u,9u,9u,9u,9u,9u,9u,9u,9u,9u,10u,10u,10u,10u,10u,10u,10u,10u,10u,10u,10u,10u,10u,10u,10u,10u,11u,11u,11u,11u,11u,11u,11u,11u,11u,11u,11u,11u,11u,11u,11u,12u,12u,12u,12u,12u,12u,12u,12u,12u,12u,12u,12u,12u,12u,12u };
const uint8 CYCODE I_HB_wave2[I_HB_WAVE2_LENGTH] = { 128u,135u,143u,151u,159u,166u,174u,181u,188u,194u,201u,207u,213u,219u,224u,229u,233u,237u,241u,244u,246u,249u,250u,252u,252u,252u,252u,252u,250u,249u,246u,244u,241u,237u,233u,229u,224u,219u,213u,207u,201u,194u,188u,181u,174u,166u,159u,151u,143u,135u,128u,120u,112u,104u,96u,89u,81u,74u,67u,61u,54u,48u,42u,36u,31u,26u,22u,18u,14u,11u,9u,6u,5u,3u,3u,3u,3u,3u,5u,6u,9u,11u,14u,18u,22u,26u,31u,36u,42u,48u,54u,61u,67u,74u,81u,89u,96u,104u,112u,120u };

static uint8  I_HB_Wave1Chan;
static uint8  I_HB_Wave2Chan;
static uint8  I_HB_Wave1TD;
static uint8  I_HB_Wave2TD;


/*******************************************************************************
* Function Name: I_HB_Init
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
void I_HB_Init(void) 
{
	I_HB_VDAC8_Init();
	I_HB_VDAC8_SetSpeed(I_HB_HIGHSPEED);
	I_HB_VDAC8_SetRange(I_HB_DAC_RANGE);

	#if(I_HB_DAC_MODE == I_HB_CURRENT_MODE)
		I_HB_IDAC8_SetPolarity(I_HB_DAC_POL);
	#endif /* I_HB_DAC_MODE == I_HB_CURRENT_MODE */

	#if(I_HB_OUT_MODE == I_HB_BUFFER_MODE)
	   I_HB_BuffAmp_Init();
	#endif /* I_HB_OUT_MODE == I_HB_BUFFER_MODE */

	/* Get the TD Number for the DMA channel 1 and 2   */
	I_HB_Wave1TD = CyDmaTdAllocate();
	I_HB_Wave2TD = CyDmaTdAllocate();
	
	/* Initialize waveform pointers  */
	I_HB_Wave1Setup(I_HB_wave1, I_HB_WAVE1_LENGTH) ;
	I_HB_Wave2Setup(I_HB_wave2, I_HB_WAVE2_LENGTH) ;
	
	/* Initialize the internal clock if one present  */
	#if defined(I_HB_DacClk_PHASE)
	   I_HB_DacClk_SetPhase(I_HB_CLK_PHASE_0nS);
	#endif /* defined(I_HB_DacClk_PHASE) */
}


/*******************************************************************************
* Function Name: I_HB_Enable
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
void I_HB_Enable(void) 
{
	I_HB_VDAC8_Enable();

	#if(I_HB_OUT_MODE == I_HB_BUFFER_MODE)
	   I_HB_BuffAmp_Enable();
	#endif /* I_HB_OUT_MODE == I_HB_BUFFER_MODE */

	/* 
	* Enable the channel. It is configured to remember the TD value so that
	* it can be restored from the place where it has been stopped.
	*/
	(void)CyDmaChEnable(I_HB_Wave1Chan, 1u);
	(void)CyDmaChEnable(I_HB_Wave2Chan, 1u);
	
	/* set the initial value */
	I_HB_SetValue(0u);
	
	#if(I_HB_CLOCK_SRC == I_HB_CLOCK_INT)  	
	   I_HB_DacClk_Start();
	#endif /* I_HB_CLOCK_SRC == I_HB_CLOCK_INT */
}


/*******************************************************************************
* Function Name: I_HB_Start
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
void I_HB_Start(void) 
{
	/* If not Initialized then initialize all required hardware and software */
	if(I_HB_initVar == 0u)
	{
		I_HB_Init();
		I_HB_initVar = 1u;
	}
	
	I_HB_Enable();
}


/*******************************************************************************
* Function Name: I_HB_StartEx
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
void I_HB_StartEx(const uint8 * wavePtr1, uint16 sampleSize1, const uint8 * wavePtr2, uint16 sampleSize2)

{
	I_HB_Wave1Setup(wavePtr1, sampleSize1);
	I_HB_Wave2Setup(wavePtr2, sampleSize2);
	I_HB_Start();
}


/*******************************************************************************
* Function Name: I_HB_Stop
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
void I_HB_Stop(void) 
{
	/* Turn off internal clock, if one present */
	#if(I_HB_CLOCK_SRC == I_HB_CLOCK_INT)  	
	   I_HB_DacClk_Stop();
	#endif /* I_HB_CLOCK_SRC == I_HB_CLOCK_INT */
	
	/* Disble DMA channels */
	(void)CyDmaChDisable(I_HB_Wave1Chan);
	(void)CyDmaChDisable(I_HB_Wave2Chan);

	/* Disable power to DAC */
	I_HB_VDAC8_Stop();
}


/*******************************************************************************
* Function Name: I_HB_Wave1Setup
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
void I_HB_Wave1Setup(const uint8 * wavePtr, uint16 sampleSize)

{
	#if (CY_PSOC3)
		uint16 memoryType; /* determining the source memory type */
		memoryType = (I_HB_HI16FLASHPTR == HI16(wavePtr)) ? HI16(CYDEV_FLS_BASE) : HI16(CYDEV_SRAM_BASE);
		
		I_HB_Wave1Chan = I_HB_Wave1_DMA_DmaInitialize(
		I_HB_Wave1_DMA_BYTES_PER_BURST, I_HB_Wave1_DMA_REQUEST_PER_BURST,
		memoryType, HI16(CYDEV_PERIPH_BASE));
	#else /* PSoC 5 */
		I_HB_Wave1Chan = I_HB_Wave1_DMA_DmaInitialize(
		I_HB_Wave1_DMA_BYTES_PER_BURST, I_HB_Wave1_DMA_REQUEST_PER_BURST,
		HI16(wavePtr), HI16(I_HB_DAC8__D));
	#endif /* CY_PSOC3 */
	
	/*
	* TD is looping on itself. 
    * Increment the source address, but not the destination address. 
	*/
	(void)CyDmaTdSetConfiguration(I_HB_Wave1TD, sampleSize, I_HB_Wave1TD, 
                                    (uint8)CY_DMA_TD_INC_SRC_ADR | (uint8)I_HB_Wave1_DMA__TD_TERMOUT_EN); 
	
	/* Set the TD source and destination address */
	(void)CyDmaTdSetAddress(I_HB_Wave1TD, LO16((uint32)wavePtr), LO16(I_HB_DAC8__D));
	
	/* Associate the TD with the channel */
	(void)CyDmaChSetInitialTd(I_HB_Wave1Chan, I_HB_Wave1TD);
}


/*******************************************************************************
* Function Name: I_HB_Wave2Setup
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
void I_HB_Wave2Setup(const uint8 * wavePtr, uint16 sampleSize)
 
{
	#if (CY_PSOC3)
		uint16 memoryType; /* determining the source memory type */
		memoryType = (I_HB_HI16FLASHPTR == HI16(wavePtr)) ? HI16(CYDEV_FLS_BASE) : HI16(CYDEV_SRAM_BASE);
			
		I_HB_Wave2Chan = I_HB_Wave2_DMA_DmaInitialize(
		I_HB_Wave2_DMA_BYTES_PER_BURST, I_HB_Wave2_DMA_REQUEST_PER_BURST,
		memoryType, HI16(CYDEV_PERIPH_BASE));
	#else /* PSoC 5 */
		I_HB_Wave2Chan = I_HB_Wave2_DMA_DmaInitialize(
		I_HB_Wave2_DMA_BYTES_PER_BURST, I_HB_Wave2_DMA_REQUEST_PER_BURST,
		HI16(wavePtr), HI16(I_HB_DAC8__D));
	#endif /* CY_PSOC3 */
	
	/*
	* TD is looping on itself. 
	* Increment the source address, but not the destination address. 
	*/
	(void)CyDmaTdSetConfiguration(I_HB_Wave2TD, sampleSize, I_HB_Wave2TD, 
                                    (uint8)CY_DMA_TD_INC_SRC_ADR | (uint8)I_HB_Wave2_DMA__TD_TERMOUT_EN); 
	
	/* Set the TD source and destination address */
	(void)CyDmaTdSetAddress(I_HB_Wave2TD, LO16((uint32)wavePtr), LO16(I_HB_DAC8__D));
	
	/* Associate the TD with the channel */
	(void)CyDmaChSetInitialTd(I_HB_Wave2Chan, I_HB_Wave2TD);
}


/* [] END OF FILE */
