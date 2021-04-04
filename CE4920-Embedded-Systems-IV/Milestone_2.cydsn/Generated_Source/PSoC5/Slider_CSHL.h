/*******************************************************************************
* File Name: Slider_CSHL.h
* Version 3.50
*
* Description:
*  This file provides constants and parameter values for the High Level APIs
*  for CapSense CSD component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CAPSENSE_CSD_CSHL_Slider_H)
#define CY_CAPSENSE_CSD_CSHL_Slider_H

#include "Slider.h"


/***************************************
*   Condition compilation parameters
***************************************/

#define Slider_SIGNAL_SIZE                (8u)
#define Slider_AUTO_RESET                 (0u)
#define Slider_RAW_FILTER_MASK            (8u)

/* Signal size definition */
#define Slider_SIGNAL_SIZE_UINT8          (8u)
#define Slider_SIGNAL_SIZE_UINT16         (16u)

/* Auto reset definition */
#define Slider_AUTO_RESET_DISABLE         (0u)
#define Slider_AUTO_RESET_ENABLE          (1u)

/* Mask for RAW and POS filters */
#define Slider_MEDIAN_FILTER              (0x01u)
#define Slider_AVERAGING_FILTER           (0x02u)
#define Slider_IIR2_FILTER                (0x04u)
#define Slider_IIR4_FILTER                (0x08u)
#define Slider_JITTER_FILTER              (0x10u)
#define Slider_IIR8_FILTER                (0x20u)
#define Slider_IIR16_FILTER               (0x40u)


/***************************************
*           API Constants
***************************************/

/* Widgets constants definition */
#define Slider_LINEARSLIDER__LS        (0u)

#define Slider_TOTAL_DIPLEXED_SLIDERS_COUNT        (0u)
#define Slider_TOTAL_LINEAR_SLIDERS_COUNT          (1u)
#define Slider_TOTAL_RADIAL_SLIDERS_COUNT          (0u)
#define Slider_TOTAL_TOUCH_PADS_COUNT              (0u)
#define Slider_TOTAL_BUTTONS_COUNT                 (0u)
#define Slider_TOTAL_MATRIX_BUTTONS_COUNT          (0u)
#define Slider_TOTAL_GENERICS_COUNT                (0u)

#define Slider_POS_FILTERS_MASK        (8u)
#define Slider_LINEAR_SLIDERS_POS_FILTERS_MASK        (8u)
#define Slider_RADIAL_SLIDERS_POS_FILTERS_MASK        (0u)
#define Slider_TOUCH_PADS_POS_FILTERS_MASK        (0u)

#define Slider_UNUSED_DEBOUNCE_COUNTER_INDEX   (0u)


#define Slider_END_OF_SLIDERS_INDEX   (0u)
#define Slider_END_OF_TOUCH_PAD_INDEX   (0u)
#define Slider_END_OF_BUTTONS_INDEX   (0u)
#define Slider_END_OF_MATRIX_BUTTONS_INDEX   (0u)
#define Slider_END_OF_WIDGETS_INDEX   (1u)


#define Slider_TOTAL_SLIDERS_COUNT            ( Slider_TOTAL_LINEAR_SLIDERS_COUNT + \
                                                          Slider_TOTAL_RADIAL_SLIDERS_COUNT )
                                                          
#define Slider_TOTAL_CENTROIDS_COUNT          ( Slider_TOTAL_SLIDERS_COUNT + \
                                                         (Slider_TOTAL_TOUCH_PADS_COUNT*2u) )

#define Slider_TOTAL_WIDGET_COUNT             ( Slider_TOTAL_LINEAR_SLIDERS_COUNT + \
                                                          Slider_TOTAL_RADIAL_SLIDERS_COUNT + \
                                                          Slider_TOTAL_TOUCH_PADS_COUNT + \
                                                          Slider_TOTAL_BUTTONS_COUNT + \
                                                          Slider_TOTAL_MATRIX_BUTTONS_COUNT )
                                                           
