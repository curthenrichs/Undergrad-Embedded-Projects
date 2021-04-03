/*******************************************************************************
* File Name: Button.c
* Version 3.50
*
* Description:
*  This file provides the source code of scanning APIs for the CapSense CSD 
*  component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "Button.h"
#include "cyapicallbacks.h"


static uint8 Button_initVar = 0u;
            
/* Global software variables */
volatile uint8 Button_csv;            /* CapSense CSD status, control variable */
volatile uint8 Button_sensorIndex;    /* Index of scannig sensor */

/* AutoTunning start */
#if (Button_TUNING_METHOD == Button_AUTO_TUNING)
    uint8 Button_lowLevelTuningDone = 1u;
#endif /* (Button_TUNING_METHOD == Button_AUTO_TUNING) */

#if (Button_CURRENT_SOURCE == Button_EXTERNAL_RB)
    uint8  Button_extRbCh0Cur = Button_RBLEED1;
    #if (Button_DESIGN_TYPE == Button_TWO_CHANNELS_DESIGN)
        uint8  Button_extRbCh1Cur = (Button_RBLEED1 + Button_TOTAL_RB_NUMBER__CH0);
    #endif /* (Button_DESIGN_TYPE == Button_TWO_CHANNELS_DESIGN)*/ 
#endif /* (Button_CURRENT_SOURCE == Button_EXTERNAL_RB) */ 
        
/* Global array of Raw Counts */
uint16 Button_sensorRaw[Button_TOTAL_SENSOR_COUNT]; 

uint8 Button_sensorEnableMask[(((Button_TOTAL_SENSOR_COUNT - 1u) / 8u) + 1u)] = {
0x3u, };

uint8 CYXDATA * const CYCODE Button_pcTable[] = {
    (uint8 CYXDATA *)Button_PortCH0__Left__BTN__PC, 
    (uint8 CYXDATA *)Button_PortCH0__Right__BTN__PC, 
};

const uint8 CYCODE Button_portTable[] = {
    Button_PortCH0__Left__BTN__PORT, 
    Button_PortCH0__Right__BTN__PORT, 
};

const uint8 CYCODE Button_maskTable[] = {
    Button_PortCH0__Left__BTN__MASK,
    Button_PortCH0__Right__BTN__MASK,
};

uint8 CYXDATA * const CYCODE Button_csTable[] = {
    (uint8 CYXDATA *)CYREG_PRT0_CAPS_SEL, (uint8 CYXDATA *)CYREG_PRT1_CAPS_SEL, (uint8 CYXDATA *)CYREG_PRT2_CAPS_SEL,
    (uint8 CYXDATA *)CYREG_PRT3_CAPS_SEL, (uint8 CYXDATA *)CYREG_PRT4_CAPS_SEL, (uint8 CYXDATA *)CYREG_PRT5_CAPS_SEL,
    (uint8 CYXDATA *)CYREG_PRT6_CAPS_SEL, (uint8 CYXDATA *)CYREG_PRT15_CAPS_SEL,
};

uint8 Button_idacSettings[] = {
    200u,200u,
};

uint8 Button_widgetResolution[] = {
    Button_PWM_RESOLUTION_10_BITS,
    Button_PWM_RESOLUTION_10_BITS,
};

uint8 Button_analogSwitchDivider[Button_TOTAL_SCANSLOT_COUNT];
const uint8 CYCODE Button_widgetNumber[] = {
    0u, /* Left__BTN */
    1u, /* Right__BTN */
    
};




/*******************************************************************************
* Function Name: Button_Init
********************************************************************************
*
* Summary:
*  Inits default CapSense configuration provided with customizer that defines 
*  mode of component operations and resets all sensors to an inactive state.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Button_Init(void) 
{
    #if ( (Button_PRS_OPTIONS) || \
          (Button_IMPLEMENTATION_CH0 == Button_MEASURE_IMPLEMENTATION_UDB) || \
          ( (Button_DESIGN_TYPE == Button_TWO_CHANNELS_DESIGN) && \
            (Button_IMPLEMENTATION_CH1 == Button_MEASURE_IMPLEMENTATION_UDB)) )
        
        uint8 enableInterrupts;
    #endif /* ( (Button_PRS_OPTIONS) || \
           * (Button_IMPLEMENTATION_CH0 == Button_MEASURE_IMPLEMENTATION_UDB) || \
           * ( (Button_DESIGN_TYPE == Button_TWO_CHANNELS_DESIGN) && \
           * (Button_IMPLEMENTATION_CH1 == Button_MEASURE_IMPLEMENTATION_UDB)) ) 
           */
    
    /* Clear all sensors */
    Button_ClearSensors();

    /* Set Prescaler */
    #if (Button_PRESCALER_OPTIONS == Button_PRESCALER_UDB)
        /* Do nothing = config without prescaler */
    #elif (Button_PRESCALER_OPTIONS == Button_PRESCALER_FF)
        Button_PRESCALER_CONTROL_REG   = (Button_PRESCALER_CTRL_ENABLE |
                                                    Button_PRESCALER_CTRL_MODE_CMP);
                                               
        Button_PRESCALER_CONTROL2_REG |= Button_PRESCALER_CTRL_CMP_LESS_EQ;
    #else
        /* Do nothing = config without prescaler */
    #endif  /* (Button_PRESCALER_OPTIONS == Button_PRESCALER_UDB) */

    /* Set PRS */
    #if (Button_PRS_OPTIONS)
        Button_SetAnalogSwitchesSource(Button_ANALOG_SWITCHES_SRC_PRS);
    #endif /* (Button_PRS_OPTIONS) */

    #if (Button_PRS_OPTIONS == Button_PRS_8BITS)
        /* Aux control set FIFO as REG */
        enableInterrupts = CyEnterCriticalSection();
        Button_AUX_CONTROL_A_REG |= Button_AUXCTRL_FIFO_SINGLE_REG;
        CyExitCriticalSection(enableInterrupts);
        
        /* Write polynomial */
        Button_POLYNOM_REG   = Button_PRS8_DEFAULT_POLYNOM;
        /* Write FIFO with seed */
        Button_SEED_COPY_REG = Button_MEASURE_FULL_RANGE_LOW;
        
    #elif (Button_PRS_OPTIONS == Button_PRS_16BITS)
        /* Aux control set FIFO as REG */ 
        enableInterrupts = CyEnterCriticalSection();  
        Button_AUX_CONTROL_A_REG |= Button_AUXCTRL_FIFO_SINGLE_REG;
        Button_AUX_CONTROL_B_REG |= Button_AUXCTRL_FIFO_SINGLE_REG;
        CyExitCriticalSection(enableInterrupts);
        
        /* Write polynomial */
        CY_SET_REG16(Button_POLYNOM_PTR, Button_PRS16_DEFAULT_POLYNOM);
        /* Write FIFO with seed */
        CY_SET_REG16(Button_SEED_COPY_PTR, Button_MEASURE_FULL_RANGE);
                
    #elif (Button_PRS_OPTIONS == Button_PRS_16BITS_4X)
        /* Aux control set FIFO as REG */
        enableInterrupts = CyEnterCriticalSection();
        Button_AUX_CONTROL_A_REG  |= Button_AUXCTRL_FIFO_SINGLE_REG;
        CyExitCriticalSection(enableInterrupts);
        
        /* Write polynomial */
        Button_POLYNOM_A__D1_REG   = HI8(Button_PRS16_DEFAULT_POLYNOM);
        Button_POLYNOM_A__D0_REG   = LO8(Button_PRS16_DEFAULT_POLYNOM);
        /* Write FIFO with seed */
        Button_SEED_COPY_A__F1_REG = Button_MEASURE_FULL_RANGE_LOW;
        Button_SEED_COPY_A__F0_REG = Button_MEASURE_FULL_RANGE_LOW; 
        
    #else
        /* Do nothing = config without PRS */
    #endif  /* (Button_PRS_OPTIONS == Button_PRS_8BITS) */ 
    
    /* Set ScanSpeed */
    Button_SCANSPEED_PERIOD_REG = Button_SCANSPEED_VALUE;
    
    /* Set the Measure */
    #if (Button_IMPLEMENTATION_CH0 == Button_MEASURE_IMPLEMENTATION_FF)
        /* Window PWM */
        Button_PWM_CH0_CONTROL_REG      = Button_MEASURE_CTRL_ENABLE;
        Button_PWM_CH0_CONTROL2_REG    |= Button_MEASURE_CTRL_PULSEWIDTH;
        CY_SET_REG16(Button_PWM_CH0_COUNTER_PTR, Button_MEASURE_FULL_RANGE);
        
        /* Raw Counter */
        Button_RAW_CH0_CONTROL_REG      = Button_MEASURE_CTRL_ENABLE;
        Button_RAW_CH0_CONTROL2_REG    |= Button_MEASURE_CTRL_PULSEWIDTH;
        CY_SET_REG16(Button_RAW_CH0_PERIOD_PTR, Button_MEASURE_FULL_RANGE);
    
    #else
        /*Window PWM and Raw Counter AUX set */
        enableInterrupts = CyEnterCriticalSection();
        Button_PWM_CH0_AUX_CONTROL_REG |= Button_AUXCTRL_FIFO_SINGLE_REG;
        Button_RAW_CH0_AUX_CONTROL_REG |= Button_AUXCTRL_FIFO_SINGLE_REG;
        CyExitCriticalSection(enableInterrupts);
        
        /* Window PWM */
        Button_PWM_CH0_ADD_VALUE_REG    = Button_MEASURE_FULL_RANGE_LOW;
        Button_PWM_CH0_PERIOD_LO_REG    = Button_MEASURE_FULL_RANGE_LOW;
        Button_PWM_CH0_COUNTER_LO_REG   = Button_MEASURE_FULL_RANGE_LOW;
        
        /* Raw Counter */
        Button_RAW_CH0_ADD_VALUE_REG    = Button_MEASURE_FULL_RANGE_LOW;
        Button_RAW_CH0_PERIOD_HI_REG    = Button_MEASURE_FULL_RANGE_LOW;
        Button_RAW_CH0_PERIOD_LO_REG    = Button_MEASURE_FULL_RANGE_LOW;
        
    #endif  /* (Button_IMPLEMENTATION_CH0 == Button_MEASURE_IMPLEMENTATION_FF) */ 
    
    #if (Button_DESIGN_TYPE == Button_TWO_CHANNELS_DESIGN)
        #if (Button_IMPLEMENTATION_CH1 == Button_MEASURE_IMPLEMENTATION_FF)
            /* Window PWM */
            Button_PWM_CH1_CONTROL_REG      = Button_MEASURE_CTRL_ENABLE;
            Button_PWM_CH1_CONTROL2_REG    |= Button_MEASURE_CTRL_PULSEWIDTH;
            CY_SET_REG16(Button_PWM_CH1_COUNTER_PTR, Button_MEASURE_FULL_RANGE);
            
            /* Raw Counter */
            Button_RAW_CH1_CONTROL_REG      = Button_MEASURE_CTRL_ENABLE;
            Button_RAW_CH1_CONTROL2_REG    |= Button_MEASURE_CTRL_PULSEWIDTH;
            CY_SET_REG16(Button_RAW_CH1_PERIOD_PTR, Button_MEASURE_FULL_RANGE);
           
        #else
            /*Window PWM and Raw Counter AUX set */
            enableInterrupts = CyEnterCriticalSection();
            Button_PWM_CH1_AUX_CONTROL_REG |= Button_AUXCTRL_FIFO_SINGLE_REG;
            Button_RAW_CH1_AUX_CONTROL_REG |= Button_AUXCTRL_FIFO_SINGLE_REG;
            CyExitCriticalSection(enableInterrupts);
            
            /* Window PWM */
            Button_PWM_CH1_ADD_VALUE_REG    = Button_MEASURE_FULL_RANGE_LOW;
            Button_PWM_CH1_PERIOD_LO_REG    = Button_MEASURE_FULL_RANGE_LOW;
            Button_PWM_CH1_COUNTER_LO_REG   = Button_MEASURE_FULL_RANGE_LOW;
            
            /* Raw Counter */
            
            Button_RAW_CH1_ADD_VALUE_REG    = Button_MEASURE_FULL_RANGE_LOW;
            Button_RAW_CH1_PERIOD_HI_REG    = Button_MEASURE_FULL_RANGE_LOW;
            Button_RAW_CH1_PERIOD_LO_REG    = Button_MEASURE_FULL_RANGE_LOW;
            
        #endif  /* (Button_IMPLEMENTATION_CH1 == Button_MEASURE_IMPLEMENTATION_FF) */
    
    #endif  /* (Button_DESIGN_TYPE == TWO_CHANNELS_DESIGN) */
    
    /* Setup ISR */
    CyIntDisable(Button_IsrCH0_ISR_NUMBER);
    (void)CyIntSetVector(Button_IsrCH0_ISR_NUMBER, &Button_IsrCH0_ISR);
    CyIntSetPriority(Button_IsrCH0_ISR_NUMBER, Button_IsrCH0_ISR_PRIORITY);
    
    #if (Button_DESIGN_TYPE == Button_TWO_CHANNELS_DESIGN)
        CyIntDisable(Button_IsrCH1_ISR_NUMBER);
        CyIntSetVector(Button_IsrCH1_ISR_NUMBER, Button_IsrCH1_ISR);
        CyIntSetPriority(Button_IsrCH1_ISR_NUMBER, Button_IsrCH1_ISR_PRIORITY);
    #endif  /* Button_DESIGN_TYPE */
    
    /* Setup AMux Bus: Connect Cmod, Cmp, Idac */
    Button_AMuxCH0_Init();
    Button_AMuxCH0_Connect(Button_AMuxCH0_CMOD_CHANNEL);
    Button_AMuxCH0_Connect(Button_AMuxCH0_CMP_VP_CHANNEL);
    #if (Button_CURRENT_SOURCE)
        Button_AMuxCH0_Connect(Button_AMuxCH0_IDAC_CHANNEL);
    #endif  /* Button_CURRENT_SOURCE */
    
    #if (Button_DESIGN_TYPE == Button_TWO_CHANNELS_DESIGN) 
        Button_AMuxCH1_Init();
        Button_AMuxCH1_Connect(Button_AMuxCH1_CMOD_CHANNEL);
        Button_AMuxCH1_Connect(Button_AMuxCH1_CMP_VP_CHANNEL);
        #if (Button_CURRENT_SOURCE)
            Button_AMuxCH1_Connect(Button_AMuxCH1_IDAC_CHANNEL);
        #endif  /* Button_CURRENT_SOURCE */
    #endif  /* Button_DESIGN_TYPE */
    
    /* Int Rb */
    #if (Button_CURRENT_SOURCE == Button_EXTERNAL_RB)
        Button_InitRb();
    #endif /* (Button_CURRENT_SOURCE == Button_EXTERNAL_RB) */
    
    /* Enable window generation */
    Button_CONTROL_REG |= Button_CTRL_WINDOW_EN__CH0;
    #if (Button_DESIGN_TYPE == Button_TWO_CHANNELS_DESIGN)
        Button_CONTROL_REG |= Button_CTRL_WINDOW_EN__CH1;
    #endif  /* Button_DESIGN_TYPE */
    
    /* Initialize Cmp and Idac */
    Button_CompCH0_Init();
    #if (Button_CURRENT_SOURCE)
        Button_IdacCH0_Init();
        Button_IdacCH0_SetPolarity(Button_IdacCH0_IDIR);
        Button_IdacCH0_SetRange(Button_IDAC_RANGE_VALUE);
        Button_IdacCH0_SetValue(Button_TURN_OFF_IDAC);
    #endif  /* Button_CURRENT_SOURCE */
    
    #if (Button_DESIGN_TYPE == Button_TWO_CHANNELS_DESIGN) 
        Button_CompCH1_Init();
        #if (Button_CURRENT_SOURCE)
            Button_IdacCH1_Init();
            Button_IdacCH1_SetPolarity(Button_IdacCH1_IDIR);
            Button_IdacCH1_SetRange(Button_IDAC_RANGE_VALUE);
            Button_IdacCH1_SetValue(Button_TURN_OFF_IDAC);
        #endif  /* Button_CURRENT_SOURCE */
    #endif  /* Button_DESIGN_TYPE */
    
    /* Initialize Vref if as VDAC */
    #if (Button_VREF_OPTIONS == Button_VREF_VDAC)
        Button_VdacRefCH0_Init();
        #if (Button_DESIGN_TYPE == Button_TWO_CHANNELS_DESIGN)
            Button_VdacRefCH1_Init();
        #endif  /* Button_DESIGN_TYPE */
    #endif  /* Button_VREF_OPTIONS */
}


