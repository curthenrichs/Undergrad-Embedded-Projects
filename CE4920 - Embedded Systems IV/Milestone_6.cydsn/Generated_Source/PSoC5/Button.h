/*******************************************************************************
* File Name: Button.h
* Version 3.50
*
* Description:
*  This file provides constants and parameter values for the CapSense CSD
*  component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2013, Cypress Semicondu)ctor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end u)ser license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CAPSENSE_CSD_Button_H)
#define CY_CAPSENSE_CSD_Button_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cydevice_trm.h"
#include "CyLib.h"


#include "Button_IntClock.h"

#include "Button_AMuxCH0.h"
#include "Button_CompCH0.h"
#include "Button_IdacCH0.h"




/***************************************
*   Condition compilation parameters
***************************************/

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component CapSense_CSD_v3_50 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */


#define Button_DESIGN_TYPE                (1u)

#define Button_CONNECT_INACTIVE_SNS       (0u)
#define Button_IS_COMPLEX_SCANSLOTS       (0u)

#define Button_CLOCK_SOURCE               (0u)

#define Button_CURRENT_SOURCE             (1u)
#define Button_IDAC_RANGE_VALUE           (4u)

#define Button_PRESCALER_OPTIONS          (1u)
#define Button_MULTIPLE_PRESCALER_ENABLED (1u)

#define Button_PRS_OPTIONS                (2u)
#define Button_SCANSPEED_VALUE            (7u)

#define Button_VREF_OPTIONS               (0u)

#define Button_WATER_PROOF                (0u)

#define Button_TUNING_METHOD              (2u)
#define Button_TUNER_API_GENERATE         (0u)

#define Button_IMPLEMENTATION_CH0         (1u)
#define Button_IMPLEMENTATION_CH1         (1u)

#define Button_GUARD_SENSOR               (0u)

/* Design types definitions */
#define Button_ONE_CHANNEL_DESIGN         (1u)
#define Button_TWO_CHANNELS_DESIGN        (2u)

/* Clock sources definitions */
#define Button_INTERNAL_CLOCK             (0u)
#define Button_EXTERNAL_CLOCK             (1u)

/* Current source definitions */
#define Button_EXTERNAL_RB                (0u)
#define Button_IDAC_SOURCE                (1u)
#define Button_IDAC_SINK                  (2u)

/* Prescaler option definitions */
#define Button_PRESCALER_NONE             (0u)
#define Button_PRESCALER_UDB              (1u)
#define Button_PRESCALER_FF               (2u)

/* Prs options definitions */
#define Button_PRS_NONE                   (0u)
#define Button_PRS_8BITS                  (1u)
#define Button_PRS_16BITS                 (2u)
#define Button_PRS_16BITS_4X              (3u)

/* Seed values */
#define Button_PRS8_SEED_VALUE            (0xFFu)
#define Button_PRS16_SEED_VALUE           (0xFFFFu)

/* Reference source types definitions */
#define Button_VREF_REFERENCE_1_024V      (0u)
#define Button_VREF_REFERENCE_1_2V        (1u)
#define Button_VREF_VDAC                  (2u)

/* Connection of inactive sensors definitions */
#define Button_CIS_GND                    (0u)
#define Button_CIS_HIGHZ                  (1u)
#define Button_CIS_SHIELD                 (2u)

/* Method of tunning */
#define Button_NO_TUNING                  (0u)
#define Button_MANUAL_TUNING              (1u)
#define Button_AUTO_TUNING                (2u)

/* Measure Channel implementation */
#define Button_MEASURE_IMPLEMENTATION_FF  (0u)
#define Button_MEASURE_IMPLEMENTATION_UDB (1u)

/* Guard sensor definition */
#define Button_GUARD_SENSOR_DISABLE       (0u)
#define Button_GUARD_SENSOR_ENABLE        (1u)


/***************************************
*       Type defines
***************************************/

/* Structure to save registers before go to sleep */
typedef struct
{
    uint8 enableState;

    /* Set CONTROL_REG */
    uint8 ctrlReg;
} Button_BACKUP_STRUCT;


/***************************************
*        Function Prototypes
***************************************/

void Button_Init(void) ;
void Button_Enable(void) ;
void Button_Start(void) ;
void Button_Stop(void) ;
void Button_SaveConfig(void) ;
void Button_Sleep(void) ;
void Button_RestoreConfig(void) ;
void Button_Wakeup(void) ;
uint8 Button_IsBusy(void) ;
void Button_ScanSensor(uint8 sensor) ;
void Button_ScanEnabledWidgets(void) ;
void Button_SetScanSlotSettings(uint8 slot) CYREENTRANT;
uint16 Button_ReadSensorRaw(uint8 sensor) ;
void Button_ClearSensors(void) ;
void Button_EnableSensor(uint8 sensor) CYREENTRANT;
void Button_DisableSensor(uint8 sensor) CYREENTRANT;

void Button_SetAnalogSwitchesSource(uint8 src) ;

#if (Button_CURRENT_SOURCE == Button_EXTERNAL_RB)
    void Button_SetRBleed(uint8 rbeed) ;
#endif  /* (Button_CURRENT_SOURCE == Button_EXTERNAL_RB) */

/* Interrupt handler */
CY_ISR_PROTO(Button_IsrCH0_ISR);
#if (Button_DESIGN_TYPE == Button_TWO_CHANNELS_DESIGN)
    CY_ISR_PROTO(Button_IsrCH1_ISR);
#endif  /* (Button_DESIGN_TYPE == Button_TWO_CHANNELS_DESIGN) */

#if (Button_PRESCALER_OPTIONS)
    uint8 Button_GetPrescaler(void) ;
#endif /* (Button_PRESCALER_OPTIONS) */

/***************************************
*           API Constants
***************************************/

