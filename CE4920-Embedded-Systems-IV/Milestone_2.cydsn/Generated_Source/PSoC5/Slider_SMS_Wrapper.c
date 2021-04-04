/*******************************************************************************
* File Name: Slider_SMS_Wrapper.c
* Version 3.50
*
* Description:
*  This file provides the source code of wrapper between CapSense CSD component 
*  and Auto Tuning library.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "Slider.h"
#include "Slider_CSHL.h"

#if (Slider_TUNING_METHOD == Slider_AUTO_TUNING)

extern uint8 Slider_noiseThreshold[Slider_WIDGET_CSHL_PARAMETERS_COUNT];
extern uint8 Slider_hysteresis[Slider_WIDGET_CSHL_PARAMETERS_COUNT];
extern uint8 Slider_widgetResolution[Slider_WIDGET_RESOLUTION_PARAMETERS_COUNT];
extern const uint8 CYCODE Slider_numberOfSensors[Slider_SENSORS_TBL_SIZE];
extern const uint8 CYCODE Slider_rawDataIndex[Slider_SENSORS_TBL_SIZE];

extern uint8 Slider_fingerThreshold[Slider_WIDGET_CSHL_PARAMETERS_COUNT];
extern uint8 Slider_idacSettings[Slider_TOTAL_SENSOR_COUNT];
extern uint8 Slider_analogSwitchDivider[Slider_TOTAL_SCANSLOT_COUNT];

extern void SMS_LIB_V3_50_CalculateThresholds(uint8 SensorNumber);
extern void SMS_LIB_V3_50_AutoTune1Ch(void);
extern void SMS_LIB_V3_50_AutoTune2Ch(void);

uint8  * SMS_LIB_noiseThreshold = Slider_noiseThreshold;
uint8 * SMS_LIB_hysteresis = Slider_hysteresis;

uint8 * SMS_LIB_widgetResolution = Slider_widgetResolution;

const uint8 CYCODE * SMS_LIB_widgetNumber = Slider_widgetNumber;
const uint8 CYCODE * SMS_LIB_numberOfSensors = Slider_numberOfSensors;
const uint8 CYCODE * SMS_LIB_rawDataIndex = Slider_rawDataIndex;

uint8 * SMS_LIB_fingerThreshold = Slider_fingerThreshold;
uint8 * SMS_LIB_idacSettings = Slider_idacSettings;
uint8 * SMS_LIB_prescaler = Slider_analogSwitchDivider;

uint16 * SMS_LIB_SensorRaw = Slider_sensorRaw;
uint16 * SMS_LIB_SensorBaseline = Slider_sensorBaseline;

const uint8 CYCODE SMS_LIB_SensorSensitivity[] = {
    2, 2, 2, 2, 2, 
};


const uint8 CYCODE SMS_LIB_PrescalerTbl[] = {
    1u, 1u, 1u, 1u, 1u, 1u, 2u, 2u, 2u, 2u, 2u, 3u, 3u, 3u, 3u, 3u, 3u, 4u, 4u, 4u, 4u, 4u, 4u, 5u, 5u, 5u, 5u, 5u, 5u, 6u, 6u, 6u, 
};



uint8 SMS_LIB_Table2[Slider_TOTAL_SENSOR_COUNT];
uint8 SMS_LIB_Table3[Slider_TOTAL_SENSOR_COUNT];
uint16 SMS_LIB_Table4[Slider_TOTAL_SENSOR_COUNT];
uint16 SMS_LIB_Table5[Slider_TOTAL_SENSOR_COUNT];
uint8 SMS_LIB_Table6[Slider_TOTAL_SENSOR_COUNT];
uint8 SMS_LIB_Table7[Slider_TOTAL_SENSOR_COUNT];

uint8 SMS_LIB_Table8[Slider_END_OF_WIDGETS_INDEX];
uint8 SMS_LIB_Table9[Slider_END_OF_WIDGETS_INDEX];

uint8 SMS_LIB_Var1 = (5u);
uint16 SMS_LIB_Var2 = (10738u);

uint8 SMS_LIB_TotalSnsCnt = Slider_TOTAL_SENSOR_COUNT;
uint8 SMS_LIB_TotalScanSlCnt = Slider_TOTAL_SCANSLOT_COUNT;
uint8 SMS_LIB_EndOfWidgInd = Slider_END_OF_WIDGETS_INDEX;

#if (Slider_DESIGN_TYPE == Slider_TWO_CHANNELS_DESIGN)
    uint8 SMS_LIB_TotalSnsCnt_CH0 = Slider_TOTAL_SENSOR_COUNT__CH0;
    uint8 SMS_LIB_TotalSnsCnt_CH1 = Slider_TOTAL_SENSOR_COUNT__CH1;
#else
    uint8 SMS_LIB_TotalSnsCnt_CH0 = 0u;
    uint8 SMS_LIB_TotalSnsCnt_CH1 = 0u;
#endif  /* (Slider_DESIGN_TYPE == Slider_TWO_CHANNELS_DESIGN) */