/*******************************************************************************
* Function Name: Button_Enable
********************************************************************************
*
* Summary:
*  Enables active mode power template bits for number of component used within 
*  CapSense.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Button_Enable(void) 
{
    uint8 enableInterrupts;
    
    enableInterrupts = CyEnterCriticalSection();
    
    /* Enable Prescaler */
    #if (Button_PRESCALER_OPTIONS == Button_PRESCALER_UDB)
        /* Do nothing  for UDB */
    #elif (Button_PRESCALER_OPTIONS == Button_PRESCALER_FF)
        Button_PRESCALER_ACT_PWRMGR_REG  |= Button_PRESCALER_ACT_PWR_EN;
        Button_PRESCALER_STBY_PWRMGR_REG |= Button_PRESCALER_STBY_PWR_EN;
        
    #else
        /* Do nothing = config without prescaler */
    #endif  /* (Button_PRESCALER_OPTIONS == Button_PRESCALER_UDB) */
    
    /* Enable ScanSpeed */
    Button_SCANSPEED_AUX_CONTROL_REG |= Button_SCANSPEED_CTRL_ENABLE;
    
    /* Enable Measure CH0 */
    #if (Button_IMPLEMENTATION_CH0 == Button_MEASURE_IMPLEMENTATION_FF)
        /* Window PWM */
        Button_PWM_CH0_ACT_PWRMGR_REG  |= Button_PWM_CH0_ACT_PWR_EN;
        Button_PWM_CH0_STBY_PWRMGR_REG |= Button_PWM_CH0_STBY_PWR_EN;
        
        /* Raw Counter */
        Button_RAW_CH0_ACT_PWRMGR_REG  |= Button_RAW_CH0_ACT_PWR_EN;
        Button_RAW_CH0_STBY_PWRMGR_REG |= Button_RAW_CH0_STBY_PWR_EN;
        
    #else
        /* Window PWM -  Do nothing */
        /* Raw Counter - Do nothing */
        
    #endif  /* (Button_IMPLEMENTATION_CH0 == Button_MEASURE_IMPLEMENTATION_FF) */ 
    
    /* Enable Measure CH1*/
    #if (Button_DESIGN_TYPE == Button_TWO_CHANNELS_DESIGN)
        #if (Button_IMPLEMENTATION_CH1 == Button_MEASURE_IMPLEMENTATION_FF)
            /* Window PWM */
            Button_PWM_CH1_ACT_PWRMGR_REG  |= Button_PWM_CH1_ACT_PWR_EN;
            Button_PWM_CH1_STBY_PWRMGR_REG |= Button_PWM_CH1_STBY_PWR_EN;
            
            /* Raw Counter */
            Button_RAW_CH1_ACT_PWRMGR_REG  |= Button_RAW_CH1_ACT_PWR_EN;
            Button_RAW_CH1_STBY_PWRMGR_REG |= Button_RAW_CH1_STBY_PWR_EN;
           
        #else
        /* Window PWM -  Do nothing */
        /* Raw Counter - Do nothing */
        
        #endif  /* (Button_IMPLEMENTATION_CH1 == Button_MEASURE_IMPLEMENTATION_FF) */
    
    #endif  /* (Button_DESIGN_TYPE == TWO_CHANNELS_DESIGN)*/
    
    /* Enable the Clock */
    #if (Button_CLOCK_SOURCE == Button_INTERNAL_CLOCK)
       Button_IntClock_Enable();
    #endif  /* Button_CLOCK_SOURCE */
    
    /* Setup Cmp and Idac */
    Button_CompCH0_Enable();
    #if (Button_CURRENT_SOURCE)
        Button_IdacCH0_Enable();
    #endif  /* Button_CURRENT_SOURCE */
    
    #if (Button_DESIGN_TYPE == Button_TWO_CHANNELS_DESIGN) 
        Button_CompCH1_Enable();
        #if (Button_CURRENT_SOURCE)
            Button_IdacCH1_Enable();
        #endif  /* Button_CURRENT_SOURCE */
    #endif  /* Button_DESIGN_TYPE */
    
    /* Enable Vref */
    #if (Button_VREF_OPTIONS == Button_VREF_VDAC)
        Button_VdacRefCH0_Enable();
        Button_VdacRefCH0_SetValue(Button_VdacRefCH0_DEFAULT_DATA);
        #if (Button_DESIGN_TYPE == Button_TWO_CHANNELS_DESIGN)
            Button_VdacRefCH1_Enable();
            Button_VdacRefCH1_SetValue(Button_VdacRefCH1_DEFAULT_DATA);
        #endif  /* (Button_DESIGN_TYPE == Button_TWO_CHANNELS_DESIGN) */
    #else
        /* Enable CapSense Buf */
        Button_BufCH0_STBY_PWRMGR_REG |= Button_BufCH0_STBY_PWR_EN;
        Button_BufCH0_ACT_PWRMGR_REG  |= Button_BufCH0_ACT_PWR_EN;
        
        #if (Button_DESIGN_TYPE == Button_TWO_CHANNELS_DESIGN)
            Button_BufCH1_STBY_PWRMGR_REG |= Button_BufCH1_STBY_PWR_EN;
            Button_BufCH1_ACT_PWRMGR_REG  |= Button_BufCH1_ACT_PWR_EN;
        #endif  /* (Button_DESIGN_TYPE == Button_TWO_CHANNELS_DESIGN) */
    #endif  /* (Button_VREF_VDAC == Button_VREF_OPTIONS) */
    
    /* Set reference on AMux Bus */
    #if (Button_VREF_OPTIONS == Button_VREF_VDAC)
        /* Connect Vdac to AMux Bus */
        Button_AMuxCH0_Connect(Button_AMuxCH0_VREF_CHANNEL);
        #if (Button_DESIGN_TYPE == Button_TWO_CHANNELS_DESIGN)
            Button_AMuxCH1_Connect(Button_AMuxCH1_VREF_CHANNEL);
        #endif  /* (Button_DESIGN_TYPE == Button_TWO_CHANNELS_DESIGN) */
        
    #else
        /* Enable CapSense Buf */
        Button_BufCH0_CAPS_CFG0_REG |= Button_CSBUF_ENABLE;
        
        #if (Button_DESIGN_TYPE == Button_TWO_CHANNELS_DESIGN)
            Button_BufCH1_CAPS_CFG0_REG |= Button_CSBUF_ENABLE;
        #endif  /* (Button_DESIGN_TYPE == Button_TWO_CHANNELS_DESIGN) */
    #endif  /* (Button_VREF_VDAC == Button_VREF_OPTIONS)*/
    
    CyExitCriticalSection(enableInterrupts);
    
    /* Enable interrupt */
    CyIntEnable(Button_IsrCH0_ISR_NUMBER);
    #if (Button_DESIGN_TYPE == Button_TWO_CHANNELS_DESIGN) 
        CyIntEnable(Button_IsrCH1_ISR_NUMBER);
    #endif  /* Button_DESIGN_TYPE */
    
    /* Set CapSense Enable state */
    Button_CONTROL_REG |= Button_CTRL_CAPSENSE_EN;
}