#define Button_TOTAL_SENSOR_COUNT            (2u)
#define Button_TOTAL_SCANSLOT_COUNT            (2u)
#define Button_TOTAL_RB_NUMBER            (1u)
#define Button_TOTAL_RB_NUMBER__CH0            (1u)

/* Define Sensors */
#define Button_SENSOR_LEFT__BTN    (0u)
#define Button_SENSOR_RIGHT__BTN    (1u)
/* AMux Cmod, Comparator and Idac Channels definitions */
#define Button_AMuxCH0_CMOD_CHANNEL          (2u)
#define Button_AMuxCH0_CMP_VP_CHANNEL        (3u)
#define Button_AMuxCH0_IDAC_CHANNEL          (4u)



/* Vdac value for Vref = Vdac */
#define Button_VREF_VDAC_VALUE            (64u)

/* Scan Speed Type */
#define Button_SCAN_SPEED_ULTRA_FAST      (0x01u)
#define Button_SCAN_SPEED_FAST            (0x03u)
#define Button_SCAN_SPEED_NORMAL          (0x07u)
#define Button_SCAN_SPEED_SLOW            (0x0Fu)

/* PWM Resolution */
#define Button_PWM_WINDOW_APPEND          (0xFEu)
#define Button_PWM_RESOLUTION_8_BITS      (0x00u)
#define Button_PWM_RESOLUTION_9_BITS      (0x01u)
#define Button_PWM_RESOLUTION_10_BITS     (0x03u)
#define Button_PWM_RESOLUTION_11_BITS     (0x07u)
#define Button_PWM_RESOLUTION_12_BITS     (0x0Fu)
#define Button_PWM_RESOLUTION_13_BITS     (0x1Fu)
#define Button_PWM_RESOLUTION_14_BITS     (0x3Fu)
#define Button_PWM_RESOLUTION_15_BITS     (0x7Fu)
#define Button_PWM_RESOLUTION_16_BITS     (0xFFu)

/* Software Status Register Bit Masks */
#define Button_SW_STS_BUSY                (0x01u)
/* Software Status Register Bit Masks */
#define Button_SW_CTRL_SINGLE_SCAN        (0x80u)

/* Init Idac current */
#define Button_TURN_OFF_IDAC              (0x00u)

/* Rbleed definitions */
#define Button_RBLEED1                    (0u)
#define Button_RBLEED2                    (1u)
#define Button_RBLEED3                    (2u)

/* Flag of complex scan slot */
#define Button_COMPLEX_SS_FLAG            (0x80u)
#define Button_CHANNEL1_FLAG              (0x80u)


#define Button_ANALOG_SWITCHES_SRC_PRESCALER (0x01u)
#define Button_ANALOG_SWITCHES_SRC_PRS       (0x02u)


/***************************************
*             Registers
***************************************/

/* Control REG */
#define Button_CONTROL_REG    (*(reg8 *) \
                                            Button_ClockGen_SyncCtrl_CtrlReg__CONTROL_REG )
#define Button_CONTROL_PTR    ( (reg8 *) \
                                            Button_ClockGen_SyncCtrl_CtrlReg__CONTROL_REG )

/* Clock Gen - ScanSpeed REGs definitions */
#define Button_SCANSPEED_AUX_CONTROL_REG (*(reg8 *) Button_ClockGen_ScanSpeed__CONTROL_AUX_CTL_REG )
#define Button_SCANSPEED_AUX_CONTROL_PTR ( (reg8 *) Button_ClockGen_ScanSpeed__CONTROL_AUX_CTL_REG )

#define Button_SCANSPEED_PERIOD_REG      (*(reg8 *) Button_ClockGen_ScanSpeed__PERIOD_REG )
#define Button_SCANSPEED_PERIOD_PTR      ( (reg8 *) Button_ClockGen_ScanSpeed__PERIOD_REG )

#define Button_SCANSPEED_COUNTER_REG     (*(reg8 *) Button_ClockGen_ScanSpeed__COUNT_REG )
#define Button_SCANSPEED_COUNTER_PTR     ( (reg8 *) Button_ClockGen_ScanSpeed__COUNT_REG )


/* Clock Gen - Prescaler REGs definitions */
#if (Button_PRESCALER_OPTIONS == Button_PRESCALER_UDB)
    #define Button_PRESCALER_PERIOD_REG       (*(reg8 *) Button_ClockGen_UDB_PrescalerDp_u0__D0_REG)
    #define Button_PRESCALER_PERIOD_PTR       ( (reg8 *) Button_ClockGen_UDB_PrescalerDp_u0__D0_REG)
    
    #define Button_PRESCALER_COMPARE_REG      (*(reg8 *) Button_ClockGen_UDB_PrescalerDp_u0__D1_REG)
    #define Button_PRESCALER_COMPARE_PTR      ( (reg8 *) Button_ClockGen_UDB_PrescalerDp_u0__D1_REG)
    
