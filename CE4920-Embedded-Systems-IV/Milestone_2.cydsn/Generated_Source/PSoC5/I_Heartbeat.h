/*******************************************************************************
* File Name: I_Heartbeat.h  
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

#if !defined(CY_WaveDAC8_I_Heartbeat_H) 
#define CY_WaveDAC8_I_Heartbeat_H

#include "cytypes.h"
#include "cyfitter.h"
#include <I_Heartbeat_Wave1_DMA_dma.h>
#include <I_Heartbeat_Wave2_DMA_dma.h>
#include <I_Heartbeat_VDAC8.h>


/***************************************
*  Initial Parameter Constants
***************************************/

#define I_Heartbeat_WAVE1_TYPE     (10u)     /* Waveform for wave1 */
#define I_Heartbeat_WAVE2_TYPE     (0u)     /* Waveform for wave2 */
#define I_Heartbeat_SINE_WAVE      (0u)
#define I_Heartbeat_SQUARE_WAVE    (1u)
#define I_Heartbeat_TRIANGLE_WAVE  (2u)
#define I_Heartbeat_SAWTOOTH_WAVE  (3u)
#define I_Heartbeat_ARB_DRAW_WAVE  (10u) /* Arbitrary (draw) */
#define I_Heartbeat_ARB_FILE_WAVE  (11u) /* Arbitrary (from file) */

#define I_Heartbeat_WAVE1_LENGTH   (100u)   /* Length for wave1 */
#define I_Heartbeat_WAVE2_LENGTH   (100u)   /* Length for wave2 */
	
#define I_Heartbeat_DEFAULT_RANGE    (0u) /* Default DAC range */
#define I_Heartbeat_DAC_RANGE_1V     (0u)
#define I_Heartbeat_DAC_RANGE_1V_BUF (16u)
#define I_Heartbeat_DAC_RANGE_4V     (1u)
#define I_Heartbeat_DAC_RANGE_4V_BUF (17u)
#define I_Heartbeat_VOLT_MODE        (0u)
#define I_Heartbeat_CURRENT_MODE     (1u)
#define I_Heartbeat_DAC_MODE         (((I_Heartbeat_DEFAULT_RANGE == I_Heartbeat_DAC_RANGE_1V) || \
									  (I_Heartbeat_DEFAULT_RANGE == I_Heartbeat_DAC_RANGE_4V) || \
							  		  (I_Heartbeat_DEFAULT_RANGE == I_Heartbeat_DAC_RANGE_1V_BUF) || \
									  (I_Heartbeat_DEFAULT_RANGE == I_Heartbeat_DAC_RANGE_4V_BUF)) ? \
									   I_Heartbeat_VOLT_MODE : I_Heartbeat_CURRENT_MODE)

#define I_Heartbeat_DACMODE I_Heartbeat_DAC_MODE /* legacy definition for backward compatibility */

#define I_Heartbeat_DIRECT_MODE (0u)
#define I_Heartbeat_BUFFER_MODE (1u)
#define I_Heartbeat_OUT_MODE    (((I_Heartbeat_DEFAULT_RANGE == I_Heartbeat_DAC_RANGE_1V_BUF) || \
								 (I_Heartbeat_DEFAULT_RANGE == I_Heartbeat_DAC_RANGE_4V_BUF)) ? \
								  I_Heartbeat_BUFFER_MODE : I_Heartbeat_DIRECT_MODE)

#if(I_Heartbeat_OUT_MODE == I_Heartbeat_BUFFER_MODE)
    #include <I_Heartbeat_BuffAmp.h>
#endif /* I_Heartbeat_OUT_MODE == I_Heartbeat_BUFFER_MODE */

#define I_Heartbeat_CLOCK_INT      (1u)
#define I_Heartbeat_CLOCK_EXT      (0u)
#define I_Heartbeat_CLOCK_SRC      (1u)

#if(I_Heartbeat_CLOCK_SRC == I_Heartbeat_CLOCK_INT)  
	#include <I_Heartbeat_DacClk.h>
	#if defined(I_Heartbeat_DacClk_PHASE)
		#define I_Heartbeat_CLK_PHASE_0nS (1u)
	#endif /* defined(I_Heartbeat_DacClk_PHASE) */
#endif /* I_Heartbeat_CLOCK_SRC == I_Heartbeat_CLOCK_INT */

#if (CY_PSOC3)
	#define I_Heartbeat_HI16FLASHPTR   (0xFFu)
#endif /* CY_PSOC3 */