/*******************************************************************************
* Function Name: Button_Start
********************************************************************************
*
* Summary:
*  Initializes registers and starts the CSD method of CapSense component. Reset 
*  all sensors to an inactive state. Enables interrupts for sensors scanning.
*  When Auto Tuning (SmartSense) mode is selected the tuning procedure is 
*  applied for all sensors.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  Button_initVar - used to check initial configuration, modified on 
*  first function call.
*  Button_lowLevelTuningDone - used to notify the Tuner GUI that 
*  tuning of scanning parameters are done.
*
* Reentrant:
*  No
*
* Note:
*  All widgets are enabled by default except proximity widgets. Proximity widgets 
*  must be manually enabled as their long scan time is incompatible 
*  with the fast response required of other widget types. 
*
*******************************************************************************/
void Button_Start(void)  
{
    if (Button_initVar == 0u)
    {
        Button_Init();
        Button_initVar = 1u;
    }
    Button_Enable();
    
    /* AutoTunning start */
    #if (Button_TUNING_METHOD == Button_AUTO_TUNING)
        /* AutoTune by sensor or pair of sensor basis */
        Button_AutoTune();
        Button_lowLevelTuningDone = 1u;
    #endif /* (Button_TUNING_METHOD == Button_AUTO_TUNING) */
}


/*******************************************************************************
* Function Name: Button_Stop
********************************************************************************
*
* Summary:
*  Stops the sensors scanner, disables internal interrupts, and resets all 
*  sensors to an inactive state. Disables Active mode power template bits for 
*  number of component used within CapSense.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  This function should be called after scans will be completed.
*
*******************************************************************************/
void Button_Stop(void) 
{
    /* Stop Capsensing */
    Button_CONTROL_REG &= (uint8)(~Button_CTRL_START);
    
    /* Disable interrupt */
    CyIntDisable(Button_IsrCH0_ISR_NUMBER);
    #if (Button_DESIGN_TYPE == Button_TWO_CHANNELS_DESIGN) 
        CyIntDisable(Button_IsrCH1_ISR_NUMBER);
    #endif  /* Button_DESIGN_TYPE */
    
    /* Clear all sensors */
    Button_ClearSensors();
    
    /* Disable Prescaler */
    #if (Button_PRESCALER_OPTIONS == Button_PRESCALER_UDB)
        /* Do nothing  for UDB */
    #elif (Button_PRESCALER_OPTIONS == Button_PRESCALER_FF)        
        Button_PRESCALER_ACT_PWRMGR_REG  &= ~Button_PRESCALER_ACT_PWR_EN;
        Button_PRESCALER_STBY_PWRMGR_REG &= ~Button_PRESCALER_STBY_PWR_EN;
        
    #else
        /* Do nothing = config without prescaler */
    #endif  /* (Button_PRESCALER_OPTIONS == Button_PRESCALER_UDB) */
    
    /* Disable ScanSpeed */
    Button_SCANSPEED_AUX_CONTROL_REG &= (uint8)(~Button_SCANSPEED_CTRL_ENABLE);
    
    /* Disable Measure CH0 */
    #if (Button_IMPLEMENTATION_CH0 == Button_MEASURE_IMPLEMENTATION_FF)
        /* Window PWM */
        Button_PWM_CH0_ACT_PWRMGR_REG  &= ~Button_PWM_CH0_ACT_PWR_EN;
        Button_PWM_CH0_STBY_PWRMGR_REG &= ~Button_PWM_CH0_STBY_PWR_EN;

        /* Raw Counter */
        Button_RAW_CH0_ACT_PWRMGR_REG  &= ~Button_RAW_CH0_ACT_PWR_EN;
        Button_RAW_CH0_STBY_PWRMGR_REG &= ~Button_RAW_CH0_STBY_PWR_EN;

    #else
        /* Window PWM -  Do nothing */
        /* Raw Counter - Do nothing */
        
    #endif  /* (Button_IMPLEMENTATION_CH0 == Button_MEASURE_IMPLEMENTATION_FF) */ 
    
    /* Disable Measure CH1 */
    #if (Button_DESIGN_TYPE == Button_TWO_CHANNELS_DESIGN)
        #if (Button_IMPLEMENTATION_CH1 == Button_MEASURE_IMPLEMENTATION_FF)
            /* Window PWM */
            Button_PWM_CH1_ACT_PWRMGR_REG  &= ~Button_PWM_CH1_ACT_PWR_EN;
            Button_PWM_CH1_STBY_PWRMGR_REG &= ~Button_PWM_CH1_STBY_PWR_EN;
    
            /* Raw Counter */
            Button_RAW_CH1_ACT_PWRMGR_REG  &= ~Button_RAW_CH1_ACT_PWR_EN;
            Button_RAW_CH1_STBY_PWRMGR_REG &= ~Button_RAW_CH1_STBY_PWR_EN;
           
        #else
        /* Window PWM -  Do nothing */
        /* Raw Counter - Do nothing */
        
        #endif  /* (Button_IMPLEMENTATION_CH1 == Button_MEASURE_IMPLEMENTATION_FF) */
    
    #endif  /* (Button_DESIGN_TYPE == TWO_CHANNELS_DESIGN)*/
    
    /* Disable the Clock */
    #if (Button_CLOCK_SOURCE == Button_INTERNAL_CLOCK)
       Button_IntClock_Stop();
    #endif  /* Button_CLOCK_SOURCE */
    
    /* Disable power from Cmp and Idac */
    Button_CompCH0_Stop();
    #if (Button_CURRENT_SOURCE)
        Button_IdacCH0_Stop();
    #endif  /* Button_CURRENT_SOURCE */
    
    #if (Button_DESIGN_TYPE == Button_TWO_CHANNELS_DESIGN) 
        Button_CompCH1_Stop();
        #if (Button_CURRENT_SOURCE)
            Button_IdacCH1_Stop();
        #endif  /* Button_CURRENT_SOURCE */
    #endif  /* Button_DESIGN_TYPE */    
    
    /* Disable Vref if as VDAC */
    #if (Button_VREF_OPTIONS == Button_VREF_VDAC)
        Button_VdacRefCH0_Stop();
        #if (Button_DESIGN_TYPE == Button_TWO_CHANNELS_DESIGN)
            Button_VdacRefCH1_Stop();
        #endif  /* Button_DESIGN_TYPE */
    #endif  /* Button_VREF_OPTIONS */

    #if (Button_VREF_VDAC == Button_VREF_OPTIONS)
        /* The Idac turn off before */
    #else
        /* Enable CapSense Buf */
        Button_BufCH0_CAPS_CFG0_REG &= (uint8)(~Button_CSBUF_ENABLE);
        Button_BufCH0_ACT_PWRMGR_REG &= (uint8)(~Button_BufCH0_ACT_PWR_EN);
        Button_BufCH0_STBY_PWRMGR_REG &= (uint8)(~Button_BufCH0_STBY_PWR_EN);
        
        #if (Button_DESIGN_TYPE == Button_TWO_CHANNELS_DESIGN)
            Button_BufCH1_CAPS_CFG0_REG &= (uint8)(~Button_CSBUF_ENABLE);
            Button_BufCH1_ACT_PWRMGR_REG &= (uint8)(~Button_BufCH1_ACT_PWR_EN);
            Button_BufCH1_STBY_PWRMGR_REG &= (uint8)(~Button_BufCH1_STBY_PWR_EN);
        #endif  /*(Button_DESIGN_TYPE == Button_TWO_CHANNELS_DESIGN) */
    #endif  /* (Button_VREF_VDAC == Button_VREF_OPTIONS) */
    
    /* Set CapSense Disable state */
    Button_CONTROL_REG &= (uint8)(~Button_CTRL_CAPSENSE_EN);
}