#elif (Button_PRESCALER_OPTIONS == Button_PRESCALER_FF)
    #define Button_PRESCALER_PERIOD_PTR       ( (reg16 *) Button_ClockGen_FF_Prescaler__PER0 )
    #define Button_PRESCALER_COMPARE_PTR      ( (reg16 *) Button_ClockGen_FF_Prescaler__CNT_CMP0 )
    
    #define Button_PRESCALER_CONTROL_REG      (*(reg8 *) Button_ClockGen_FF_Prescaler__CFG0 )
    #define Button_PRESCALER_CONTROL_PTR      ( (reg8 *) Button_ClockGen_FF_Prescaler__CFG0 )
    
    #if (CY_PSOC5A)
        #define Button_PRESCALER_CONTROL2_REG     (*(reg8 *) Button_ClockGen_FF_Prescaler__CFG1 )
        #define Button_PRESCALER_CONTROL2_PTR     ( (reg8 *) Button_ClockGen_FF_Prescaler__CFG1 )
    #else
        #define Button_PRESCALER_CONTROL2_REG     (*(reg8 *) Button_ClockGen_FF_Prescaler__CFG2 )
        #define Button_PRESCALER_CONTROL2_PTR     ( (reg8 *) Button_ClockGen_FF_Prescaler__CFG2 )
    #endif  /* (CY_PSOC5A) */
    
    #define Button_PRESCALER_ACT_PWRMGR_REG   (*(reg8 *) Button_ClockGen_FF_Prescaler__PM_ACT_CFG )
    #define Button_PRESCALER_ACT_PWRMGR_PTR   ( (reg8 *) Button_ClockGen_FF_Prescaler__PM_ACT_CFG )
    #define Button_PRESCALER_ACT_PWR_EN                 (Button_ClockGen_FF_Prescaler__PM_ACT_MSK )
    
    #define Button_PRESCALER_STBY_PWRMGR_REG  (*(reg8 *) Button_ClockGen_FF_Prescaler__PM_STBY_CFG )
    #define Button_PRESCALER_STBY_PWRMGR_PTR  ( (reg8 *) Button_ClockGen_FF_Prescaler__PM_STBY_CFG )
    #define Button_PRESCALER_STBY_PWR_EN                (Button_ClockGen_FF_Prescaler__PM_STBY_MSK )

#else
    /* No prescaler */
#endif  /* (Button_PRESCALER_OPTIONS == Button_PRESCALER_UDB) */

/* PRS */
#if (Button_PRS_OPTIONS == Button_PRS_8BITS)  
    /* Polynomial */
    #define Button_POLYNOM_REG        (*(reg8 *) Button_ClockGen_sC8_PRSdp_u0__D0_REG )
    #define Button_POLYNOM_PTR        ( (reg8 *) Button_ClockGen_sC8_PRSdp_u0__D0_REG )
    /* Seed */
    #define Button_SEED_REG           (*(reg8 *) Button_ClockGen_sC8_PRSdp_u0__A0_REG )
    #define Button_SEED_PTR           ( (reg8 *) Button_ClockGen_sC8_PRSdp_u0__A0_REG )
    /* Seed COPY */
    #define Button_SEED_COPY_REG      (*(reg8 *) Button_ClockGen_sC8_PRSdp_u0__F0_REG )
    #define Button_SEED_COPY_PTR      ( (reg8 *) Button_ClockGen_sC8_PRSdp_u0__F0_REG )
    /* Aux control */
    #define Button_AUX_CONTROL_A_REG  (*(reg8 *) Button_ClockGen_sC8_PRSdp_u0__DP_AUX_CTL_REG )
    #define Button_AUX_CONTROL_A_PTR  ( (reg8 *) Button_ClockGen_sC8_PRSdp_u0__DP_AUX_CTL_REG )
        
#elif (Button_PRS_OPTIONS == Button_PRS_16BITS)
    /* Polynomial */
    #define Button_POLYNOM_REG        (*(reg16 *) Button_ClockGen_sC16_PRSdp_u0__D0_REG )
    #define Button_POLYNOM_PTR        ( (reg16 *) Button_ClockGen_sC16_PRSdp_u0__D0_REG )
    /* Seed */
    #define Button_SEED_REG           (*(reg16 *) Button_ClockGen_sC16_PRSdp_u0__A0_REG )
    #define Button_SEED_PTR           ( (reg16 *) Button_ClockGen_sC16_PRSdp_u0__A0_REG )
    /* Seed COPY */
    #define Button_SEED_COPY_REG      (*(reg16 *) Button_ClockGen_sC16_PRSdp_u0__F0_REG )
    #define Button_SEED_COPY_PTR      ( (reg16 *) Button_ClockGen_sC16_PRSdp_u0__F0_REG )
    /* Aux control */
    #define Button_AUX_CONTROL_A_REG  (*(reg8 *) Button_ClockGen_sC16_PRSdp_u0__DP_AUX_CTL_REG )
    #define Button_AUX_CONTROL_A_PTR  ( (reg8 *) Button_ClockGen_sC16_PRSdp_u0__DP_AUX_CTL_REG )
    
    #define Button_AUX_CONTROL_B_REG  (*(reg8 *) Button_ClockGen_sC16_PRSdp_u1__DP_AUX_CTL_REG )
    #define Button_AUX_CONTROL_B_PTR  ( (reg8 *) Button_ClockGen_sC16_PRSdp_u1__DP_AUX_CTL_REG )
    
#elif (Button_PRS_OPTIONS == Button_PRS_16BITS_4X)
    /* Polynomial */   
    #define Button_POLYNOM_A__D1_REG      (*(reg8 *) Button_ClockGen_b0_PRSdp_a__D1_REG )
    #define Button_POLYNOM_A__D1_PTR      ( (reg8 *) Button_ClockGen_b0_PRSdp_a__D1_REG )
    
    #define Button_POLYNOM_A__D0_REG      (*(reg8 *) Button_ClockGen_b0_PRSdp_a__D0_REG )
    #define Button_POLYNOM_A__D0_PTR      ( (reg8 *) Button_ClockGen_b0_PRSdp_a__D0_REG )
    /* Seed */
    #define Button_SEED_A__A1_REG         (*(reg8 *) Button_ClockGen_b0_PRSdp_a__A1_REG )
    #define Button_SEED_A__A1_PTR         ( (reg8 *) Button_ClockGen_b0_PRSdp_a__A1_REG )
    
    #define Button_SEED_A__A0_REG         (*(reg8 *) Button_ClockGen_b0_PRSdp_a__A0_REG )
    #define Button_SEED_A__A0_PTR         ( (reg8 *) Button_ClockGen_b0_PRSdp_a__A0_REG )
    /* Seed COPY */
    #define Button_SEED_COPY_A__F1_REG    (*(reg8 *) Button_ClockGen_b0_PRSdp_a__F1_REG )
    #define Button_SEED_COPY_A__F1_PTR    ( (reg8 *) Button_ClockGen_b0_PRSdp_a__F1_REG )
    
    #define Button_SEED_COPY_A__F0_REG    (*(reg8 *) Button_ClockGen_b0_PRSdp_a__F0_REG )
    #define Button_SEED_COPY_A__F0_PTR    ( (reg8 *) Button_ClockGen_b0_PRSdp_a__F0_REG )
    /* Aux control */
    #define Button_AUX_CONTROL_A_REG  (*(reg8 *) Button_ClockGen_b0_PRSdp_a__DP_AUX_CTL_REG )
    #define Button_AUX_CONTROL_A_PTR  ( (reg8 *) Button_ClockGen_b0_PRSdp_a__DP_AUX_CTL_REG )
    
