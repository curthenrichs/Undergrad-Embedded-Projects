/*******************************************************************************
* File Name: Button_CSHL.c
* Version 3.50
*
* Description:
*  This file provides the source code to the High Level APIs for the CapSesne
*  CSD component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "Button_CSHL.h"

/* Storage of filters data */
#if ( (Button_RAW_FILTER_MASK & Button_MEDIAN_FILTER) | \
      (Button_RAW_FILTER_MASK & Button_AVERAGING_FILTER) )

    uint16 Button_rawFilterData1[Button_TOTAL_SENSOR_COUNT];
    uint16 Button_rawFilterData2[Button_TOTAL_SENSOR_COUNT];

#elif ( (Button_RAW_FILTER_MASK & Button_IIR2_FILTER)   | \
        (Button_RAW_FILTER_MASK & Button_IIR4_FILTER)   | \
        (Button_RAW_FILTER_MASK & Button_JITTER_FILTER) | \
        (Button_RAW_FILTER_MASK & Button_IIR8_FILTER)   | \
        (Button_RAW_FILTER_MASK & Button_IIR16_FILTER) )
        
    uint16 Button_rawFilterData1[Button_TOTAL_SENSOR_COUNT];

#else
    /* No Raw filters */
#endif  /* ( (Button_RAW_FILTER_MASK & Button_MEDIAN_FILTER) || \
        *    (Button_RAW_FILTER_MASK & Button_AVERAGING_FILTER) )
        */

uint16 Button_sensorBaseline[Button_TOTAL_SENSOR_COUNT];
uint8 Button_sensorBaselineLow[Button_TOTAL_SENSOR_COUNT];
uint8 Button_sensorSignal[Button_TOTAL_SENSOR_COUNT];
uint8 Button_sensorOnMask[(((Button_TOTAL_SENSOR_COUNT - 1u) / 8u) + 1u)];

uint8 Button_lowBaselineResetCnt[Button_TOTAL_SENSOR_COUNT];

uint8 Button_fingerThreshold[] = {
    100u, 100u, 
};

uint8 Button_noiseThreshold[] = {
    20u, 20u, 
};

uint8 Button_hysteresis[] = {
    10u, 10u, 
};

uint8 Button_debounce[] = {
    1u, 1u, 
};

uint8 Button_debounceCounter[] = {
    0u, 0u, 0u, 
};

const uint8 CYCODE Button_rawDataIndex[] = {
    0u, /* Left__BTN */
    1u, /* Right__BTN */

};

const uint8 CYCODE Button_numberOfSensors[] = {
    1u, /* Left__BTN */
    1u, /* Right__BTN */

};




/*******************************************************************************
* Function Name: Button_BaseInit
********************************************************************************
*
* Summary:
*  Loads the Button_sensorBaseline[sensor] array element with an 
*  initial value which is equal to raw count value. 
*  Resets to zero Button_sensorBaselineLow[sensor] and 
*  Button_sensorSignal[sensor] array element.
*  Loads Button_debounceCounter[sensor] array element with initial 
*  value equal Button_debounce[].
*  Loads the Button_rawFilterData2[sensor] and 
*  Button_rawFilterData2[sensor] array element with an 
*  initial value which is equal raw count value if raw data filter is enabled.
*
* Parameters:
*  sensor:  Sensor number.
*
* Return:
*  None
*
* Global Variables:
*  Button_sensorBaseline[]    - used to store baseline value.
*  Button_sensorBaselineLow[] - used to store fraction byte of 
*  baseline value.
*  Button_sensorSignal[]      - used to store diffence between 
*  current value of raw data and previous value of baseline.
*  Button_debounceCounter[]   - used to store current debounce 
*  counter of sensor. Widget which has this parameter are buttons, matrix 
*  buttons, proximity, guard. All other widgets haven't debounce parameter
*  and use the last element of this array with value 0 (it means no debounce).
*  Button_rawFilterData1[]    - used to store previous sample of 
*  any enabled raw data filter.
*  Button_rawFilterData2[]    - used to store before previous sample
*  of enabled raw data filter. Only required for median or average filters.
*
* Reentrant:
*  No
*
*******************************************************************************/
void Button_BaseInit(uint8 sensor) 
{
    #if ((Button_TOTAL_BUTTONS_COUNT) || (Button_TOTAL_MATRIX_BUTTONS_COUNT) || \
         (Button_TOTAL_GENERICS_COUNT))
        uint8 widget = Button_widgetNumber[sensor];
    #endif /* ((Button_TOTAL_MATRIX_BUTTONS_COUNT) || (Button_TOTAL_MATRIX_BUTTONS_COUNT)) */
    
    #if (Button_TOTAL_MATRIX_BUTTONS_COUNT)
        uint8 debounceIndex;
    #endif  /* (Button_TOTAL_MATRIX_BUTTONS_COUNT) */
    
    #if (Button_TOTAL_GENERICS_COUNT)
        /* Exclude generic widget */
        if(widget < Button_END_OF_WIDGETS_INDEX)
        {
    #endif  /* Button_TOTAL_GENERICS_COUNT */
    
    /* Initialize Baseline */
    Button_sensorBaseline[sensor] = Button_sensorRaw[sensor];
    Button_sensorBaselineLow[sensor] = 0u;
    Button_sensorSignal[sensor] = 0u;
        
    Button_debounceCounter[widget] =  Button_debounce[widget];

    
    #if ((Button_RAW_FILTER_MASK & Button_MEDIAN_FILTER) |\
         (Button_RAW_FILTER_MASK & Button_AVERAGING_FILTER))

        Button_rawFilterData1[sensor] = Button_sensorRaw[sensor];
        Button_rawFilterData2[sensor] = Button_sensorRaw[sensor];
    
    #elif ((Button_RAW_FILTER_MASK & Button_IIR2_FILTER) |\
           (Button_RAW_FILTER_MASK & Button_IIR4_FILTER) |\
           (Button_RAW_FILTER_MASK & Button_JITTER_FILTER) |\
           (Button_RAW_FILTER_MASK & Button_IIR8_FILTER) |\
           (Button_RAW_FILTER_MASK & Button_IIR16_FILTER))
            
        Button_rawFilterData1[sensor] = Button_sensorRaw[sensor];
    
    #else
        /* No Raw filters */
    #endif  /* ((Button_RAW_FILTER_MASK & Button_MEDIAN_FILTER) | \
            *   (Button_RAW_FILTER_MASK & Button_AVERAGING_FILTER))
            */
    
    #if (Button_TOTAL_GENERICS_COUNT)
        /* Exclude generic widget */
        }
    #endif  /* Button_TOTAL_GENERICS_COUNT */
}


/*******************************************************************************
* Function Name: Button_InitializeSensorBaseline
********************************************************************************
*
* Summary:
*  Loads the Button_sensorBaseline[sensor] array element with an 
*  initial value by scanning the selected sensor (one channel design) or pair 
*  of sensors (two channels designs). The raw count value is copied into the 
*  baseline array for each sensor. The raw data filters are initialized if 
*  enabled.
*
* Parameters:
*  sensor:  Sensor number.
*
* Return:
*  None
*
* Reentrant:
*  No
*
*******************************************************************************/
void Button_InitializeSensorBaseline(uint8 sensor)
                          
{
    /* Scan sensor */
    Button_ScanSensor(sensor);
    while(Button_IsBusy() != 0u)
    {
        /* Wait while busy */
    }
    
    #if (Button_DESIGN_TYPE == Button_ONE_CHANNEL_DESIGN)
        /* Initialize Baseline, Signal and debounce counters */       
        Button_BaseInit(sensor);
        
    #else
    
        if(sensor < Button_TOTAL_SENSOR_COUNT__CH0)
        {
            /* Initialize Baseline, Signal and debounce counters */ 
            Button_BaseInit(sensor);
        }
        
        if(sensor < Button_TOTAL_SENSOR_COUNT__CH1)
        {
            /* Initialize Baseline, Signal and debounce counters */
            Button_BaseInit(sensor + Button_TOTAL_SENSOR_COUNT__CH0);
        }
    
    #endif  /* (Button_DESIGN_TYPE == Button_ONE_CHANNEL_DESIGN) */
}


/*******************************************************************************
* Function Name: Button_InitializeAllBaselines
********************************************************************************
*
* Summary:
*  Uses the Button_InitializeSensorBaseline function to loads the 
*  Button_sensorBaseline[] array with an initial values by scanning 
*  all sensors. The raw count values are copied into the baseline array for 
*  all sensors. The raw data filters are initialized if enabled.
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
void Button_InitializeAllBaselines(void)
                          
{
    uint8 i;
    
    /* The baseline initialize by sensor of sensor pair */
    for(i = 0u; i < Button_TOTAL_SCANSLOT_COUNT; i++)
    {
        Button_InitializeSensorBaseline(i);
    }
}


/*******************************************************************************
* Function Name: Button_InitializeEnabledBaselines
********************************************************************************
*
* Summary:
*  Scans all enabled widgets and the raw count values are copied into the 
*  baseline array for all sensors enabled in scanning process. Baselines 
*  initialize with zero values for sensors disabled from scanning process. 
*  The raw data filters are initialized if enabled.
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
void Button_InitializeEnabledBaselines(void)
                             
{
    uint8 i;
    uint8 pos;
    uint8 enMask;
    
    Button_ScanEnabledWidgets();
    while(Button_IsBusy() != 0u)
    {
        /* Wait while busy */
    }
    
    for(i = 0u; i < Button_TOTAL_SENSOR_COUNT; i++)
    {
        pos = (i >> 3u);
        enMask = 0x01u << (i & 0x07u);
        
        /* Clear raw data if sensor is disabled from scanning process */
        if((Button_sensorEnableMask[pos] & enMask) == 0u)
        {
            Button_sensorRaw[i] = 0u;
        }
        
        /* Initialize baselines */
        Button_BaseInit(i);
    }
}  