#if (Button_DESIGN_TYPE == Button_ONE_CHANNEL_DESIGN)
    /*******************************************************************************
    * Function Name: Button_FindNextSensor
    ********************************************************************************
    *
    * Summary:
    *  Finds next sensor to scan. 
    *
    * Parameters:
    *  snsIndex:  Current index of sensor.
    *
    * Return:
    *  Returns the next sensor index to scan.
    *
    * Global Variables:
    *  Button_sensorEnableMask[ ] - used to store bit masks of enabled 
    *  sensors.
    *  Button_sensorEnableMask[0] contains the masked bits for sensors 0
    *  through 7 (sensor 0 is bit 0, sensor 1 is bit 1).
    *  Button_sensorEnableMask[1] contains the masked bits for sensors 
    *  8 through 15 (if needed), and so on.
    *    0 - sensor doesn't scan by Button_ScanEnabledWidgets().
    *    1 - sensor scans by Button_ScanEnabledWidgets().
    *
    * Note: 
    *  This function has effect on current scanning scanning and should not
    *  be used outisde of component.
    *
    *******************************************************************************/
    uint8 Button_FindNextSensor(uint8 snsIndex) CYREENTRANT
    {
        uint8 pos;
        uint8 enMask;
        
        /* Check if sensor enabled */
        do
        {
            /* Proceed with the next sensor */
            snsIndex++;
            if(snsIndex == Button_TOTAL_SENSOR_COUNT)
            {
                break;
            }
            pos = (snsIndex >> 3u);
            enMask = 0x01u << (snsIndex & 0x07u);
        }    
        while((Button_sensorEnableMask[pos] & enMask) == 0u);
        
        return snsIndex;
    }
 #endif  /* Button_DESIGN_TYPE == Button_ONE_CHANNEL_DESIGN */
 
 
#if (Button_DESIGN_TYPE == Button_TWO_CHANNELS_DESIGN)
    /*******************************************************************************
    * Function Name: Button_FindNextPair
    ********************************************************************************
    *
    * Summary:
    *  Finds next pair or sensor to scan. Sets condition bits to skip scanning.
    *  
    * Parameters:
    *  snsIndex:  Current index pair of sensors.
    *
    * Return:
    *  Returns the next pair of sensors index to scan.
    *
    * Global Variables:
    *  Button_sensorEnableMask[ ] - used to store bit masks of enabled 
    *  sensors.
    *  Button_sensorEnableMask[0] contains the masked bits for sensors 0
    *  through 7 (sensor 0 is bit 0, sensor 1 is bit 1).
    *  Button_sensorEnableMask[1] contains the masked bits for sensors 
    *  8 through 15 (if needed), and so on.
    *    0 - sensor doesn't scan by Button_ScanEnabledWidgets().
    *    1 - sensor scans by Button_ScanEnabledWidgets().
    *
    * Note: 
    *  This function has effect on control signals set for scanning and should not
    *  be used outisde of component.
    *
    *******************************************************************************/
    uint8 Button_FindNextPair(uint8 snsIndex) CYREENTRANT
    {
        uint8 posCh;
        uint8 enMaskCh;
        uint8 newRegValue;
        uint8 indexCh0 = snsIndex;
        uint8 indexCh1 = snsIndex + Button_TOTAL_SENSOR_COUNT__CH0;
        
        /* Find enabled sensor on channel 0 */
        do
        {
            /* Procced the scanning */
            indexCh0++;
            if (indexCh0 >= Button_TOTAL_SENSOR_COUNT__CH0)
            {
                /* Lets hadle now all from CH1 */
                indexCh0 = Button_END_OF_SCAN__CH0;
                break;
            }
            
            posCh = (indexCh0 >> 3u);
            enMaskCh = 0x01u << (indexCh0 & 0x07u);
        }
        while((Button_sensorEnableMask[posCh] & enMaskCh) == 0u);
        
        /* Find enabled sensor on channel 1 */
        do
        {
            /* Procced the scanning */
            indexCh1++;        
            if (indexCh1 >= Button_TOTAL_SENSOR_COUNT)
            {
                /* Lets hadle now all from CH0 */
                indexCh1 = Button_END_OF_SCAN__CH1;
                break;
            }
            
            posCh = (indexCh1 >> 3u);
            enMaskCh = 0x01u << (indexCh1 & 0x07u);
        } 
        while((Button_sensorEnableMask[posCh] & enMaskCh) == 0u);
        
        indexCh1 -= Button_TOTAL_SENSOR_COUNT__CH0;
        
        /* Find the pair to scan */
        if(indexCh0 == indexCh1)
        {
            /* Scans TWO Channels */
            snsIndex = indexCh0;
            
            Button_CONTROL_REG |= (Button_CTRL_WINDOW_EN__CH0 | 
                                             Button_CTRL_WINDOW_EN__CH1);
        }
        else if(indexCh0 < indexCh1)
        {
           /* Scans Channel ONE only */
           snsIndex = indexCh0;
           
            newRegValue = Button_CONTROL_REG;
            newRegValue |= Button_CTRL_WINDOW_EN__CH0;
            newRegValue &= ~Button_CTRL_WINDOW_EN__CH1;
            Button_CONTROL_REG = newRegValue;
        }
        else
        {
            /* Scans Channel TWO only */
            snsIndex = indexCh1;
            
            newRegValue = Button_CONTROL_REG;
            newRegValue |= Button_CTRL_WINDOW_EN__CH1;
            newRegValue &= ~Button_CTRL_WINDOW_EN__CH0;
            Button_CONTROL_REG = newRegValue;
        }
        
        return (snsIndex);
    }
#endif  /* Button_DESIGN_TYPE == Button_TWO_CHANNELS_DESIGN */


/*******************************************************************************
* Function Name: Button_SetScanSlotSettings
********************************************************************************
*
* Summary:
*  Sets the scan settings of the selected scan slot (sensor or pair of sensors). 
*  The scan settings incorporate IDAC value (for IDAC configurations) for every 
*  sensor and resolution. The resolution is the same for all sensors within 
*  widget.
*
* Parameters:
*  slot:  Scan slot number (sensor or pair of sensors).
*
* Return:
*  None
*
* Global Variables:
*  Button_idacSettings[] - used to store idac value for every sensor.
*  Button_widgetResolution[] - used to store scan resolution of every 
*  widget.
*
*******************************************************************************/
void Button_SetScanSlotSettings(uint8 slot) CYREENTRANT
{
    uint8 widget;
    
    #if (Button_DESIGN_TYPE == Button_ONE_CHANNEL_DESIGN)
        /* Define widget sensor belongs to */
        widget = Button_widgetNumber[slot];
        
        /* Set Idac Value */
        #if (Button_CURRENT_SOURCE)
            Button_IdacCH0_SetValue(Button_idacSettings[slot]);
        #endif  /* Button_CURRENT_SOURCE */
        
        /* Window PWM */
        #if (Button_IMPLEMENTATION_CH0 == Button_MEASURE_IMPLEMENTATION_FF)
            CY_SET_REG16(Button_PWM_CH0_PERIOD_PTR,
                ((uint16) Button_widgetResolution[widget] << 8u) | Button_MEASURE_FULL_RANGE_LOW);
        #else
            Button_PWM_CH0_PERIOD_HI_REG = Button_widgetResolution[widget];
        #endif  /* (Button_IMPLEMENTATION_CH0 == Button_MEASURE_IMPLEMENTATION_FF) */ 

        #if ( (Button_MULTIPLE_PRESCALER_ENABLED) || \
              (Button_TUNING_METHOD == Button_AUTO_TUNING) )
            Button_SetPrescaler(Button_analogSwitchDivider[slot]);
        #elif (Button_PRESCALER_OPTIONS)
            Button_SetPrescaler(Button_analogSwitchDivider);
        #endif /* ((Button_MULTIPLE_PRESCALER_ENABLED) || \
               *   (Button_TUNING_METHOD == Button_AUTO_TUNING))
               */

    #else
        if(slot < Button_TOTAL_SENSOR_COUNT__CH0)
        {
            /* Define widget sensor belongs to */
            widget = Button_widgetNumber[slot];
            
            /* Set Idac Value */
            #if (Button_CURRENT_SOURCE)
                Button_IdacCH0_SetValue(Button_idacSettings[slot]);
            #endif  /* Button_CURRENT_SOURCE */
            
            /* Set Pwm Resolution */
            #if (Button_IMPLEMENTATION_CH0 == Button_MEASURE_IMPLEMENTATION_FF)
                CY_SET_REG16(Button_PWM_CH0_PERIOD_PTR,
                  ((uint16) Button_widgetResolution[widget] << 8u) | Button_MEASURE_FULL_RANGE_LOW);
            #else
                Button_PWM_CH0_PERIOD_HI_REG = Button_widgetResolution[widget];
            #endif  /* (Button_IMPLEMENTATION_CH0 == Button_MEASURE_IMPLEMENTATION_FF)*/ 
        }
        
        if(slot < Button_TOTAL_SENSOR_COUNT__CH1)
        {
            widget = Button_widgetNumber[slot+Button_TOTAL_SENSOR_COUNT__CH0];
        
            /* Set Idac Value */
            #if (Button_CURRENT_SOURCE)
                Button_IdacCH1_SetValue(Button_idacSettings[slot+
                                                                             Button_TOTAL_SENSOR_COUNT__CH0]);
            #endif  /* Button_CURRENT_SOURCE */
            
            /* Set Pwm Resolution */
            #if (Button_IMPLEMENTATION_CH1 == Button_MEASURE_IMPLEMENTATION_FF)
                CY_SET_REG16(Button_PWM_CH1_PERIOD_PTR,
                  ((uint16) Button_widgetResolution[widget] << 8u) | Button_MEASURE_FULL_RANGE_LOW);
            #else
                Button_PWM_CH1_PERIOD_HI_REG = Button_widgetResolution[widget];
            #endif  /* (Button_IMPLEMENTATION_CH1 == Button_MEASURE_IMPLEMENTATION_FF)*/ 
        }

        #if ( (Button_MULTIPLE_PRESCALER_ENABLED) || \
              (Button_TUNING_METHOD == Button_AUTO_TUNING) )
            Button_SetPrescaler(Button_analogSwitchDivider[slot]);
        #elif (Button_PRESCALER_OPTIONS)
            Button_SetPrescaler(Button_analogSwitchDivider);
        #endif /* ((Button_MULTIPLE_PRESCALER_ENABLED) || \
               *   (Button_TUNING_METHOD == Button_AUTO_TUNING))
               */

    #endif  /* Button_DESIGN_TYPE */
}


