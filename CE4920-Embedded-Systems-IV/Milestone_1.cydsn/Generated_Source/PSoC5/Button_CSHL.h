/*******************************************************************************
* File Name: Button_CSHL.h
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

#if !defined(CY_CAPSENSE_CSD_CSHL_Button_H)
#define CY_CAPSENSE_CSD_CSHL_Button_H

#include "Button.h"


/***************************************
*   Condition compilation parameters
***************************************/

#define Button_SIGNAL_SIZE                (8u)
#define Button_AUTO_RESET                 (0u)
#define Button_RAW_FILTER_MASK            (8u)

/* Signal size definition */
#define Button_SIGNAL_SIZE_UINT8          (8u)
#define Button_SIGNAL_SIZE_UINT16         (16u)

/* Auto reset definition */
#define Button_AUTO_RESET_DISABLE         (0u)
#define Button_AUTO_RESET_ENABLE          (1u)

/* Mask for RAW and POS filters */
#define Button_MEDIAN_FILTER              (0x01u)
#define Button_AVERAGING_FILTER           (0x02u)
#define Button_IIR2_FILTER                (0x04u)
#define Button_IIR4_FILTER                (0x08u)
#define Button_JITTER_FILTER              (0x10u)
#define Button_IIR8_FILTER                (0x20u)
#define Button_IIR16_FILTER               (0x40u)


/***************************************
*           API Constants
***************************************/

/* Widgets constants definition */
#define Button_LEFT__BTN        (0u)
#define Button_RIGHT__BTN        (1u)

#define Button_TOTAL_DIPLEXED_SLIDERS_COUNT        (0u)
#define Button_TOTAL_LINEAR_SLIDERS_COUNT          (0u)
#define Button_TOTAL_RADIAL_SLIDERS_COUNT          (0u)
#define Button_TOTAL_TOUCH_PADS_COUNT              (0u)
#define Button_TOTAL_BUTTONS_COUNT                 (2u)
#define Button_TOTAL_MATRIX_BUTTONS_COUNT          (0u)
#define Button_TOTAL_GENERICS_COUNT                (0u)

#define Button_POS_FILTERS_MASK        (0u)
#define Button_LINEAR_SLIDERS_POS_FILTERS_MASK        (0u)
#define Button_RADIAL_SLIDERS_POS_FILTERS_MASK        (0u)
#define Button_TOUCH_PADS_POS_FILTERS_MASK        (0u)

#define Button_UNUSED_DEBOUNCE_COUNTER_INDEX   (2u)


#define Button_END_OF_SLIDERS_INDEX   (0u)
#define Button_END_OF_TOUCH_PAD_INDEX   (0u)
#define Button_END_OF_BUTTONS_INDEX   (1u)
#define Button_END_OF_MATRIX_BUTTONS_INDEX   (1u)
#define Button_END_OF_WIDGETS_INDEX   (2u)


#define Button_TOTAL_SLIDERS_COUNT            ( Button_TOTAL_LINEAR_SLIDERS_COUNT + \
                                                          Button_TOTAL_RADIAL_SLIDERS_COUNT )
                                                          
#define Button_TOTAL_CENTROIDS_COUNT          ( Button_TOTAL_SLIDERS_COUNT + \
                                                         (Button_TOTAL_TOUCH_PADS_COUNT*2u) )

#define Button_TOTAL_WIDGET_COUNT             ( Button_TOTAL_LINEAR_SLIDERS_COUNT + \
                                                          Button_TOTAL_RADIAL_SLIDERS_COUNT + \
                                                          Button_TOTAL_TOUCH_PADS_COUNT + \
                                                          Button_TOTAL_BUTTONS_COUNT + \
                                                          Button_TOTAL_MATRIX_BUTTONS_COUNT )
                                                           
#define Button_ANY_POS_FILTER                 ( Button_MEDIAN_FILTER | \
                                                          Button_AVERAGING_FILTER | \
                                                          Button_IIR2_FILTER | \
                                                          Button_IIR4_FILTER | \
                                                          Button_JITTER_FILTER )
                                                         
#define Button_IS_DIPLEX_SLIDER               ( Button_TOTAL_DIPLEXED_SLIDERS_COUNT > 0u)

#define Button_IS_NON_DIPLEX_SLIDER           ( (Button_TOTAL_LINEAR_SLIDERS_COUNT - \
                                                           Button_TOTAL_DIPLEXED_SLIDERS_COUNT) > 0u)
#define Button_ADD_SLIDER_TYPE                ((Button_TOTAL_RADIAL_SLIDERS_COUNT > 0u) ? \
                                                        ((Button_TOTAL_TOUCH_PADS_COUNT > 0u) || \
                                                         (Button_TOTAL_LINEAR_SLIDERS_COUNT > 0u)) : 0u)
                                                         
#define Button_WIDGET_CSHL_PARAMETERS_COUNT           (Button_TOTAL_WIDGET_COUNT + \
                                                                 Button_TOTAL_TOUCH_PADS_COUNT + \
                                                                 Button_TOTAL_MATRIX_BUTTONS_COUNT)

