/*******************************************************************************
* File Name: Slider.c
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

#include "Slider.h"
#include "cyapicallbacks.h"


static uint8 Slider_initVar = 0u;
            
/* Global software variables */
volatile uint8 Slider_csv;            /* CapSense CSD status, control variable */
volatile uint8 Slider_sensorIndex;    /* Index of scannig sensor */

/* AutoTunning start */
#if (Slider_TUNING_METHOD == Slider_AUTO_TUNING)
    uint8 Slider_lowLevelTuningDone = 1u;
#endif /* (Slider_TUNING_METHOD == Slider_AUTO_TUNING) */

#if (Slider_CURRENT_SOURCE == Slider_EXTERNAL_RB)
    uint8  Slider_extRbCh0Cur = Slider_RBLEED1;
    #if (Slider_DESIGN_TYPE == Slider_TWO_CHANNELS_DESIGN)
        uint8  Slider_extRbCh1Cur = (Slider_RBLEED1 + Slider_TOTAL_RB_NUMBER__CH0);
    #endif /* (Slider_DESIGN_TYPE == Slider_TWO_CHANNELS_DESIGN)*/ 
#endif /* (Slider_CURRENT_SOURCE == Slider_EXTERNAL_RB) */ 
        
/* Global array of Raw Counts */
uint16 Slider_sensorRaw[Slider_TOTAL_SENSOR_COUNT]; 

uint8 Slider_sensorEnableMask[(((Slider_TOTAL_SENSOR_COUNT - 1u) / 8u) + 1u)] = {
0x1Fu, };

uint8 CYXDATA * const CYCODE Slider_pcTable[] = {
    (uint8 CYXDATA *)Slider_PortCH0__LinearSlider_e0__LS__PC, 
    (uint8 CYXDATA *)Slider_PortCH0__LinearSlider_e1__LS__PC, 
    (uint8 CYXDATA *)Slider_PortCH0__LinearSlider_e2__LS__PC, 
    (uint8 CYXDATA *)Slider_PortCH0__LinearSlider_e3__LS__PC, 
    (uint8 CYXDATA *)Slider_PortCH0__LinearSlider_e4__LS__PC, 
};

const uint8 CYCODE Slider_portTable[] = {
    Slider_PortCH0__LinearSlider_e0__LS__PORT, 
    Slider_PortCH0__LinearSlider_e1__LS__PORT, 
    Slider_PortCH0__LinearSlider_e2__LS__PORT, 
    Slider_PortCH0__LinearSlider_e3__LS__PORT, 
    Slider_PortCH0__LinearSlider_e4__LS__PORT, 
};

const uint8 CYCODE Slider_maskTable[] = {
    Slider_PortCH0__LinearSlider_e0__LS__MASK,
    Slider_PortCH0__LinearSlider_e1__LS__MASK,
    Slider_PortCH0__LinearSlider_e2__LS__MASK,
    Slider_PortCH0__LinearSlider_e3__LS__MASK,
    Slider_PortCH0__LinearSlider_e4__LS__MASK,
};

uint8 CYXDATA * const CYCODE Slider_csTable[] = {
    (uint8 CYXDATA *)CYREG_PRT0_CAPS_SEL, (uint8 CYXDATA *)CYREG_PRT1_CAPS_SEL, (uint8 CYXDATA *)CYREG_PRT2_CAPS_SEL,
    (uint8 CYXDATA *)CYREG_PRT3_CAPS_SEL, (uint8 CYXDATA *)CYREG_PRT4_CAPS_SEL, (uint8 CYXDATA *)CYREG_PRT5_CAPS_SEL,
    (uint8 CYXDATA *)CYREG_PRT6_CAPS_SEL, (uint8 CYXDATA *)CYREG_PRT15_CAPS_SEL,
};

uint8 Slider_idacSettings[] = {
    200u,200u,200u,200u,200u,
};

uint8 Slider_widgetResolution[] = {
    Slider_PWM_RESOLUTION_10_BITS,
};

uint8 Slider_analogSwitchDivider[Slider_TOTAL_SCANSLOT_COUNT];
const uint8 CYCODE Slider_widgetNumber[] = {
    0u, 0u, 0u, 0u, 0u, /* LinearSlider__LS */
    
};