/*******************************************************************************
* Function Name: Button_ScanSensor
********************************************************************************
*
* Summary:
*  Sets scan settings and starts scanning a sensor or pair of combined sensors
*  on each channel. If two channels are configured, two sensors may be scanned 
*  at the same time. After scanning is complete the isr copies the measured 
*  sensor raw data to the global array. Use of the isr ensures this function 
*  is non-blocking. Each sensor has a unique number within the sensor array. 
*  This number is assigned by the CapSense customizer in sequence.
*
* Parameters:
*  sensor:  Sensor number.
*
* Return:
*  None
*
* Global Variables:
*  Button_csv - used to provide status and mode of scanning process. 
*  Sets busy status(scan in progress) and mode of scan as single scan.
*  For two channel design the additional bits are set to define if scan a 
*  pair of sensors or single one.
*  Button_sensorIndex - used to store sensor scanning sensor number.
*  Sets to provided sensor argument.
*
* Reentrant:
*  No
*
*******************************************************************************/
void Button_ScanSensor(uint8 sensor)  
{
    /* Clears status/control variable and set sensorIndex */
    Button_csv = 0u;
    Button_sensorIndex = sensor;
    
    #if (Button_DESIGN_TYPE == Button_ONE_CHANNEL_DESIGN)
        /* Start of sensor scan */
        Button_csv = (Button_SW_STS_BUSY | Button_SW_CTRL_SINGLE_SCAN);
        Button_PreScan(sensor);
        
    #else
        /* CH0: check end of scan conditions */
        if(sensor < Button_TOTAL_SENSOR_COUNT__CH0)
        {
            Button_CONTROL_REG |= Button_CTRL_WINDOW_EN__CH0;
        }
        else
        {
            Button_CONTROL_REG &= ~Button_CTRL_WINDOW_EN__CH0;
        }
        
        /* CH1: check end of scan conditions */
        if(sensor < Button_TOTAL_SENSOR_COUNT__CH1)
        {
            Button_CONTROL_REG |= Button_CTRL_WINDOW_EN__CH1;
        }
        else
        {
            Button_CONTROL_REG &= ~Button_CTRL_WINDOW_EN__CH1;
        }
        
        /* Start sensor scan */
        if( ((Button_CONTROL_REG & Button_CTRL_WINDOW_EN__CH0) != 0u) || 
            ((Button_CONTROL_REG & Button_CTRL_WINDOW_EN__CH1) != 0u) )
        {
        
            Button_csv |= (Button_SW_STS_BUSY | Button_SW_CTRL_SINGLE_SCAN);
            Button_PreScan(sensor);
        }
        
    #endif  /* Button_DESIGN_TYPE */
}


/*******************************************************************************
* Function Name: Button_ScanEnableWidgets
********************************************************************************
*
* Summary:
*  Scans all of the enabled widgets. Starts scanning a sensor or pair of sensors 
*  within enabled widget. The isr proceeding scanning next sensor or pair till 
*  all enabled widgets will be scanned. Use of the isr ensures this function is 
*  non-blocking. All widgets are enabled by default except proximity widgets. 
*  Proximity widgets must be manually enabled as their long scan time is 
*  incompatible with fast response desired of other widget types.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  Button_csv - used to provide status and mode of scanning process. 
*  Sets busy status(scan in progress) and clears single scan mode.
*  For two channel design the additional bits are set to define if scan a 
*  pair of sensors or single one. 
*  Button_sensorIndex - used to store sensor scanning sensor number.
*  Sets to 0xFF and provided to function Button_FindNextSensor or
*  Button_FindNextPair, these functions starts with sensor index
*  increment and overflow of uint8 gives desired index 0.
*
* Reentrant:
*  No
*
*******************************************************************************/
void Button_ScanEnabledWidgets(void) 
{
    /* Clears status/control variable and set sensorIndex */
    Button_csv = 0u;
    Button_sensorIndex = 0xFFu;
    
    #if (Button_DESIGN_TYPE == Button_ONE_CHANNEL_DESIGN)
        /* Find next sensor */
        Button_sensorIndex = Button_FindNextSensor(Button_sensorIndex);

        /* Check end of scan condition */
        if(Button_sensorIndex < Button_TOTAL_SENSOR_COUNT)
        {
            Button_csv |= Button_SW_STS_BUSY;
            Button_PreScan(Button_sensorIndex);
        }
        
    #else
        /* Find next sensor and set proper control register */
        Button_sensorIndex = Button_FindNextPair(Button_sensorIndex);
        
        /* Start sensor scan */
        if((Button_sensorIndex < Button_TOTAL_SENSOR_COUNT__CH0) || 
           (Button_sensorIndex < Button_TOTAL_SENSOR_COUNT__CH1))
        {
            Button_csv |= Button_SW_STS_BUSY;
            Button_PreScan(Button_sensorIndex);
        }
        
    #endif  /* Button_DESIGN_TYPE */
}


/*******************************************************************************
* Function Name: Button_IsBusy
********************************************************************************
*
* Summary:
*  Returns the state of CapSense component. The 1 means that scanning in 
*  progress and 0 means that scanning is complete.
*
* Parameters:
*  None
*
* Return:
*  Returns the state of scanning. 1 - scanning in progress, 0 - scanning 
*  completed.
*
* Global Variables:
*  Button_csv - used to provide status and mode of scanning process. 
*  Checks the busy status.
*
*******************************************************************************/
uint8 Button_IsBusy(void) 
{
    return ((0u != (Button_csv & Button_SW_STS_BUSY)) ? 1u : 0u);
}


/*******************************************************************************
* Function Name: Button_ReadSensorRaw
********************************************************************************
*
* Summary:
*  Returns scan sensor raw data from the Button_sensorRaw[] array. 
*  Each scan sensor has a unique number within the sensor array. This number 
*  is assigned by the CapSense customizer in sequence.
*
* Parameters:
*  sensor:  Sensor number.
*
* Return:
*  Returns current raw data value for defined sensor number.
*
* Global Variables:
*  Button_sensorRaw[] - used to store sensors raw data.
*
*******************************************************************************/
uint16 Button_ReadSensorRaw(uint8 sensor) 
{
    return (Button_sensorRaw[sensor]);
}


/*******************************************************************************
* Function Name: Button_ClearSensors
********************************************************************************
*
* Summary:
*  Resets all sensors to the non-sampling state by sequentially disconnecting
*  all sensors from Analog MUX Bus and putting them to inactive state.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Button_ClearSensors(void) 
{
    uint8 i;
   
    for (i = 0u; i < Button_TOTAL_SENSOR_COUNT; i++)
    {
        Button_DisableScanSlot(i);
    }
}


#if (Button_IS_COMPLEX_SCANSLOTS)
    /*******************************************************************************
    * Function Name: Button_EnableScanSlot
    ********************************************************************************
    *
    * Summary:
    *  Configures the selected slot to measure during the next measurement 
    *  cycle. The corresponding pin/pins are set to Analog High-Z mode and 
    *  connected to the Analog Mux Bus. This also enables the comparator function.
    *
    * Parameters:
    *  slot:  Slot number.
    *
    * Return:
    *  None
    *
    * Global Constants:
    *  Button_portTable[]  - used to store the port number that pin 
    *  belongs to for every sensor.
    *  Button_maskTable[]  - used to store the pin within the port for 
    *  every sensor.
    *  Button_indexTable[] - used to store indexes of complex sensors.
    *  The offset and position in this array are stored in port and mask table for 
    *  complex sensors.
    *  The bit 7 (msb) is used to define the sensor type: single or complex.
    *
    *******************************************************************************/
    void Button_EnableScanSlot(uint8 slot) CYREENTRANT
    {
        uint8 j;
        uint8 snsNumber;
        const uint8 CYCODE *index;
        /* Read the sensor type: single or complex */
        uint8 snsType = Button_portTable[slot];
        
        /* Check if sensor is complex */
        if ((snsType & Button_COMPLEX_SS_FLAG) == 0u)
        {
            /* Enable sensor (signle) */
            Button_EnableSensor(slot);
        }
        else
        {
            /* Enable complex sensor */
            snsType &= ~Button_COMPLEX_SS_FLAG;
            index = &Button_indexTable[snsType];
            snsNumber = Button_maskTable[slot];
                        
            for (j=0u; j < snsNumber; j++)
            {
                Button_EnableSensor(index[j]);
            }
        } 
    }
    
    
    /*******************************************************************************
    * Function Name: Button_DisableScanSlot
    ********************************************************************************
    *
    * Summary:
    *  Disables the selected slot. The corresponding pin/pis is/are disconnected 
    *  from the Analog Mux Bus and connected to GND, High_Z or Shield electrode.
    *
    * Parameters:
    *  slot:  Slot number.
    *
    * Return:
    *  None
    *
    * Global Variables:
    *  Button_portTable[]  - used to store the port number that pin 
    *  belongs to for every sensor.
    *  Button_maskTable[]  - used to store the pin within the port for 
    *  every sensor.
    *  Button_indexTable[] - used to store indexes of complex sensors.
    *  The offset and position in this array are stored in port and mask table for 
    *  complex sensors.
    *  The 7bit(msb) is used to define the sensor type: single or complex.
    *
    *******************************************************************************/
    void Button_DisableScanSlot(uint8 slot) CYREENTRANT
    {
        uint8 j;
        uint8 snsNumber;
        const uint8 CYCODE *index;
        /* Read the sensor type: single or complex */
        uint8 snsType = Button_portTable[slot];
        
        /* Check if sensor is complex */
        if ((snsType & Button_COMPLEX_SS_FLAG) == 0u)
        {
            /* Disable sensor (signle) */
            Button_DisableSensor(slot);
        }
        else
        {
            /* Disable complex sensor */
            snsType &= ~Button_COMPLEX_SS_FLAG;
            index = &Button_indexTable[snsType];
            snsNumber = Button_maskTable[slot];
                        
            for (j=0u; j < snsNumber; j++)
            {
                Button_DisableSensor(index[j]);
            }
        } 
    }