#define Button_WIDGET_RESOLUTION_PARAMETERS_COUNT     (Button_WIDGET_CSHL_PARAMETERS_COUNT + \
                                                                 Button_TOTAL_GENERICS_COUNT)
                                                                 
#define Button_SENSORS_TBL_SIZE (Button_TOTAL_LINEAR_SLIDERS_COUNT + \
                                        Button_TOTAL_RADIAL_SLIDERS_COUNT + \
                                        Button_TOTAL_BUTTONS_COUNT + \
                                        Button_TOTAL_GENERICS_COUNT + \
                                        (Button_TOTAL_TOUCH_PADS_COUNT * 2u) + \
                                        (Button_TOTAL_MATRIX_BUTTONS_COUNT * 2u))
                                        
/*Types of centroids */
#define Button_TYPE_RADIAL_SLIDER         (0x01u)
#define Button_TYPE_LINEAR_SLIDER         (0x02u)
#define Button_TYPE_GENERIC               (0xFFu)

/* Defines is slot active */
#define Button_SENSOR_1_IS_ACTIVE     (0x01u)
#define Button_SENSOR_2_IS_ACTIVE     (0x02u)
#define Button_WIDGET_IS_ACTIVE       (0x01u)

/* Defines diplex type of Slider */
#define Button_IS_DIPLEX              (0x80u)

/* Defines max fingers on TouchPad  */
#define Button_POS_PREV               (0u)
#define Button_POS                    (1u)
#define Button_POS_NEXT               (2u)
#define Button_CENTROID_ROUND_VALUE   (0x7F00u)

#define Button_NEGATIVE_NOISE_THRESHOLD        (20u)
#define Button_LOW_BASELINE_RESET              (5u)


/***************************************
*        Function Prototypes
***************************************/
void Button_BaseInit(uint8 sensor) ;
void Button_InitializeSensorBaseline(uint8 sensor) \
                                               ;
void Button_InitializeAllBaselines(void) ;
void Button_InitializeEnabledBaselines(void) \
                                                 ;
void Button_UpdateSensorBaseline(uint8 sensor) ;
void Button_UpdateEnabledBaselines(void) ;
uint8 Button_CheckIsSensorActive(uint8 sensor) ;
uint8 Button_CheckIsWidgetActive(uint8 widget) ;
uint8 Button_CheckIsAnyWidgetActive(void) ;
void Button_EnableWidget(uint8 widget) ;
void Button_DisableWidget(uint8 widget) ;
#if (Button_TOTAL_MATRIX_BUTTONS_COUNT)
    uint8 Button_GetMatrixButtonPos(uint8 widget, uint8* pos) \
	                                          ;
#endif /* (Button_TOTAL_MATRIX_BUTTONS_COUNT) */

#if (Button_TOTAL_LINEAR_SLIDERS_COUNT)
    uint16 Button_GetCentroidPos(uint8 widget) ;
#endif /* (Button_TOTAL_LINEAR_SLIDERS_COUNT) */
#if (Button_TOTAL_RADIAL_SLIDERS_COUNT)
    uint16 Button_GetRadialCentroidPos(uint8 widget) \
                                                 ;
#endif /* (Button_TOTAL_RADIAL_SLIDERS_COUNT) */
#if (Button_TOTAL_TOUCH_PADS_COUNT)
    uint8 Button_GetTouchCentroidPos(uint8 widget, uint16* pos) \
	                                           ;
#endif /* (Button_TOTAL_TOUCH_PADS_COUNT) */

#if (Button_IS_DIPLEX_SLIDER)
    uint8 Button_FindMaximum(uint8 offset, uint8 count, uint8 fingerThreshold, const uint8 CYCODE *diplex)
                                       ;
#else 
    uint8 Button_FindMaximum(uint8 offset, uint8 count, uint8 fingerThreshold)
                                       ;
#endif /* (Button_IS_DIPLEX_SLIDER) */

#if(Button_TOTAL_CENTROIDS_COUNT)
    uint8 Button_CalcCentroid(uint8 maximum, uint8 offset, 
                                        uint8 count, uint16 resolution, uint8 noiseThreshold)
	                                    ;
#endif /* (Button_TOTAL_CENTROIDS_COUNT) */

/* SmartSense functions */
#if (Button_TUNING_METHOD == Button_AUTO_TUNING)
    extern void Button_CalculateThresholds(uint8 SensorNumber)
           ;
#endif /* (Button_TUNING_METHOD == Button_AUTO_TUNING) */

/* Median filter function prototype */
#if ( (Button_RAW_FILTER_MASK & Button_MEDIAN_FILTER) | \
      (Button_POS_FILTERS_MASK & Button_MEDIAN_FILTER) )
    uint16 Button_MedianFilter(uint16 x1, uint16 x2, uint16 x3)
    ;