/*******************************************************************************
* Function Name: Button_UpdateSensorBaseline
********************************************************************************
*
* Summary:
*  Updates the Button_sensorBaseline[sensor] array element using the 
*  LP filter with k = 256. The signal calculates the difference of count by 
*  subtracting the previous baseline from the current raw count value and stores
*  it in Button_SensorSignal[sensor]. 
*  If auto reset option is enabled the baseline updated regards noise threshold. 
*  If auto reset option is disabled the baseline stops updating if signal is 
*  greater that zero and baseline loads with raw count value if signal is less 
*  that noise threshold.
*  Raw data filters are applied to the values if enabled before baseline 
*  calculation.
*
* Parameters:
*  sensor:  Sensor number.
*
* Return:
*  None
*
* Global Variables:
*  Button_sensorBaseline[]    - used to store baseline value.
*  Button_sensorBaselineLow[] - used to store fraction byte of 
*  baseline value.
*  Button_sensorSignal[]      - used to store diffence between 
*  current value of raw data and previous value of baseline.
*  Button_rawFilterData1[]    - used to store previous sample of 
*  any enabled raw data filter.
*  Button_rawFilterData2[]    - used to store before previous sample
*  of enabled raw data filter. Only required for median or average filters.
*
* Reentrant:
*  No
*
*******************************************************************************/
 void Button_UpdateSensorBaseline(uint8 sensor)
                                 
{
    uint32 calc;
    uint16 tempRaw;
    uint16 filteredRawData;
    uint8 widget = Button_widgetNumber[sensor];
    uint8 noiseThreshold = Button_noiseThreshold[widget];
    
    #if (Button_TOTAL_GENERICS_COUNT)
        /* Exclude generic widget */
        if(widget < Button_END_OF_WIDGETS_INDEX)
        {
    #endif  /* Button_TOTAL_GENERICS_COUNT */
    
    filteredRawData = Button_sensorRaw[sensor];
    
    #if (Button_RAW_FILTER_MASK & Button_MEDIAN_FILTER)
        tempRaw = filteredRawData;
        filteredRawData = Button_MedianFilter(filteredRawData, Button_rawFilterData1[sensor], 
                                                        Button_rawFilterData2[sensor]);
        Button_rawFilterData2[sensor] = Button_rawFilterData1[sensor];
        Button_rawFilterData1[sensor] = tempRaw;
        
    #elif (Button_RAW_FILTER_MASK & Button_AVERAGING_FILTER)
        tempRaw = filteredRawData;
        filteredRawData = Button_AveragingFilter(filteredRawData, Button_rawFilterData1[sensor],
                                                           Button_rawFilterData2[sensor]);
        Button_rawFilterData2[sensor] = Button_rawFilterData1[sensor];
        Button_rawFilterData1[sensor] = tempRaw;
        
    #elif (Button_RAW_FILTER_MASK & Button_IIR2_FILTER)
        filteredRawData = Button_IIR2Filter(filteredRawData, Button_rawFilterData1[sensor]);
        Button_rawFilterData1[sensor] = filteredRawData;
        
    #elif (Button_RAW_FILTER_MASK & Button_IIR4_FILTER)
        filteredRawData = Button_IIR4Filter(filteredRawData, Button_rawFilterData1[sensor]);
        Button_rawFilterData1[sensor] = filteredRawData;
            
    #elif (Button_RAW_FILTER_MASK & Button_JITTER_FILTER)
        filteredRawData = Button_JitterFilter(filteredRawData, Button_rawFilterData1[sensor]);
        Button_rawFilterData1[sensor] = filteredRawData;
        
    #elif (Button_RAW_FILTER_MASK & Button_IIR8_FILTER)
        filteredRawData = Button_IIR8Filter(filteredRawData, Button_rawFilterData1[sensor]);
        Button_rawFilterData1[sensor] = filteredRawData;
        
    #elif (Button_RAW_FILTER_MASK & Button_IIR16_FILTER)
        filteredRawData = Button_IIR16Filter(filteredRawData, Button_rawFilterData1[sensor]);
        Button_rawFilterData1[sensor] = filteredRawData;
        
    #else
        /* No Raw filters */
    #endif  /* (Button_RAW_FILTER_MASK & Button_MEDIAN_FILTER) */
    
    #if (Button_TUNING_METHOD == Button_AUTO_TUNING)
        Button_CalculateThresholds(sensor);
    #endif /* (Button_TUNING_METHOD == Button_AUTO_TUNING) */


    /* Baseline calculation */
    /* Calculate difference RawData[cur] - Baseline[prev] */
    if(filteredRawData >= Button_sensorBaseline[sensor])
    {
        tempRaw = filteredRawData - Button_sensorBaseline[sensor];
        widget = 1u;    /* Positive difference - Calculate the Signal */
    }
    else
    {
        tempRaw = Button_sensorBaseline[sensor] - filteredRawData;
        widget = 0u;    /* Negative difference - Do NOT calculate the Signal */
    }

    if((widget == 0u) && (tempRaw > (uint16) Button_NEGATIVE_NOISE_THRESHOLD))
    {
        if(Button_lowBaselineResetCnt[sensor] >= Button_LOW_BASELINE_RESET)
        {
            Button_BaseInit(sensor);
            Button_lowBaselineResetCnt[sensor] = 0u;
        }
        else
        {
            Button_lowBaselineResetCnt[sensor]++;
        }
    }
    else
    {
        #if (Button_AUTO_RESET == Button_AUTO_RESET_DISABLE)
            /* Update Baseline if lower that noiseThreshold */
            if ( (tempRaw <= (uint16) noiseThreshold) || 
                 ((tempRaw < (uint16) Button_NEGATIVE_NOISE_THRESHOLD)
                   && (widget == 0u)))
            {
        #endif /* (Button_AUTO_RESET == Button_AUTO_RESET_DISABLE) */
                /* Make full Baseline 23 bits */
                calc = (uint32) Button_sensorBaseline[sensor] << 8u;
                calc |= (uint32) Button_sensorBaselineLow[sensor];

                /* Add Raw Data to Baseline */
                calc += filteredRawData;

                /* Sub the high Baseline */
                calc -= Button_sensorBaseline[sensor];

                /* Put Baseline and BaselineLow */
                Button_sensorBaseline[sensor] = ((uint16) (calc >> 8u));
                Button_sensorBaselineLow[sensor] = ((uint8) calc);

                Button_lowBaselineResetCnt[sensor] = 0u;
        #if (Button_AUTO_RESET == Button_AUTO_RESET_DISABLE)
            }
        #endif /* (Button_AUTO_RESET == Button_AUTO_RESET_DISABLE) */
    }

    /* Calculate Signal if possitive difference > noiseThreshold */
    if((tempRaw > (uint16) noiseThreshold) && (widget != 0u))
    {
        #if (Button_SIGNAL_SIZE == Button_SIGNAL_SIZE_UINT8)
            /* Over flow defence for uint8 */
            if (tempRaw > 0xFFu)
            {
                Button_sensorSignal[sensor] = 0xFFu;
            }    
            else 
            {
                Button_sensorSignal[sensor] = ((uint8) tempRaw);
            }
        #else
            Button_sensorSignal[sensor] = ((uint16) tempRaw);
        #endif  /* (Button_SIGNAL_SIZE == Button_SIGNAL_SIZE_UINT8) */
    }
    else
    {
        /* Signal is zero */
        Button_sensorSignal[sensor] = 0u;
    }

    #if (Button_TOTAL_GENERICS_COUNT)
        /* Exclude generic widget */
        }
    #endif  /* Button_TOTAL_GENERICS_COUNT */
}


/*******************************************************************************
* Function Name: Button_UpdateEnabledBaselines
********************************************************************************
*
* Summary:
*  Checks Button_sensorEnableMask[] array and calls the 
*  Button_UpdateSensorBaseline function to update the baselines 
*  for enabled sensors.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  Button_sensorEnableMask[] - used to store the sensor scanning 
*  state.
*  Button_sensorEnableMask[0] contains the masked bits for sensors 
*   0 through 7 (sensor 0 is bit 0, sensor 1 is bit 1).
*  Button_sensorEnableMask[1] contains the masked bits for 
*  sensors 8 through 15 (if needed), and so on.
*  0 - sensor doesn't scan by Button_ScanEnabledWidgets().
*  1 - sensor scans by Button_ScanEnabledWidgets().
*
* Reentrant:
*  No
*
*******************************************************************************/
 void Button_UpdateEnabledBaselines(void)
                                 
{
    uint8 i;
    uint8 pos;
    uint8 enMask;
    
    for(i = 0u; i < Button_TOTAL_SENSOR_COUNT; i++)
    {
        pos = (i >> 3u);
        enMask = (uint8)(0x01u << (i & 0x07u));
        if((Button_sensorEnableMask[pos] & enMask) != 0u)
        {
            Button_UpdateSensorBaseline(i);
        }
    }
}