/*******************************************************************************
* Function Name: Slider_Init
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
void Slider_Init(void) 
{
    #if ( (Slider_PRS_OPTIONS) || \
          (Slider_IMPLEMENTATION_CH0 == Slider_MEASURE_IMPLEMENTATION_UDB) || \
          ( (Slider_DESIGN_TYPE == Slider_TWO_CHANNELS_DESIGN) && \
            (Slider_IMPLEMENTATION_CH1 == Slider_MEASURE_IMPLEMENTATION_UDB)) )
        
        uint8 enableInterrupts;
    #endif /* ( (Slider_PRS_OPTIONS) || \
           * (Slider_IMPLEMENTATION_CH0 == Slider_MEASURE_IMPLEMENTATION_UDB) || \
           * ( (Slider_DESIGN_TYPE == Slider_TWO_CHANNELS_DESIGN) && \
           * (Slider_IMPLEMENTATION_CH1 == Slider_MEASURE_IMPLEMENTATION_UDB)) ) 
           */
    
    /* Clear all sensors */
    Slider_ClearSensors();

    /* Set Prescaler */
    #if (Slider_PRESCALER_OPTIONS == Slider_PRESCALER_UDB)
        /* Do nothing = config without prescaler */
    #elif (Slider_PRESCALER_OPTIONS == Slider_PRESCALER_FF)
        Slider_PRESCALER_CONTROL_REG   = (Slider_PRESCALER_CTRL_ENABLE |
                                                    Slider_PRESCALER_CTRL_MODE_CMP);
                                               
        Slider_PRESCALER_CONTROL2_REG |= Slider_PRESCALER_CTRL_CMP_LESS_EQ;
    #else
        /* Do nothing = config without prescaler */
    #endif  /* (Slider_PRESCALER_OPTIONS == Slider_PRESCALER_UDB) */

    /* Set PRS */
    #if (Slider_PRS_OPTIONS)
        Slider_SetAnalogSwitchesSource(Slider_ANALOG_SWITCHES_SRC_PRS);
    #endif /* (Slider_PRS_OPTIONS) */

    #if (Slider_PRS_OPTIONS == Slider_PRS_8BITS)
        /* Aux control set FIFO as REG */
        enableInterrupts = CyEnterCriticalSection();
        Slider_AUX_CONTROL_A_REG |= Slider_AUXCTRL_FIFO_SINGLE_REG;
        CyExitCriticalSection(enableInterrupts);
        
        /* Write polynomial */
        Slider_POLYNOM_REG   = Slider_PRS8_DEFAULT_POLYNOM;
        /* Write FIFO with seed */
        Slider_SEED_COPY_REG = Slider_MEASURE_FULL_RANGE_LOW;
        
    #elif (Slider_PRS_OPTIONS == Slider_PRS_16BITS)
        /* Aux control set FIFO as REG */ 
        enableInterrupts = CyEnterCriticalSection();  
        Slider_AUX_CONTROL_A_REG |= Slider_AUXCTRL_FIFO_SINGLE_REG;
        Slider_AUX_CONTROL_B_REG |= Slider_AUXCTRL_FIFO_SINGLE_REG;
        CyExitCriticalSection(enableInterrupts);
        
        /* Write polynomial */
        CY_SET_REG16(Slider_POLYNOM_PTR, Slider_PRS16_DEFAULT_POLYNOM);
        /* Write FIFO with seed */
        CY_SET_REG16(Slider_SEED_COPY_PTR, Slider_MEASURE_FULL_RANGE);
                
    #elif (Slider_PRS_OPTIONS == Slider_PRS_16BITS_4X)
        /* Aux control set FIFO as REG */
        enableInterrupts = CyEnterCriticalSection();
        Slider_AUX_CONTROL_A_REG  |= Slider_AUXCTRL_FIFO_SINGLE_REG;
        CyExitCriticalSection(enableInterrupts);
        
        /* Write polynomial */
        Slider_POLYNOM_A__D1_REG   = HI8(Slider_PRS16_DEFAULT_POLYNOM);
        Slider_POLYNOM_A__D0_REG   = LO8(Slider_PRS16_DEFAULT_POLYNOM);
        /* Write FIFO with seed */
        Slider_SEED_COPY_A__F1_REG = Slider_MEASURE_FULL_RANGE_LOW;
        Slider_SEED_COPY_A__F0_REG = Slider_MEASURE_FULL_RANGE_LOW; 
        
    #else
        /* Do nothing = config without PRS */
    #endif  /* (Slider_PRS_OPTIONS == Slider_PRS_8BITS) */ 
    
    /* Set ScanSpeed */
    Slider_SCANSPEED_PERIOD_REG = Slider_SCANSPEED_VALUE;
    
    /* Set the Measure */
    #if (Slider_IMPLEMENTATION_CH0 == Slider_MEASURE_IMPLEMENTATION_FF)
        /* Window PWM */
        Slider_PWM_CH0_CONTROL_REG      = Slider_MEASURE_CTRL_ENABLE;
        Slider_PWM_CH0_CONTROL2_REG    |= Slider_MEASURE_CTRL_PULSEWIDTH;
        CY_SET_REG16(Slider_PWM_CH0_COUNTER_PTR, Slider_MEASURE_FULL_RANGE);
        
        /* Raw Counter */
        Slider_RAW_CH0_CONTROL_REG      = Slider_MEASURE_CTRL_ENABLE;
        Slider_RAW_CH0_CONTROL2_REG    |= Slider_MEASURE_CTRL_PULSEWIDTH;
        CY_SET_REG16(Slider_RAW_CH0_PERIOD_PTR, Slider_MEASURE_FULL_RANGE);
    
    #else
        /*Window PWM and Raw Counter AUX set */
        enableInterrupts = CyEnterCriticalSection();
        Slider_PWM_CH0_AUX_CONTROL_REG |= Slider_AUXCTRL_FIFO_SINGLE_REG;
        Slider_RAW_CH0_AUX_CONTROL_REG |= Slider_AUXCTRL_FIFO_SINGLE_REG;
        CyExitCriticalSection(enableInterrupts);
        
        /* Window PWM */
        Slider_PWM_CH0_ADD_VALUE_REG    = Slider_MEASURE_FULL_RANGE_LOW;
        Slider_PWM_CH0_PERIOD_LO_REG    = Slider_MEASURE_FULL_RANGE_LOW;
        Slider_PWM_CH0_COUNTER_LO_REG   = Slider_MEASURE_FULL_RANGE_LOW;
        
        /* Raw Counter */
        Slider_RAW_CH0_ADD_VALUE_REG    = Slider_MEASURE_FULL_RANGE_LOW;
        Slider_RAW_CH0_PERIOD_HI_REG    = Slider_MEASURE_FULL_RANGE_LOW;
        Slider_RAW_CH0_PERIOD_LO_REG    = Slider_MEASURE_FULL_RANGE_LOW;
        
    #endif  /* (Slider_IMPLEMENTATION_CH0 == Slider_MEASURE_IMPLEMENTATION_FF) */ 
    
    #if (Slider_DESIGN_TYPE == Slider_TWO_CHANNELS_DESIGN)
        #if (Slider_IMPLEMENTATION_CH1 == Slider_MEASURE_IMPLEMENTATION_FF)
            /* Window PWM */
            Slider_PWM_CH1_CONTROL_REG      = Slider_MEASURE_CTRL_ENABLE;
            Slider_PWM_CH1_CONTROL2_REG    |= Slider_MEASURE_CTRL_PULSEWIDTH;
            CY_SET_REG16(Slider_PWM_CH1_COUNTER_PTR, Slider_MEASURE_FULL_RANGE);
            
            /* Raw Counter */
            Slider_RAW_CH1_CONTROL_REG      = Slider_MEASURE_CTRL_ENABLE;
            Slider_RAW_CH1_CONTROL2_REG    |= Slider_MEASURE_CTRL_PULSEWIDTH;
            CY_SET_REG16(Slider_RAW_CH1_PERIOD_PTR, Slider_MEASURE_FULL_RANGE);
           
        #else
            /*Window PWM and Raw Counter AUX set */
            enableInterrupts = CyEnterCriticalSection();
            Slider_PWM_CH1_AUX_CONTROL_REG |= Slider_AUXCTRL_FIFO_SINGLE_REG;
            Slider_RAW_CH1_AUX_CONTROL_REG |= Slider_AUXCTRL_FIFO_SINGLE_REG;
            CyExitCriticalSection(enableInterrupts);
            
            /* Window PWM */
            Slider_PWM_CH1_ADD_VALUE_REG    = Slider_MEASURE_FULL_RANGE_LOW;
            Slider_PWM_CH1_PERIOD_LO_REG    = Slider_MEASURE_FULL_RANGE_LOW;
            Slider_PWM_CH1_COUNTER_LO_REG   = Slider_MEASURE_FULL_RANGE_LOW;
            
            /* Raw Counter */
            
            Slider_RAW_CH1_ADD_VALUE_REG    = Slider_MEASURE_FULL_RANGE_LOW;
            Slider_RAW_CH1_PERIOD_HI_REG    = Slider_MEASURE_FULL_RANGE_LOW;
            Slider_RAW_CH1_PERIOD_LO_REG    = Slider_MEASURE_FULL_RANGE_LOW;
            
        #endif  /* (Slider_IMPLEMENTATION_CH1 == Slider_MEASURE_IMPLEMENTATION_FF) */
    
    #endif  /* (Slider_DESIGN_TYPE == TWO_CHANNELS_DESIGN) */
    
    /* Setup ISR */
    CyIntDisable(Slider_IsrCH0_ISR_NUMBER);
    (void)CyIntSetVector(Slider_IsrCH0_ISR_NUMBER, &Slider_IsrCH0_ISR);
    CyIntSetPriority(Slider_IsrCH0_ISR_NUMBER, Slider_IsrCH0_ISR_PRIORITY);
    
    #if (Slider_DESIGN_TYPE == Slider_TWO_CHANNELS_DESIGN)
        CyIntDisable(Slider_IsrCH1_ISR_NUMBER);
        CyIntSetVector(Slider_IsrCH1_ISR_NUMBER, Slider_IsrCH1_ISR);
        CyIntSetPriority(Slider_IsrCH1_ISR_NUMBER, Slider_IsrCH1_ISR_PRIORITY);
    #endif  /* Slider_DESIGN_TYPE */
    
    /* Setup AMux Bus: Connect Cmod, Cmp, Idac */
    Slider_AMuxCH0_Init();
    Slider_AMuxCH0_Connect(Slider_AMuxCH0_CMOD_CHANNEL);
    Slider_AMuxCH0_Connect(Slider_AMuxCH0_CMP_VP_CHANNEL);
    #if (Slider_CURRENT_SOURCE)
        Slider_AMuxCH0_Connect(Slider_AMuxCH0_IDAC_CHANNEL);
    #endif  /* Slider_CURRENT_SOURCE */
    
    #if (Slider_DESIGN_TYPE == Slider_TWO_CHANNELS_DESIGN) 
        Slider_AMuxCH1_Init();
        Slider_AMuxCH1_Connect(Slider_AMuxCH1_CMOD_CHANNEL);
        Slider_AMuxCH1_Connect(Slider_AMuxCH1_CMP_VP_CHANNEL);
        #if (Slider_CURRENT_SOURCE)
            Slider_AMuxCH1_Connect(Slider_AMuxCH1_IDAC_CHANNEL);
        #endif  /* Slider_CURRENT_SOURCE */
    #endif  /* Slider_DESIGN_TYPE */
    
    /* Int Rb */
    #if (Slider_CURRENT_SOURCE == Slider_EXTERNAL_RB)
        Slider_InitRb();
    #endif /* (Slider_CURRENT_SOURCE == Slider_EXTERNAL_RB) */
    
    /* Enable window generation */
    Slider_CONTROL_REG |= Slider_CTRL_WINDOW_EN__CH0;
    #if (Slider_DESIGN_TYPE == Slider_TWO_CHANNELS_DESIGN)
        Slider_CONTROL_REG |= Slider_CTRL_WINDOW_EN__CH1;
    #endif  /* Slider_DESIGN_TYPE */
    
    /* Initialize Cmp and Idac */
    Slider_CompCH0_Init();
    #if (Slider_CURRENT_SOURCE)
        Slider_IdacCH0_Init();
        Slider_IdacCH0_SetPolarity(Slider_IdacCH0_IDIR);
        Slider_IdacCH0_SetRange(Slider_IDAC_RANGE_VALUE);
        Slider_IdacCH0_SetValue(Slider_TURN_OFF_IDAC);
    #endif  /* Slider_CURRENT_SOURCE */
    
    #if (Slider_DESIGN_TYPE == Slider_TWO_CHANNELS_DESIGN) 
        Slider_CompCH1_Init();
        #if (Slider_CURRENT_SOURCE)
            Slider_IdacCH1_Init();
            Slider_IdacCH1_SetPolarity(Slider_IdacCH1_IDIR);
            Slider_IdacCH1_SetRange(Slider_IDAC_RANGE_VALUE);
            Slider_IdacCH1_SetValue(Slider_TURN_OFF_IDAC);
        #endif  /* Slider_CURRENT_SOURCE */
    #endif  /* Slider_DESIGN_TYPE */
    
    /* Initialize Vref if as VDAC */
    #if (Slider_VREF_OPTIONS == Slider_VREF_VDAC)
        Slider_VdacRefCH0_Init();
        #if (Slider_DESIGN_TYPE == Slider_TWO_CHANNELS_DESIGN)
            Slider_VdacRefCH1_Init();
        #endif  /* Slider_DESIGN_TYPE */
    #endif  /* Slider_VREF_OPTIONS */
}