#define Slider_ANY_POS_FILTER                 ( Slider_MEDIAN_FILTER | \
                                                          Slider_AVERAGING_FILTER | \
                                                          Slider_IIR2_FILTER | \
                                                          Slider_IIR4_FILTER | \
                                                          Slider_JITTER_FILTER )
                                                         
#define Slider_IS_DIPLEX_SLIDER               ( Slider_TOTAL_DIPLEXED_SLIDERS_COUNT > 0u)

#define Slider_IS_NON_DIPLEX_SLIDER           ( (Slider_TOTAL_LINEAR_SLIDERS_COUNT - \
                                                           Slider_TOTAL_DIPLEXED_SLIDERS_COUNT) > 0u)
#define Slider_ADD_SLIDER_TYPE                ((Slider_TOTAL_RADIAL_SLIDERS_COUNT > 0u) ? \
                                                        ((Slider_TOTAL_TOUCH_PADS_COUNT > 0u) || \
                                                         (Slider_TOTAL_LINEAR_SLIDERS_COUNT > 0u)) : 0u)
                                                         
#define Slider_WIDGET_CSHL_PARAMETERS_COUNT           (Slider_TOTAL_WIDGET_COUNT + \
                                                                 Slider_TOTAL_TOUCH_PADS_COUNT + \
                                                                 Slider_TOTAL_MATRIX_BUTTONS_COUNT)

#define Slider_WIDGET_RESOLUTION_PARAMETERS_COUNT     (Slider_WIDGET_CSHL_PARAMETERS_COUNT + \
                                                                 Slider_TOTAL_GENERICS_COUNT)
                                                                 
#define Slider_SENSORS_TBL_SIZE (Slider_TOTAL_LINEAR_SLIDERS_COUNT + \
                                        Slider_TOTAL_RADIAL_SLIDERS_COUNT + \
                                        Slider_TOTAL_BUTTONS_COUNT + \
                                        Slider_TOTAL_GENERICS_COUNT + \
                                        (Slider_TOTAL_TOUCH_PADS_COUNT * 2u) + \
                                        (Slider_TOTAL_MATRIX_BUTTONS_COUNT * 2u))
                                        
/*Types of centroids */
#define Slider_TYPE_RADIAL_SLIDER         (0x01u)
#define Slider_TYPE_LINEAR_SLIDER         (0x02u)
#define Slider_TYPE_GENERIC               (0xFFu)

/* Defines is slot active */
#define Slider_SENSOR_1_IS_ACTIVE     (0x01u)
#define Slider_SENSOR_2_IS_ACTIVE     (0x02u)
#define Slider_WIDGET_IS_ACTIVE       (0x01u)

/* Defines diplex type of Slider */
#define Slider_IS_DIPLEX              (0x80u)

/* Defines max fingers on TouchPad  */
#define Slider_POS_PREV               (0u)
#define Slider_POS                    (1u)
#define Slider_POS_NEXT               (2u)
#define Slider_CENTROID_ROUND_VALUE   (0x7F00u)

#define Slider_NEGATIVE_NOISE_THRESHOLD        (20u)
#define Slider_LOW_BASELINE_RESET              (5u)


/***************************************
*        Function Prototypes
***************************************/
void Slider_BaseInit(uint8 sensor) ;
void Slider_InitializeSensorBaseline(uint8 sensor) \
                                               ;
void Slider_InitializeAllBaselines(void) ;
void Slider_InitializeEnabledBaselines(void) \
                                                 ;
void Slider_UpdateSensorBaseline(uint8 sensor) ;
void Slider_UpdateEnabledBaselines(void) ;
uint8 Slider_CheckIsSensorActive(uint8 sensor) ;
uint8 Slider_CheckIsWidgetActive(uint8 widget) ;
uint8 Slider_CheckIsAnyWidgetActive(void) ;
void Slider_EnableWidget(uint8 widget) ;
void Slider_DisableWidget(uint8 widget) ;
#if (Slider_TOTAL_MATRIX_BUTTONS_COUNT)
    uint8 Slider_GetMatrixButtonPos(uint8 widget, uint8* pos) \
	                                          ;