/*******************************************************************************
* Function Name: Button_CheckIsSensorActive
********************************************************************************
*
* Summary:
*  Compares the Button_SensorSignal[sensor] array element to finger
*  threshold of widget it belongs to. The hysteresis and debounce are taken into 
*  account. The hysteresis is added or subtracted from the finger threshold 
*  based on whether the sensor is currently active. 
*  If the sensor is active, the threshold is lowered by the hysteresis amount.
*  If the sensor is inactive, the threshold is raised by the hysteresis amount.
*  The debounce counter added to the sensor active transition.
*  This function updates Button_SensorOnMask[] array element.
*
* Parameters:
*  sensor:  Sensor number.
*
* Return:
*  Returns sensor state 1 if active, 0 if not active.
*
* Global Variables:
*  Button_sensorSignal[]      - used to store diffence between 
*  current value of raw data and previous value of baseline.
*  Button_debounceCounter[]   - used to store current debounce 
*  counter of sensor. Widget which has this parameter are buttons, matrix 
*  buttons, proximity, guard. All other widgets haven't debounce parameter
*  and use the last element of this array with value 0 (it means no debounce).
*  Button_sensorOnMask[] - used to store sensors on/off state.
*  Button_sensorOnMask[0] contains the masked bits for sensors 
*   0 through 7 (sensor 0 is bit 0, sensor 1 is bit 1).
*  Button_sensorEnableMask[1] contains the masked bits for 
*  sensors 8 through 15 (if needed), and so on.
*  0 - sensor is inactive.
*  1 - sensor is active.
*
* Reentrant:
*  No
*
*******************************************************************************/
uint8 Button_CheckIsSensorActive(uint8 sensor)
                                  
{
    uint8 debounceIndex;
    /* Get On/Off mask */
    uint8 pos = (sensor >> 3u);
    uint8 onMask = 0x01u << (sensor & 0x07u);
    /* Prepare to find debounce counter index */
    uint8 widget = Button_widgetNumber[sensor];
    uint8 fingerThreshold = Button_fingerThreshold[widget];
    uint8 hysteresis = Button_hysteresis[widget];
    uint8 debounce = Button_debounce[widget];
    
    debounceIndex = widget;

    
    /* Was on */
    if (0u != (Button_sensorOnMask[pos] & onMask))
    {
        /* Hysteresis minus */
        if (Button_sensorSignal[sensor] < (fingerThreshold - hysteresis))
        {
            Button_sensorOnMask[pos] &= (uint8)~onMask;
            Button_debounceCounter[debounceIndex] = debounce; 
        }
    }
    else    /* Was off */
    {
        /* Hysteresis plus */
        if (Button_sensorSignal[sensor] > (fingerThreshold + hysteresis))
        {
            /* Sensor active, decrement debounce counter */
            if (Button_debounceCounter[debounceIndex] <= 1u)
            {
                Button_sensorOnMask[pos] |= onMask; 
            }
            Button_debounceCounter[debounceIndex]--;
        }
        else
        {
            /* Sensor inactive - reset Debounce counter */
            Button_debounceCounter[debounceIndex] = debounce;
        }
    }
    
    return (((Button_sensorOnMask[pos] & onMask) != 0u) ? 1u : 0u);
}


/*******************************************************************************
* Function Name: Button_CheckIsWidgetActive
********************************************************************************
*
* Summary:
*  Use function Button_CheckIsSensorActive() to update 
*  Button_sensorOnMask[] for all sensors within the widget.
*  If one of sensors within widget is active the function return that widget is 
*  active.
*  The touch pad and matrix buttons widgets need to have active sensor within 
*  col and row to return widget active status.
*
* Parameters:
*  widget:  widget number.
*
* Return:
*  Returns widget sensor state 1 if one or more sensors within widget is/are 
*  active, 0 if all sensors within widget are inactive.
*
* Reentrant:
*  No
*
*******************************************************************************/
uint8 Button_CheckIsWidgetActive(uint8 widget)
                                 
{
    uint8 rawIndex = Button_rawDataIndex[widget];
    uint8 numberOfSensors = Button_numberOfSensors[widget] + rawIndex;
    uint8 state = 0u;

    /* Check all sensors of the widget */
    do
    {
        if(Button_CheckIsSensorActive(rawIndex) != 0u)
        {
            state = Button_SENSOR_1_IS_ACTIVE;
        }
        rawIndex++;
    }
    while(rawIndex < numberOfSensors);
    

    
    return (state);
}


/*******************************************************************************
* Function Name: Button_CheckIsAnyWidgetActive
********************************************************************************
*
* Summary:
*  Compares all sensors of the Button_signal[] array to their finger 
*  threshold. Calls Button_CheckIsWidgetActive() for each widget so 
*  the Button_SensorOnMask[] array is up to date after calling this 
*  function.
*
* Parameters:
*  widget:  widget number.
*
* Return:
*  Returns 1 if any widget is active, 0 none of widgets are active.
*
* Reentrant:
*  No
*
*******************************************************************************/
uint8 Button_CheckIsAnyWidgetActive(void)
                                 
{
    uint8 i;
    uint8 state = 0u;
    
    for(i = 0u; i < Button_TOTAL_WIDGET_COUNT; i++)
    {
        if (Button_CheckIsWidgetActive(i) != 0u)
        {
            state = Button_WIDGET_IS_ACTIVE;
        }
    }
    

    return (state);
}


/*******************************************************************************
* Function Name: Button_EnableWidget
********************************************************************************
*
* Summary:
*  Enable all widget elements (sensors) to scanning process.
*
* Parameters:
*  widget:  widget number.
*
* Return:
*  None
*
* Global Variables:
*  Button_sensorEnableMask[] - used to store the sensor scanning 
*  state.
*  Button_sensorEnableMask[0] contains the masked bits for sensors 
*  0 through 7 (sensor 0 is bit 0, sensor 1 is bit 1).
*  Button_sensorEnableMask[1] contains the masked bits for 
*  sensors 8 through 15 (if needed), and so on.
*  0 - sensor doesn't scan by Button_ScanEnabledWidgets().
*  1 - sensor scans by Button_ScanEnabledWidgets().
*
* Reentrant:
*  No
*
*******************************************************************************/
void Button_EnableWidget(uint8 widget)
                                   
{
    uint8 pos;
    uint8 enMask;
    uint8 rawIndex = Button_rawDataIndex[widget];
    uint8 numberOfSensors = Button_numberOfSensors[widget] + rawIndex;
    
    /* Enable all sensors of the widget */
    do
    {
        pos = (rawIndex >> 3u);
        enMask = 0x01u << (rawIndex & 0x07u);
        
        Button_sensorEnableMask[pos] |= enMask;
        rawIndex++;
    }
    while(rawIndex < numberOfSensors);
    

}


/*******************************************************************************
* Function Name: Button_DisableWidget
********************************************************************************
*
* Summary:
*  Disable all widget elements (sensors) from scanning process.
*
* Parameters:
*  widget:  widget number.
*
* Return:
*  None
*
* Global Variables:
*  Button_sensorEnableMask[] - used to store the sensor scanning 
*  state.
*  Button_sensorEnableMask[0] contains the masked bits for sensors 
*  0 through 7 (sensor 0 is bit 0, sensor 1 is bit 1).
*  Button_sensorEnableMask[1] contains the masked bits for 
*  sensors 8 through 15 (if needed), and so on.
*  0 - sensor doesn't scan by Button_ScanEnabledWidgets().
*  1 - sensor scans by Button_ScanEnabledWidgets().
*
* Reentrant:
*  No
*
*******************************************************************************/
void Button_DisableWidget(uint8 widget)
                                    