/*******************************************************************************
* Function Name: Slider_Enable
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
void Slider_Enable(void) 
{
    uint8 enableInterrupts;
    
    enableInterrupts = CyEnterCriticalSection();
    
    /* Enable Prescaler */
    #if (Slider_PRESCALER_OPTIONS == Slider_PRESCALER_UDB)
        /* Do nothing  for UDB */
    #elif (Slider_PRESCALER_OPTIONS == Slider_PRESCALER_FF)
        Slider_PRESCALER_ACT_PWRMGR_REG  |= Slider_PRESCALER_ACT_PWR_EN;
        Slider_PRESCALER_STBY_PWRMGR_REG |= Slider_PRESCALER_STBY_PWR_EN;
        
    #else
        /* Do nothing = config without prescaler */
    #endif  /* (Slider_PRESCALER_OPTIONS == Slider_PRESCALER_UDB) */
    
    /* Enable ScanSpeed */
    Slider_SCANSPEED_AUX_CONTROL_REG |= Slider_SCANSPEED_CTRL_ENABLE;
    
    /* Enable Measure CH0 */
    #if (Slider_IMPLEMENTATION_CH0 == Slider_MEASURE_IMPLEMENTATION_FF)
        /* Window PWM */
        Slider_PWM_CH0_ACT_PWRMGR_REG  |= Slider_PWM_CH0_ACT_PWR_EN;
        Slider_PWM_CH0_STBY_PWRMGR_REG |= Slider_PWM_CH0_STBY_PWR_EN;
        
        /* Raw Counter */
        Slider_RAW_CH0_ACT_PWRMGR_REG  |= Slider_RAW_CH0_ACT_PWR_EN;
        Slider_RAW_CH0_STBY_PWRMGR_REG |= Slider_RAW_CH0_STBY_PWR_EN;
        
    #else
        /* Window PWM -  Do nothing */
        /* Raw Counter - Do nothing */
        
    #endif  /* (Slider_IMPLEMENTATION_CH0 == Slider_MEASURE_IMPLEMENTATION_FF) */ 
    
    /* Enable Measure CH1*/
    #if (Slider_DESIGN_TYPE == Slider_TWO_CHANNELS_DESIGN)
        #if (Slider_IMPLEMENTATION_CH1 == Slider_MEASURE_IMPLEMENTATION_FF)
            /* Window PWM */
            Slider_PWM_CH1_ACT_PWRMGR_REG  |= Slider_PWM_CH1_ACT_PWR_EN;
            Slider_PWM_CH1_STBY_PWRMGR_REG |= Slider_PWM_CH1_STBY_PWR_EN;
            
            /* Raw Counter */
            Slider_RAW_CH1_ACT_PWRMGR_REG  |= Slider_RAW_CH1_ACT_PWR_EN;
            Slider_RAW_CH1_STBY_PWRMGR_REG |= Slider_RAW_CH1_STBY_PWR_EN;
           
        #else
        /* Window PWM -  Do nothing */
        /* Raw Counter - Do nothing */
        
        #endif  /* (Slider_IMPLEMENTATION_CH1 == Slider_MEASURE_IMPLEMENTATION_FF) */
    
    #endif  /* (Slider_DESIGN_TYPE == TWO_CHANNELS_DESIGN)*/
    
    /* Enable the Clock */
    #if (Slider_CLOCK_SOURCE == Slider_INTERNAL_CLOCK)
       Slider_IntClock_Enable();
    #endif  /* Slider_CLOCK_SOURCE */
    
    /* Setup Cmp and Idac */
    Slider_CompCH0_Enable();
    #if (Slider_CURRENT_SOURCE)
        Slider_IdacCH0_Enable();
    #endif  /* Slider_CURRENT_SOURCE */
    
    #if (Slider_DESIGN_TYPE == Slider_TWO_CHANNELS_DESIGN) 
        Slider_CompCH1_Enable();
        #if (Slider_CURRENT_SOURCE)
            Slider_IdacCH1_Enable();
        #endif  /* Slider_CURRENT_SOURCE */
    #endif  /* Slider_DESIGN_TYPE */
    
    /* Enable Vref */
    #if (Slider_VREF_OPTIONS == Slider_VREF_VDAC)
        Slider_VdacRefCH0_Enable();
        Slider_VdacRefCH0_SetValue(Slider_VdacRefCH0_DEFAULT_DATA);
        #if (Slider_DESIGN_TYPE == Slider_TWO_CHANNELS_DESIGN)
            Slider_VdacRefCH1_Enable();
            Slider_VdacRefCH1_SetValue(Slider_VdacRefCH1_DEFAULT_DATA);
        #endif  /* (Slider_DESIGN_TYPE == Slider_TWO_CHANNELS_DESIGN) */
    #else
        /* Enable CapSense Buf */
        Slider_BufCH0_STBY_PWRMGR_REG |= Slider_BufCH0_STBY_PWR_EN;
        Slider_BufCH0_ACT_PWRMGR_REG  |= Slider_BufCH0_ACT_PWR_EN;
        
        #if (Slider_DESIGN_TYPE == Slider_TWO_CHANNELS_DESIGN)
            Slider_BufCH1_STBY_PWRMGR_REG |= Slider_BufCH1_STBY_PWR_EN;
            Slider_BufCH1_ACT_PWRMGR_REG  |= Slider_BufCH1_ACT_PWR_EN;
        #endif  /* (Slider_DESIGN_TYPE == Slider_TWO_CHANNELS_DESIGN) */
    #endif  /* (Slider_VREF_VDAC == Slider_VREF_OPTIONS) */
    
    /* Set reference on AMux Bus */
    #if (Slider_VREF_OPTIONS == Slider_VREF_VDAC)
        /* Connect Vdac to AMux Bus */
        Slider_AMuxCH0_Connect(Slider_AMuxCH0_VREF_CHANNEL);
        #if (Slider_DESIGN_TYPE == Slider_TWO_CHANNELS_DESIGN)
            Slider_AMuxCH1_Connect(Slider_AMuxCH1_VREF_CHANNEL);
        #endif  /* (Slider_DESIGN_TYPE == Slider_TWO_CHANNELS_DESIGN) */
        
    #else
        /* Enable CapSense Buf */
        Slider_BufCH0_CAPS_CFG0_REG |= Slider_CSBUF_ENABLE;
        
        #if (Slider_DESIGN_TYPE == Slider_TWO_CHANNELS_DESIGN)
            Slider_BufCH1_CAPS_CFG0_REG |= Slider_CSBUF_ENABLE;
        #endif  /* (Slider_DESIGN_TYPE == Slider_TWO_CHANNELS_DESIGN) */
    #endif  /* (Slider_VREF_VDAC == Slider_VREF_OPTIONS)*/
    
    CyExitCriticalSection(enableInterrupts);
    
    /* Enable interrupt */
    CyIntEnable(Slider_IsrCH0_ISR_NUMBER);
    #if (Slider_DESIGN_TYPE == Slider_TWO_CHANNELS_DESIGN) 
        CyIntEnable(Slider_IsrCH1_ISR_NUMBER);
    #endif  /* Slider_DESIGN_TYPE */
    
    /* Set CapSense Enable state */
    Slider_CONTROL_REG |= Slider_CTRL_CAPSENSE_EN;
}


/*******************************************************************************
* Function Name: Slider_Start
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
*  Slider_initVar - used to check initial configuration, modified on 
*  first function call.
*  Slider_lowLevelTuningDone - used to notify the Tuner GUI that 
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
void Slider_Start(void)  
{
    if (Slider_initVar == 0u)
    {
        Slider_Init();
        Slider_initVar = 1u;
    }
    Slider_Enable();
    
    /* AutoTunning start */
    #if (Slider_TUNING_METHOD == Slider_AUTO_TUNING)
        /* AutoTune by sensor or pair of sensor basis */
        Slider_AutoTune();
        Slider_lowLevelTuningDone = 1u;
    #endif /* (Slider_TUNING_METHOD == Slider_AUTO_TUNING) */
}