#else
    /* No PRS */
#endif  /* (Button_PRS_OPTIONS == Button_PRS_8BITS)  */

/* Measure REGs  definitions */
#if (Button_IMPLEMENTATION_CH0 == Button_MEASURE_IMPLEMENTATION_FF)
    /* Window PWM */
    #define Button_PWM_CH0_PERIOD_PTR         ( (reg16 *) Button_MeasureCH0_FF_Window__PER0 )
    #define Button_PWM_CH0_COUNTER_PTR        ( (reg16 *) Button_MeasureCH0_FF_Window__CNT_CMP0 )
    
    #define Button_PWM_CH0_CONTROL_REG        (*(reg8 *) Button_MeasureCH0_FF_Window__CFG0 )
    #define Button_PWM_CH0_CONTROL_PTR        ( (reg8 *) Button_MeasureCH0_FF_Window__CFG0 )
    
    #define Button_PWM_CH0_CONTROL2_REG       (*(reg8 *) Button_MeasureCH0_FF_Window__CFG2 )
    #define Button_PWM_CH0_CONTROL2_PTR       ( (reg8 *) Button_MeasureCH0_FF_Window__CFG2 )
	
    #define Button_PWM_CH0_ACT_PWRMGR_REG     (*(reg8 *) Button_MeasureCH0_FF_Window__PM_ACT_CFG )
    #define Button_PWM_CH0_ACT_PWRMGR_PTR     ( (reg8 *) Button_MeasureCH0_FF_Window__PM_ACT_CFG )
    #define Button_PWM_CH0_ACT_PWR_EN                   (Button_MeasureCH0_FF_Window__PM_ACT_MSK )
    
    #define Button_PWM_CH0_STBY_PWRMGR_REG    (*(reg8 *) Button_MeasureCH0_FF_Window__PM_STBY_CFG )
    #define Button_PWM_CH0_STBY_PWRMGR_PTR    ( (reg8 *) Button_MeasureCH0_FF_Window__PM_STBY_CFG )
    #define Button_PWM_CH0_STBY_PWR_EN                  (Button_MeasureCH0_FF_Window__PM_STBY_MSK )
    
    /* Raw Counter */
    #define Button_RAW_CH0_PERIOD_PTR         ( (reg16 *) Button_MeasureCH0_FF_Counter__PER0 )
    #define Button_RAW_CH0_COUNTER_PTR        ( (reg16 *) Button_MeasureCH0_FF_Counter__CNT_CMP0 )
    
    #define Button_RAW_CH0_CONTROL_REG        (*(reg8 *) Button_MeasureCH0_FF_Counter__CFG0 )
    #define Button_RAW_CH0_CONTROL_PTR        ( (reg8 *) Button_MeasureCH0_FF_Counter__CFG0 )
    

    #define Button_RAW_CH0_CONTROL2_REG   (*(reg8 *) Button_MeasureCH0_FF_Counter__CFG2 )
    #define Button_RAW_CH0_CONTROL2_PTR   ( (reg8 *) Button_MeasureCH0_FF_Counter__CFG2 )

    
    #define Button_RAW_CH0_ACT_PWRMGR_REG     (*(reg8 *) Button_MeasureCH0_FF_Counter__PM_ACT_CFG )
    #define Button_RAW_CH0_ACT_PWRMGR_PTR     ( (reg8 *) Button_MeasureCH0_FF_Counter__PM_ACT_CFG )
    #define Button_RAW_CH0_ACT_PWR_EN                   (Button_MeasureCH0_FF_Counter__PM_ACT_MSK )
    
    #define Button_RAW_CH0_STBY_PWRMGR_REG    (*(reg8 *) Button_MeasureCH0_FF_Counter__PM_STBY_CFG )
    #define Button_RAW_CH0_STBY_PWRMGR_PTR    ( (reg8 *) Button_MeasureCH0_FF_Counter__PM_STBY_CFG )
    #define Button_RAW_CH0_STBY_PWR_EN                  (Button_MeasureCH0_FF_Counter__PM_STBY_MSK )