{
    uint8 pos;
    uint8 enMask;
    uint8 rawIndex = Button_rawDataIndex[widget];
    uint8 numberOfSensors = Button_numberOfSensors[widget] + rawIndex;
   
    /* Disable all sensors of the widget */
    do
    {
        pos = (rawIndex >> 3u);
        enMask = 0x01u << (rawIndex & 0x07u);
        
        Button_sensorEnableMask[pos] &= (uint8)~enMask;
        rawIndex++;
    }
    while(rawIndex < numberOfSensors);
    

}
#if(Button_TOTAL_CENTROIDS_COUNT)
    /*******************************************************************************
    * Function Name: Button_FindMaximum
    ********************************************************************************
    *
    * Summary:
    *  Finds index of maximum element within defined centroid. Checks 
    *  Button_SensorSignal[] within defined cenrtoid and 
    *  returns index of maximum element. The values below finger threshold are 
    *  ignored.
    *  The centrod defines by offset of first element and number of elements - count.
    *  The diplexed centroid requires at least consecutive two elements above
    *  FingerThreshold to find index of maximum element.
    * 
    * Parameters:
    *  offset:  Start index of cetroid in Button_sensorSignal[] array.
    *  count:   number of elements within centroid.
    *  fingerThreshold:  Finger threshould.
    *  diplex:   pointer to diplex table.
    * 
    * Return:
    *  Returns index of maximum element within defined centroid.
    *  If index of maximum element doesn't find the 0xFF returns.
    * 
    *******************************************************************************/
    #if (Button_IS_DIPLEX_SLIDER)
        uint8 Button_FindMaximum(uint8 offset, uint8 count, uint8 fingerThreshold, const uint8 CYCODE *diplex)
	                                       
    #else 
        uint8 Button_FindMaximum(uint8 offset, uint8 count, uint8 fingerThreshold)
	                                       
    #endif /* (Button_IS_DIPLEX_SLIDER) */
    {
        uint8 i;
        #if (Button_IS_DIPLEX_SLIDER)        
            uint8 curPos = 0u;
            /* No centroid at the Start */
            uint8 curCntrdSize = 0u;
            uint8 curCtrdStartPos = 0xFFu;
            /* The biggset centroid is zero */
            uint8 biggestCtrdSize = 0u;
            uint8 biggestCtrdStartPos = 0u;
        #endif /* (Button_IS_DIPLEX_SLIDER) */
        uint8 maximum = 0xFFu;
        uint8 temp = 0u;
        uint8 *startOfSlider = &Button_sensorSignal[offset]; 

        #if (Button_IS_DIPLEX_SLIDER)        
            if(diplex != 0u)
            {
                /* Initialize */
                i = 0u;
                
                /* Make slider x2 as Diplexed */
                count <<= 1u;
                while(1u)
                { 
                    if (startOfSlider[curPos] > 0u)    /* Looking for centroids */
                    {
                        if (curCtrdStartPos == 0xFFu)
                        {
                            /* Start of centroid */
                            curCtrdStartPos = i;
                            curCntrdSize++;
                        }
                        else
                        {
                            curCntrdSize++;
                        }
                    }
                    else   /* Select the bigest and indicate zero start */
                    {          
                        if(curCntrdSize > 0u)
                        {
                            /* We are in the end of current */
                            if(curCntrdSize > biggestCtrdSize)
                            {
                                biggestCtrdSize = curCntrdSize;
                                biggestCtrdStartPos = curCtrdStartPos;
                            }
                            
                            curCntrdSize = 0u;
                            curCtrdStartPos = 0xFFu;
                        }
                    }
                    
                    i++; 
                    curPos = diplex[i];
                    if(i == count)
                    {
                        break;
                    }            
                }
                    
                    /* Find the biggest centroid if two are the same size, last one wins
                       We are in the end of current */
                if (curCntrdSize >= biggestCtrdSize) 
                {
                    biggestCtrdSize = curCntrdSize;
                    biggestCtrdStartPos = curCtrdStartPos;
                }
            }
            else
            {
                /* Without diplexing */ 
                biggestCtrdSize = count;
            }
                        

            /* Check centroid size */
            #if (Button_IS_NON_DIPLEX_SLIDER)
                if((biggestCtrdSize >= 2u) || ((biggestCtrdSize == 1u) && (diplex == 0u)))
            #else                    
                if(biggestCtrdSize >= 2u)
            #endif /* (Button_IS_NON_DIPLEX_SLIDER) */
                {
                    for (i = biggestCtrdStartPos; i < (biggestCtrdStartPos + biggestCtrdSize); i++)
                    {
                        #if (Button_IS_DIPLEX_SLIDER && Button_IS_NON_DIPLEX_SLIDER)
                            if (diplex == 0u)
                            {
                                curPos = i;
                            }
                            else
                            {
                                curPos = diplex[i];
                            }                    
                        #elif (Button_IS_DIPLEX_SLIDER)                    
                            curPos = diplex[i];                    
                        #endif /* (Button_IS_DIPLEX_SLIDER && Button_IS_NON_DIPLEX_SLIDER) */
                        /* Looking for the grater element within centroid */
                        if(startOfSlider[curPos] > fingerThreshold)
                        {
                            if(startOfSlider[curPos] > temp)
                            {
                                maximum = i;
                                temp = startOfSlider[curPos];
                            }
                        }
                    }
                } 
        #else
            for (i = 0u; i < count; i++)
            {                      
                /* Looking for the grater element within centroid */
                if(startOfSlider[i] > fingerThreshold)
                {
                    if(startOfSlider[i] > temp)
                    {
                        maximum = i;
                        temp = startOfSlider[i];
                    }
                }
            }    
        #endif /* (Button_IS_DIPLEX_SLIDER) */
        return (maximum);
    }
    
    
    /*******************************************************************************
    * Function Name: Button_CalcCentroid
    ********************************************************************************
    *
    * Summary:
    *  Returns position value calculated accoring index of maximum element and API
    *  resolution.
    *
    * Parameters:
    *  type:  widget type.
    *  diplex:  pointer to diplex table.
    *  maximum:  Index of maximum element within centroid.
    *  offset:   Start index of cetroid in Button_SensorSignal[] array.
    *  count:    Number of elements within centroid.
    *  resolution:  multiplicator calculated according to centroid type and
    *  API resolution.
    *  noiseThreshold:  Noise threshould.
    * 
    * Return:
    *  Returns position value of the slider.
    * 
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint8 Button_CalcCentroid(uint8 maximum, uint8 offset, 
                                        uint8 count, uint16 resolution, uint8 noiseThreshold)
	                                    
    {
        #if ((Button_TOTAL_LINEAR_SLIDERS_COUNT > 0u) || (Button_TOTAL_TOUCH_PADS_COUNT > 0u))
            uint8 posPrev = 0u;
            uint8 posNext = 0u;
        #endif /* ((Button_TOTAL_LINEAR_SLIDERS_COUNT>0u) || (Button_TOTAL_TOUCH_PADS_COUNT>0u)) */
               
        #if (Button_IS_DIPLEX_SLIDER)                
            uint8 pos;
        #endif /* (Button_IS_DIPLEX_SLIDER) */
        
        /* Helps while centroid calulation */
        static uint8 Button_centroid[3u];

        uint8 position;
        uint32 numerator;
        int32 denominator;
        uint8 *startOfSlider = &Button_sensorSignal[offset];
                    
        #if (Button_ADD_SLIDER_TYPE)
            if(type == Button_TYPE_RADIAL_SLIDER)
            {
        #endif /* (Button_ADD_SLIDER_TYPE) */

            #if (Button_TOTAL_RADIAL_SLIDERS_COUNT > 0u)                
                /* Copy Signal for found centriod */
                Button_centroid[Button_POS] = startOfSlider[maximum];
                 
                /* Check borders for ROTARY Slider */
                if (maximum == 0u)                   /* Start of centroid */
                { 
                    Button_centroid[Button_POS_PREV] = startOfSlider[count - 1u];
                    Button_centroid[Button_POS_NEXT] = startOfSlider[1u];
                }
                else if (maximum == (count - 1u))    /* End of centroid */
                {
                    Button_centroid[Button_POS_PREV] = startOfSlider[maximum - 1u];
                    Button_centroid[Button_POS_NEXT] = startOfSlider[0u];
                }
                else                                /* Not first Not last */
                {
                    Button_centroid[Button_POS_PREV] = startOfSlider[maximum - 1u];
                    Button_centroid[Button_POS_NEXT] = startOfSlider[maximum + 1u];
                }
            #endif /* (Button_TOTAL_RADIAL_SLIDERS_COUNT > 0u) */

        #if (Button_ADD_SLIDER_TYPE)
            }
            else
            {
        #endif /* (Button_ADD_SLIDER_TYPE) */

            #if ((Button_TOTAL_LINEAR_SLIDERS_COUNT > 0u) || (Button_TOTAL_TOUCH_PADS_COUNT > 0u))
                #if (Button_IS_DIPLEX_SLIDER && Button_IS_NON_DIPLEX_SLIDER)                    
                    /* Calculate next and previous near to maximum */
                    if(diplex == 0u)
                    {
                        pos     = maximum;
                        posPrev = maximum - 1u;
                        posNext = maximum + 1u; 
                    }
                    else
                    {
                        pos     = diplex[maximum];
                        posPrev = diplex[maximum - 1u];
                        posNext = diplex[maximum + 1u];
                        count <<= 1u;
                    }                    
                #elif (Button_IS_DIPLEX_SLIDER)
                    /* Calculate next and previous near to maximum */
                    pos     = diplex[maximum];
                    posPrev = diplex[maximum - 1u];
                    posNext = diplex[maximum + 1u];
                    count <<= 1u;                    
                #else                    
                    /* Calculate next and previous near to maximum */
                    if (maximum >= 1u)
                    {
                        posPrev = maximum - 1u;
                    }
                    posNext = maximum + 1u; 
                #endif /* (Button_IS_DIPLEX_SLIDER && Button_IS_NON_DIPLEX_SLIDER) */
                        
                /* Copy Signal for found centriod */
                #if (Button_IS_DIPLEX_SLIDER)
                    Button_centroid[Button_POS] = startOfSlider[pos];
                #else
                    Button_centroid[Button_POS] = startOfSlider[maximum];
                #endif /* (Button_IS_DIPLEX_SLIDER) */
                    
                /* Check borders for LINEAR Slider */
                if (maximum == 0u)                   /* Start of centroid */
                { 
                    Button_centroid[Button_POS_PREV] = 0u;
                    Button_centroid[Button_POS_NEXT] = startOfSlider[posNext];
                }
                else if (maximum == ((count) - 1u)) /* End of centroid */
                {
                    Button_centroid[Button_POS_PREV] = startOfSlider[posPrev];
                    Button_centroid[Button_POS_NEXT] = 0u;
                }
                else                                /* Not first Not last */
                {
                    Button_centroid[Button_POS_PREV] = startOfSlider[posPrev];
                    Button_centroid[Button_POS_NEXT] = startOfSlider[posNext];
                }
            #endif /* ((Button_TOTAL_LINEAR_SLIDERS_COUNT>0u)||(Button_TOTAL_TOUCH_PADS_COUNT>0u))*/

        #if (Button_ADD_SLIDER_TYPE)
            }
        #endif /* (Button_ADD_SLIDER_TYPE) */
    
        /* Subtract noiseThreshold */
        if(Button_centroid[Button_POS_PREV] > noiseThreshold)
        {
            Button_centroid[Button_POS_PREV] -= noiseThreshold;
        }
        else
        {
            Button_centroid[Button_POS_PREV] = 0u;
        }
        
        /* Maximum always grater than fingerThreshold, so grate than noiseThreshold */
        Button_centroid[Button_POS] -= noiseThreshold;
        
        /* Subtract noiseThreshold */
        if(Button_centroid[Button_POS_NEXT] > noiseThreshold)
        {
            Button_centroid[Button_POS_NEXT] -= noiseThreshold;
        }
        else
        {
            Button_centroid[Button_POS_NEXT] = 0u;
        }
        
        
        /* Si+1 - Si-1 */
        numerator = (uint32) Button_centroid[Button_POS_NEXT] -
                    (uint32) Button_centroid[Button_POS_PREV];

        /* Si+1 + Si + Si-1 */
        denominator = (int32) Button_centroid[Button_POS_PREV] + 
                      (int32) Button_centroid[Button_POS] + 
                      (int32) Button_centroid[Button_POS_NEXT];
        
        /* (numerator/denominator) + maximum */
        denominator = (((int32)(uint32)((uint32)numerator << 8u)/denominator) + (int32)(uint32)((uint32) maximum << 8u));

        #if(Button_TOTAL_RADIAL_SLIDERS_COUNT > 0u)
            /* Only required for RADIAL Slider */
            if(denominator < 0)
            {
                denominator += (int32)((uint16)((uint16) count << 8u));
            }
        #endif /* (Button_TOTAL_RADIAL_SLIDERS_COUNT > 0u) */

        denominator *= (int16)resolution;

        /* Round the relust and put it to uint8 */
        position = ((uint8) HI16((uint32)denominator + Button_CENTROID_ROUND_VALUE));

        return (position);
    }    