#endif  /* Button_IS_COMPLEX_SCANSLOTS */


/*******************************************************************************
* Function Name: Button_EnableSensor
********************************************************************************
*
* Summary:
*  Configures the selected sensor to measure during the next measurement cycle.
*  The corresponding pins are set to Analog High-Z mode and connected to the
*  Analog Mux Bus. This also enables the comparator function.
*
* Parameters:
*  sensor:  Sensor number.
*
* Return:
*  None
*
* Global Variables:
*  Button_portTable[] - used to store the port number that pin 
*  belongs to for every sensor.
*  Button_maskTable[] - used to store the pin within the port for 
*  every sensor.
*  Button_csTable[]   - used to store the pointers to CAPS_SEL 
*  registers for every port.
*  Button_pcTable[]   - used to store the pointers to PC pin 
*  register for every sensor.
*  Button_amuxIndex[] - used to store corrected AMUX index when 
*  complex sensors are defeined.
*
*******************************************************************************/
void Button_EnableSensor(uint8 sensor) CYREENTRANT
{
    uint8 port = Button_portTable[sensor];
    uint8 mask = Button_maskTable[sensor];
    
    #if ((Button_DESIGN_TYPE == Button_TWO_CHANNELS_DESIGN) && \
         (Button_IS_COMPLEX_SCANSLOTS))
        uint8 amuxCh = Button_amuxIndex[sensor];
    #endif  /* ((Button_DESIGN_TYPE == Button_TWO_CHANNELS_DESIGN) && \
            *   (Button_IS_COMPLEX_SCANSLOTS))
            */
    
    /* Make sensor High-Z */
    *Button_pcTable[sensor] = Button_PRT_PC_HIGHZ;
    
    /* Connect to DSI output */
	if(port == 15u)
	{
		port = 7u;
	}
    *Button_csTable[port] |= mask;
    
    /* Connect to AMUX */
    #if (Button_DESIGN_TYPE == Button_ONE_CHANNEL_DESIGN)
        #if (Button_IS_COMPLEX_SCANSLOTS)
            Button_AMuxCH0_Connect(Button_amuxIndex[sensor]);
        #else
            Button_AMuxCH0_Connect(sensor);
        #endif  /* Button_IS_COMPLEX_SCANSLOTS */
                
    #else
        #if (Button_IS_COMPLEX_SCANSLOTS)
            if ((amuxCh & Button_CHANNEL1_FLAG) == 0u)
            {
                Button_AMuxCH0_Connect(amuxCh);
            } 
            else
            {
                amuxCh &= ~ Button_CHANNEL1_FLAG;
                Button_AMuxCH1_Connect(amuxCh);
            }
            
        #else
            if (sensor < Button_TOTAL_SENSOR_COUNT__CH0) 
            {
                Button_AMuxCH0_Connect(sensor);
            } 
            else
            {
                Button_AMuxCH1_Connect(sensor - Button_TOTAL_SENSOR_COUNT__CH0);
            }
            
        #endif  /* Button_IS_COMPLEX_SCANSLOTS */
        
    #endif  /* Button_DESIGN_TYPE == Button_ONE_CHANNEL_DESIGN */
}


/*******************************************************************************
* Function Name: Button_DisableSensor
********************************************************************************
*
* Summary:
*  Disables the selected sensor. The corresponding pin is disconnected from the
*  Analog Mux Bus and connected to GND, High_Z or Shield electrode.
*
* Parameters:
*  sensor:  Sensor number
*
* Return:
*  None
*
* Global Variables:
*  Button_portTable[] - used to store the port number that pin 
*  belongs to for every sensor.
*  Button_maskTable[] - used to store the pin within the port for 
*  every sensor.
*  Button_csTable[]   - used to store the pointers to CAPS_SEL 
*  registers for every port.
*  Button_pcTable[]   - used to store the pointers to PC pin 
*  register for every sensor.
*  Button_amuxIndex[] - used to store corrected AMUX index when 
*  complex sensors are defeined.
*
*******************************************************************************/
void Button_DisableSensor(uint8 sensor) CYREENTRANT
{
    uint8 port = Button_portTable[sensor];
    uint8 mask = Button_maskTable[sensor];
    
    #if ((Button_DESIGN_TYPE == Button_TWO_CHANNELS_DESIGN) && \
         (Button_IS_COMPLEX_SCANSLOTS))
        uint8 amuxCh = Button_amuxIndex[sensor];
    #endif  /* ((Button_DESIGN_TYPE == Button_TWO_CHANNELS_DESIGN) && \
            *   (Button_IS_COMPLEX_SCANSLOTS))
            */
    
    /* Disconnect from AMUX */
    #if (Button_DESIGN_TYPE == Button_ONE_CHANNEL_DESIGN)
        #if (Button_IS_COMPLEX_SCANSLOTS)
            Button_AMuxCH0_Disconnect(Button_amuxIndex[sensor]);
        #else
            Button_AMuxCH0_Disconnect(sensor);
        #endif  /* Button_IS_COMPLEX_SCANSLOTS */
                
    #else
        #if (Button_IS_COMPLEX_SCANSLOTS)
            if ((amuxCh & Button_CHANNEL1_FLAG) == 0u)
            {
                Button_AMuxCH0_Disconnect(amuxCh);
            } 
            else
            {
                amuxCh &= ~ Button_CHANNEL1_FLAG;
                Button_AMuxCH1_Disconnect(amuxCh);
            }
            
        #else
            if (sensor < Button_TOTAL_SENSOR_COUNT__CH0) 
            {
                Button_AMuxCH0_Disconnect(sensor);
            } 
            else
            {
                Button_AMuxCH1_Disconnect(sensor - Button_TOTAL_SENSOR_COUNT__CH0);
            }
            
        #endif  /* Button_IS_COMPLEX_SCANSLOTS */
        
    #endif  /* Button_DESIGN_TYPE */
    
    /* Disconnect from DSI output */
	if(port == 15u)
	{
		port = 7u;
	}
    *Button_csTable[port] &= (uint8)~mask;
    
    /* Set sensor to inactive state */
    #if (Button_CONNECT_INACTIVE_SNS == Button_CIS_GND)
        *Button_pcTable[sensor] = Button_PRT_PC_GND;
    #elif (Button_CONNECT_INACTIVE_SNS == Button_CIS_HIGHZ)
        *Button_pcTable[sensor] = Button_PRT_PC_HIGHZ;
    #else
        *Button_pcTable[sensor] = Button_PRT_PC_SHIELD;
    #endif  /* (Button_CONNECT_INACTIVE_SNS == Button_CIS_GND) */
}


