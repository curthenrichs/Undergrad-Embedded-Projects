/*******************************************************************************
* File Name: I_HB.h  
* Version 2.10
*
* Description:
*  This file contains the function prototypes and constants used in
*  the 8-bit Waveform DAC (WaveDAC8) Component.
*
********************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_WaveDAC8_I_HB_H) 
#define CY_WaveDAC8_I_HB_H

#include "cytypes.h"
#include "cyfitter.h"
#include <I_HB_Wave1_DMA_dma.h>
#include <I_HB_Wave2_DMA_dma.h>
#include <I_HB_VDAC8.h>


/***************************************
*  Initial Parameter Constants
***************************************/

#define I_HB_WAVE1_TYPE     (0u)     /* Waveform for wave1 */
#define I_HB_WAVE2_TYPE     (0u)     /* Waveform for wave2 */
#define I_HB_SINE_WAVE      (0u)
#define I_HB_SQUARE_WAVE    (1u)
#define I_HB_TRIANGLE_WAVE  (2u)
#define I_HB_SAWTOOTH_WAVE  (3u)
#define I_HB_ARB_DRAW_WAVE  (10u) /* Arbitrary (draw) */
#define I_HB_ARB_FILE_WAVE  (11u) /* Arbitrary (from file) */

#define I_HB_WAVE1_LENGTH   (1200u)   /* Length for wave1 */
#define I_HB_WAVE2_LENGTH   (100u)   /* Length for wave2 */
	
#define I_HB_DEFAULT_RANGE    (0u) /* Default DAC range */
#define I_HB_DAC_RANGE_1V     (0u)
#define I_HB_DAC_RANGE_1V_BUF (16u)
#define I_HB_DAC_RANGE_4V     (1u)
#define I_HB_DAC_RANGE_4V_BUF (17u)
#define I_HB_VOLT_MODE        (0u)
#define I_HB_CURRENT_MODE     (1u)
#define I_HB_DAC_MODE         (((I_HB_DEFAULT_RANGE == I_HB_DAC_RANGE_1V) || \
									  (I_HB_DEFAULT_RANGE == I_HB_DAC_RANGE_4V) || \
							  		  (I_HB_DEFAULT_RANGE == I_HB_DAC_RANGE_1V_BUF) || \
									  (I_HB_DEFAULT_RANGE == I_HB_DAC_RANGE_4V_BUF)) ? \
									   I_HB_VOLT_MODE : I_HB_CURRENT_MODE)

#define I_HB_DACMODE I_HB_DAC_MODE /* legacy definition for backward compatibility */

#define I_HB_DIRECT_MODE (0u)
#define I_HB_BUFFER_MODE (1u)
#define I_HB_OUT_MODE    (((I_HB_DEFAULT_RANGE == I_HB_DAC_RANGE_1V_BUF) || \
								 (I_HB_DEFAULT_RANGE == I_HB_DAC_RANGE_4V_BUF)) ? \
								  I_HB_BUFFER_MODE : I_HB_DIRECT_MODE)

#if(I_HB_OUT_MODE == I_HB_BUFFER_MODE)
    #include <I_HB_BuffAmp.h>
#endif /* I_HB_OUT_MODE == I_HB_BUFFER_MODE */

#define I_HB_CLOCK_INT      (1u)
#define I_HB_CLOCK_EXT      (0u)
#define I_HB_CLOCK_SRC      (1u)

#if(I_HB_CLOCK_SRC == I_HB_CLOCK_INT)  
	#include <I_HB_DacClk.h>
	#if defined(I_HB_DacClk_PHASE)
		#define I_HB_CLK_PHASE_0nS (1u)
	#endif /* defined(I_HB_DacClk_PHASE) */
#endif /* I_HB_CLOCK_SRC == I_HB_CLOCK_INT */

#if (CY_PSOC3)
	#define I_HB_HI16FLASHPTR   (0xFFu)
#endif /* CY_PSOC3 */