#endif /* (Button_TOTAL_CENTROIDS_COUNT) */


#if(Button_TOTAL_LINEAR_SLIDERS_COUNT > 0u)
    /*******************************************************************************
    * Function Name: Button_GetCentroidPos
    ********************************************************************************
    *
    * Summary:
    *  Checks the Button_Signal[ ] array for a centroid within
    *  slider specified range. The centroid position is calculated to the resolution
    *  specified in the CapSense customizer. The position filters are applied to the
    *  result if enabled.
    *
    * Parameters:
    *  widget:  Widget number.
    *  For every linear slider widget there are defines in this format:
    *  #define Button_LS__"widget_name"            5
    * 
    * Return:
    *  Returns position value of the linear slider.
    *
    * Side Effects:
    *  If any sensor within the slider widget is active, the function returns values
    *  from zero to the API resolution value set in the CapSense customizer. If no
    *  sensors are active, the function returns 0xFFFF. If an error occurs during
    *  execution of the centroid/diplexing algorithm, the function returns 0xFFFF.
    *  There are no checks of widget type argument provided to this function.
    *  The unproper widget type provided will cause unexpected position calculations.
    *
    * Note:
    *  If noise counts on the slider segments are greater than the noise
    *  threshold, this subroutine may generate a false centroid result. The noise
    *  threshold should be set carefully (high enough above the noise level) so
    *  that noise will not generate a false centroid.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint16 Button_GetCentroidPos(uint8 widget) 
    {
        #if (Button_IS_DIPLEX_SLIDER)
            const uint8 CYCODE *diplex;
        #endif /* (Button_IS_DIPLEX_SLIDER) */
                
        #if (0u != Button_LINEAR_SLIDERS_POS_FILTERS_MASK)
            uint8 posIndex;
            uint8 firstTimeIndex = Button_posFiltersData[widget];
            uint8 posFiltersMask = Button_posFiltersMask[widget];  
        #endif /* (0u != Button_LINEAR_SLIDERS_POS_FILTERS_MASK) */

        #if ((0u != (Button_MEDIAN_FILTER & Button_LINEAR_SLIDERS_POS_FILTERS_MASK)) || \
             (0u != (Button_AVERAGING_FILTER & Button_LINEAR_SLIDERS_POS_FILTERS_MASK)))
            uint8 tempPos;
        #endif /* ((0u != (Button_MEDIAN_FILTER & Button_LINEAR_SLIDERS_POS_FILTERS_MASK)) || \
               *   (0u != (Button_AVERAGING_FILTER & Button_LINEAR_SLIDERS_POS_FILTERS_MASK)))
               */

        uint8 maximum;
        uint16 position;
        uint8 offset = Button_rawDataIndex[widget];
        uint8 count = Button_numberOfSensors[widget];
                        
        #if (Button_IS_DIPLEX_SLIDER)
            if(widget < Button_TOTAL_DIPLEXED_SLIDERS_COUNT)
            {
                maximum = Button_diplexTable[widget];
                diplex = &Button_diplexTable[maximum];
            }
            else
            {
                diplex = 0u;
            }
        #endif /* (Button_IS_DIPLEX_SLIDER) */

        /* Find Maximum within centroid */      
        #if (Button_IS_DIPLEX_SLIDER)        
            maximum = Button_FindMaximum(offset, count, Button_fingerThreshold[widget], diplex);
        #else
            maximum = Button_FindMaximum(offset, count, Button_fingerThreshold[widget]);
        #endif /* (Button_IS_DIPLEX_SLIDER) */

        if (maximum != 0xFFu)
        {
            /* Calculate centroid */
            position = (uint16) Button_CalcCentroid(maximum, 
                         offset, count, Button_centroidMult[widget], Button_noiseThreshold[widget]);

            #if (0u != Button_LINEAR_SLIDERS_POS_FILTERS_MASK)
                /* Check if this linear slider has enabled filters */
                if (0u != (posFiltersMask & Button_ANY_POS_FILTER))
                {
                    /* Caluclate position to store filters data */
                    posIndex  = firstTimeIndex + 1u;
                    
                    if (0u == Button_posFiltersData[firstTimeIndex])
                    {
                        /* Init filters */
                        Button_posFiltersData[posIndex] = (uint8) position;
                        #if ((0u != (Button_MEDIAN_FILTER & \
                                     Button_LINEAR_SLIDERS_POS_FILTERS_MASK)) || \
                             (0u != (Button_AVERAGING_FILTER & \
                                     Button_LINEAR_SLIDERS_POS_FILTERS_MASK)))

                            if ( (0u != (posFiltersMask & Button_MEDIAN_FILTER)) || 
                                 (0u != (posFiltersMask & Button_AVERAGING_FILTER)) )
                            {
                                Button_posFiltersData[posIndex + 1u] = (uint8) position;
                            }
                        #endif /* ((0u != (Button_MEDIAN_FILTER & \
                               *           Button_LINEAR_SLIDERS_POS_FILTERS_MASK)) || \
                               *   (0u != (Button_AVERAGING_FILTER & \
                               *           Button_LINEAR_SLIDERS_POS_FILTERS_MASK)))
                               */
                        
                        Button_posFiltersData[firstTimeIndex] = 1u;
                    }
                    else
                    {
                        /* Do filtering */
                        #if (0u != (Button_MEDIAN_FILTER & Button_LINEAR_SLIDERS_POS_FILTERS_MASK))
                            if (0u != (posFiltersMask & Button_MEDIAN_FILTER))
                            {
                                tempPos = (uint8) position;
                                position = Button_MedianFilter(position,
                                                                (uint16)Button_posFiltersData[posIndex],
                                                                (uint16)Button_posFiltersData[posIndex + 1u]);
                                Button_posFiltersData[posIndex + 1u] =
                                                                             Button_posFiltersData[posIndex];
                                Button_posFiltersData[posIndex] = tempPos;
                            }
                        #endif /*(0u != (Button_MEDIAN_FILTER &
                               *         Button_LINEAR_SLIDERS_POS_FILTERS_MASK))
                               */

                        #if(0u!=(Button_AVERAGING_FILTER & Button_LINEAR_SLIDERS_POS_FILTERS_MASK))
                            if (0u != (posFiltersMask & Button_AVERAGING_FILTER)) 
                            {
                                tempPos = (uint8) position;
                                position = Button_AveragingFilter(position,
                                                                (uint16)Button_posFiltersData[posIndex],
                                                                (uint16)Button_posFiltersData[posIndex + 1u]);
                                Button_posFiltersData[posIndex+1u]=Button_posFiltersData[posIndex];
                                Button_posFiltersData[posIndex] = tempPos;
                            }
                        #endif /* (0u != (Button_AVERAGING_FILTER & \
                               *           Button_LINEAR_SLIDERS_POS_FILTERS_MASK))
                               */

                        #if (0u != (Button_IIR2_FILTER & Button_LINEAR_SLIDERS_POS_FILTERS_MASK)) 
                            if (0u != (posFiltersMask & Button_IIR2_FILTER)) 
                            {
                                position = Button_IIR2Filter(position,
                                                                    (uint16)Button_posFiltersData[posIndex]);
                                Button_posFiltersData[posIndex] = (uint8) position;
                            }
                        #endif /* (0u != (Button_IIR2_FILTER & \
                               *          Button_LINEAR_SLIDERS_POS_FILTERS_MASK))
                               */

                        #if (0u != (Button_IIR4_FILTER & Button_LINEAR_SLIDERS_POS_FILTERS_MASK))
                            if (0u != (posFiltersMask & Button_IIR4_FILTER))
                            {
                                position = Button_IIR4Filter(position,
                                                                    (uint16)Button_posFiltersData[posIndex]);
                                Button_posFiltersData[posIndex] = (uint8) position;
                            }                                
                        #endif /* (0u != (Button_IIR4_FILTER & \
                               *          Button_LINEAR_SLIDERS_POS_FILTERS_MASK))
                               */

                        #if (0u != (Button_JITTER_FILTER & Button_LINEAR_SLIDERS_POS_FILTERS_MASK))
                            if (0u != (posFiltersMask & Button_JITTER_FILTER))
                            {
                                position = Button_JitterFilter(position,
                                                                    (uint16)Button_posFiltersData[posIndex]);
                                Button_posFiltersData[posIndex] = (uint8) position;
                            }
                        #endif /* (0u != (Button_JITTER_FILTER & \
                               *           Button_LINEAR_SLIDERS_POS_FILTERS_MASK))
                               */
                    }
                }
            #endif /* (0u != Button_LINEAR_SLIDERS_POS_FILTERS_MASK) */

        }
        else
        {
            /* The maximum wasn't found */
            position = 0xFFFFu;

            #if(0u != Button_LINEAR_SLIDERS_POS_FILTERS_MASK)
                /* Reset the filters */
                if(0u != (posFiltersMask & Button_ANY_POS_FILTER))
                {
                    Button_posFiltersData[firstTimeIndex] = 0u;
                }
            #endif /* (0u != Button_LINEAR_SLIDERS_POS_FILTERS_MASK) */
        }

        
        return (position);
    }