/*******************************************************************************
* Function Name: Slider_Stop
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
void Slider_Stop(void) 
{
    /* Stop Capsensing */
    Slider_CONTROL_REG &= (uint8)(~Slider_CTRL_START);
    
    /* Disable interrupt */
    CyIntDisable(Slider_IsrCH0_ISR_NUMBER);
    #if (Slider_DESIGN_TYPE == Slider_TWO_CHANNELS_DESIGN) 
        CyIntDisable(Slider_IsrCH1_ISR_NUMBER);
    #endif  /* Slider_DESIGN_TYPE */
    
    /* Clear all sensors */
    Slider_ClearSensors();
    
    /* Disable Prescaler */
    #if (Slider_PRESCALER_OPTIONS == Slider_PRESCALER_UDB)
        /* Do nothing  for UDB */
    #elif (Slider_PRESCALER_OPTIONS == Slider_PRESCALER_FF)        
        Slider_PRESCALER_ACT_PWRMGR_REG  &= ~Slider_PRESCALER_ACT_PWR_EN;
        Slider_PRESCALER_STBY_PWRMGR_REG &= ~Slider_PRESCALER_STBY_PWR_EN;
        
    #else
        /* Do nothing = config without prescaler */
    #endif  /* (Slider_PRESCALER_OPTIONS == Slider_PRESCALER_UDB) */
    
    /* Disable ScanSpeed */
    Slider_SCANSPEED_AUX_CONTROL_REG &= (uint8)(~Slider_SCANSPEED_CTRL_ENABLE);
    
    /* Disable Measure CH0 */
    #if (Slider_IMPLEMENTATION_CH0 == Slider_MEASURE_IMPLEMENTATION_FF)
        /* Window PWM */
        Slider_PWM_CH0_ACT_PWRMGR_REG  &= ~Slider_PWM_CH0_ACT_PWR_EN;
        Slider_PWM_CH0_STBY_PWRMGR_REG &= ~Slider_PWM_CH0_STBY_PWR_EN;

        /* Raw Counter */
        Slider_RAW_CH0_ACT_PWRMGR_REG  &= ~Slider_RAW_CH0_ACT_PWR_EN;
        Slider_RAW_CH0_STBY_PWRMGR_REG &= ~Slider_RAW_CH0_STBY_PWR_EN;

    #else
        /* Window PWM -  Do nothing */
        /* Raw Counter - Do nothing */
        
    #endif  /* (Slider_IMPLEMENTATION_CH0 == Slider_MEASURE_IMPLEMENTATION_FF) */ 
    
    /* Disable Measure CH1 */
    #if (Slider_DESIGN_TYPE == Slider_TWO_CHANNELS_DESIGN)
        #if (Slider_IMPLEMENTATION_CH1 == Slider_MEASURE_IMPLEMENTATION_FF)
            /* Window PWM */
            Slider_PWM_CH1_ACT_PWRMGR_REG  &= ~Slider_PWM_CH1_ACT_PWR_EN;
            Slider_PWM_CH1_STBY_PWRMGR_REG &= ~Slider_PWM_CH1_STBY_PWR_EN;
    
            /* Raw Counter */
            Slider_RAW_CH1_ACT_PWRMGR_REG  &= ~Slider_RAW_CH1_ACT_PWR_EN;
            Slider_RAW_CH1_STBY_PWRMGR_REG &= ~Slider_RAW_CH1_STBY_PWR_EN;
           
        #else
        /* Window PWM -  Do nothing */
        /* Raw Counter - Do nothing */
        
        #endif  /* (Slider_IMPLEMENTATION_CH1 == Slider_MEASURE_IMPLEMENTATION_FF) */
    
    #endif  /* (Slider_DESIGN_TYPE == TWO_CHANNELS_DESIGN)*/
    
    /* Disable the Clock */
    #if (Slider_CLOCK_SOURCE == Slider_INTERNAL_CLOCK)
       Slider_IntClock_Stop();
    #endif  /* Slider_CLOCK_SOURCE */
    
    /* Disable power from Cmp and Idac */
    Slider_CompCH0_Stop();
    #if (Slider_CURRENT_SOURCE)
        Slider_IdacCH0_Stop();
    #endif  /* Slider_CURRENT_SOURCE */
    
    #if (Slider_DESIGN_TYPE == Slider_TWO_CHANNELS_DESIGN) 
        Slider_CompCH1_Stop();
        #if (Slider_CURRENT_SOURCE)
            Slider_IdacCH1_Stop();
        #endif  /* Slider_CURRENT_SOURCE */
    #endif  /* Slider_DESIGN_TYPE */    
    
    /* Disable Vref if as VDAC */
    #if (Slider_VREF_OPTIONS == Slider_VREF_VDAC)
        Slider_VdacRefCH0_Stop();
        #if (Slider_DESIGN_TYPE == Slider_TWO_CHANNELS_DESIGN)
            Slider_VdacRefCH1_Stop();
        #endif  /* Slider_DESIGN_TYPE */
    #endif  /* Slider_VREF_OPTIONS */

    #if (Slider_VREF_VDAC == Slider_VREF_OPTIONS)
        /* The Idac turn off before */
    #else
        /* Enable CapSense Buf */
        Slider_BufCH0_CAPS_CFG0_REG &= (uint8)(~Slider_CSBUF_ENABLE);
        Slider_BufCH0_ACT_PWRMGR_REG &= (uint8)(~Slider_BufCH0_ACT_PWR_EN);
        Slider_BufCH0_STBY_PWRMGR_REG &= (uint8)(~Slider_BufCH0_STBY_PWR_EN);
        
        #if (Slider_DESIGN_TYPE == Slider_TWO_CHANNELS_DESIGN)
            Slider_BufCH1_CAPS_CFG0_REG &= (uint8)(~Slider_CSBUF_ENABLE);
            Slider_BufCH1_ACT_PWRMGR_REG &= (uint8)(~Slider_BufCH1_ACT_PWR_EN);
            Slider_BufCH1_STBY_PWRMGR_REG &= (uint8)(~Slider_BufCH1_STBY_PWR_EN);
        #endif  /*(Slider_DESIGN_TYPE == Slider_TWO_CHANNELS_DESIGN) */
    #endif  /* (Slider_VREF_VDAC == Slider_VREF_OPTIONS) */
    
    /* Set CapSense Disable state */
    Slider_CONTROL_REG &= (uint8)(~Slider_CTRL_CAPSENSE_EN);
}


#if (Slider_DESIGN_TYPE == Slider_ONE_CHANNEL_DESIGN)
    /*******************************************************************************
    * Function Name: Slider_FindNextSensor
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
    *  Slider_sensorEnableMask[ ] - used to store bit masks of enabled 
    *  sensors.
    *  Slider_sensorEnableMask[0] contains the masked bits for sensors 0
    *  through 7 (sensor 0 is bit 0, sensor 1 is bit 1).
    *  Slider_sensorEnableMask[1] contains the masked bits for sensors 
    *  8 through 15 (if needed), and so on.
    *    0 - sensor doesn't scan by Slider_ScanEnabledWidgets().
    *    1 - sensor scans by Slider_ScanEnabledWidgets().
    *
    * Note: 
    *  This function has effect on current scanning scanning and should not
    *  be used outisde of component.
    *
    *******************************************************************************/
    uint8 Slider_FindNextSensor(uint8 snsIndex) CYREENTRANT
    {
        uint8 pos;
        uint8 enMask;
        
        /* Check if sensor enabled */
        do
        {
            /* Proceed with the next sensor */
            snsIndex++;
            if(snsIndex == Slider_TOTAL_SENSOR_COUNT)
            {
                break;
            }
            pos = (snsIndex >> 3u);
            enMask = 0x01u << (snsIndex & 0x07u);
        }    
        while((Slider_sensorEnableMask[pos] & enMask) == 0u);
        
        return snsIndex;
    }
 #endif  /* Slider_DESIGN_TYPE == Slider_ONE_CHANNEL_DESIGN */
 
 
#if (Slider_DESIGN_TYPE == Slider_TWO_CHANNELS_DESIGN)
    /*******************************************************************************
    * Function Name: Slider_FindNextPair
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
    *  Slider_sensorEnableMask[ ] - used to store bit masks of enabled 
    *  sensors.
    *  Slider_sensorEnableMask[0] contains the masked bits for sensors 0
    *  through 7 (sensor 0 is bit 0, sensor 1 is bit 1).
    *  Slider_sensorEnableMask[1] contains the masked bits for sensors 
    *  8 through 15 (if needed), and so on.
    *    0 - sensor doesn't scan by Slider_ScanEnabledWidgets().
    *    1 - sensor scans by Slider_ScanEnabledWidgets().
    *
    * Note: 
    *  This function has effect on control signals set for scanning and should not
    *  be used outisde of component.
    *
    *******************************************************************************/
    uint8 Slider_FindNextPair(uint8 snsIndex) CYREENTRANT
    {
        uint8 posCh;
        uint8 enMaskCh;
        uint8 newRegValue;
        uint8 indexCh0 = snsIndex;
        uint8 indexCh1 = snsIndex + Slider_TOTAL_SENSOR_COUNT__CH0;
        
        /* Find enabled sensor on channel 0 */
        do
        {
            /* Procced the scanning */
            indexCh0++;
            if (indexCh0 >= Slider_TOTAL_SENSOR_COUNT__CH0)
            {
                /* Lets hadle now all from CH1 */
                indexCh0 = Slider_END_OF_SCAN__CH0;
                break;
            }
            
            posCh = (indexCh0 >> 3u);
            enMaskCh = 0x01u << (indexCh0 & 0x07u);
        }
        while((Slider_sensorEnableMask[posCh] & enMaskCh) == 0u);
        
        /* Find enabled sensor on channel 1 */
        do
        {
            /* Procced the scanning */
            indexCh1++;        
            if (indexCh1 >= Slider_TOTAL_SENSOR_COUNT)
            {
                /* Lets hadle now all from CH0 */
                indexCh1 = Slider_END_OF_SCAN__CH1;
                break;
            }
            
            posCh = (indexCh1 >> 3u);
            enMaskCh = 0x01u << (indexCh1 & 0x07u);
        } 
        while((Slider_sensorEnableMask[posCh] & enMaskCh) == 0u);
        
        indexCh1 -= Slider_TOTAL_SENSOR_COUNT__CH0;
        
        /* Find the pair to scan */
        if(indexCh0 == indexCh1)
        {
            /* Scans TWO Channels */
            snsIndex = indexCh0;
            
            Slider_CONTROL_REG |= (Slider_CTRL_WINDOW_EN__CH0 | 
                                             Slider_CTRL_WINDOW_EN__CH1);
        }
        else if(indexCh0 < indexCh1)
        {
           /* Scans Channel ONE only */
           snsIndex = indexCh0;
           
            newRegValue = Slider_CONTROL_REG;
            newRegValue |= Slider_CTRL_WINDOW_EN__CH0;
            newRegValue &= ~Slider_CTRL_WINDOW_EN__CH1;
            Slider_CONTROL_REG = newRegValue;
        }
        else
        {
            /* Scans Channel TWO only */
            snsIndex = indexCh1;
            
            newRegValue = Slider_CONTROL_REG;
            newRegValue |= Slider_CTRL_WINDOW_EN__CH1;
            newRegValue &= ~Slider_CTRL_WINDOW_EN__CH0;
            Slider_CONTROL_REG = newRegValue;
        }
        
        return (snsIndex);
    }
#endif  /* Slider_DESIGN_TYPE == Slider_TWO_CHANNELS_DESIGN */


