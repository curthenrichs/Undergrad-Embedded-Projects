/*******************************************************************************
* File Name: Button_SMS_Wrapper.c
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

#include "Button.h"
#include "Button_CSHL.h"

#if (Button_TUNING_METHOD == Button_AUTO_TUNING)

extern uint8 Button_noiseThreshold[Button_WIDGET_CSHL_PARAMETERS_COUNT];
extern uint8 Button_hysteresis[Button_WIDGET_CSHL_PARAMETERS_COUNT];
extern uint8 Button_widgetResolution[Button_WIDGET_RESOLUTION_PARAMETERS_COUNT];
extern const uint8 CYCODE Button_numberOfSensors[Button_SENSORS_TBL_SIZE];
extern const uint8 CYCODE Button_rawDataIndex[Button_SENSORS_TBL_SIZE];

extern uint8 Button_fingerThreshold[Button_WIDGET_CSHL_PARAMETERS_COUNT];
extern uint8 Button_idacSettings[Button_TOTAL_SENSOR_COUNT];
extern uint8 Button_analogSwitchDivider[Button_TOTAL_SCANSLOT_COUNT];

extern void SMS_LIB_V3_50_CalculateThresholds(uint8 SensorNumber);
extern void SMS_LIB_V3_50_AutoTune1Ch(void);
extern void SMS_LIB_V3_50_AutoTune2Ch(void);

uint8  * SMS_LIB_noiseThreshold = Button_noiseThreshold;
uint8 * SMS_LIB_hysteresis = Button_hysteresis;

uint8 * SMS_LIB_widgetResolution = Button_widgetResolution;

const uint8 CYCODE * SMS_LIB_widgetNumber = Button_widgetNumber;
const uint8 CYCODE * SMS_LIB_numberOfSensors = Button_numberOfSensors;
const uint8 CYCODE * SMS_LIB_rawDataIndex = Button_rawDataIndex;

uint8 * SMS_LIB_fingerThreshold = Button_fingerThreshold;
uint8 * SMS_LIB_idacSettings = Button_idacSettings;
uint8 * SMS_LIB_prescaler = Button_analogSwitchDivider;

uint16 * SMS_LIB_SensorRaw = Button_sensorRaw;
uint16 * SMS_LIB_SensorBaseline = Button_sensorBaseline;

const uint8 CYCODE SMS_LIB_SensorSensitivity[] = {
    2, 2, 
};


const uint8 CYCODE SMS_LIB_PrescalerTbl[] = {
    1u, 1u, 1u, 1u, 1u, 1u, 2u, 2u, 2u, 2u, 2u, 3u, 3u, 3u, 3u, 3u, 3u, 4u, 4u, 4u, 4u, 4u, 4u, 5u, 5u, 5u, 5u, 5u, 5u, 6u, 6u, 6u, 
};



uint8 SMS_LIB_Table2[Button_TOTAL_SENSOR_COUNT];
uint8 SMS_LIB_Table3[Button_TOTAL_SENSOR_COUNT];
uint16 SMS_LIB_Table4[Button_TOTAL_SENSOR_COUNT];
uint16 SMS_LIB_Table5[Button_TOTAL_SENSOR_COUNT];
uint8 SMS_LIB_Table6[Button_TOTAL_SENSOR_COUNT];
uint8 SMS_LIB_Table7[Button_TOTAL_SENSOR_COUNT];

uint8 SMS_LIB_Table8[Button_END_OF_WIDGETS_INDEX];
uint8 SMS_LIB_Table9[Button_END_OF_WIDGETS_INDEX];

uint8 SMS_LIB_Var1 = (5u);
uint16 SMS_LIB_Var2 = (10738u);

uint8 SMS_LIB_TotalSnsCnt = Button_TOTAL_SENSOR_COUNT;
uint8 SMS_LIB_TotalScanSlCnt = Button_TOTAL_SCANSLOT_COUNT;
uint8 SMS_LIB_EndOfWidgInd = Button_END_OF_WIDGETS_INDEX;

#if (Button_DESIGN_TYPE == Button_TWO_CHANNELS_DESIGN)
    uint8 SMS_LIB_TotalSnsCnt_CH0 = Button_TOTAL_SENSOR_COUNT__CH0;
    uint8 SMS_LIB_TotalSnsCnt_CH1 = Button_TOTAL_SENSOR_COUNT__CH1;
#else
    uint8 SMS_LIB_TotalSnsCnt_CH0 = 0u;
    uint8 SMS_LIB_TotalSnsCnt_CH1 = 0u;
#endif  /* (Button_DESIGN_TYPE == Button_TWO_CHANNELS_DESIGN) */

/*******************************************************************************
* Function Name: SMS_LIB_ScanSensor
********************************************************************************
*
* Summary:
*  Wrapper to Button_ScanSensor function.
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
    Button_ScanSensor(SensorNumber);
}

/*******************************************************************************
* Function Name: SMS_LIB_IsBusy
********************************************************************************
*
* Summary:
*  Wrapper to Button_IsBusy function.
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
    return Button_IsBusy();
}


/*******************************************************************************
* Function Name: Button_CalculateThresholds
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
void Button_CalculateThresholds(uint8 SensorNumber) 
{
    SMS_LIB_V3_50_CalculateThresholds(SensorNumber);
}


/*******************************************************************************
* Function Name: Button_AutoTune
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
void Button_AutoTune(void) 
{
    #if (Button_DESIGN_TYPE == Button_ONE_CHANNEL_DESIGN)
        SMS_LIB_V3_50_AutoTune1Ch();
    #elif (Button_DESIGN_TYPE == Button_TWO_CHANNELS_DESIGN)
        SMS_LIB_V3_50_AutoTune2Ch();
    #endif /* (Button_DESIGN_TYPE == Button_ONE_CHANNEL_DESIGN) */
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
	Button_SetAnalogSwitchesSource(Button_ANALOG_SWITCHES_SRC_PRS);
}

void SMS_LIB_V3_50_SetAnalogSwitchesSrc_Prescaler(void) 
{
	Button_SetAnalogSwitchesSource(Button_ANALOG_SWITCHES_SRC_PRESCALER);
}

#endif  /* (Button_TUNING_METHOD == Button_AUTO_TUNING) */


/* [] END OF FILE */