#endif /* (Button_TOTAL_LINEAR_SLIDERS_COUNT > 0u) */


#if(Button_TOTAL_RADIAL_SLIDERS_COUNT > 0u)
    /*******************************************************************************
    * Function Name: Button_GetRadialCentroidPos
    ********************************************************************************
    *
    * Summary:
    *  Checks the Button_Signal[ ] array for a centroid within
    *  slider specified range. The centroid position is calculated to the resolution
    *  specified in the CapSense customizer. The position filters are applied to the
    *  result if enabled.
    *
    * Parameters:
    *  widget:  Widget number.
    *  For every radial slider widget there are defines in this format:
    *  #define Button_RS_"widget_name"            5
    * 
    * Return:
    *  Returns position value of the radial slider.
    *
    * Side Effects:
    *  If any sensor within the slider widget is active, the function returns values
    *  from zero to the API resolution value set in the CapSense customizer. If no
    *  sensors are active, the function returns 0xFFFF.
    *  There are no checks of widget type argument provided to this function.
    *  The unproper widget type provided will cause unexpected position calculations.
    *
    * Note:
    *  If noise counts on the slider segments are greater than the noise
    *  threshold, this subroutine may generate a false centroid result. The noise
    *  threshold should be set carefully (high enough above the noise level) so 
    *  that noise will not generate a false centroid.
    *
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
     uint16 Button_GetRadialCentroidPos(uint8 widget)
	                                       
    {
        #if (0u != Button_RADIAL_SLIDERS_POS_FILTERS_MASK)
            uint8 posIndex;
            uint8 firstTimeIndex = Button_posFiltersData[widget];
            uint8 posFiltersMask = Button_posFiltersMask[widget]; 
        #endif /* (0u != Button_RADIAL_SLIDERS_POS_FILTERS_MASK) */

        #if ((0u != (Button_MEDIAN_FILTER & Button_RADIAL_SLIDERS_POS_FILTERS_MASK)) || \
             (0u != (Button_AVERAGING_FILTER & Button_RADIAL_SLIDERS_POS_FILTERS_MASK)))
            uint8 tempPos;
        #endif /* ((0u != (Button_MEDIAN_FILTER & Button_RADIAL_SLIDERS_POS_FILTERS_MASK)) || \
               *   (0u != (Button_AVERAGING_FILTER & Button_RADIAL_SLIDERS_POS_FILTERS_MASK)))
               */

        uint8 maximum;
        uint16 position;
        uint8 offset = Button_rawDataIndex[widget];
        uint8 count = Button_numberOfSensors[widget];
        
        /* Find Maximum within centroid */        
        #if (Button_IS_DIPLEX_SLIDER)
            maximum = Button_FindMaximum(offset, count, Button_fingerThreshold[widget], 0u);
        #else
            maximum = Button_FindMaximum(offset, count, Button_fingerThreshold[widget]);
        #endif /* (Button_IS_DIPLEX_SLIDER) */
        
        if (maximum != 0xFFu)
        {
            /* Calculate centroid */
            position = (uint16) Button_CalcCentroid(maximum, 
                         offset, count, Button_centroidMult[widget], Button_noiseThreshold[widget]);

            #if (0u != Button_RADIAL_SLIDERS_POS_FILTERS_MASK)
                /* Check if this Radial slider has enabled filters */
                if (0u != (posFiltersMask & Button_ANY_POS_FILTER))
                {
                    /* Caluclate position to store filters data */
                    posIndex  = firstTimeIndex + 1u;
                    
                    if (0u == Button_posFiltersData[firstTimeIndex])
                    {
                        /* Init filters */
                        Button_posFiltersData[posIndex] = (uint8) position;
                        #if ((0u != (Button_MEDIAN_FILTER & \
                                     Button_RADIAL_SLIDERS_POS_FILTERS_MASK)) || \
                             (0u != (Button_AVERAGING_FILTER & \
                                     Button_RADIAL_SLIDERS_POS_FILTERS_MASK)))

                            if ( (0u != (posFiltersMask & Button_MEDIAN_FILTER))  || 
                                 (0u != (posFiltersMask & Button_AVERAGING_FILTER)) )
                            {
                                Button_posFiltersData[posIndex + 1u] = (uint8) position;
                            }
                        #endif /* ((0u != (Button_MEDIAN_FILTER & \
                               *           Button_RADIAL_SLIDERS_POS_FILTERS_MASK)) || \
                               *   (0u != (Button_AVERAGING_FILTER & \
                               *           Button_RADIAL_SLIDERS_POS_FILTERS_MASK)))
                               */
                        
                        Button_posFiltersData[firstTimeIndex] = 1u;
                    }
                    else
                    {
                        /* Do filtering */
                        #if (0u != (Button_MEDIAN_FILTER & Button_RADIAL_SLIDERS_POS_FILTERS_MASK))
                            if (0u != (posFiltersMask & Button_MEDIAN_FILTER))
                            {
                                tempPos = (uint8) position;
                                position = Button_MedianFilter(position,
                                                                        Button_posFiltersData[posIndex],
                                                                        Button_posFiltersData[posIndex + 1u]);
                                Button_posFiltersData[posIndex + 1u] = 
                                                                              Button_posFiltersData[posIndex];
                                Button_posFiltersData[posIndex] = tempPos;
                            }
                        #endif /* (0u != (Button_MEDIAN_FILTER & 
                               *          Button_RADIAL_SLIDERS_POS_FILTERS_MASK))
                               */

                        #if (0u != (Button_AVERAGING_FILTER & \
                                    Button_RADIAL_SLIDERS_POS_FILTERS_MASK))
                            if (0u != (posFiltersMask & Button_AVERAGING_FILTER))
                            {
                                tempPos = (uint8) position;
                                position = Button_AveragingFilter(position, 
                                                                       Button_posFiltersData[posIndex],
                                                                       Button_posFiltersData[posIndex + 1u]);
                                Button_posFiltersData[posIndex+1u]= Button_posFiltersData[posIndex];
                                Button_posFiltersData[posIndex] = tempPos;
                            }
                        #endif /* (0u != (Button_AVERAGING_FILTER & \
                               *          Button_RADIAL_SLIDERS_POS_FILTERS_MASK))
                               */

                        #if (0u != (Button_IIR2_FILTER & Button_RADIAL_SLIDERS_POS_FILTERS_MASK))
                            if (0u != (posFiltersMask & Button_IIR2_FILTER))
                            {
                                position = Button_IIR2Filter(position,
                                                                    (uint16)Button_posFiltersData[posIndex]);
                                Button_posFiltersData[posIndex] = (uint8) position;
                            }
                        #endif /* (0u != (Button_IIR2_FILTER & 
                               *          Button_RADIAL_SLIDERS_POS_FILTERS_MASK))
                               */

                        #if (0u != (Button_IIR4_FILTER & Button_RADIAL_SLIDERS_POS_FILTERS_MASK))
                            if (0u != (posFiltersMask & Button_IIR4_FILTER))
                            {
                                position = Button_IIR4Filter(position,
                                                                    (uint16)Button_posFiltersData[posIndex]);
                                Button_posFiltersData[posIndex] = (uint8) position;
                            }
                        #endif /* (0u != (Button_IIR4_FILTER & 
                               *          Button_RADIAL_SLIDERS_POS_FILTERS_MASK))
                               */

                        #if (0u != (Button_JITTER_FILTER & Button_RADIAL_SLIDERS_POS_FILTERS_MASK))
                            if (0u != (posFiltersMask & Button_JITTER_FILTER))
                            {
                                position = Button_JitterFilter(position, 
                                                                         Button_posFiltersData[posIndex]);
                                Button_posFiltersData[posIndex] = (uint8) position;
                            }
                        #endif /* (0u != (Button_JITTER_FILTER &
                               *           Button_RADIAL_SLIDERS_POS_FILTERS_MASK))
                               */
                    }
                }
            #endif /* (0u != Button_RADIAL_SLIDERS_POS_FILTERS_MASK) */

        }
        else
        {
            /* The maximum didn't find */
            position = 0xFFFFu;

            #if (0u != Button_RADIAL_SLIDERS_POS_FILTERS_MASK)
                /* Reset the filters */
                if((posFiltersMask & Button_ANY_POS_FILTER) != 0u)
                {
                    Button_posFiltersData[firstTimeIndex] = 0u;
                }
            #endif /* (0u != Button_RADIAL_SLIDERS_POS_FILTERS_MASK) */
        }
        
        return (position);
    }
#endif