#endif /* (Slider_TOTAL_MATRIX_BUTTONS_COUNT) */

#if (Slider_TOTAL_LINEAR_SLIDERS_COUNT)
    uint16 Slider_GetCentroidPos(uint8 widget) ;
#endif /* (Slider_TOTAL_LINEAR_SLIDERS_COUNT) */
#if (Slider_TOTAL_RADIAL_SLIDERS_COUNT)
    uint16 Slider_GetRadialCentroidPos(uint8 widget) \
                                                 ;
#endif /* (Slider_TOTAL_RADIAL_SLIDERS_COUNT) */
#if (Slider_TOTAL_TOUCH_PADS_COUNT)
    uint8 Slider_GetTouchCentroidPos(uint8 widget, uint16* pos) \
	                                           ;
#endif /* (Slider_TOTAL_TOUCH_PADS_COUNT) */

#if (Slider_IS_DIPLEX_SLIDER)
    uint8 Slider_FindMaximum(uint8 offset, uint8 count, uint8 fingerThreshold, const uint8 CYCODE *diplex)
                                       ;
#else 
    uint8 Slider_FindMaximum(uint8 offset, uint8 count, uint8 fingerThreshold)
                                       ;
#endif /* (Slider_IS_DIPLEX_SLIDER) */

#if(Slider_TOTAL_CENTROIDS_COUNT)
    uint8 Slider_CalcCentroid(uint8 maximum, uint8 offset, 
                                        uint8 count, uint16 resolution, uint8 noiseThreshold)
	                                    ;
#endif /* (Slider_TOTAL_CENTROIDS_COUNT) */

/* SmartSense functions */
#if (Slider_TUNING_METHOD == Slider_AUTO_TUNING)
    extern void Slider_CalculateThresholds(uint8 SensorNumber)
           ;
#endif /* (Slider_TUNING_METHOD == Slider_AUTO_TUNING) */

/* Median filter function prototype */
#if ( (Slider_RAW_FILTER_MASK & Slider_MEDIAN_FILTER) | \
      (Slider_POS_FILTERS_MASK & Slider_MEDIAN_FILTER) )
    uint16 Slider_MedianFilter(uint16 x1, uint16 x2, uint16 x3)
    ;
#endif /* Slider_RAW_FILTER_MASK && Slider_POS_FILTERS_MASK */

/* Averaging filter function prototype */
#if ( (Slider_RAW_FILTER_MASK & Slider_AVERAGING_FILTER) | \
      (Slider_POS_FILTERS_MASK & Slider_AVERAGING_FILTER) )
    uint16 Slider_AveragingFilter(uint16 x1, uint16 x2, uint16 x3)
    ;
#endif /* Slider_RAW_FILTER_MASK && Slider_POS_FILTERS_MASK */

/* IIR2Filter(1/2prev + 1/2cur) filter function prototype */
#if ( (Slider_RAW_FILTER_MASK & Slider_IIR2_FILTER) | \
      (Slider_POS_FILTERS_MASK & Slider_IIR2_FILTER) )
    uint16 Slider_IIR2Filter(uint16 x1, uint16 x2) ;
#endif /* Slider_RAW_FILTER_MASK && Slider_POS_FILTERS_MASK */

/* IIR4Filter(3/4prev + 1/4cur) filter function prototype */
#if ( (Slider_RAW_FILTER_MASK & Slider_IIR4_FILTER) | \
      (Slider_POS_FILTERS_MASK & Slider_IIR4_FILTER) )
    uint16 Slider_IIR4Filter(uint16 x1, uint16 x2) ;