/*******************************************************************************
* Function Name: Slider_SetScanSlotSettings
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
*  Slider_idacSettings[] - used to store idac value for every sensor.
*  Slider_widgetResolution[] - used to store scan resolution of every 
*  widget.
*
*******************************************************************************/
void Slider_SetScanSlotSettings(uint8 slot) CYREENTRANT
{
    uint8 widget;
    
    #if (Slider_DESIGN_TYPE == Slider_ONE_CHANNEL_DESIGN)
        /* Define widget sensor belongs to */
        widget = Slider_widgetNumber[slot];
        
        /* Set Idac Value */
        #if (Slider_CURRENT_SOURCE)
            Slider_IdacCH0_SetValue(Slider_idacSettings[slot]);
        #endif  /* Slider_CURRENT_SOURCE */
        
        /* Window PWM */
        #if (Slider_IMPLEMENTATION_CH0 == Slider_MEASURE_IMPLEMENTATION_FF)
            CY_SET_REG16(Slider_PWM_CH0_PERIOD_PTR,
                ((uint16) Slider_widgetResolution[widget] << 8u) | Slider_MEASURE_FULL_RANGE_LOW);
        #else
            Slider_PWM_CH0_PERIOD_HI_REG = Slider_widgetResolution[widget];
        #endif  /* (Slider_IMPLEMENTATION_CH0 == Slider_MEASURE_IMPLEMENTATION_FF) */ 

        #if ( (Slider_MULTIPLE_PRESCALER_ENABLED) || \
              (Slider_TUNING_METHOD == Slider_AUTO_TUNING) )
            Slider_SetPrescaler(Slider_analogSwitchDivider[slot]);
        #elif (Slider_PRESCALER_OPTIONS)
            Slider_SetPrescaler(Slider_analogSwitchDivider);
        #endif /* ((Slider_MULTIPLE_PRESCALER_ENABLED) || \
               *   (Slider_TUNING_METHOD == Slider_AUTO_TUNING))
               */

    #else
        if(slot < Slider_TOTAL_SENSOR_COUNT__CH0)
        {
            /* Define widget sensor belongs to */
            widget = Slider_widgetNumber[slot];
            
            /* Set Idac Value */
            #if (Slider_CURRENT_SOURCE)
                Slider_IdacCH0_SetValue(Slider_idacSettings[slot]);
            #endif  /* Slider_CURRENT_SOURCE */
            
            /* Set Pwm Resolution */
            #if (Slider_IMPLEMENTATION_CH0 == Slider_MEASURE_IMPLEMENTATION_FF)
                CY_SET_REG16(Slider_PWM_CH0_PERIOD_PTR,
                  ((uint16) Slider_widgetResolution[widget] << 8u) | Slider_MEASURE_FULL_RANGE_LOW);
            #else
                Slider_PWM_CH0_PERIOD_HI_REG = Slider_widgetResolution[widget];
            #endif  /* (Slider_IMPLEMENTATION_CH0 == Slider_MEASURE_IMPLEMENTATION_FF)*/ 
        }
        
        if(slot < Slider_TOTAL_SENSOR_COUNT__CH1)
        {
            widget = Slider_widgetNumber[slot+Slider_TOTAL_SENSOR_COUNT__CH0];
        
            /* Set Idac Value */
            #if (Slider_CURRENT_SOURCE)
                Slider_IdacCH1_SetValue(Slider_idacSettings[slot+
                                                                             Slider_TOTAL_SENSOR_COUNT__CH0]);
            #endif  /* Slider_CURRENT_SOURCE */
            
            /* Set Pwm Resolution */
            #if (Slider_IMPLEMENTATION_CH1 == Slider_MEASURE_IMPLEMENTATION_FF)
                CY_SET_REG16(Slider_PWM_CH1_PERIOD_PTR,
                  ((uint16) Slider_widgetResolution[widget] << 8u) | Slider_MEASURE_FULL_RANGE_LOW);
            #else
                Slider_PWM_CH1_PERIOD_HI_REG = Slider_widgetResolution[widget];
            #endif  /* (Slider_IMPLEMENTATION_CH1 == Slider_MEASURE_IMPLEMENTATION_FF)*/ 
        }

        #if ( (Slider_MULTIPLE_PRESCALER_ENABLED) || \
              (Slider_TUNING_METHOD == Slider_AUTO_TUNING) )
            Slider_SetPrescaler(Slider_analogSwitchDivider[slot]);
        #elif (Slider_PRESCALER_OPTIONS)
            Slider_SetPrescaler(Slider_analogSwitchDivider);
        #endif /* ((Slider_MULTIPLE_PRESCALER_ENABLED) || \
               *   (Slider_TUNING_METHOD == Slider_AUTO_TUNING))
               */

    #endif  /* Slider_DESIGN_TYPE */
}


/*******************************************************************************
* Function Name: Slider_ScanSensor
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
*  Slider_csv - used to provide status and mode of scanning process. 
*  Sets busy status(scan in progress) and mode of scan as single scan.
*  For two channel design the additional bits are set to define if scan a 
*  pair of sensors or single one.
*  Slider_sensorIndex - used to store sensor scanning sensor number.
*  Sets to provided sensor argument.
*
* Reentrant:
*  No
*
*******************************************************************************/
void Slider_ScanSensor(uint8 sensor)  
{
    /* Clears status/control variable and set sensorIndex */
    Slider_csv = 0u;
    Slider_sensorIndex = sensor;
    
    #if (Slider_DESIGN_TYPE == Slider_ONE_CHANNEL_DESIGN)
        /* Start of sensor scan */
        Slider_csv = (Slider_SW_STS_BUSY | Slider_SW_CTRL_SINGLE_SCAN);
        Slider_PreScan(sensor);
        
    #else
        /* CH0: check end of scan conditions */
        if(sensor < Slider_TOTAL_SENSOR_COUNT__CH0)
        {
            Slider_CONTROL_REG |= Slider_CTRL_WINDOW_EN__CH0;
        }
        else
        {
            Slider_CONTROL_REG &= ~Slider_CTRL_WINDOW_EN__CH0;
        }
        
        /* CH1: check end of scan conditions */
        if(sensor < Slider_TOTAL_SENSOR_COUNT__CH1)
        {
            Slider_CONTROL_REG |= Slider_CTRL_WINDOW_EN__CH1;
        }
        else
        {
            Slider_CONTROL_REG &= ~Slider_CTRL_WINDOW_EN__CH1;
        }
        
        /* Start sensor scan */
        if( ((Slider_CONTROL_REG & Slider_CTRL_WINDOW_EN__CH0) != 0u) || 
            ((Slider_CONTROL_REG & Slider_CTRL_WINDOW_EN__CH1) != 0u) )
        {
        
            Slider_csv |= (Slider_SW_STS_BUSY | Slider_SW_CTRL_SINGLE_SCAN);
            Slider_PreScan(sensor);
        }
        
    #endif  /* Slider_DESIGN_TYPE */
}


/*******************************************************************************
* Function Name: Slider_ScanEnableWidgets
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
*  Slider_csv - used to provide status and mode of scanning process. 
*  Sets busy status(scan in progress) and clears single scan mode.
*  For two channel design the additional bits are set to define if scan a 
*  pair of sensors or single one. 
*  Slider_sensorIndex - used to store sensor scanning sensor number.
*  Sets to 0xFF and provided to function Slider_FindNextSensor or
*  Slider_FindNextPair, these functions starts with sensor index
*  increment and overflow of uint8 gives desired index 0.
*
* Reentrant:
*  No
*
*******************************************************************************/
void Slider_ScanEnabledWidgets(void) 
{
    /* Clears status/control variable and set sensorIndex */
    Slider_csv = 0u;
    Slider_sensorIndex = 0xFFu;
    
    #if (Slider_DESIGN_TYPE == Slider_ONE_CHANNEL_DESIGN)
        /* Find next sensor */
        Slider_sensorIndex = Slider_FindNextSensor(Slider_sensorIndex);

        /* Check end of scan condition */
        if(Slider_sensorIndex < Slider_TOTAL_SENSOR_COUNT)
        {
            Slider_csv |= Slider_SW_STS_BUSY;
            Slider_PreScan(Slider_sensorIndex);
        }
        
    #else
        /* Find next sensor and set proper control register */
        Slider_sensorIndex = Slider_FindNextPair(Slider_sensorIndex);
        
        /* Start sensor scan */
        if((Slider_sensorIndex < Slider_TOTAL_SENSOR_COUNT__CH0) || 
           (Slider_sensorIndex < Slider_TOTAL_SENSOR_COUNT__CH1))
        {
            Slider_csv |= Slider_SW_STS_BUSY;
            Slider_PreScan(Slider_sensorIndex);
        }
        
    #endif  /* Slider_DESIGN_TYPE */
}


/*******************************************************************************
* Function Name: Slider_IsBusy
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
*  Slider_csv - used to provide status and mode of scanning process. 
*  Checks the busy status.
*
*******************************************************************************/
uint8 Slider_IsBusy(void) 
{
    return ((0u != (Slider_csv & Slider_SW_STS_BUSY)) ? 1u : 0u);
}


/*******************************************************************************
* Function Name: Slider_ReadSensorRaw
********************************************************************************
*
* Summary:
*  Returns scan sensor raw data from the Slider_sensorRaw[] array. 
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
*  Slider_sensorRaw[] - used to store sensors raw data.
*
*******************************************************************************/
uint16 Slider_ReadSensorRaw(uint8 sensor) 
{
    return (Slider_sensorRaw[sensor]);
}