#if(Button_TOTAL_TOUCH_PADS_COUNT > 0u)
    /*******************************************************************************
    * Function Name: Button_GetTouchCentroidPos
    ********************************************************************************
    *
    * Summary:
    *  If a finger is present on touch pad, this function calculates the X and Y
    *  position of the finger by calculating the centroids within touch pad specified
    *  range. The X and Y positions are calculated to the API resolutions set in the
    *  CapSense customizer. Returns a 1 if a finger is on the touchpad.
    *  The position filter is applied to the result if enabled.
    *  This function is available only if a touch pad is defined by the CapSense
    *  customizer.
    *
    * Parameters:
    *  widget:  Widget number. 
    *  For every touchpad widget there are defines in this format:
    *  #define Button_TP_"widget_name"            5
    *
    *  pos:     Pointer to the array of two uint16 elements, where result
    *  result of calculation of X and Y position are stored.
    *  pos[0u]  - position of X
    *  pos[1u]  - position of Y
    *
    * Return:
    *  Returns a 1 if a finger is on the touch pad, 0 - if not.
    *
    * Side Effects:
    *   There are no checks of widget type argument provided to this function.
    *   The unproper widget type provided will cause unexpected position
    *   calculations.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint8 Button_GetTouchCentroidPos(uint8 widget, uint16* pos)
	                                    
    {
        #if (0u != Button_TOUCH_PADS_POS_FILTERS_MASK)
            uint8 posXIndex;
            uint8 posYIndex;
            uint8 firstTimeIndex = Button_posFiltersData[widget];
            uint8 posFiltersMask = Button_posFiltersMask[widget];
        #endif /* (0u != Button_TOUCH_PADS_POS_FILTERS_MASK) */

        #if ((0u != (Button_MEDIAN_FILTER & Button_TOUCH_PADS_POS_FILTERS_MASK)) || \
             (0u != (Button_AVERAGING_FILTER & Button_TOUCH_PADS_POS_FILTERS_MASK)))
            uint16 tempPos;
        #endif /* ((0u != (Button_MEDIAN_FILTER & Button_TOUCH_PADS_POS_FILTERS_MASK)) || \
               *   (0u != (Button_AVERAGING_FILTER & Button_TOUCH_PADS_POS_FILTERS_MASK)))
               */

        uint8 maxX;
        uint8 maxY;
        uint8 posX;
        uint8 posY;
        uint8 touch = 0u;
        uint8 offset = Button_rawDataIndex[widget];
        uint8 count = Button_numberOfSensors[widget];
        
        /* Find Maximum within X centroid */
        #if (Button_IS_DIPLEX_SLIDER)
            maxX = Button_FindMaximum(offset, count, Button_fingerThreshold[widget], 0u);
        #else
            maxX = Button_FindMaximum(offset, count, Button_fingerThreshold[widget]);
        #endif /* (Button_IS_DIPLEX_SLIDER) */

        if (maxX != 0xFFu)
        {
            offset = Button_rawDataIndex[widget + 1u];
            count = Button_numberOfSensors[widget + 1u];

            /* Find Maximum within Y centroid */
            #if (Button_IS_DIPLEX_SLIDER)
                maxY = Button_FindMaximum(offset, count, Button_fingerThreshold[widget + 1u], 0u);
            #else
                maxY = Button_FindMaximum(offset, count, Button_fingerThreshold[widget + 1u]);
            #endif /* (Button_IS_DIPLEX_SLIDER) */

            if (maxY != 0xFFu)
            {
                /* X and Y maximums are found = true touch */
                touch = 1u;
                
                /* Calculate Y centroid */
                posY = Button_CalcCentroid(maxY, offset, count, 
                            Button_centroidMult[widget + 1u], Button_noiseThreshold[widget + 1u]);
                
                /* Calculate X centroid */
                offset = Button_rawDataIndex[widget];
                count = Button_numberOfSensors[widget];
                
                posX = Button_CalcCentroid(maxX, offset, count, 
                            Button_centroidMult[widget],Button_noiseThreshold[widget]);
    
                #if (0u != Button_TOUCH_PADS_POS_FILTERS_MASK)
                    /* Check if this TP has enabled filters */
                    if (0u != (posFiltersMask & Button_ANY_POS_FILTER))
                    {
                        /* Caluclate position to store filters data */
                        posXIndex  = firstTimeIndex + 1u;
                        posYIndex  = Button_posFiltersData[widget + 1u];
                        
                        if (0u == Button_posFiltersData[firstTimeIndex])
                        {
                            /* Init filters */
                            Button_posFiltersData[posXIndex] = posX;
                            Button_posFiltersData[posYIndex] = posY;

                            #if((0u != (Button_MEDIAN_FILTER & \
                                        Button_TOUCH_PADS_POS_FILTERS_MASK))|| \
                                (0u != (Button_AVERAGING_FILTER & \
                                        Button_TOUCH_PADS_POS_FILTERS_MASK)))

                                if ( (0u != (posFiltersMask & Button_MEDIAN_FILTER)) || 
                                     (0u != (posFiltersMask & Button_AVERAGING_FILTER)) )
                                {
                                    Button_posFiltersData[posXIndex + 1u] = posX;
                                    Button_posFiltersData[posYIndex + 1u] = posY;
                                }
                            #endif /* ((0u != (Button_MEDIAN_FILTER & \
                                   *           Button_TOUCH_PADS_POS_FILTERS_MASK)) || \
                                   *    (0u != (Button_AVERAGING_FILTER & \
                                   *            Button_TOUCH_PADS_POS_FILTERS_MASK)))
                                   */
                            
                            Button_posFiltersData[firstTimeIndex] = 1u;
                        }
                        else
                        {
                            /* Do filtering */
                            #if (0u != (Button_MEDIAN_FILTER & Button_TOUCH_PADS_POS_FILTERS_MASK))
                                if (0u != (posFiltersMask & Button_MEDIAN_FILTER))
                                {
                                    tempPos = posX;
                                    posX = (uint8) Button_MedianFilter(posX,
                                                                      Button_posFiltersData[posXIndex],
                                                                      Button_posFiltersData[posXIndex + 1u]);
                                    Button_posFiltersData[posXIndex + 1u] = 
                                                                             Button_posFiltersData[posXIndex];
                                    Button_posFiltersData[posXIndex] = tempPos;
                                    
                                    tempPos = posY;
                                    posY = (uint8) Button_MedianFilter(posY,
                                                                       Button_posFiltersData[posYIndex], 
                                                                       Button_posFiltersData[posYIndex + 1u]);
                                    Button_posFiltersData[posYIndex + 1u] = 
                                                                             Button_posFiltersData[posYIndex];
                                    Button_posFiltersData[posYIndex] = tempPos;
                                }
                                
                            #endif /* (0u != (Button_MEDIAN_FILTER & \
                                   *          Button_TOUCH_PADS_POS_FILTERS_MASK))
                                   */

                            #if(0u !=(Button_AVERAGING_FILTER & Button_TOUCH_PADS_POS_FILTERS_MASK))
                                if (0u != (posFiltersMask & Button_AVERAGING_FILTER))
                                {
                                    tempPos = posX;
                                    posX = (uint8) Button_AveragingFilter(posX,
                                                                       Button_posFiltersData[posXIndex], 
                                                                       Button_posFiltersData[posXIndex + 1u]);
                                    Button_posFiltersData[posXIndex + 1u] = 
                                                                             Button_posFiltersData[posXIndex];
                                    Button_posFiltersData[posXIndex] = tempPos;
                                    
                                    tempPos = posY;
                                    posY = (uint8) Button_AveragingFilter(posY, 
                                                                      Button_posFiltersData[posYIndex], 
                                                                      Button_posFiltersData[posYIndex + 1u]);
                                    Button_posFiltersData[posYIndex + 1u] = 
                                                                            Button_posFiltersData[posYIndex];
                                    Button_posFiltersData[posYIndex] = tempPos;
                                }

                            #endif /* (0u != (Button_AVERAGING_FILTER & \
                                   *           Button_TOUCH_PADS_POS_FILTERS_MASK))
                                   */

                            #if (0u != (Button_IIR2_FILTER & Button_TOUCH_PADS_POS_FILTERS_MASK))
                                if (0u != (posFiltersMask & Button_IIR2_FILTER))
                                {
                                    posX = (uint8) Button_IIR2Filter(posX, 
                                                                           Button_posFiltersData[posXIndex]);
                                    Button_posFiltersData[posXIndex] = posX;
                                    
                                    posY = (uint8) Button_IIR2Filter(posY, 
                                                                            Button_posFiltersData[posYIndex]);
                                    Button_posFiltersData[posYIndex] = posY;
                                }
                                
                            #endif /* (0u != (Button_IIR2_FILTER & \
                                   *          Button_TOUCH_PADS_POS_FILTERS_MASK))
                                   */

                            #if (0u != (Button_IIR4_FILTER & Button_TOUCH_PADS_POS_FILTERS_MASK))
                                if (0u != (posFiltersMask & Button_IIR4_FILTER))
                                {
                                    posX = (uint8) Button_IIR4Filter((uint16)posX,
                                                                    (uint16)Button_posFiltersData[posXIndex]);
                                    Button_posFiltersData[posXIndex] = posX;

                                    posY = (uint8) Button_IIR4Filter((uint16)posY,
                                                                    (uint16)Button_posFiltersData[posYIndex]);
                                    Button_posFiltersData[posYIndex] = posY;
                                }
                                
                            #endif /* (0u != (Button_IIR4_FILTER & \
                                   *           Button_TOUCH_PADS_POS_FILTERS_MASK))
                                   */

                            #if (0u != (Button_JITTER_FILTER & Button_TOUCH_PADS_POS_FILTERS_MASK))
                                if (0u != (posFiltersMask & Button_JITTER_FILTER))
                                    {
                                        posX = (uint8) Button_JitterFilter(posX, 
                                                                            Button_posFiltersData[posXIndex]);
                                        Button_posFiltersData[posXIndex] = posX;
                                        
                                        posY = (uint8) Button_JitterFilter(posY, 
                                                                            Button_posFiltersData[posYIndex]);
                                        Button_posFiltersData[posYIndex] = posY;
                                    }
                            #endif /* (0u != (Button_JITTER_FILTER & \
                                   *           Button_TOUCH_PADS_POS_FILTERS_MASK))
                                   */
                        }
                    }
                #endif /* (0u != Button_TOUCH_PADS_POS_FILTERS_MASK) */

                /* Save positions */
                pos[0u] = posX;
                pos[1u] = posY;
            }
        }

        #if (0u != Button_TOUCH_PADS_POS_FILTERS_MASK)
            if(touch == 0u)
            {
                /* Reset the filters */
                if ((posFiltersMask & Button_ANY_POS_FILTER) != 0u)
                {
                    Button_posFiltersData[firstTimeIndex] = 0u;
                }
            }
        #endif /* (0u != Button_TOUCH_PADS_POS_FILTERS_MASK) */
        
        return (touch);
    }