#define I_Heartbeat_Wave1_DMA_BYTES_PER_BURST      (1u)
#define I_Heartbeat_Wave1_DMA_REQUEST_PER_BURST    (1u)
#define I_Heartbeat_Wave2_DMA_BYTES_PER_BURST      (1u)
#define I_Heartbeat_Wave2_DMA_REQUEST_PER_BURST    (1u)


/***************************************
*   Data Struct Definition
***************************************/

/* Low power Mode API Support */
typedef struct
{
	uint8   enableState;
}I_Heartbeat_BACKUP_STRUCT;


/***************************************
*        Function Prototypes 
***************************************/

void I_Heartbeat_Start(void)             ;
void I_Heartbeat_StartEx(const uint8 * wavePtr1, uint16 sampleSize1, const uint8 * wavePtr2, uint16 sampleSize2)
                                        ;
void I_Heartbeat_Init(void)              ;
void I_Heartbeat_Enable(void)            ;
void I_Heartbeat_Stop(void)              ;

void I_Heartbeat_Wave1Setup(const uint8 * wavePtr, uint16 sampleSize)
                                        ;
void I_Heartbeat_Wave2Setup(const uint8 * wavePtr, uint16 sampleSize)
                                        ;

void I_Heartbeat_Sleep(void)             ;
void I_Heartbeat_Wakeup(void)            ;

#define I_Heartbeat_SetSpeed       I_Heartbeat_VDAC8_SetSpeed
#define I_Heartbeat_SetRange       I_Heartbeat_VDAC8_SetRange
#define I_Heartbeat_SetValue       I_Heartbeat_VDAC8_SetValue
#define I_Heartbeat_DacTrim        I_Heartbeat_VDAC8_DacTrim
#define I_Heartbeat_SaveConfig     I_Heartbeat_VDAC8_SaveConfig
#define I_Heartbeat_RestoreConfig  I_Heartbeat_VDAC8_RestoreConfig


/***************************************
*    Variable with external linkage 
***************************************/

extern uint8 I_Heartbeat_initVar;

extern const uint8 CYCODE I_Heartbeat_wave1[I_Heartbeat_WAVE1_LENGTH];
extern const uint8 CYCODE I_Heartbeat_wave2[I_Heartbeat_WAVE2_LENGTH];


/***************************************
*            API Constants
***************************************/

/* SetRange constants */
#if(I_Heartbeat_DAC_MODE == I_Heartbeat_VOLT_MODE)
    #define I_Heartbeat_RANGE_1V       (0x00u)
    #define I_Heartbeat_RANGE_4V       (0x04u)
#else /* current mode */
    #define I_Heartbeat_RANGE_32uA     (0x00u)
    #define I_Heartbeat_RANGE_255uA    (0x04u)
    #define I_Heartbeat_RANGE_2mA      (0x08u)
    #define I_Heartbeat_RANGE_2048uA   I_Heartbeat_RANGE_2mA
#endif /* I_Heartbeat_DAC_MODE == I_Heartbeat_VOLT_MODE */

/* Power setting for SetSpeed API */
#define I_Heartbeat_LOWSPEED       (0x00u)
#define I_Heartbeat_HIGHSPEED      (0x02u)


/***************************************
*              Registers        
***************************************/

#define I_Heartbeat_DAC8__D I_Heartbeat_VDAC8_viDAC8__D


/***************************************
*         Register Constants       
***************************************/

/* CR0 vDac Control Register 0 definitions */

/* Bit Field  DAC_HS_MODE */
#define I_Heartbeat_HS_MASK        (0x02u)
#define I_Heartbeat_HS_LOWPOWER    (0x00u)
#define I_Heartbeat_HS_HIGHSPEED   (0x02u)

/* Bit Field  DAC_MODE */
#define I_Heartbeat_MODE_MASK      (0x10u)
#define I_Heartbeat_MODE_V         (0x00u)
#define I_Heartbeat_MODE_I         (0x10u)

/* Bit Field  DAC_RANGE */
#define I_Heartbeat_RANGE_MASK     (0x0Cu)
#define I_Heartbeat_RANGE_0        (0x00u)
#define I_Heartbeat_RANGE_1        (0x04u)
#define I_Heartbeat_RANGE_2        (0x08u)
#define I_Heartbeat_RANGE_3        (0x0Cu)
#define I_Heartbeat_IDIR_MASK      (0x04u)

#define I_Heartbeat_DAC_RANGE      ((uint8)(0u << 2u) & I_Heartbeat_RANGE_MASK)
#define I_Heartbeat_DAC_POL        ((uint8)(0u >> 1u) & I_Heartbeat_IDIR_MASK)


#endif /* CY_WaveDAC8_I_Heartbeat_H  */

/* [] END OF FILE */