/*******************************************************************************
* Function Name: Slider_ClearSensors
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
void Slider_ClearSensors(void) 
{
    uint8 i;
   
    for (i = 0u; i < Slider_TOTAL_SENSOR_COUNT; i++)
    {
        Slider_DisableScanSlot(i);
    }
}


#if (Slider_IS_COMPLEX_SCANSLOTS)
    /*******************************************************************************
    * Function Name: Slider_EnableScanSlot
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
    *  Slider_portTable[]  - used to store the port number that pin 
    *  belongs to for every sensor.
    *  Slider_maskTable[]  - used to store the pin within the port for 
    *  every sensor.
    *  Slider_indexTable[] - used to store indexes of complex sensors.
    *  The offset and position in this array are stored in port and mask table for 
    *  complex sensors.
    *  The bit 7 (msb) is used to define the sensor type: single or complex.
    *
    *******************************************************************************/
    void Slider_EnableScanSlot(uint8 slot) CYREENTRANT
    {
        uint8 j;
        uint8 snsNumber;
        const uint8 CYCODE *index;
        /* Read the sensor type: single or complex */
        uint8 snsType = Slider_portTable[slot];
        
        /* Check if sensor is complex */
        if ((snsType & Slider_COMPLEX_SS_FLAG) == 0u)
        {
            /* Enable sensor (signle) */
            Slider_EnableSensor(slot);
        }
        else
        {
            /* Enable complex sensor */
            snsType &= ~Slider_COMPLEX_SS_FLAG;
            index = &Slider_indexTable[snsType];
            snsNumber = Slider_maskTable[slot];
                        
            for (j=0u; j < snsNumber; j++)
            {
                Slider_EnableSensor(index[j]);
            }
        } 
    }
    
    
    /*******************************************************************************
    * Function Name: Slider_DisableScanSlot
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
    *  Slider_portTable[]  - used to store the port number that pin 
    *  belongs to for every sensor.
    *  Slider_maskTable[]  - used to store the pin within the port for 
    *  every sensor.
    *  Slider_indexTable[] - used to store indexes of complex sensors.
    *  The offset and position in this array are stored in port and mask table for 
    *  complex sensors.
    *  The 7bit(msb) is used to define the sensor type: single or complex.
    *
    *******************************************************************************/
    void Slider_DisableScanSlot(uint8 slot) CYREENTRANT
    {
        uint8 j;
        uint8 snsNumber;
        const uint8 CYCODE *index;
        /* Read the sensor type: single or complex */
        uint8 snsType = Slider_portTable[slot];
        
        /* Check if sensor is complex */
        if ((snsType & Slider_COMPLEX_SS_FLAG) == 0u)
        {
            /* Disable sensor (signle) */
            Slider_DisableSensor(slot);
        }
        else
        {
            /* Disable complex sensor */
            snsType &= ~Slider_COMPLEX_SS_FLAG;
            index = &Slider_indexTable[snsType];
            snsNumber = Slider_maskTable[slot];
                        
            for (j=0u; j < snsNumber; j++)
            {
                Slider_DisableSensor(index[j]);
            }
        } 
    }
#endif  /* Slider_IS_COMPLEX_SCANSLOTS */


/*******************************************************************************
* Function Name: Slider_EnableSensor
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
*  Slider_portTable[] - used to store the port number that pin 
*  belongs to for every sensor.
*  Slider_maskTable[] - used to store the pin within the port for 
*  every sensor.
*  Slider_csTable[]   - used to store the pointers to CAPS_SEL 
*  registers for every port.
*  Slider_pcTable[]   - used to store the pointers to PC pin 
*  register for every sensor.
*  Slider_amuxIndex[] - used to store corrected AMUX index when 
*  complex sensors are defeined.
*
*******************************************************************************/
void Slider_EnableSensor(uint8 sensor) CYREENTRANT
{
    uint8 port = Slider_portTable[sensor];
    uint8 mask = Slider_maskTable[sensor];
    
    #if ((Slider_DESIGN_TYPE == Slider_TWO_CHANNELS_DESIGN) && \
         (Slider_IS_COMPLEX_SCANSLOTS))
        uint8 amuxCh = Slider_amuxIndex[sensor];
    #endif  /* ((Slider_DESIGN_TYPE == Slider_TWO_CHANNELS_DESIGN) && \
            *   (Slider_IS_COMPLEX_SCANSLOTS))
            */
    
    /* Make sensor High-Z */
    *Slider_pcTable[sensor] = Slider_PRT_PC_HIGHZ;
    
    /* Connect to DSI output */
	if(port == 15u)
	{
		port = 7u;
	}
    *Slider_csTable[port] |= mask;
    
    /* Connect to AMUX */
    #if (Slider_DESIGN_TYPE == Slider_ONE_CHANNEL_DESIGN)
        #if (Slider_IS_COMPLEX_SCANSLOTS)
            Slider_AMuxCH0_Connect(Slider_amuxIndex[sensor]);
        #else
            Slider_AMuxCH0_Connect(sensor);
        #endif  /* Slider_IS_COMPLEX_SCANSLOTS */
                
    #else
        #if (Slider_IS_COMPLEX_SCANSLOTS)
            if ((amuxCh & Slider_CHANNEL1_FLAG) == 0u)
            {
                Slider_AMuxCH0_Connect(amuxCh);
            } 
            else
            {
                amuxCh &= ~ Slider_CHANNEL1_FLAG;
                Slider_AMuxCH1_Connect(amuxCh);
            }
            
        #else
            if (sensor < Slider_TOTAL_SENSOR_COUNT__CH0) 
            {
                Slider_AMuxCH0_Connect(sensor);
            } 
            else
            {
                Slider_AMuxCH1_Connect(sensor - Slider_TOTAL_SENSOR_COUNT__CH0);
            }
            
        #endif  /* Slider_IS_COMPLEX_SCANSLOTS */
        
    #endif  /* Slider_DESIGN_TYPE == Slider_ONE_CHANNEL_DESIGN */
}


/*******************************************************************************
* Function Name: Slider_DisableSensor
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
*  Slider_portTable[] - used to store the port number that pin 
*  belongs to for every sensor.
*  Slider_maskTable[] - used to store the pin within the port for 
*  every sensor.
*  Slider_csTable[]   - used to store the pointers to CAPS_SEL 
*  registers for every port.
*  Slider_pcTable[]   - used to store the pointers to PC pin 
*  register for every sensor.
*  Slider_amuxIndex[] - used to store corrected AMUX index when 
*  complex sensors are defeined.
*
*******************************************************************************/
void Slider_DisableSensor(uint8 sensor) CYREENTRANT
{
    uint8 port = Slider_portTable[sensor];
    uint8 mask = Slider_maskTable[sensor];
    
    #if ((Slider_DESIGN_TYPE == Slider_TWO_CHANNELS_DESIGN) && \
         (Slider_IS_COMPLEX_SCANSLOTS))
        uint8 amuxCh = Slider_amuxIndex[sensor];
    #endif  /* ((Slider_DESIGN_TYPE == Slider_TWO_CHANNELS_DESIGN) && \
            *   (Slider_IS_COMPLEX_SCANSLOTS))
            */
    
    /* Disconnect from AMUX */
    #if (Slider_DESIGN_TYPE == Slider_ONE_CHANNEL_DESIGN)
        #if (Slider_IS_COMPLEX_SCANSLOTS)
            Slider_AMuxCH0_Disconnect(Slider_amuxIndex[sensor]);
        #else
            Slider_AMuxCH0_Disconnect(sensor);
        #endif  /* Slider_IS_COMPLEX_SCANSLOTS */
                
    #else
        #if (Slider_IS_COMPLEX_SCANSLOTS)
            if ((amuxCh & Slider_CHANNEL1_FLAG) == 0u)
            {
                Slider_AMuxCH0_Disconnect(amuxCh);
            } 
            else
            {
                amuxCh &= ~ Slider_CHANNEL1_FLAG;
                Slider_AMuxCH1_Disconnect(amuxCh);
            }
            
        #else
            if (sensor < Slider_TOTAL_SENSOR_COUNT__CH0) 
            {
                Slider_AMuxCH0_Disconnect(sensor);
            } 
            else
            {
                Slider_AMuxCH1_Disconnect(sensor - Slider_TOTAL_SENSOR_COUNT__CH0);
            }
            
        #endif  /* Slider_IS_COMPLEX_SCANSLOTS */
        
    #endif  /* Slider_DESIGN_TYPE */
    
    /* Disconnect from DSI output */
	if(port == 15u)
	{
		port = 7u;
	}
    *Slider_csTable[port] &= (uint8)~mask;
    
    /* Set sensor to inactive state */
    #if (Slider_CONNECT_INACTIVE_SNS == Slider_CIS_GND)
        *Slider_pcTable[sensor] = Slider_PRT_PC_GND;
    #elif (Slider_CONNECT_INACTIVE_SNS == Slider_CIS_HIGHZ)
        *Slider_pcTable[sensor] = Slider_PRT_PC_HIGHZ;
    #else
        *Slider_pcTable[sensor] = Slider_PRT_PC_SHIELD;
    #endif  /* (Slider_CONNECT_INACTIVE_SNS == Slider_CIS_GND) */
}