#else
     /* Window PWM */
    #define Button_PWM_CH0_COUNTER_LO_REG     (*(reg8 *) Button_MeasureCH0_UDB_Window_u0__A0_REG )
    #define Button_PWM_CH0_COUNTER_LO_PTR     ( (reg8 *) Button_MeasureCH0_UDB_Window_u0__A0_REG )
    
    #define Button_PWM_CH0_COUNTER_HI_REG     (*(reg8 *) Button_MeasureCH0_UDB_Window_u0__A1_REG )
    #define Button_PWM_CH0_COUNTER_HI_PTR     ( (reg8 *) Button_MeasureCH0_UDB_Window_u0__A1_REG )
    
    #define Button_PWM_CH0_PERIOD_LO_REG      (*(reg8 *) Button_MeasureCH0_UDB_Window_u0__F0_REG )
    #define Button_PWM_CH0_PERIOD_LO_PTR      ( (reg8 *) Button_MeasureCH0_UDB_Window_u0__F0_REG )
    
    #define Button_PWM_CH0_PERIOD_HI_REG      (*(reg8 *) Button_MeasureCH0_UDB_Window_u0__F1_REG )
    #define Button_PWM_CH0_PERIOD_HI_PTR      ( (reg8 *) Button_MeasureCH0_UDB_Window_u0__F1_REG )
    
    #define Button_PWM_CH0_ADD_VALUE_REG      (*(reg8 *) Button_MeasureCH0_UDB_Window_u0__D0_REG )
    #define Button_PWM_CH0_ADD_VALUE_PTR      ( (reg8 *) Button_MeasureCH0_UDB_Window_u0__D0_REG )
    
    #define Button_PWM_CH0_AUX_CONTROL_REG    (*(reg8 *) \
                                                            Button_MeasureCH0_UDB_Window_u0__DP_AUX_CTL_REG )
    #define Button_PWM_CH0_AUX_CONTROL_PTR    ( (reg8 *) \
                                                            Button_MeasureCH0_UDB_Window_u0__DP_AUX_CTL_REG )
    
    /* Raw Counter */
    #define Button_RAW_CH0_COUNTER_LO_REG      (*(reg8 *) Button_MeasureCH0_UDB_Counter_u0__A0_REG )
    #define Button_RAW_CH0_COUNTER_LO_PTR      ( (reg8 *) Button_MeasureCH0_UDB_Counter_u0__A0_REG )
    
    #define Button_RAW_CH0_COUNTER_HI_REG      (*(reg8 *) Button_MeasureCH0_UDB_Counter_u0__A1_REG )
    #define Button_RAW_CH0_COUNTER_HI_PTR      ( (reg8 *) Button_MeasureCH0_UDB_Counter_u0__A1_REG )
    
    #define Button_RAW_CH0_PERIOD_LO_REG       (*(reg8 *) Button_MeasureCH0_UDB_Counter_u0__F0_REG )
    #define Button_RAW_CH0_PERIOD_LO_PTR       ( (reg8 *) Button_MeasureCH0_UDB_Counter_u0__F0_REG )
    
    #define Button_RAW_CH0_PERIOD_HI_REG       (*(reg8 *) Button_MeasureCH0_UDB_Counter_u0__F1_REG )
    #define Button_RAW_CH0_PERIOD_HI_PTR       ( (reg8 *) Button_MeasureCH0_UDB_Counter_u0__F1_REG )
    
    #define Button_RAW_CH0_ADD_VALUE_REG       (*(reg8 *) Button_MeasureCH0_UDB_Counter_u0__D0_REG )
    #define Button_RAW_CH0_ADD_VALUE_PTR       ( (reg8 *) Button_MeasureCH0_UDB_Counter_u0__D0_REG )
    
    #define Button_RAW_CH0_AUX_CONTROL_REG     (*(reg8 *) \
                                                            Button_MeasureCH0_UDB_Counter_u0__DP_AUX_CTL_REG )
    #define Button_RAW_CH0_AUX_CONTROL_PTR     ( (reg8 *) \
                                                            Button_MeasureCH0_UDB_Counter_u0__DP_AUX_CTL_REG )

#endif  /* (Button_IMPLEMENTATION_CH0 == Button_MEASURE_IMPLEMENTATION_FF) */
    