/*******************************************************************************
* Function Name: Button_PreScan
********************************************************************************
*
* Summary:
*  Set required settings, enable sensor, remove Vref from AMUX and start the 
*  scanning process of the sensor.
*
* Parameters:
*  sensor:  Sensor number.
*
* Return:
*  None
*
* Global Variables:
*  Button_rbTable[] - used to store pointers to PC pin registers for 
*  every bleed resistor (Rb). Only available when Current Source is External 
*  resistor.
*
*******************************************************************************/
void Button_PreScan(uint8 sensor) CYREENTRANT
{
    /* Set Sensor Settings */
    Button_SetScanSlotSettings(sensor);
    
    /* Place disable interrupts here to eliminate influence on start of scanning */
    /* `#START Button_PreScan_DisableInt` */

    /* `#END` */
    
    #ifdef Button_PRE_SCAN_DISABLE_INT_CALLBACK
    Button_PreScan_DisableInt_Callback();
    #endif /* Button_PRE_SCAN_DISABLE_INT_CALLBACK */

    /* Resets digital and pre-charge clocks */
    Button_CONTROL_REG |= Button_CTRL_SYNC_EN;
        
    #if (Button_DESIGN_TYPE == Button_ONE_CHANNEL_DESIGN)
        #if (Button_CURRENT_SOURCE == Button_IDAC_SOURCE)
            /* Disable Vref from AMux */
            #if (Button_VREF_VDAC == Button_VREF_OPTIONS)
                Button_AMuxCH0_Disconnect(Button_AMuxCH0_VREF_CHANNEL);
            #else
                Button_BufCH0_CAPS_CFG0_REG &= ~Button_CSBUF_ENABLE;
            #endif  /* (Button_VREF_VDAC != Button_VREF_OPTIONS) */

            /* Enable Sensor */
            Button_EnableScanSlot(sensor);
            
        #elif (Button_CURRENT_SOURCE == Button_IDAC_SINK)
            /* Connect IDAC */
            Button_AMuxCH0_Connect(Button_AMuxCH0_IDAC_CHANNEL);
            
            /* Enable Sensor */
            Button_EnableScanSlot(sensor);
                
            /* Disable CapSense Buffer */
            Button_BufCH0_CAPS_CFG0_REG &= (uint8)~Button_CSBUF_ENABLE;
            
        #else
            /* Connect DSI output to Rb */
            *Button_rbTable[Button_extRbCh0Cur] |= Button_BYP_MASK;
            
            /* Enable Sensor */
            Button_EnableScanSlot(sensor);
             
            /* Disable CapSense Buffer */
            Button_BufCH0_CAPS_CFG0_REG &= ~Button_CSBUF_ENABLE;
        
        #endif  /* (Button_CURRENT_SOURCE == Button_IDAC_SOURCE) */
        
    #else

        if((Button_CONTROL_REG & Button_CTRL_WINDOW_EN__CH0) != 0u)
        {
            #if (Button_CURRENT_SOURCE == Button_IDAC_SOURCE)
                /* Disable Vref from AMux */
                #if (Button_VREF_VDAC == Button_VREF_OPTIONS)
                    Button_AMuxCH0_Disconnect(Button_AMuxCH0_VREF_CHANNEL);
                #else
                    Button_BufCH0_CAPS_CFG0_REG &= ~Button_CSBUF_ENABLE;
                #endif  /* (Button_VREF_VDAC != Button_VREF_OPTIONS) */
                
                /* Enable Sensor */
                Button_EnableScanSlot(sensor);
                
            #elif (Button_CURRENT_SOURCE == Button_IDAC_SINK)
                /* Connect IDAC */
                Button_AMuxCH0_Connect(Button_AMuxCH0_IDAC_CHANNEL);
                
                /* Enable Sensor */
                Button_EnableScanSlot(sensor);
                    
                /* Disable Vref from AMux */
                Button_BufCH0_CAPS_CFG0_REG &= ~Button_CSBUF_ENABLE;
                
            #else
                /* Connect DSI output to Rb */
                *Button_rbTable[Button_extRbCh0Cur] |= Button_BYP_MASK;
                
                /* Enable Sensor */
                Button_EnableScanSlot(sensor);
                    
                /* Disable Vref from AMux */
                Button_BufCH0_CAPS_CFG0_REG &= ~Button_CSBUF_ENABLE;
            
            #endif  /* (Button_CURRENT_SOURCE == Button_IDAC_SOURCE) */
            
        }
        
        if((Button_CONTROL_REG & Button_CTRL_WINDOW_EN__CH1) != 0u)
        {
            sensor += Button_TOTAL_SENSOR_COUNT__CH0;
            
            #if (Button_CURRENT_SOURCE == Button_IDAC_SOURCE)
                /* Disable Vref from AMux */
                #if (Button_VREF_VDAC == Button_VREF_OPTIONS)
                   Button_AMuxCH1_Disconnect(Button_AMuxCH1_VREF_CHANNEL);
                #else 
                    Button_BufCH1_CAPS_CFG0_REG &= ~Button_CSBUF_ENABLE;
                #endif  /* (Button_VREF_VDAC == Button_VREF_OPTIONS) */
                
                /* Enable Sensor */
                Button_EnableScanSlot(sensor);
                
            #elif (Button_CURRENT_SOURCE == Button_IDAC_SINK)
                /* Connect IDAC */
                Button_AMuxCH1_Connect(Button_AMuxCH1_IDAC_CHANNEL);
                
                /* Enable Sensor */
                Button_EnableScanSlot(sensor);
                    
                /* Disable Vref from AMux */
                Button_BufCH1_CAPS_CFG0_REG &= ~Button_CSBUF_ENABLE;
                
            #else
                /* Connect DSI output to Rb */
                *Button_rbTable[Button_extRbCh1Cur] |= Button_BYP_MASK;
                
                /* Enable Sensor */
                Button_EnableScanSlot(sensor);
                
                /* Disable Vref from AMux */
                Button_BufCH1_CAPS_CFG0_REG &= ~Button_CSBUF_ENABLE;
            
            #endif  /* (Button_CURRENT_SOURCE == Button_IDAC_SOURCE) */
        }
    
    #endif  /* (Button_DESIGN_TYPE == Button_ONE_CHANNEL_DESIGN) */
    
    /* Start measurament, pre-charge clocks are running and PRS as well */
    Button_CONTROL_REG |= Button_CTRL_START;
    
    /* Place enable interrupts here to eliminate influence on start of scanning */
    /* `#START Button_PreScan_EnableInt` */

    /* `#END` */

    #ifdef Button_PRE_SCAN_ENABLE_INT_CALLBACK
        Button_PreScan_EnableInt_Callback();
    #endif /* Button_PRE_SCAN_ENABLE_INT_CALLBACK */
}


#if (Button_DESIGN_TYPE == Button_ONE_CHANNEL_DESIGN)
    /*******************************************************************************
    * Function Name: Button_PostScan
    ********************************************************************************
    *
    * Summary:
    *  Store results of measurament in Button_SensorResult[] array,
    *  sets scan sensor in none sampling state, turn off Idac(Current Source IDAC),
    *  disconnect IDAC(Sink mode) or bleed resistor (Rb) and apply Vref on AMUX.
    *  Only one channel designs.
    *
    * Parameters:
    *  sensor:  Sensor number.
    *
    * Return:
    *  None
    *
    * Global Variables:
    *  Button_sensorRaw[] - used to store sensors raw data.
    *
    * Reentrant:
    *  No
    *
    *******************************************************************************/
    void Button_PostScan(uint8 sensor) CYREENTRANT
    {
        /* Stop Capsensing and rearm sync */
        Button_CONTROL_REG &= (uint8)~(Button_CTRL_START | Button_CTRL_SYNC_EN);
        
        /* Read SlotResult from Raw Counter */
        #if (Button_IMPLEMENTATION_CH0 == Button_MEASURE_IMPLEMENTATION_FF)
            Button_sensorRaw[sensor]  = Button_MEASURE_FULL_RANGE - 
                                                      CY_GET_REG16(Button_RAW_CH0_COUNTER_PTR);
        #else
            Button_sensorRaw[sensor]  = ((uint16) Button_RAW_CH0_COUNTER_HI_REG << 8u);
            Button_sensorRaw[sensor] |= (uint16) Button_RAW_CH0_COUNTER_LO_REG;
            Button_sensorRaw[sensor]  = Button_MEASURE_FULL_RANGE -
                                                      Button_sensorRaw[sensor];
        #endif  /* (Button_IMPLEMENTATION == Button_MEASURE_IMPLEMENTATION_FF) */
        
        /* Disable Sensor */
        Button_DisableScanSlot(sensor);
        
        #if(Button_CURRENT_SOURCE)
            /* Turn off IDAC */
            Button_IdacCH0_SetValue(Button_TURN_OFF_IDAC);
            #if (Button_CURRENT_SOURCE == Button_IDAC_SINK)
                /* Disconnect IDAC */
                Button_AMuxCH0_Disconnect(Button_AMuxCH0_IDAC_CHANNEL);
            #endif  /* (Button_CURRENT_SOURCE == Button_IDAC_SINK) */
        #else
            /* Disconnect DSI output from Rb */
            *Button_rbTable[Button_extRbCh0Cur] &= ~Button_BYP_MASK; 
        #endif  /* (Button_CURRENT_SOURCE)*/
            
        /* Enable Vref on AMUX */
        #if (Button_VREF_OPTIONS == Button_VREF_VDAC)
            Button_AMuxCH0_Connect(Button_AMuxCH0_VREF_CHANNEL);
        #else
            Button_BufCH0_CAPS_CFG0_REG |= Button_CSBUF_ENABLE;
        #endif  /* (Button_VREF_VDAC == Button_VREF_OPTIONS) */
    }
    