/*******************************************************************************
* Function Name: Slider_PreScan
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
*  Slider_rbTable[] - used to store pointers to PC pin registers for 
*  every bleed resistor (Rb). Only available when Current Source is External 
*  resistor.
*
*******************************************************************************/
void Slider_PreScan(uint8 sensor) CYREENTRANT
{
    /* Set Sensor Settings */
    Slider_SetScanSlotSettings(sensor);
    
    /* Place disable interrupts here to eliminate influence on start of scanning */
    /* `#START Slider_PreScan_DisableInt` */

    /* `#END` */
    
    #ifdef Slider_PRE_SCAN_DISABLE_INT_CALLBACK
    Slider_PreScan_DisableInt_Callback();
    #endif /* Slider_PRE_SCAN_DISABLE_INT_CALLBACK */

    /* Resets digital and pre-charge clocks */
    Slider_CONTROL_REG |= Slider_CTRL_SYNC_EN;
        
    #if (Slider_DESIGN_TYPE == Slider_ONE_CHANNEL_DESIGN)
        #if (Slider_CURRENT_SOURCE == Slider_IDAC_SOURCE)
            /* Disable Vref from AMux */
            #if (Slider_VREF_VDAC == Slider_VREF_OPTIONS)
                Slider_AMuxCH0_Disconnect(Slider_AMuxCH0_VREF_CHANNEL);
            #else
                Slider_BufCH0_CAPS_CFG0_REG &= ~Slider_CSBUF_ENABLE;
            #endif  /* (Slider_VREF_VDAC != Slider_VREF_OPTIONS) */

            /* Enable Sensor */
            Slider_EnableScanSlot(sensor);
            
        #elif (Slider_CURRENT_SOURCE == Slider_IDAC_SINK)
            /* Connect IDAC */
            Slider_AMuxCH0_Connect(Slider_AMuxCH0_IDAC_CHANNEL);
            
            /* Enable Sensor */
            Slider_EnableScanSlot(sensor);
                
            /* Disable CapSense Buffer */
            Slider_BufCH0_CAPS_CFG0_REG &= (uint8)~Slider_CSBUF_ENABLE;
            
        #else
            /* Connect DSI output to Rb */
            *Slider_rbTable[Slider_extRbCh0Cur] |= Slider_BYP_MASK;
            
            /* Enable Sensor */
            Slider_EnableScanSlot(sensor);
             
            /* Disable CapSense Buffer */
            Slider_BufCH0_CAPS_CFG0_REG &= ~Slider_CSBUF_ENABLE;
        
        #endif  /* (Slider_CURRENT_SOURCE == Slider_IDAC_SOURCE) */
        
    #else

        if((Slider_CONTROL_REG & Slider_CTRL_WINDOW_EN__CH0) != 0u)
        {
            #if (Slider_CURRENT_SOURCE == Slider_IDAC_SOURCE)
                /* Disable Vref from AMux */
                #if (Slider_VREF_VDAC == Slider_VREF_OPTIONS)
                    Slider_AMuxCH0_Disconnect(Slider_AMuxCH0_VREF_CHANNEL);
                #else
                    Slider_BufCH0_CAPS_CFG0_REG &= ~Slider_CSBUF_ENABLE;
                #endif  /* (Slider_VREF_VDAC != Slider_VREF_OPTIONS) */
                
                /* Enable Sensor */
                Slider_EnableScanSlot(sensor);
                
            #elif (Slider_CURRENT_SOURCE == Slider_IDAC_SINK)
                /* Connect IDAC */
                Slider_AMuxCH0_Connect(Slider_AMuxCH0_IDAC_CHANNEL);
                
                /* Enable Sensor */
                Slider_EnableScanSlot(sensor);
                    
                /* Disable Vref from AMux */
                Slider_BufCH0_CAPS_CFG0_REG &= ~Slider_CSBUF_ENABLE;
                
            #else
                /* Connect DSI output to Rb */
                *Slider_rbTable[Slider_extRbCh0Cur] |= Slider_BYP_MASK;
                
                /* Enable Sensor */
                Slider_EnableScanSlot(sensor);
                    
                /* Disable Vref from AMux */
                Slider_BufCH0_CAPS_CFG0_REG &= ~Slider_CSBUF_ENABLE;
            
            #endif  /* (Slider_CURRENT_SOURCE == Slider_IDAC_SOURCE) */
            
        }
        
        if((Slider_CONTROL_REG & Slider_CTRL_WINDOW_EN__CH1) != 0u)
        {
            sensor += Slider_TOTAL_SENSOR_COUNT__CH0;
            
            #if (Slider_CURRENT_SOURCE == Slider_IDAC_SOURCE)
                /* Disable Vref from AMux */
                #if (Slider_VREF_VDAC == Slider_VREF_OPTIONS)
                   Slider_AMuxCH1_Disconnect(Slider_AMuxCH1_VREF_CHANNEL);
                #else 
                    Slider_BufCH1_CAPS_CFG0_REG &= ~Slider_CSBUF_ENABLE;
                #endif  /* (Slider_VREF_VDAC == Slider_VREF_OPTIONS) */
                
                /* Enable Sensor */
                Slider_EnableScanSlot(sensor);
                
            #elif (Slider_CURRENT_SOURCE == Slider_IDAC_SINK)
                /* Connect IDAC */
                Slider_AMuxCH1_Connect(Slider_AMuxCH1_IDAC_CHANNEL);
                
                /* Enable Sensor */
                Slider_EnableScanSlot(sensor);
                    
                /* Disable Vref from AMux */
                Slider_BufCH1_CAPS_CFG0_REG &= ~Slider_CSBUF_ENABLE;
                
            #else
                /* Connect DSI output to Rb */
                *Slider_rbTable[Slider_extRbCh1Cur] |= Slider_BYP_MASK;
                
                /* Enable Sensor */
                Slider_EnableScanSlot(sensor);
                
                /* Disable Vref from AMux */
                Slider_BufCH1_CAPS_CFG0_REG &= ~Slider_CSBUF_ENABLE;
            
            #endif  /* (Slider_CURRENT_SOURCE == Slider_IDAC_SOURCE) */
        }
    
    #endif  /* (Slider_DESIGN_TYPE == Slider_ONE_CHANNEL_DESIGN) */
    
    /* Start measurament, pre-charge clocks are running and PRS as well */
    Slider_CONTROL_REG |= Slider_CTRL_START;
    
    /* Place enable interrupts here to eliminate influence on start of scanning */
    /* `#START Slider_PreScan_EnableInt` */

    /* `#END` */

    #ifdef Slider_PRE_SCAN_ENABLE_INT_CALLBACK
        Slider_PreScan_EnableInt_Callback();
    #endif /* Slider_PRE_SCAN_ENABLE_INT_CALLBACK */
}


#if (Slider_DESIGN_TYPE == Slider_ONE_CHANNEL_DESIGN)
    /*******************************************************************************
    * Function Name: Slider_PostScan
    ********************************************************************************
    *
    * Summary:
    *  Store results of measurament in Slider_SensorResult[] array,
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
    *  Slider_sensorRaw[] - used to store sensors raw data.
    *
    * Reentrant:
    *  No
    *
    *******************************************************************************/
    void Slider_PostScan(uint8 sensor) CYREENTRANT
    {
        /* Stop Capsensing and rearm sync */
        Slider_CONTROL_REG &= (uint8)~(Slider_CTRL_START | Slider_CTRL_SYNC_EN);
        
        /* Read SlotResult from Raw Counter */
        #if (Slider_IMPLEMENTATION_CH0 == Slider_MEASURE_IMPLEMENTATION_FF)
            Slider_sensorRaw[sensor]  = Slider_MEASURE_FULL_RANGE - 
                                                      CY_GET_REG16(Slider_RAW_CH0_COUNTER_PTR);
        #else
            Slider_sensorRaw[sensor]  = ((uint16) Slider_RAW_CH0_COUNTER_HI_REG << 8u);
            Slider_sensorRaw[sensor] |= (uint16) Slider_RAW_CH0_COUNTER_LO_REG;
            Slider_sensorRaw[sensor]  = Slider_MEASURE_FULL_RANGE -
                                                      Slider_sensorRaw[sensor];
        #endif  /* (Slider_IMPLEMENTATION == Slider_MEASURE_IMPLEMENTATION_FF) */
        
        /* Disable Sensor */
        Slider_DisableScanSlot(sensor);
        
        #if(Slider_CURRENT_SOURCE)
            /* Turn off IDAC */
            Slider_IdacCH0_SetValue(Slider_TURN_OFF_IDAC);
            #if (Slider_CURRENT_SOURCE == Slider_IDAC_SINK)
                /* Disconnect IDAC */
                Slider_AMuxCH0_Disconnect(Slider_AMuxCH0_IDAC_CHANNEL);
            #endif  /* (Slider_CURRENT_SOURCE == Slider_IDAC_SINK) */
        #else
            /* Disconnect DSI output from Rb */
            *Slider_rbTable[Slider_extRbCh0Cur] &= ~Slider_BYP_MASK; 
        #endif  /* (Slider_CURRENT_SOURCE)*/
            
        /* Enable Vref on AMUX */
        #if (Slider_VREF_OPTIONS == Slider_VREF_VDAC)
            Slider_AMuxCH0_Connect(Slider_AMuxCH0_VREF_CHANNEL);
        #else
            Slider_BufCH0_CAPS_CFG0_REG |= Slider_CSBUF_ENABLE;
        #endif  /* (Slider_VREF_VDAC == Slider_VREF_OPTIONS) */
    }
    