#define I_HB_Wave1_DMA_BYTES_PER_BURST      (1u)
#define I_HB_Wave1_DMA_REQUEST_PER_BURST    (1u)
#define I_HB_Wave2_DMA_BYTES_PER_BURST      (1u)
#define I_HB_Wave2_DMA_REQUEST_PER_BURST    (1u)


/***************************************
*   Data Struct Definition
***************************************/

/* Low power Mode API Support */
typedef struct
{
	uint8   enableState;
}I_HB_BACKUP_STRUCT;


/***************************************
*        Function Prototypes 
***************************************/

void I_HB_Start(void)             ;
void I_HB_StartEx(const uint8 * wavePtr1, uint16 sampleSize1, const uint8 * wavePtr2, uint16 sampleSize2)
                                        ;
void I_HB_Init(void)              ;
void I_HB_Enable(void)            ;
void I_HB_Stop(void)              ;

void I_HB_Wave1Setup(const uint8 * wavePtr, uint16 sampleSize)
                                        ;
void I_HB_Wave2Setup(const uint8 * wavePtr, uint16 sampleSize)
                                        ;

void I_HB_Sleep(void)             ;
void I_HB_Wakeup(void)            ;

#define I_HB_SetSpeed       I_HB_VDAC8_SetSpeed
#define I_HB_SetRange       I_HB_VDAC8_SetRange
#define I_HB_SetValue       I_HB_VDAC8_SetValue
#define I_HB_DacTrim        I_HB_VDAC8_DacTrim
#define I_HB_SaveConfig     I_HB_VDAC8_SaveConfig
#define I_HB_RestoreConfig  I_HB_VDAC8_RestoreConfig


/***************************************
*    Variable with external linkage 
***************************************/

extern uint8 I_HB_initVar;

extern const uint8 CYCODE I_HB_wave1[I_HB_WAVE1_LENGTH];
extern const uint8 CYCODE I_HB_wave2[I_HB_WAVE2_LENGTH];


/***************************************
*            API Constants
***************************************/

/* SetRange constants */
#if(I_HB_DAC_MODE == I_HB_VOLT_MODE)
    #define I_HB_RANGE_1V       (0x00u)
    #define I_HB_RANGE_4V       (0x04u)
#else /* current mode */
    #define I_HB_RANGE_32uA     (0x00u)
    #define I_HB_RANGE_255uA    (0x04u)
    #define I_HB_RANGE_2mA      (0x08u)
    #define I_HB_RANGE_2048uA   I_HB_RANGE_2mA
#endif /* I_HB_DAC_MODE == I_HB_VOLT_MODE */

/* Power setting for SetSpeed API */
#define I_HB_LOWSPEED       (0x00u)
#define I_HB_HIGHSPEED      (0x02u)


/***************************************
*              Registers        
***************************************/

#define I_HB_DAC8__D I_HB_VDAC8_viDAC8__D


/***************************************
*         Register Constants       
***************************************/

/* CR0 vDac Control Register 0 definitions */

/* Bit Field  DAC_HS_MODE */
#define I_HB_HS_MASK        (0x02u)
#define I_HB_HS_LOWPOWER    (0x00u)
#define I_HB_HS_HIGHSPEED   (0x02u)

/* Bit Field  DAC_MODE */
#define I_HB_MODE_MASK      (0x10u)
#define I_HB_MODE_V         (0x00u)
#define I_HB_MODE_I         (0x10u)

/* Bit Field  DAC_RANGE */
#define I_HB_RANGE_MASK     (0x0Cu)
#define I_HB_RANGE_0        (0x00u)
#define I_HB_RANGE_1        (0x04u)
#define I_HB_RANGE_2        (0x08u)
#define I_HB_RANGE_3        (0x0Cu)
#define I_HB_IDIR_MASK      (0x04u)

#define I_HB_DAC_RANGE      ((uint8)(0u << 2u) & I_HB_RANGE_MASK)
#define I_HB_DAC_POL        ((uint8)(0u >> 1u) & I_HB_IDIR_MASK)


#endif /* CY_WaveDAC8_I_HB_H  */

/* [] END OF FILE */