#if (Button_DESIGN_TYPE == Button_TWO_CHANNELS_DESIGN)
    #if (Button_IMPLEMENTATION_CH1 == Button_MEASURE_IMPLEMENTATION_FF)
        /* Window PWM */
        #define Button_PWM_CH1_PERIOD_PTR        ( (reg16 *) Button_MeasureCH1_FF_Window__PER0 )
        #define Button_PWM_CH1_COUNTER_PTR       ( (reg16 *) Button_MeasureCH1_FF_Window__CNT_CMP0 )
        
        #define Button_PWM_CH1_CONTROL_REG        (*(reg8 *) Button_MeasureCH1_FF_Window__CFG0 )
        #define Button_PWM_CH1_CONTROL_PTR        ( (reg8 *) Button_MeasureCH1_FF_Window__CFG0 )
        
        #define Button_PWM_CH1_CONTROL2_REG   (*(reg8 *) Button_MeasureCH1_FF_Window__CFG2 )
        #define Button_PWM_CH1_CONTROL2_PTR   ( (reg8 *) Button_MeasureCH1_FF_Window__CFG2 )
        
        #define Button_PWM_CH1_ACT_PWRMGR_REG   (*(reg8 *) Button_MeasureCH1_FF_Window__PM_ACT_CFG )
        #define Button_PWM_CH1_ACT_PWRMGR_PTR   ( (reg8 *) Button_MeasureCH1_FF_Window__PM_ACT_CFG )
        #define Button_PWM_CH1_ACT_PWR_EN                 (Button_MeasureCH1_FF_Window__PM_ACT_MSK )
        
        #define Button_PWM_CH1_STBY_PWRMGR_REG (*(reg8 *) Button_MeasureCH1_FF_Window__PM_STBY_CFG )
        #define Button_PWM_CH1_STBY_PWRMGR_PTR ( (reg8 *) Button_MeasureCH1_FF_Window__PM_STBY_CFG )
        #define Button_PWM_CH1_STBY_PWR_EN               (Button_MeasureCH1_FF_Window__PM_STBY_MSK )
            
        /* Raw Counter */
        #define Button_RAW_CH1_PERIOD_PTR       ( (reg16 *) Button_MeasureCH1_FF_Counter__PER0 )
        #define Button_RAW_CH1_COUNTER_PTR      ( (reg16 *) Button_MeasureCH1_FF_Counter__CNT_CMP0 )
        
        #define Button_RAW_CH1_CONTROL_REG      (*(reg8 *) Button_MeasureCH1_FF_Counter__CFG0 )
        #define Button_RAW_CH1_CONTROL_PTR      ( (reg8 *) Button_MeasureCH1_FF_Counter__CFG0 )
        
        #define Button_RAW_CH1_CONTROL2_REG   (*(reg8 *) Button_MeasureCH1_FF_Counter__CFG2 )
        #define Button_RAW_CH1_CONTROL2_PTR   ( (reg8 *) Button_MeasureCH1_FF_Counter__CFG2 )
        
        #define Button_RAW_CH1_ACT_PWRMGR_REG  (*(reg8 *) Button_MeasureCH1_FF_Counter__PM_ACT_CFG )
        #define Button_RAW_CH1_ACT_PWRMGR_PTR  ( (reg8 *) Button_MeasureCH1_FF_Counter__PM_ACT_CFG )
        #define Button_RAW_CH1_ACT_PWR_EN                (Button_MeasureCH1_FF_Counter__PM_ACT_MSK )
        
        #define Button_RAW_CH1_STBY_PWRMGR_REG (*(reg8 *) Button_MeasureCH1_FF_Counter__PM_STBY_CFG)
        #define Button_RAW_CH1_STBY_PWRMGR_PTR ( (reg8 *) Button_MeasureCH1_FF_Counter__PM_STBY_CFG)
        #define Button_RAW_CH1_STBY_PWR_EN               (Button_MeasureCH1_FF_Counter__PM_STBY_MSK)
    
    #else
        /* Window PWM */
        #define Button_PWM_CH1_COUNTER_LO_REG   (*(reg8 *) Button_MeasureCH1_UDB_Window_u0__A0_REG )
        #define Button_PWM_CH1_COUNTER_LO_PTR   ( (reg8 *) Button_MeasureCH1_UDB_Window_u0__A0_REG )
        
        #define Button_PWM_CH1_COUNTER_HI_REG   (*(reg8 *) Button_MeasureCH1_UDB_Window_u0__A1_REG )
        #define Button_PWM_CH1_COUNTER_HI_PTR   ( (reg8 *) Button_MeasureCH1_UDB_Window_u0__A1_REG )
        
        #define Button_PWM_CH1_PERIOD_LO_REG    (*(reg8 *) Button_MeasureCH1_UDB_Window_u0__F0_REG )
        #define Button_PWM_CH1_PERIOD_LO_PTR    ( (reg8 *) Button_MeasureCH1_UDB_Window_u0__F0_REG )
        
        #define Button_PWM_CH1_PERIOD_HI_REG    (*(reg8 *) Button_MeasureCH1_UDB_Window_u0__F1_REG )
        #define Button_PWM_CH1_PERIOD_HI_PTR    ( (reg8 *) Button_MeasureCH1_UDB_Window_u0__F1_REG )
        
        #define Button_PWM_CH1_ADD_VALUE_REG    (*(reg8 *) Button_MeasureCH1_UDB_Window_u0__D0_REG )
        #define Button_PWM_CH1_ADD_VALUE_PTR    ( (reg8 *) Button_MeasureCH1_UDB_Window_u0__D0_REG )
        
        #define Button_PWM_CH1_AUX_CONTROL_REG  (*(reg8 *) \
                                                            Button_MeasureCH1_UDB_Window_u0__DP_AUX_CTL_REG )
        #define Button_PWM_CH1_AUX_CONTROL_PTR  ( (reg8 *) \
                                                            Button_MeasureCH1_UDB_Window_u0__DP_AUX_CTL_REG )
        
        /* Raw Counter */
        #define Button_RAW_CH1_COUNTER_LO_REG  (*(reg8 *) Button_MeasureCH1_UDB_Counter_u0__A0_REG )
        #define Button_RAW_CH1_COUNTER_LO_PTR  ( (reg8 *) Button_MeasureCH1_UDB_Counter_u0__A0_REG )
        
        #define Button_RAW_CH1_COUNTER_HI_REG  (*(reg8 *) Button_MeasureCH1_UDB_Counter_u0__A1_REG )
        #define Button_RAW_CH1_COUNTER_HI_PTR  ( (reg8 *) Button_MeasureCH1_UDB_Counter_u0__A1_REG )
        
        #define Button_RAW_CH1_PERIOD_LO_REG   (*(reg8 *) Button_MeasureCH1_UDB_Counter_u0__F0_REG )
        #define Button_RAW_CH1_PERIOD_LO_PTR   ( (reg8 *) Button_MeasureCH1_UDB_Counter_u0__F0_REG )
        
        #define Button_RAW_CH1_PERIOD_HI_REG   (*(reg8 *) Button_MeasureCH1_UDB_Counter_u0__F1_REG )
        #define Button_RAW_CH1_PERIOD_HI_PTR   ( (reg8 *) Button_MeasureCH1_UDB_Counter_u0__F1_REG )
        
        #define Button_RAW_CH1_ADD_VALUE_REG   (*(reg8 *) Button_MeasureCH1_UDB_Counter_u0__D0_REG )
        #define Button_RAW_CH1_ADD_VALUE_PTR   ( (reg8 *) Button_MeasureCH1_UDB_Counter_u0__D0_REG )
        
        #define Button_RAW_CH1_AUX_CONTROL_REG  (*(reg8 *) \
                                                            Button_MeasureCH1_UDB_Counter_u0__DP_AUX_CTL_REG )
        #define Button_RAW_CH1_AUX_CONTROL_PTR  ( (reg8 *) \
                                                            Button_MeasureCH1_UDB_Counter_u0__DP_AUX_CTL_REG )
        
    #endif  /* Button_DESIGN_TYPE */
    
#endif  /* Button_DESIGN_TYPE */


/* CapSense Buffer REGs definitions */
#define Button_BufCH0_CAPS_CFG0_REG           (*(reg8 *) Button_BufCH0__CFG0 )
#define Button_BufCH0_CAPS_CFG0_PTR           ( (reg8 *) Button_BufCH0__CFG0 )

#define Button_BufCH0_CAPS_CFG1_REG           (*(reg8 *) Button_BufCH0__CFG1 )
#define Button_BufCH0_CAPS_CFG1_PTR           ( (reg8 *) Button_BufCH0__CFG1 )