#else

    /*******************************************************************************
    * Function Name: Button_PostScan
    ********************************************************************************
    *
    * Summary:
    *  Store results of measurament in Button_SensorResult[] array,
    *  sets scan sensor in none sampling state, turn off Idac(Current Source IDAC),
    *  disconnect IDAC(Sink mode) or bleed resistor (Rb) and apply Vref on AMUX.
    *  Only used for channel 0 in two channes designs.
    *
    * Parameters:
    *  sensor:  Sensor number.
    *
    * Return:
    *  None
    *
    * Global Variables:
    *  Button_sensorRaw[] - used to store sensors raw data.
    *
    * Reentrant:
    *  No
    *
    *******************************************************************************/
    void Button_PostScanCh0(uint8 sensor) CYREENTRANT
    {
        if (((Button_CONTROL_REG & Button_CTRL_WINDOW_EN__CH0) == 0u) && 
            ((Button_CONTROL_REG & Button_CTRL_WINDOW_EN__CH1) == 0u)) 
        {
            /* Stop Capsensing and rearm sync */
            Button_CONTROL_REG &= ~(Button_CTRL_START | Button_CTRL_SYNC_EN);
        }
        
        /* Read SlotResult from Raw Counter */
        #if (Button_IMPLEMENTATION_CH0 == Button_MEASURE_IMPLEMENTATION_FF)
            Button_sensorRaw[sensor]  = Button_MEASURE_FULL_RANGE - 
                                                      CY_GET_REG16(Button_RAW_CH0_COUNTER_PTR);
        #else
            Button_sensorRaw[sensor]  = ((uint16) Button_RAW_CH0_COUNTER_HI_REG << 8u);
            Button_sensorRaw[sensor] |= (uint16) Button_RAW_CH0_COUNTER_LO_REG;
            Button_sensorRaw[sensor]  = Button_MEASURE_FULL_RANGE - 
                                                      Button_sensorRaw[sensor];
        #endif  /* (Button_IMPLEMENTATION_CH0 == Button_MEASURE_IMPLEMENTATION_FF)*/
        
        /* Disable Sensor */
        Button_DisableScanSlot(sensor);
        
        #if (Button_CURRENT_SOURCE)
            /* Turn off IDAC */
            Button_IdacCH0_SetValue(Button_TURN_OFF_IDAC);
            #if (Button_CURRENT_SOURCE == Button_IDAC_SINK)
                /* Disconnect IDAC */
                Button_AMuxCH0_Disconnect(Button_AMuxCH0_IDAC_CHANNEL);
            #endif  /* (Button_CURRENT_SOURCE == Button_IDAC_SINK) */
        #else
            /* Disconnect DSI output from Rb */
            *Button_rbTable[Button_extRbCh0Cur] &= ~Button_BYP_MASK; 
        #endif  /* (Button_CURRENT_SOURCE)*/
        
        /* Enable Vref on AMUX */
        #if (Button_VREF_OPTIONS == Button_VREF_VDAC)
            Button_AMuxCH0_Connect(Button_AMuxCH0_VREF_CHANNEL);
        #else
            Button_BufCH0_CAPS_CFG0_REG |= Button_CSBUF_ENABLE;
        #endif  /* (Button_VREF_VDAC == Button_VREF_OPTIONS) */
    }
    
    
    /*******************************************************************************
    * Function Name: Button_PostScanCh1
    ********************************************************************************
    *
    * Summary:
    *  Store results of measurament in Button_SensorResult[] array,
    *  sets scan sensor in none sampling state, turn off Idac(Current Source IDAC), 
    *  disconnect IDAC(Sink mode) or bleed resistor (Rb) and apply Vref on AMUX.
    *  Only used for channel 1 in two channes designs.
    *
    * Parameters:
    *  sensor:  Sensor number.
    *
    * Return:
    *  None
    *
    * Global Variables:
    *  Button_sensorRaw[] - used to store sensors raw data.
    *
    * Reentrant:
    *  No
    *
    *******************************************************************************/
    void Button_PostScanCh1(uint8 sensor) CYREENTRANT
    {
        if (((Button_CONTROL_REG & Button_CTRL_WINDOW_EN__CH0) == 0u) && 
            ((Button_CONTROL_REG & Button_CTRL_WINDOW_EN__CH1) == 0u))
        {
            /* Stop Capsensing and rearm sync */
            Button_CONTROL_REG &= ~(Button_CTRL_START | Button_CTRL_SYNC_EN);
        }
        
        /* Read SlotResult from Raw Counter */
        #if (Button_IMPLEMENTATION_CH1 == Button_MEASURE_IMPLEMENTATION_FF)
            Button_sensorRaw[sensor]  = Button_MEASURE_FULL_RANGE - 
                                                      CY_GET_REG16(Button_RAW_CH1_COUNTER_PTR);
        #else
            Button_sensorRaw[sensor]  = ((uint16) Button_RAW_CH1_COUNTER_HI_REG << 8u);
            Button_sensorRaw[sensor] |= (uint16) Button_RAW_CH1_COUNTER_LO_REG;
            Button_sensorRaw[sensor]  = Button_MEASURE_FULL_RANGE - 
                                                      Button_sensorRaw[sensor];
        #endif  /* (Button_IMPLEMENTATION_CH1 == Button_MEASURE_IMPLEMENTATION_FF)*/
        
        /* Disable Sensor */
        Button_DisableScanSlot(sensor);
        
        #if (Button_CURRENT_SOURCE)
            /* Turn off IDAC */
            Button_IdacCH1_SetValue(Button_TURN_OFF_IDAC);
            #if (Button_CURRENT_SOURCE == Button_IDAC_SINK)
                /* Disconnect IDAC */
                Button_AMuxCH1_Disconnect(Button_AMuxCH1_IDAC_CHANNEL);
            #endif  /* (Button_CURRENT_SOURCE == Button_IDAC_SINK) */
        #else
            /* Disconnect DSI output from Rb */
            *Button_rbTable[Button_extRbCh1Cur] &= ~Button_BYP_MASK; 
        #endif  /* (Button_CURRENT_SOURCE)*/

        /* Enable Vref on AMUX */
        #if (Button_VREF_OPTIONS == Button_VREF_VDAC)
            Button_AMuxCH1_Connect(Button_AMuxCH1_VREF_CHANNEL);
        #else
            Button_BufCH1_CAPS_CFG0_REG |= Button_CSBUF_ENABLE;
        #endif  /* (Button_VREF_VDAC == Button_VREF_OPTIONS) */
    }
    
#endif  /* Button_DESIGN_TYPE */


#if (Button_CURRENT_SOURCE == Button_EXTERNAL_RB)
    /*******************************************************************************
    * Function Name:  Button_InitRb
    ********************************************************************************
    *
    * Summary:
    *  Sets all Rbleed resistor to High-Z mode. The first Rbleed resistor is active
    *  while next measure.
    *  This function is available only if Current Source is External Resistor.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    ********************************************************************************/
    void Button_InitRb(void) 
    {
        uint8 i;
        
        /* Disable all Rb */
        for(i = 0u; i < Button_TOTAL_RB_NUMBER; i++)
        {
            /* Make High-Z */
            *Button_rbTable[i] = Button_PRT_PC_HIGHZ;
        }
    }
    
    
    /*******************************************************************************
    * Function Name: Button_SetRBleed
    ********************************************************************************
    *
    * Summary:
    *  Sets the pin to use for the bleed resistor (Rb) connection. This function
    *  can be called at runtime to select the current Rb pin setting from those 
    *  defined customizer. The function overwrites the component parameter setting. 
    *  This function is available only if Current Source is External Resistor.
    * 
    * Parameters:
    *  rbleed:  Ordering number for bleed resistor terminal defined in CapSense
    *  customizer.
    *
    * Return:
    *  None
    *
    * Global Variables:
    *  Button_extRbCh0Cur - used to store current number of active 
    *  bleed resistor (Rb) of channel 0.
    *  Button_extRbCh1Cur - used to store current number of active 
    *  bleed resistor (Rb) of channel 1.
    *  The active bleed resistor (Rb) pin will be used while next measurement  
    *  cycle.
    *
    * Reentrant:
    *  No
    *
    *******************************************************************************/
    void Button_SetRBleed(uint8 rbleed) 
    {
        #if (Button_DESIGN_TYPE == Button_ONE_CHANNEL_DESIGN)
            Button_extRbCh0Cur = rbleed;
            
        #else
            if(rbleed < Button_TOTAL_RB_NUMBER__CH0)
            {
                Button_extRbCh0Cur = rbleed;
            }
            else
            {
                Button_extRbCh1Cur = (rbleed - Button_TOTAL_RB_NUMBER__CH0);   
            }
    
        #endif  /* Button_DESIGN_TYPE == Button_ONE_CHANNEL_DESIGN */ 
    }
#endif /* Button_CURRENT_SOURCE == Button_EXTERNAL_RB */ 

#if (Button_PRESCALER_OPTIONS)
    /*******************************************************************************
    * Function Name: Button_SetPrescaler
    ********************************************************************************
    *
    * Summary:
    *  Sets analog switch divider.
    *
    * Parameters:
    *  prescaler:  Sets prescaler divider values.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void Button_SetPrescaler(uint8 prescaler) CYREENTRANT
    {
        /* Set Prescaler */
        #if (Button_PRESCALER_OPTIONS == Button_PRESCALER_UDB)
            Button_PRESCALER_PERIOD_REG = prescaler;
            Button_PRESCALER_COMPARE_REG = (prescaler >> 1u);
        #elif (Button_PRESCALER_OPTIONS == Button_PRESCALER_FF)
            CY_SET_REG16(Button_PRESCALER_PERIOD_PTR, (uint16) prescaler);
            CY_SET_REG16(Button_PRESCALER_COMPARE_PTR, (uint16) (prescaler >> 1u));
        #else
            /* Do nothing = config without prescaler */
        #endif  /* (Button_PRESCALER_OPTIONS == Button_PRESCALER_UDB) */
    }


    /*******************************************************************************
    * Function Name: Button_GetPrescaler
    ********************************************************************************
    *
    * Summary:
    *  Gets analog switch divider.
    *
    * Parameters:
    *  None
    *
    * Return:
    *   Returns the prescaler divider value.
    *
    *******************************************************************************/
    uint8 Button_GetPrescaler(void) 
    {
        uint8 prescaler = 0u;

        /* Get Prescaler */
        #if (Button_PRESCALER_OPTIONS == Button_PRESCALER_UDB)
            prescaler = Button_PRESCALER_PERIOD_REG;
            
        #elif (Button_PRESCALER_OPTIONS == Button_PRESCALER_FF)
            prescaler = (uint8) CY_GET_REG16(Button_PRESCALER_PERIOD_PTR);
            
        #else
            /* Do nothing = config without prescaler */
        #endif  /* (Button_PRESCALER_OPTIONS == Button_PRESCALER_UDB) */
        
        return (prescaler);
    }
#endif  /* Button_PRESCALER_OPTIONS */


/*******************************************************************************
* Function Name: Button_SetScanSpeed
********************************************************************************
*
* Summary:
*  Sets ScanSpeed divider.
*
* Parameters:
*  scanspeed:  Sets ScanSpeed divider.
*
* Return:
*  None
*
*******************************************************************************/
void Button_SetScanSpeed(uint8 scanSpeed) 
{
    Button_SCANSPEED_PERIOD_REG = scanSpeed; 
}


#if (Button_PRS_OPTIONS)
    /*******************************************************************************
    * Function Name: Button_SetAnalogSwitchesSource
    ********************************************************************************
    *
    * Summary:
    *  Selects the Analog switches source between PRS and prescaler. It is useful
    *  for sensor capacitance determination for sensors with low self-capacitance.
    *  This function is used in auto-tuning procedure.
    *
    * Parameters:
    *  src:  analog switches source:
    *           Button_ANALOG_SWITCHES_SRC_PRESCALER - selects prescaler
    *           Button_ANALOG_SWITCHES_SRC_PRS - selects PRS
    *
    * Return:
    *  None
    *
    * Reentrant:
    *  No
    *******************************************************************************/
    void Button_SetAnalogSwitchesSource(uint8 src)
                      
    {
        if(src == Button_ANALOG_SWITCHES_SRC_PRESCALER)
        {
            Button_CONTROL_REG &= (uint8)~0x10u;
        }
        else
        {
            Button_CONTROL_REG |= 0x10u;
        }
    }
#endif /* (Button_PRS_OPTIONS) */

/* [] END OF FILE */