#else

    /*******************************************************************************
    * Function Name: Slider_PostScan
    ********************************************************************************
    *
    * Summary:
    *  Store results of measurament in Slider_SensorResult[] array,
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
    *  Slider_sensorRaw[] - used to store sensors raw data.
    *
    * Reentrant:
    *  No
    *
    *******************************************************************************/
    void Slider_PostScanCh0(uint8 sensor) CYREENTRANT
    {
        if (((Slider_CONTROL_REG & Slider_CTRL_WINDOW_EN__CH0) == 0u) && 
            ((Slider_CONTROL_REG & Slider_CTRL_WINDOW_EN__CH1) == 0u)) 
        {
            /* Stop Capsensing and rearm sync */
            Slider_CONTROL_REG &= ~(Slider_CTRL_START | Slider_CTRL_SYNC_EN);
        }
        
        /* Read SlotResult from Raw Counter */
        #if (Slider_IMPLEMENTATION_CH0 == Slider_MEASURE_IMPLEMENTATION_FF)
            Slider_sensorRaw[sensor]  = Slider_MEASURE_FULL_RANGE - 
                                                      CY_GET_REG16(Slider_RAW_CH0_COUNTER_PTR);
        #else
            Slider_sensorRaw[sensor]  = ((uint16) Slider_RAW_CH0_COUNTER_HI_REG << 8u);
            Slider_sensorRaw[sensor] |= (uint16) Slider_RAW_CH0_COUNTER_LO_REG;
            Slider_sensorRaw[sensor]  = Slider_MEASURE_FULL_RANGE - 
                                                      Slider_sensorRaw[sensor];
        #endif  /* (Slider_IMPLEMENTATION_CH0 == Slider_MEASURE_IMPLEMENTATION_FF)*/
        
        /* Disable Sensor */
        Slider_DisableScanSlot(sensor);
        
        #if (Slider_CURRENT_SOURCE)
            /* Turn off IDAC */
            Slider_IdacCH0_SetValue(Slider_TURN_OFF_IDAC);
            #if (Slider_CURRENT_SOURCE == Slider_IDAC_SINK)
                /* Disconnect IDAC */
                Slider_AMuxCH0_Disconnect(Slider_AMuxCH0_IDAC_CHANNEL);
            #endif  /* (Slider_CURRENT_SOURCE == Slider_IDAC_SINK) */
        #else
            /* Disconnect DSI output from Rb */
            *Slider_rbTable[Slider_extRbCh0Cur] &= ~Slider_BYP_MASK; 
        #endif  /* (Slider_CURRENT_SOURCE)*/
        
        /* Enable Vref on AMUX */
        #if (Slider_VREF_OPTIONS == Slider_VREF_VDAC)
            Slider_AMuxCH0_Connect(Slider_AMuxCH0_VREF_CHANNEL);
        #else
            Slider_BufCH0_CAPS_CFG0_REG |= Slider_CSBUF_ENABLE;
        #endif  /* (Slider_VREF_VDAC == Slider_VREF_OPTIONS) */
    }
    
    
    /*******************************************************************************
    * Function Name: Slider_PostScanCh1
    ********************************************************************************
    *
    * Summary:
    *  Store results of measurament in Slider_SensorResult[] array,
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
    *  Slider_sensorRaw[] - used to store sensors raw data.
    *
    * Reentrant:
    *  No
    *
    *******************************************************************************/
    void Slider_PostScanCh1(uint8 sensor) CYREENTRANT
    {
        if (((Slider_CONTROL_REG & Slider_CTRL_WINDOW_EN__CH0) == 0u) && 
            ((Slider_CONTROL_REG & Slider_CTRL_WINDOW_EN__CH1) == 0u))
        {
            /* Stop Capsensing and rearm sync */
            Slider_CONTROL_REG &= ~(Slider_CTRL_START | Slider_CTRL_SYNC_EN);
        }
        
        /* Read SlotResult from Raw Counter */
        #if (Slider_IMPLEMENTATION_CH1 == Slider_MEASURE_IMPLEMENTATION_FF)
            Slider_sensorRaw[sensor]  = Slider_MEASURE_FULL_RANGE - 
                                                      CY_GET_REG16(Slider_RAW_CH1_COUNTER_PTR);
        #else
            Slider_sensorRaw[sensor]  = ((uint16) Slider_RAW_CH1_COUNTER_HI_REG << 8u);
            Slider_sensorRaw[sensor] |= (uint16) Slider_RAW_CH1_COUNTER_LO_REG;
            Slider_sensorRaw[sensor]  = Slider_MEASURE_FULL_RANGE - 
                                                      Slider_sensorRaw[sensor];
        #endif  /* (Slider_IMPLEMENTATION_CH1 == Slider_MEASURE_IMPLEMENTATION_FF)*/
        
        /* Disable Sensor */
        Slider_DisableScanSlot(sensor);
        
        #if (Slider_CURRENT_SOURCE)
            /* Turn off IDAC */
            Slider_IdacCH1_SetValue(Slider_TURN_OFF_IDAC);
            #if (Slider_CURRENT_SOURCE == Slider_IDAC_SINK)
                /* Disconnect IDAC */
                Slider_AMuxCH1_Disconnect(Slider_AMuxCH1_IDAC_CHANNEL);
            #endif  /* (Slider_CURRENT_SOURCE == Slider_IDAC_SINK) */
        #else
            /* Disconnect DSI output from Rb */
            *Slider_rbTable[Slider_extRbCh1Cur] &= ~Slider_BYP_MASK; 
        #endif  /* (Slider_CURRENT_SOURCE)*/

        /* Enable Vref on AMUX */
        #if (Slider_VREF_OPTIONS == Slider_VREF_VDAC)
            Slider_AMuxCH1_Connect(Slider_AMuxCH1_VREF_CHANNEL);
        #else
            Slider_BufCH1_CAPS_CFG0_REG |= Slider_CSBUF_ENABLE;
        #endif  /* (Slider_VREF_VDAC == Slider_VREF_OPTIONS) */
    }
    
#endif  /* Slider_DESIGN_TYPE */


#if (Slider_CURRENT_SOURCE == Slider_EXTERNAL_RB)
    /*******************************************************************************
    * Function Name:  Slider_InitRb
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
    void Slider_InitRb(void) 
    {
        uint8 i;
        
        /* Disable all Rb */
        for(i = 0u; i < Slider_TOTAL_RB_NUMBER; i++)
        {
            /* Make High-Z */
            *Slider_rbTable[i] = Slider_PRT_PC_HIGHZ;
        }
    }
    
    
    /*******************************************************************************
    * Function Name: Slider_SetRBleed
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
    *  Slider_extRbCh0Cur - used to store current number of active 
    *  bleed resistor (Rb) of channel 0.
    *  Slider_extRbCh1Cur - used to store current number of active 
    *  bleed resistor (Rb) of channel 1.
    *  The active bleed resistor (Rb) pin will be used while next measurement  
    *  cycle.
    *
    * Reentrant:
    *  No
    *
    *******************************************************************************/
    void Slider_SetRBleed(uint8 rbleed) 
    {
        #if (Slider_DESIGN_TYPE == Slider_ONE_CHANNEL_DESIGN)
            Slider_extRbCh0Cur = rbleed;
            
        #else
            if(rbleed < Slider_TOTAL_RB_NUMBER__CH0)
            {
                Slider_extRbCh0Cur = rbleed;
            }
            else
            {
                Slider_extRbCh1Cur = (rbleed - Slider_TOTAL_RB_NUMBER__CH0);   
            }
    
        #endif  /* Slider_DESIGN_TYPE == Slider_ONE_CHANNEL_DESIGN */ 
    }
#endif /* Slider_CURRENT_SOURCE == Slider_EXTERNAL_RB */ 

#if (Slider_PRESCALER_OPTIONS)
    /*******************************************************************************
    * Function Name: Slider_SetPrescaler
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
    void Slider_SetPrescaler(uint8 prescaler) CYREENTRANT
    {
        /* Set Prescaler */
        #if (Slider_PRESCALER_OPTIONS == Slider_PRESCALER_UDB)
            Slider_PRESCALER_PERIOD_REG = prescaler;
            Slider_PRESCALER_COMPARE_REG = (prescaler >> 1u);
        #elif (Slider_PRESCALER_OPTIONS == Slider_PRESCALER_FF)
            CY_SET_REG16(Slider_PRESCALER_PERIOD_PTR, (uint16) prescaler);
            CY_SET_REG16(Slider_PRESCALER_COMPARE_PTR, (uint16) (prescaler >> 1u));
        #else
            /* Do nothing = config without prescaler */
        #endif  /* (Slider_PRESCALER_OPTIONS == Slider_PRESCALER_UDB) */
    }


    /*******************************************************************************
    * Function Name: Slider_GetPrescaler
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
    uint8 Slider_GetPrescaler(void) 
    {
        uint8 prescaler = 0u;

        /* Get Prescaler */
        #if (Slider_PRESCALER_OPTIONS == Slider_PRESCALER_UDB)
            prescaler = Slider_PRESCALER_PERIOD_REG;
            
        #elif (Slider_PRESCALER_OPTIONS == Slider_PRESCALER_FF)
            prescaler = (uint8) CY_GET_REG16(Slider_PRESCALER_PERIOD_PTR);
            
        #else
            /* Do nothing = config without prescaler */
        #endif  /* (Slider_PRESCALER_OPTIONS == Slider_PRESCALER_UDB) */
        
        return (prescaler);
    }
#endif  /* Slider_PRESCALER_OPTIONS */


/*******************************************************************************
* Function Name: Slider_SetScanSpeed
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
void Slider_SetScanSpeed(uint8 scanSpeed) 
{
    Slider_SCANSPEED_PERIOD_REG = scanSpeed; 
}


#if (Slider_PRS_OPTIONS)
    /*******************************************************************************
    * Function Name: Slider_SetAnalogSwitchesSource
    ********************************************************************************
    *
    * Summary:
    *  Selects the Analog switches source between PRS and prescaler. It is useful
    *  for sensor capacitance determination for sensors with low self-capacitance.
    *  This function is used in auto-tuning procedure.
    *
    * Parameters:
    *  src:  analog switches source:
    *           Slider_ANALOG_SWITCHES_SRC_PRESCALER - selects prescaler
    *           Slider_ANALOG_SWITCHES_SRC_PRS - selects PRS
    *
    * Return:
    *  None
    *
    * Reentrant:
    *  No
    *******************************************************************************/
    void Slider_SetAnalogSwitchesSource(uint8 src)
                      
    {
        if(src == Slider_ANALOG_SWITCHES_SRC_PRESCALER)
        {
            Slider_CONTROL_REG &= (uint8)~0x10u;
        }
        else
        {
            Slider_CONTROL_REG |= 0x10u;
        }
    }
#endif /* (Slider_PRS_OPTIONS) */

/* [] END OF FILE */