#endif /* (Button_TOTAL_TOUCH_PADS_COUNT > 0u) */


#if ( (Button_RAW_FILTER_MASK & Button_MEDIAN_FILTER) | \
      (Button_POS_FILTERS_MASK & Button_MEDIAN_FILTER) )
    /*******************************************************************************
    * Function Name: Button_MedianFilter
    ********************************************************************************
    *
    * Summary:
    *  Median filter function. 
    *  The median filter looks at the three most recent samples and reports the 
    *  median value.
    *
    * Parameters:
    *  x1:  Current value.
    *  x2:  Previous value.
    *  x3:  Before previous value.
    *
    * Return:
    *  Returns filtered value.
    *
    *******************************************************************************/
    uint16 Button_MedianFilter(uint16 x1, uint16 x2, uint16 x3)
                                         
    {
        uint16 tmp;
        
        if (x1 > x2)
        {
            tmp = x2;
            x2 = x1;
            x1 = tmp;
        }
        
        if (x2 > x3)
        {
            x2 = x3;
        }
        
        return ((x1 > x2) ? x1 : x2);
    }
#endif /* ( (Button_RAW_FILTER_MASK & Button_MEDIAN_FILTER) | \
       *    (Button_POS_FILTERS_MASK & Button_MEDIAN_FILTER) )
       */


#if ( (Button_RAW_FILTER_MASK & Button_AVERAGING_FILTER) | \
      (Button_POS_FILTERS_MASK & Button_AVERAGING_FILTER) )
    /*******************************************************************************
    * Function Name: Button_AveragingFilter
    ********************************************************************************
    *
    * Summary:
    *  Averaging filter function.
    *  The averaging filter looks at the three most recent samples of position and
    *  reports the averaging value.
    *
    * Parameters:
    *  x1:  Current value.
    *  x2:  Previous value.
    *  x3:  Before previous value.
    *
    * Return:
    *  Returns filtered value.
    *
    *******************************************************************************/
    uint16 Button_AveragingFilter(uint16 x1, uint16 x2, uint16 x3)
                                            
    {
        uint32 tmp = ((uint32)x1 + (uint32)x2 + (uint32)x3) / 3u;
        
        return ((uint16) tmp);
    }
#endif /* ( (Button_RAW_FILTER_MASK & Button_AVERAGING_FILTER) | \
       *    (Button_POS_FILTERS_MASK & Button_AVERAGING_FILTER) )
       */


#if ( (Button_RAW_FILTER_MASK & Button_IIR2_FILTER) | \
      (Button_POS_FILTERS_MASK & Button_IIR2_FILTER) )
    /*******************************************************************************
    * Function Name: Button_IIR2Filter
    ********************************************************************************
    *
    * Summary:
    *  IIR1/2 filter function. IIR1/2 = 1/2current + 1/2previous.
    *
    * Parameters:
    *  x1:  Current value.
    *  x2:  Previous value.
    *
    * Return:
    *  Returns filtered value.
    *
    *******************************************************************************/
    uint16 Button_IIR2Filter(uint16 x1, uint16 x2)
                                       
    {
        uint32 tmp;
        
        /* IIR = 1/2 Current Value+ 1/2 Previous Value */
        tmp = (uint32)x1 + (uint32)x2;
        tmp >>= 1u;
    
        return ((uint16) tmp);
    }
#endif /* ( (Button_RAW_FILTER_MASK & Button_IIR2_FILTER) | \
       *    (Button_POS_FILTERS_MASK & Button_IIR2_FILTER) )
       */


#if ( (Button_RAW_FILTER_MASK & Button_IIR4_FILTER) | \
      (Button_POS_FILTERS_MASK & Button_IIR4_FILTER) )
    /*******************************************************************************
    * Function Name: Button_IIR4Filter
    ********************************************************************************
    *
    * Summary:
    *  IIR1/4 filter function. IIR1/4 = 1/4current + 3/4previous.
    *
    * Parameters:
    *  x1:  Current value.
    *  x2:  Previous value.
    *
    * Return:
    *  Returns filtered value.
    *
    *******************************************************************************/
    uint16 Button_IIR4Filter(uint16 x1, uint16 x2)
                                       
    {
        uint32 tmp;
        
        /* IIR = 1/4 Current Value + 3/4 Previous Value */
        tmp = (uint32)x1 + (uint32)x2;
        tmp += ((uint32)x2 << 1u);
        tmp >>= 2u;
        
        return ((uint16) tmp);
    }
#endif /* ( (Button_RAW_FILTER_MASK & Button_IIR4_FILTER) | \
       *    (Button_POS_FILTERS_MASK & Button_IIR4_FILTER) )
       */


#if ( (Button_RAW_FILTER_MASK & Button_JITTER_FILTER) | \
      (Button_POS_FILTERS_MASK & Button_JITTER_FILTER) )
    /*******************************************************************************
    * Function Name: uint16 Button_JitterFilter
    ********************************************************************************
    *
    * Summary:
    *  Jitter filter function.
    *
    * Parameters:
    *  x1:  Current value.
    *  x2:  Previous value.
    *
    * Return:
    *  Returns filtered value.
    *
    *******************************************************************************/
    uint16 Button_JitterFilter(uint16 x1, uint16 x2)
                                         
    {
        if (x1 > x2)
        {
            x1--;
        }
        else
        {
            if (x1 < x2)
            {
                x1++;
            }
        }
    
        return x1;
    }
#endif /* ( (Button_RAW_FILTER_MASK & Button_JITTER_FILTER) | \
       *    (Button_POS_FILTERS_MASK & Button_JITTER_FILTER) )
       */


#if (Button_RAW_FILTER_MASK & Button_IIR8_FILTER)
    /*******************************************************************************
    * Function Name: Button_IIR8Filter
    ********************************************************************************
    *
    * Summary:
    *  IIR1/8 filter function. IIR1/8 = 1/8current + 7/8previous.
    *  Only applies for raw data.
    *
    * Parameters:
    *  x1:  Current value.
    *  x2:  Previous value.
    *
    * Return:
    *  Returns filtered value.
    *
    *******************************************************************************/
    uint16 Button_IIR8Filter(uint16 x1, uint16 x2)
                                       
    {
        uint32 tmp;
        
        /* IIR = 1/8 Current Value + 7/8 Previous Value */
        tmp = (uint32)x1;
        tmp += (((uint32)x2 << 3u) - ((uint32)x2));
        tmp >>= 3u;
    
        return ((uint16) tmp);
    }
#endif /* (Button_RAW_FILTER_MASK & Button_IIR8_FILTER) */


#if (Button_RAW_FILTER_MASK & Button_IIR16_FILTER)
    /*******************************************************************************
    * Function Name: Button_IIR16Filter
    ********************************************************************************
    *
    * Summary:
    *  IIR1/16 filter function. IIR1/16 = 1/16current + 15/16previous.
    *  Only applies for raw data.
    *
    * Parameters:
    *  x1:  Current value.
    *  x2:  Previous value.
    *
    * Return:
    *  Returns filtered value.
    *
    *******************************************************************************/
    uint16 Button_IIR16Filter(uint16 x1, uint16 x2)
                                        
    {
        uint32 tmp;
        
        /* IIR = 1/16 Current Value + 15/16 Previous Value */
        tmp = (uint32)x1;
        tmp += (((uint32)x2 << 4u) - ((uint32)x2));
        tmp >>= 4u;
        
        return ((uint16) tmp);
    }
#endif /* (Button_RAW_FILTER_MASK & Button_IIR16_FILTER) */


#if (Button_TOTAL_MATRIX_BUTTONS_COUNT)

    /*******************************************************************************
    * Function Name: Button_GetMatrixButtonPos
    ********************************************************************************
    *
    * Summary:
    *  Function calculates and returns touch position (column and row) for matrix
    *  button widget.
    *
    * Parameters:
    *  widget:  widget number;
    *  pos:     pointer to an array of two uint8, where touch postion will be 
    *           stored:
    *           pos[0] - column position;
    *           pos[1] - raw position.
    *
    * Return:
    *  Returns 1 if row and column sensors of matrix button are active, 0 - in other
    *  cases.
    *
    * Reentrant:
    *  No
    *
    *******************************************************************************/
    uint8 Button_GetMatrixButtonPos(uint8 widget, uint8* pos)
	                                          
    {
        uint8 i;
        uint8 retVal = 0u;
        uint16 row_sig_max = 0u;
        uint16 col_sig_max = 0u;
        uint8 row_ind = 0u;
        uint8 col_ind = 0u;

        if (0u != Button_CheckIsWidgetActive(widget))
        {
            /* Find row number with maximal signal value */
            for(i = Button_rawDataIndex[widget]; i < (Button_rawDataIndex[widget] + \
                 Button_numberOfSensors[widget]); i++)
            {
                if (Button_sensorSignal[i] > col_sig_max)
                {
                    col_ind = i;
                    col_sig_max = Button_sensorSignal[i];
                }
            }

            /* Find row number with maximal signal value */
            for(i = Button_rawDataIndex[widget+1u]; i < (Button_rawDataIndex[widget+1u] + \
                 Button_numberOfSensors[widget+1u]); i++)
            {
                if (Button_sensorSignal[i] > row_sig_max)
                {
                    row_ind = i;
                    row_sig_max = Button_sensorSignal[i];
                }
            }

            if((col_sig_max >= Button_fingerThreshold[widget]) && \
               (row_sig_max >= Button_fingerThreshold[widget+1u]))
            {
                pos[0u] = col_ind - Button_rawDataIndex[widget];
                pos[1u] = row_ind - Button_rawDataIndex[widget+1u];
                retVal = 1u;
            }
        }
        return (retVal);
    }

#endif /* (Button_TOTAL_MATRIX_BUTTONS_COUNT) */

/* [] END OF FILE */