#define Button_BufCH0_ACT_PWRMGR_REG          (*(reg8 *) Button_BufCH0__PM_ACT_CFG )
#define Button_BufCH0_ACT_PWRMGR_PTR          ( (reg8 *) Button_BufCH0__PM_ACT_CFG )
#define Button_BufCH0_ACT_PWR_EN                        (Button_BufCH0__PM_ACT_MSK )

#define Button_BufCH0_STBY_PWRMGR_REG         (*(reg8 *) Button_BufCH0__PM_STBY_CFG )
#define Button_BufCH0_STBY_PWRMGR_PTR         ( (reg8 *) Button_BufCH0__PM_STBY_CFG )
#define Button_BufCH0_STBY_PWR_EN                       (Button_BufCH0__PM_STBY_MSK )

#if (Button_DESIGN_TYPE == Button_TWO_CHANNELS_DESIGN)
    #define Button_BufCH1_CAPS_CFG0_REG       (*(reg8 *) Button_BufCH1__CFG0 )
    #define Button_BufCH1_CAPS_CFG0_PTR       ( (reg8 *) Button_BufCH1__CFG0 )
    
    #define Button_BufCH1_CAPS_CFG1_REG       (*(reg8 *) Button_BufCH1__CFG1 )
    #define Button_BufCH1_CAPS_CFG1_PTR       ( (reg8 *) Button_BufCH1__CFG1 )
    
    #define Button_BufCH1_ACT_PWRMGR_REG      (*(reg8 *) Button_BufCH1__PM_ACT_CFG )
    #define Button_BufCH1_ACT_PWRMGR_PTR      ( (reg8 *) Button_BufCH1__PM_ACT_CFG )
    #define Button_BufCH1_ACT_PWR_EN                    (Button_BufCH1__PM_ACT_MSK )
    
    #define Button_BufCH1_STBY_PWRMGR_REG     (*(reg8 *) Button_BufCH1__PM_STBY_CFG )
    #define Button_BufCH1_STBY_PWRMGR_PTR     ( (reg8 *) Button_BufCH1__PM_STBY_CFG )
    #define Button_BufCH1_STBY_PWR_EN                   (Button_BufCH1__PM_STBY_MSK )
#endif  /* Button_DESIGN_TYPE == Button_TWO_CHANNELS_DESIGN */

/* ISR Number and Priority to work with CyLib functions */
#define Button_IsrCH0_ISR_NUMBER        (Button_IsrCH0__INTC_NUMBER)
#define Button_IsrCH0_ISR_PRIORITY      (Button_IsrCH0__INTC_PRIOR_NUM)

#if (Button_DESIGN_TYPE == Button_TWO_CHANNELS_DESIGN)
    #define Button_IsrCH1_ISR_NUMBER        (Button_IsrCH1__INTC_NUMBER)
    #define Button_IsrCH1_ISR_PRIORITY      (Button_IsrCH1__INTC_PRIOR_NUM)
#endif /* Button_DESIGN_TYPE == Button_TWO_CHANNELS_DESIGN */


/***************************************
*       Register Constants
***************************************/

/* Control Register definitions */
#define Button_CTRL_SYNC_EN                       (0x01u)
#define Button_CTRL_START                         (0x02u)
#define Button_CTRL_WINDOW_EN__CH0                (0x04u)
#define Button_CTRL_WINDOW_EN__CH1                (0x08u)
/* Addtional bit to verify if component is enalbed */
#define Button_CTRL_CAPSENSE_EN                   (0x80u)

#define Button_IS_CAPSENSE_ENABLE(reg)            ( ((reg) & Button_CTRL_CAPSENSE_EN) != 0u )

/* ClockGen defines */
/* Prescaler */
#define Button_PRESCALER_CTRL_ENABLE          (0x01u)
#define Button_PRESCALER_CTRL_MODE_CMP        (0x02u) 

#define Button_PRESCALER_CTRL_CMP_MODE_SHIFT  (0x04u)

#define Button_PRESCALER_CTRL_CMP_LESS_EQ         (0x02u << Button_PRESCALER_CTRL_CMP_MODE_SHIFT)

/* Set PRS polynomial */
#define Button_PRS8_DEFAULT_POLYNOM           (0xB8u)
#define Button_PRS16_DEFAULT_POLYNOM          (0xB400u)

/* Scan Speed */
#define Button_SCANSPEED_CTRL_ENABLE          (0x20u)

/* Measure defines */
/* FF Timers */
#define Button_MEASURE_FULL_RANGE             (0xFFFFu)
#define Button_MEASURE_FULL_RANGE_LOW         (0xFFu)
#define Button_MEASURE_CTRL_ENABLE            (0x01u)

#define Button_MEASURE_CTRL_MODE_SHIFT        (0x00u)


#define Button_MEASURE_CTRL_PULSEWIDTH        (0x01u << Button_MEASURE_CTRL_MODE_SHIFT)

/* UDB timers */
#define Button_AUXCTRL_FIFO_SINGLE_REG        (0x03u)
 
/* Masks of PTR PC Register */
#define Button_DR_MASK            (0x01u)
#define Button_DM0_MASK           (0x02u)
#define Button_DM1_MASK           (0x04u)
#define Button_DM2_MASK           (0x08u)
#define Button_BYP_MASK           (0x80u)

#define Button_PRT_PC_GND         (Button_DM2_MASK)
#define Button_PRT_PC_HIGHZ       (Button_DM2_MASK |Button_DR_MASK)
#define Button_PRT_PC_SHIELD      (Button_DM2_MASK | Button_DM1_MASK | \
                                             Button_BYP_MASK)

/* CapSense Buffer definitions */
#define Button_CSBUF_BOOST_ENABLE         (0x02u)
#define Button_CSBUF_ENABLE               (0x01u)