/*******************************************************************************
* Function Name: SMS_LIB_ScanSensor
********************************************************************************
*
* Summary:
*  Wrapper to Slider_ScanSensor function.
*
* Parameters:
*  SensorNumber:  Sensor number.
*
* Return:
*  None
*
* Reentrant:
*  No
*
*******************************************************************************/
void SMS_LIB_ScanSensor(uint8 SensorNumber) 
{
    Slider_ScanSensor(SensorNumber);
}

/*******************************************************************************
* Function Name: SMS_LIB_IsBusy
********************************************************************************
*
* Summary:
*  Wrapper to Slider_IsBusy function.
*  
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
uint8 SMS_LIB_IsBusy(void) 
{
    return Slider_IsBusy();
}


/*******************************************************************************
* Function Name: Slider_CalculateThresholds
********************************************************************************
*
* Summary:
*  Wrapper to SMS_LIB_CalculateThresholds function.
*
* Parameters:
*  SensorNumber:  Sensor number.
*
* Return:
*  None
*
* Reentrant:
*  No
*
*******************************************************************************/
void Slider_CalculateThresholds(uint8 SensorNumber) 
{
    SMS_LIB_V3_50_CalculateThresholds(SensorNumber);
}


/*******************************************************************************
* Function Name: Slider_AutoTune
********************************************************************************
*
* Summary:
*  Wrapper for SMS_LIB_AutoTune1Ch or SMS_LIB_AutoTune2Ch function.
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
void Slider_AutoTune(void) 
{
    #if (Slider_DESIGN_TYPE == Slider_ONE_CHANNEL_DESIGN)
        SMS_LIB_V3_50_AutoTune1Ch();
    #elif (Slider_DESIGN_TYPE == Slider_TWO_CHANNELS_DESIGN)
        SMS_LIB_V3_50_AutoTune2Ch();
    #endif /* (Slider_DESIGN_TYPE == Slider_ONE_CHANNEL_DESIGN) */
}

/*******************************************************************************
* Function Name: SMS_LIB_SetPrescaler
********************************************************************************
*
* Summary:
*  Empty wrapper for version compliance.
*
* Parameters:
*  prescaler:  prascaler value.
*
* Return:
*  None
*
*******************************************************************************/
void SMS_LIB_SetPrescaler(uint8 prescaler) 
{
    prescaler = prescaler;
}

void SMS_LIB_V3_50_SetAnalogSwitchesSrc_PRS(void) 
{
	Slider_SetAnalogSwitchesSource(Slider_ANALOG_SWITCHES_SRC_PRS);
}

void SMS_LIB_V3_50_SetAnalogSwitchesSrc_Prescaler(void) 
{
	Slider_SetAnalogSwitchesSource(Slider_ANALOG_SWITCHES_SRC_PRESCALER);
}

#endif  /* (Slider_TUNING_METHOD == Slider_AUTO_TUNING) */


/* [] END OF FILE */