#endif /* Slider_RAW_FILTER_MASK && Slider_POS_FILTERS_MASK */

/* IIR8Filter(7/8prev + 1/8cur) filter function prototype - RawCounts only */
#if (Slider_RAW_FILTER_MASK & Slider_IIR8_FILTER)
    uint16 Slider_IIR8Filter(uint16 x1, uint16 x2) ;
#endif /* Slider_RAW_FILTER_MASK */

/* IIR16Filter(15/16prev + 1/16cur) filter function prototype - RawCounts only */
#if (Slider_RAW_FILTER_MASK & Slider_IIR16_FILTER)
    uint16 Slider_IIR16Filter(uint16 x1, uint16 x2) ;
#endif /* Slider_RAW_FILTER_MASK */

/* JitterFilter filter function prototype */
#if ( (Slider_RAW_FILTER_MASK & Slider_JITTER_FILTER) | \
      (Slider_POS_FILTERS_MASK & Slider_JITTER_FILTER) )
    uint16 Slider_JitterFilter(uint16 x1, uint16 x2) ;
#endif /* Slider_RAW_FILTER_MASK && Slider_POS_FILTERS_MASK */

/* Storage of filters data */
#if ( (Slider_RAW_FILTER_MASK & Slider_MEDIAN_FILTER) | \
      (Slider_RAW_FILTER_MASK & Slider_AVERAGING_FILTER) )

    extern uint16 Slider_rawFilterData1[Slider_TOTAL_SENSOR_COUNT];
    extern uint16 Slider_rawFilterData2[Slider_TOTAL_SENSOR_COUNT];

#elif ( (Slider_RAW_FILTER_MASK & Slider_IIR2_FILTER)   | \
        (Slider_RAW_FILTER_MASK & Slider_IIR4_FILTER)   | \
        (Slider_RAW_FILTER_MASK & Slider_JITTER_FILTER) | \
        (Slider_RAW_FILTER_MASK & Slider_IIR8_FILTER)   | \
        (Slider_RAW_FILTER_MASK & Slider_IIR16_FILTER) )
        
    extern uint16 Slider_rawFilterData1[Slider_TOTAL_SENSOR_COUNT];

#else
    /* No Raw filters */
#endif  /* ( (Slider_RAW_FILTER_MASK & Slider_MEDIAN_FILTER) || \
        *    (Slider_RAW_FILTER_MASK & Slider_AVERAGING_FILTER) )
        */

extern uint8 Slider_sensorEnableMask[(((Slider_TOTAL_SENSOR_COUNT - 1u) / 8u) + 1u)];
extern const uint8 CYCODE Slider_widgetNumber[Slider_TOTAL_SENSOR_COUNT];

extern uint16 Slider_sensorBaseline[Slider_TOTAL_SENSOR_COUNT];
extern uint8 Slider_sensorBaselineLow[Slider_TOTAL_SENSOR_COUNT];
extern uint8 Slider_sensorSignal[Slider_TOTAL_SENSOR_COUNT];
extern uint8 Slider_sensorOnMask[(((Slider_TOTAL_SENSOR_COUNT - 1u) / 8u) + 1u)];

extern uint8 Slider_lowBaselineResetCnt[Slider_TOTAL_SENSOR_COUNT];

/****************************************************************************************
*       Obsolete definitions. Not recommended to use. Will be removed in future releases.
*****************************************************************************************/

/* Obsolete names of variables */
//#define Slider_SensorBaseline          Slider_sensorBaseline
//#define Slider_SensorBaselineLow       Slider_sensorBaselineLow
//#define Slider_SensorSignal            Slider_sensorSignal
//#define Slider_SensorOnMask            Slider_sensorOnMask
//#define Slider_LowBaselineResetCnt     Slider_lowBaselineResetCnt


#endif /* CY_CAPSENSE_CSD_CSHL_Slider_H */

/* [] END OF FILE */