#endif /* Button_RAW_FILTER_MASK && Button_POS_FILTERS_MASK */

/* Averaging filter function prototype */
#if ( (Button_RAW_FILTER_MASK & Button_AVERAGING_FILTER) | \
      (Button_POS_FILTERS_MASK & Button_AVERAGING_FILTER) )
    uint16 Button_AveragingFilter(uint16 x1, uint16 x2, uint16 x3)
    ;
#endif /* Button_RAW_FILTER_MASK && Button_POS_FILTERS_MASK */

/* IIR2Filter(1/2prev + 1/2cur) filter function prototype */
#if ( (Button_RAW_FILTER_MASK & Button_IIR2_FILTER) | \
      (Button_POS_FILTERS_MASK & Button_IIR2_FILTER) )
    uint16 Button_IIR2Filter(uint16 x1, uint16 x2) ;
#endif /* Button_RAW_FILTER_MASK && Button_POS_FILTERS_MASK */

/* IIR4Filter(3/4prev + 1/4cur) filter function prototype */
#if ( (Button_RAW_FILTER_MASK & Button_IIR4_FILTER) | \
      (Button_POS_FILTERS_MASK & Button_IIR4_FILTER) )
    uint16 Button_IIR4Filter(uint16 x1, uint16 x2) ;
#endif /* Button_RAW_FILTER_MASK && Button_POS_FILTERS_MASK */

/* IIR8Filter(7/8prev + 1/8cur) filter function prototype - RawCounts only */
#if (Button_RAW_FILTER_MASK & Button_IIR8_FILTER)
    uint16 Button_IIR8Filter(uint16 x1, uint16 x2) ;
#endif /* Button_RAW_FILTER_MASK */

/* IIR16Filter(15/16prev + 1/16cur) filter function prototype - RawCounts only */
#if (Button_RAW_FILTER_MASK & Button_IIR16_FILTER)
    uint16 Button_IIR16Filter(uint16 x1, uint16 x2) ;
#endif /* Button_RAW_FILTER_MASK */

/* JitterFilter filter function prototype */
#if ( (Button_RAW_FILTER_MASK & Button_JITTER_FILTER) | \
      (Button_POS_FILTERS_MASK & Button_JITTER_FILTER) )
    uint16 Button_JitterFilter(uint16 x1, uint16 x2) ;
#endif /* Button_RAW_FILTER_MASK && Button_POS_FILTERS_MASK */

/* Storage of filters data */
#if ( (Button_RAW_FILTER_MASK & Button_MEDIAN_FILTER) | \
      (Button_RAW_FILTER_MASK & Button_AVERAGING_FILTER) )

    extern uint16 Button_rawFilterData1[Button_TOTAL_SENSOR_COUNT];
    extern uint16 Button_rawFilterData2[Button_TOTAL_SENSOR_COUNT];

#elif ( (Button_RAW_FILTER_MASK & Button_IIR2_FILTER)   | \
        (Button_RAW_FILTER_MASK & Button_IIR4_FILTER)   | \
        (Button_RAW_FILTER_MASK & Button_JITTER_FILTER) | \
        (Button_RAW_FILTER_MASK & Button_IIR8_FILTER)   | \
        (Button_RAW_FILTER_MASK & Button_IIR16_FILTER) )
        
    extern uint16 Button_rawFilterData1[Button_TOTAL_SENSOR_COUNT];

#else
    /* No Raw filters */
#endif  /* ( (Button_RAW_FILTER_MASK & Button_MEDIAN_FILTER) || \
        *    (Button_RAW_FILTER_MASK & Button_AVERAGING_FILTER) )
        */

extern uint8 Button_sensorEnableMask[(((Button_TOTAL_SENSOR_COUNT - 1u) / 8u) + 1u)];
extern const uint8 CYCODE Button_widgetNumber[Button_TOTAL_SENSOR_COUNT];

extern uint16 Button_sensorBaseline[Button_TOTAL_SENSOR_COUNT];
extern uint8 Button_sensorBaselineLow[Button_TOTAL_SENSOR_COUNT];
extern uint8 Button_sensorSignal[Button_TOTAL_SENSOR_COUNT];
extern uint8 Button_sensorOnMask[(((Button_TOTAL_SENSOR_COUNT - 1u) / 8u) + 1u)];

extern uint8 Button_lowBaselineResetCnt[Button_TOTAL_SENSOR_COUNT];

/****************************************************************************************
*       Obsolete definitions. Not recommended to use. Will be removed in future releases.
*****************************************************************************************/

/* Obsolete names of variables */
//#define Button_SensorBaseline          Button_sensorBaseline
//#define Button_SensorBaselineLow       Button_sensorBaselineLow
//#define Button_SensorSignal            Button_sensorSignal
//#define Button_SensorOnMask            Button_sensorOnMask
//#define Button_LowBaselineResetCnt     Button_lowBaselineResetCnt


#endif /* CY_CAPSENSE_CSD_CSHL_Button_H */

/* [] END OF FILE */