/* Define direction of Current - Souce as Sink */
#if (Button_CURRENT_SOURCE == Button_IDAC_SOURCE)
    #define Button_IdacCH0_IDIR      (Button_IdacCH0_SOURCE)
    #define Button_IdacCH1_IDIR      (Button_IdacCH1_SOURCE)
#elif (Button_CURRENT_SOURCE == Button_IDAC_SINK)
    #define Button_IdacCH0_IDIR      (Button_IdacCH0_SINK)
    #define Button_IdacCH1_IDIR      (Button_IdacCH1_SINK)
#else
    /* No Idac - Rb selected */
#endif  /* (Button_CURRENT_SOURCE == Button_IDAC_SOURCE) */


/* Rb init function */
#if (Button_CURRENT_SOURCE == Button_EXTERNAL_RB)
    void Button_InitRb(void);
#endif /* End Button_CURRENT_SOURCE */ 

#if (Button_IS_COMPLEX_SCANSLOTS)
    void Button_EnableScanSlot(uint8 slot) CYREENTRANT;
    void Button_DisableScanSlot(uint8 slot) CYREENTRANT;
    
#else
    #define Button_EnableScanSlot(slot)   Button_EnableSensor(slot)
    #define Button_DisableScanSlot(slot)  Button_DisableSensor(slot)

#endif  /* End Button_IS_COMPLEX_SCANSLOTS */

/* Helper functions - do nto part of public interface*/

/* Find next sensor for One Channel design */
#if (Button_DESIGN_TYPE == Button_ONE_CHANNEL_DESIGN)
    uint8 Button_FindNextSensor(uint8 snsIndex) CYREENTRANT;
#endif  /* End Button_DESIGN_TYPE */

/* Find next pair for Two Channels design */
 #if (Button_DESIGN_TYPE == Button_TWO_CHANNELS_DESIGN)
    uint8 Button_FindNextPair(uint8 snsIndex) CYREENTRANT;
#endif  /* End Button_DESIGN_TYPE */

/* Start and Compete the scan */
void Button_PreScan(uint8 sensor) CYREENTRANT;
#if (Button_DESIGN_TYPE == Button_ONE_CHANNEL_DESIGN)
    void Button_PostScan(uint8 sensor) CYREENTRANT;
#else
    void Button_PostScanCh0(uint8 sensor) CYREENTRANT;
    void Button_PostScanCh1(uint8 sensor) CYREENTRANT;
#endif  /* End Button_DESIGN_TYPE */

#if (Button_PRESCALER_OPTIONS)
    void Button_SetPrescaler(uint8 prescaler) CYREENTRANT;
#endif  /* End Button_PRESCALER_OPTIONS */

void Button_SetScanSpeed(uint8 scanSpeed) ;

/* SmartSense functions */
#if (Button_TUNING_METHOD == Button_AUTO_TUNING)
    extern uint8 Button_lowLevelTuningDone;
    extern void Button_AutoTune(void) ;
#endif /* End (Button_TUNING_METHOD == Button_AUTO_TUNING) */

/* Global software variables */
extern volatile uint8 Button_csv;            /* CapSense CSD status, control variable */
extern volatile uint8 Button_sensorIndex;    /* Index of scannig sensor */

#if (Button_CURRENT_SOURCE == Button_EXTERNAL_RB)
    extern uint8  Button_extRbCh0Cur;
    #if (Button_DESIGN_TYPE == Button_TWO_CHANNELS_DESIGN)
        extern uint8  Button_extRbCh1Cur;
    #endif /* (Button_DESIGN_TYPE == Button_TWO_CHANNELS_DESIGN)*/ 
#endif /* (Button_CURRENT_SOURCE == Button_EXTERNAL_RB) */ 
        
/* Global array of Raw Counts */
extern uint16 Button_sensorRaw[Button_TOTAL_SENSOR_COUNT];


extern Button_BACKUP_STRUCT Button_backup;

/****************************************************************************************
*       Obsolete definitions. Not recommended to use. Will be removed in future releases.
*****************************************************************************************/

/* Auto Tuning defualt raw counts value */
#define Button_DEFUALT_RAW_COUNTS_VALUE    (384u)

/* IDAC_CR0 registers save/restore flags */
#define Button_IdacCH0_RESTORE_CFG (1u)
#define Button_IdacCH1_RESTORE_CFG (2u)
/* DAC_CR0 register value when IDAC is stopped*/
#define Button_IDAC_STOP_CR0_VAL   (0x1Eu)

/* Idac SetRange */
#define Button_IDAC_RANGE_MASK            (0x0Cu)
#define Button_IDAC_RANGE_32uA            (0x00u)
#define Button_IDAC_RANGE_255uA           (0x04u)
#define Button_IDAC_RANGE_2mA             (0x08u)

/* CR0 IDAC Control Register 0 definitions */
/* Bit Field DAC_MODE */
#define Button_IDAC_MODE_MASK         (0x10u)
#define Button_IDAC_MODE_V            (0x00u)
#define Button_IDAC_MODE_I            (0x10u)

/* CR1 Idac Control Register 1 definitions */
/* Bit Field  DAC_I_DIR */
#define Button_IDAC_IDIR_MASK         (0x04u)
#define Button_IDAC_IDIR_SINK         (0x04u)
#define Button_IDAC_IDIR_SRC          (0x00u)

/* Bit Field  DAC_MX_IOFF_SRC */
#define Button_IDAC_IDIR_CTL_MASK     (0x02u)
#define Button_IDAC_IDIR_CTL_REG      (0x00u)
#define Button_IDAC_IDIR_CTL_UDB      (0x02u)

/* Obsolete names of variables */
//#define Button_SensorRaw              Button_sensorRaw
//#define Button_SensorEnableMask       Button_sensorEnableMask
//#define Button_AnalogSwitchDivider    Button_analogSwitchDivider


#endif /* CY_CAPSENSE_CSD_Button_H */

 /* [] END OF FILE */
