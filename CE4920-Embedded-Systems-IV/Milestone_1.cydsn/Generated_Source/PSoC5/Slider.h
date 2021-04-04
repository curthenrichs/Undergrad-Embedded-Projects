/*******************************************************************************
* File Name: Slider.h
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

#if !defined(CY_CAPSENSE_CSD_Slider_H)
#define CY_CAPSENSE_CSD_Slider_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cydevice_trm.h"
#include "CyLib.h"


#include "Slider_IntClock.h"

#include "Slider_AMuxCH0.h"
#include "Slider_CompCH0.h"
#include "Slider_IdacCH0.h"




/***************************************
*   Condition compilation parameters
***************************************/

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component CapSense_CSD_v3_50 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */


#define Slider_DESIGN_TYPE                (1u)

#define Slider_CONNECT_INACTIVE_SNS       (0u)
#define Slider_IS_COMPLEX_SCANSLOTS       (0u)

#define Slider_CLOCK_SOURCE               (0u)

#define Slider_CURRENT_SOURCE             (1u)
#define Slider_IDAC_RANGE_VALUE           (4u)

#define Slider_PRESCALER_OPTIONS          (1u)
#define Slider_MULTIPLE_PRESCALER_ENABLED (1u)

#define Slider_PRS_OPTIONS                (2u)
#define Slider_SCANSPEED_VALUE            (7u)

#define Slider_VREF_OPTIONS               (0u)

#define Slider_WATER_PROOF                (0u)

#define Slider_TUNING_METHOD              (2u)
#define Slider_TUNER_API_GENERATE         (0u)

#define Slider_IMPLEMENTATION_CH0         (1u)
#define Slider_IMPLEMENTATION_CH1         (1u)

#define Slider_GUARD_SENSOR               (0u)

/* Design types definitions */
#define Slider_ONE_CHANNEL_DESIGN         (1u)
#define Slider_TWO_CHANNELS_DESIGN        (2u)

/* Clock sources definitions */
#define Slider_INTERNAL_CLOCK             (0u)
#define Slider_EXTERNAL_CLOCK             (1u)

/* Current source definitions */
#define Slider_EXTERNAL_RB                (0u)
#define Slider_IDAC_SOURCE                (1u)
#define Slider_IDAC_SINK                  (2u)

/* Prescaler option definitions */
#define Slider_PRESCALER_NONE             (0u)
#define Slider_PRESCALER_UDB              (1u)
#define Slider_PRESCALER_FF               (2u)

/* Prs options definitions */
#define Slider_PRS_NONE                   (0u)
#define Slider_PRS_8BITS                  (1u)
#define Slider_PRS_16BITS                 (2u)
#define Slider_PRS_16BITS_4X              (3u)

/* Seed values */
#define Slider_PRS8_SEED_VALUE            (0xFFu)
#define Slider_PRS16_SEED_VALUE           (0xFFFFu)

/* Reference source types definitions */
#define Slider_VREF_REFERENCE_1_024V      (0u)
#define Slider_VREF_REFERENCE_1_2V        (1u)
#define Slider_VREF_VDAC                  (2u)

/* Connection of inactive sensors definitions */
#define Slider_CIS_GND                    (0u)
#define Slider_CIS_HIGHZ                  (1u)
#define Slider_CIS_SHIELD                 (2u)

/* Method of tunning */
#define Slider_NO_TUNING                  (0u)
#define Slider_MANUAL_TUNING              (1u)
#define Slider_AUTO_TUNING                (2u)

/* Measure Channel implementation */
#define Slider_MEASURE_IMPLEMENTATION_FF  (0u)
#define Slider_MEASURE_IMPLEMENTATION_UDB (1u)

/* Guard sensor definition */
#define Slider_GUARD_SENSOR_DISABLE       (0u)
#define Slider_GUARD_SENSOR_ENABLE        (1u)


/***************************************
*       Type defines
***************************************/

/* Structure to save registers before go to sleep */
typedef struct
{
    uint8 enableState;

    /* Set CONTROL_REG */
    uint8 ctrlReg;
} Slider_BACKUP_STRUCT;


/***************************************
*        Function Prototypes
***************************************/

void Slider_Init(void) ;
void Slider_Enable(void) ;
void Slider_Start(void) ;
void Slider_Stop(void) ;
void Slider_SaveConfig(void) ;
void Slider_Sleep(void) ;
void Slider_RestoreConfig(void) ;
void Slider_Wakeup(void) ;
uint8 Slider_IsBusy(void) ;
void Slider_ScanSensor(uint8 sensor) ;
void Slider_ScanEnabledWidgets(void) ;
void Slider_SetScanSlotSettings(uint8 slot) CYREENTRANT;
uint16 Slider_ReadSensorRaw(uint8 sensor) ;
void Slider_ClearSensors(void) ;
void Slider_EnableSensor(uint8 sensor) CYREENTRANT;
void Slider_DisableSensor(uint8 sensor) CYREENTRANT;

void Slider_SetAnalogSwitchesSource(uint8 src) ;

#if (Slider_CURRENT_SOURCE == Slider_EXTERNAL_RB)
    void Slider_SetRBleed(uint8 rbeed) ;
#endif  /* (Slider_CURRENT_SOURCE == Slider_EXTERNAL_RB) */

/* Interrupt handler */
CY_ISR_PROTO(Slider_IsrCH0_ISR);
#if (Slider_DESIGN_TYPE == Slider_TWO_CHANNELS_DESIGN)
    CY_ISR_PROTO(Slider_IsrCH1_ISR);
#endif  /* (Slider_DESIGN_TYPE == Slider_TWO_CHANNELS_DESIGN) */

#if (Slider_PRESCALER_OPTIONS)
    uint8 Slider_GetPrescaler(void) ;
#endif /* (Slider_PRESCALER_OPTIONS) */

/***************************************
*           API Constants
***************************************/

#define Slider_TOTAL_SENSOR_COUNT            (5u)
#define Slider_TOTAL_SCANSLOT_COUNT            (5u)
#define Slider_TOTAL_RB_NUMBER            (1u)
#define Slider_TOTAL_RB_NUMBER__CH0            (1u)

/* Define Sensors */
#define Slider_SENSOR_LINEARSLIDER_E0__LS    (0u)
#define Slider_SENSOR_LINEARSLIDER_E1__LS    (1u)
#define Slider_SENSOR_LINEARSLIDER_E2__LS    (2u)
#define Slider_SENSOR_LINEARSLIDER_E3__LS    (3u)
#define Slider_SENSOR_LINEARSLIDER_E4__LS    (4u)
/* AMux Cmod, Comparator and Idac Channels definitions */
#define Slider_AMuxCH0_CMOD_CHANNEL          (5u)
#define Slider_AMuxCH0_CMP_VP_CHANNEL        (6u)
#define Slider_AMuxCH0_IDAC_CHANNEL          (7u)



/* Vdac value for Vref = Vdac */
#define Slider_VREF_VDAC_VALUE            (64u)

/* Scan Speed Type */
#define Slider_SCAN_SPEED_ULTRA_FAST      (0x01u)
#define Slider_SCAN_SPEED_FAST            (0x03u)
#define Slider_SCAN_SPEED_NORMAL          (0x07u)
#define Slider_SCAN_SPEED_SLOW            (0x0Fu)

/* PWM Resolution */
#define Slider_PWM_WINDOW_APPEND          (0xFEu)
#define Slider_PWM_RESOLUTION_8_BITS      (0x00u)
#define Slider_PWM_RESOLUTION_9_BITS      (0x01u)
#define Slider_PWM_RESOLUTION_10_BITS     (0x03u)
#define Slider_PWM_RESOLUTION_11_BITS     (0x07u)
#define Slider_PWM_RESOLUTION_12_BITS     (0x0Fu)
#define Slider_PWM_RESOLUTION_13_BITS     (0x1Fu)
#define Slider_PWM_RESOLUTION_14_BITS     (0x3Fu)
#define Slider_PWM_RESOLUTION_15_BITS     (0x7Fu)
#define Slider_PWM_RESOLUTION_16_BITS     (0xFFu)

/* Software Status Register Bit Masks */
#define Slider_SW_STS_BUSY                (0x01u)
/* Software Status Register Bit Masks */
#define Slider_SW_CTRL_SINGLE_SCAN        (0x80u)

/* Init Idac current */
#define Slider_TURN_OFF_IDAC              (0x00u)

/* Rbleed definitions */
#define Slider_RBLEED1                    (0u)
#define Slider_RBLEED2                    (1u)
#define Slider_RBLEED3                    (2u)

/* Flag of complex scan slot */
#define Slider_COMPLEX_SS_FLAG            (0x80u)
#define Slider_CHANNEL1_FLAG              (0x80u)


#define Slider_ANALOG_SWITCHES_SRC_PRESCALER (0x01u)
#define Slider_ANALOG_SWITCHES_SRC_PRS       (0x02u)


/***************************************
*             Registers
***************************************/

/* Control REG */
#define Slider_CONTROL_REG    (*(reg8 *) \
                                            Slider_ClockGen_SyncCtrl_CtrlReg__CONTROL_REG )
#define Slider_CONTROL_PTR    ( (reg8 *) \
                                            Slider_ClockGen_SyncCtrl_CtrlReg__CONTROL_REG )

/* Clock Gen - ScanSpeed REGs definitions */
#define Slider_SCANSPEED_AUX_CONTROL_REG (*(reg8 *) Slider_ClockGen_ScanSpeed__CONTROL_AUX_CTL_REG )
#define Slider_SCANSPEED_AUX_CONTROL_PTR ( (reg8 *) Slider_ClockGen_ScanSpeed__CONTROL_AUX_CTL_REG )

#define Slider_SCANSPEED_PERIOD_REG      (*(reg8 *) Slider_ClockGen_ScanSpeed__PERIOD_REG )
#define Slider_SCANSPEED_PERIOD_PTR      ( (reg8 *) Slider_ClockGen_ScanSpeed__PERIOD_REG )

#define Slider_SCANSPEED_COUNTER_REG     (*(reg8 *) Slider_ClockGen_ScanSpeed__COUNT_REG )
#define Slider_SCANSPEED_COUNTER_PTR     ( (reg8 *) Slider_ClockGen_ScanSpeed__COUNT_REG )


/* Clock Gen - Prescaler REGs definitions */
#if (Slider_PRESCALER_OPTIONS == Slider_PRESCALER_UDB)
    #define Slider_PRESCALER_PERIOD_REG       (*(reg8 *) Slider_ClockGen_UDB_PrescalerDp_u0__D0_REG)
    #define Slider_PRESCALER_PERIOD_PTR       ( (reg8 *) Slider_ClockGen_UDB_PrescalerDp_u0__D0_REG)
    
    #define Slider_PRESCALER_COMPARE_REG      (*(reg8 *) Slider_ClockGen_UDB_PrescalerDp_u0__D1_REG)
    #define Slider_PRESCALER_COMPARE_PTR      ( (reg8 *) Slider_ClockGen_UDB_PrescalerDp_u0__D1_REG)
    
#elif (Slider_PRESCALER_OPTIONS == Slider_PRESCALER_FF)
    #define Slider_PRESCALER_PERIOD_PTR       ( (reg16 *) Slider_ClockGen_FF_Prescaler__PER0 )
    #define Slider_PRESCALER_COMPARE_PTR      ( (reg16 *) Slider_ClockGen_FF_Prescaler__CNT_CMP0 )
    
    #define Slider_PRESCALER_CONTROL_REG      (*(reg8 *) Slider_ClockGen_FF_Prescaler__CFG0 )
    #define Slider_PRESCALER_CONTROL_PTR      ( (reg8 *) Slider_ClockGen_FF_Prescaler__CFG0 )
    
    #if (CY_PSOC5A)
        #define Slider_PRESCALER_CONTROL2_REG     (*(reg8 *) Slider_ClockGen_FF_Prescaler__CFG1 )
        #define Slider_PRESCALER_CONTROL2_PTR     ( (reg8 *) Slider_ClockGen_FF_Prescaler__CFG1 )
    #else
        #define Slider_PRESCALER_CONTROL2_REG     (*(reg8 *) Slider_ClockGen_FF_Prescaler__CFG2 )
        #define Slider_PRESCALER_CONTROL2_PTR     ( (reg8 *) Slider_ClockGen_FF_Prescaler__CFG2 )
    #endif  /* (CY_PSOC5A) */
    
    #define Slider_PRESCALER_ACT_PWRMGR_REG   (*(reg8 *) Slider_ClockGen_FF_Prescaler__PM_ACT_CFG )
    #define Slider_PRESCALER_ACT_PWRMGR_PTR   ( (reg8 *) Slider_ClockGen_FF_Prescaler__PM_ACT_CFG )
    #define Slider_PRESCALER_ACT_PWR_EN                 (Slider_ClockGen_FF_Prescaler__PM_ACT_MSK )
    
    #define Slider_PRESCALER_STBY_PWRMGR_REG  (*(reg8 *) Slider_ClockGen_FF_Prescaler__PM_STBY_CFG )
    #define Slider_PRESCALER_STBY_PWRMGR_PTR  ( (reg8 *) Slider_ClockGen_FF_Prescaler__PM_STBY_CFG )
    #define Slider_PRESCALER_STBY_PWR_EN                (Slider_ClockGen_FF_Prescaler__PM_STBY_MSK )

#else
    /* No prescaler */
#endif  /* (Slider_PRESCALER_OPTIONS == Slider_PRESCALER_UDB) */

/* PRS */
#if (Slider_PRS_OPTIONS == Slider_PRS_8BITS)  
    /* Polynomial */
    #define Slider_POLYNOM_REG        (*(reg8 *) Slider_ClockGen_sC8_PRSdp_u0__D0_REG )
    #define Slider_POLYNOM_PTR        ( (reg8 *) Slider_ClockGen_sC8_PRSdp_u0__D0_REG )
    /* Seed */
    #define Slider_SEED_REG           (*(reg8 *) Slider_ClockGen_sC8_PRSdp_u0__A0_REG )
    #define Slider_SEED_PTR           ( (reg8 *) Slider_ClockGen_sC8_PRSdp_u0__A0_REG )
    /* Seed COPY */
    #define Slider_SEED_COPY_REG      (*(reg8 *) Slider_ClockGen_sC8_PRSdp_u0__F0_REG )
    #define Slider_SEED_COPY_PTR      ( (reg8 *) Slider_ClockGen_sC8_PRSdp_u0__F0_REG )
    /* Aux control */
    #define Slider_AUX_CONTROL_A_REG  (*(reg8 *) Slider_ClockGen_sC8_PRSdp_u0__DP_AUX_CTL_REG )
    #define Slider_AUX_CONTROL_A_PTR  ( (reg8 *) Slider_ClockGen_sC8_PRSdp_u0__DP_AUX_CTL_REG )
        
#elif (Slider_PRS_OPTIONS == Slider_PRS_16BITS)
    /* Polynomial */
    #define Slider_POLYNOM_REG        (*(reg16 *) Slider_ClockGen_sC16_PRSdp_u0__D0_REG )
    #define Slider_POLYNOM_PTR        ( (reg16 *) Slider_ClockGen_sC16_PRSdp_u0__D0_REG )
    /* Seed */
    #define Slider_SEED_REG           (*(reg16 *) Slider_ClockGen_sC16_PRSdp_u0__A0_REG )
    #define Slider_SEED_PTR           ( (reg16 *) Slider_ClockGen_sC16_PRSdp_u0__A0_REG )
    /* Seed COPY */
    #define Slider_SEED_COPY_REG      (*(reg16 *) Slider_ClockGen_sC16_PRSdp_u0__F0_REG )
    #define Slider_SEED_COPY_PTR      ( (reg16 *) Slider_ClockGen_sC16_PRSdp_u0__F0_REG )
    /* Aux control */
    #define Slider_AUX_CONTROL_A_REG  (*(reg8 *) Slider_ClockGen_sC16_PRSdp_u0__DP_AUX_CTL_REG )
    #define Slider_AUX_CONTROL_A_PTR  ( (reg8 *) Slider_ClockGen_sC16_PRSdp_u0__DP_AUX_CTL_REG )
    
    #define Slider_AUX_CONTROL_B_REG  (*(reg8 *) Slider_ClockGen_sC16_PRSdp_u1__DP_AUX_CTL_REG )
    #define Slider_AUX_CONTROL_B_PTR  ( (reg8 *) Slider_ClockGen_sC16_PRSdp_u1__DP_AUX_CTL_REG )
    
#elif (Slider_PRS_OPTIONS == Slider_PRS_16BITS_4X)
    /* Polynomial */   
    #define Slider_POLYNOM_A__D1_REG      (*(reg8 *) Slider_ClockGen_b0_PRSdp_a__D1_REG )
    #define Slider_POLYNOM_A__D1_PTR      ( (reg8 *) Slider_ClockGen_b0_PRSdp_a__D1_REG )
    
    #define Slider_POLYNOM_A__D0_REG      (*(reg8 *) Slider_ClockGen_b0_PRSdp_a__D0_REG )
    #define Slider_POLYNOM_A__D0_PTR      ( (reg8 *) Slider_ClockGen_b0_PRSdp_a__D0_REG )
    /* Seed */
    #define Slider_SEED_A__A1_REG         (*(reg8 *) Slider_ClockGen_b0_PRSdp_a__A1_REG )
    #define Slider_SEED_A__A1_PTR         ( (reg8 *) Slider_ClockGen_b0_PRSdp_a__A1_REG )
    
    #define Slider_SEED_A__A0_REG         (*(reg8 *) Slider_ClockGen_b0_PRSdp_a__A0_REG )
    #define Slider_SEED_A__A0_PTR         ( (reg8 *) Slider_ClockGen_b0_PRSdp_a__A0_REG )
    /* Seed COPY */
    #define Slider_SEED_COPY_A__F1_REG    (*(reg8 *) Slider_ClockGen_b0_PRSdp_a__F1_REG )
    #define Slider_SEED_COPY_A__F1_PTR    ( (reg8 *) Slider_ClockGen_b0_PRSdp_a__F1_REG )
    
    #define Slider_SEED_COPY_A__F0_REG    (*(reg8 *) Slider_ClockGen_b0_PRSdp_a__F0_REG )
    #define Slider_SEED_COPY_A__F0_PTR    ( (reg8 *) Slider_ClockGen_b0_PRSdp_a__F0_REG )
    /* Aux control */
    #define Slider_AUX_CONTROL_A_REG  (*(reg8 *) Slider_ClockGen_b0_PRSdp_a__DP_AUX_CTL_REG )
    #define Slider_AUX_CONTROL_A_PTR  ( (reg8 *) Slider_ClockGen_b0_PRSdp_a__DP_AUX_CTL_REG )
    
#else
    /* No PRS */
#endif  /* (Slider_PRS_OPTIONS == Slider_PRS_8BITS)  */

/* Measure REGs  definitions */
#if (Slider_IMPLEMENTATION_CH0 == Slider_MEASURE_IMPLEMENTATION_FF)
    /* Window PWM */
    #define Slider_PWM_CH0_PERIOD_PTR         ( (reg16 *) Slider_MeasureCH0_FF_Window__PER0 )
    #define Slider_PWM_CH0_COUNTER_PTR        ( (reg16 *) Slider_MeasureCH0_FF_Window__CNT_CMP0 )
    
    #define Slider_PWM_CH0_CONTROL_REG        (*(reg8 *) Slider_MeasureCH0_FF_Window__CFG0 )
    #define Slider_PWM_CH0_CONTROL_PTR        ( (reg8 *) Slider_MeasureCH0_FF_Window__CFG0 )
    
    #define Slider_PWM_CH0_CONTROL2_REG       (*(reg8 *) Slider_MeasureCH0_FF_Window__CFG2 )
    #define Slider_PWM_CH0_CONTROL2_PTR       ( (reg8 *) Slider_MeasureCH0_FF_Window__CFG2 )
	
    #define Slider_PWM_CH0_ACT_PWRMGR_REG     (*(reg8 *) Slider_MeasureCH0_FF_Window__PM_ACT_CFG )
    #define Slider_PWM_CH0_ACT_PWRMGR_PTR     ( (reg8 *) Slider_MeasureCH0_FF_Window__PM_ACT_CFG )
    #define Slider_PWM_CH0_ACT_PWR_EN                   (Slider_MeasureCH0_FF_Window__PM_ACT_MSK )
    
    #define Slider_PWM_CH0_STBY_PWRMGR_REG    (*(reg8 *) Slider_MeasureCH0_FF_Window__PM_STBY_CFG )
    #define Slider_PWM_CH0_STBY_PWRMGR_PTR    ( (reg8 *) Slider_MeasureCH0_FF_Window__PM_STBY_CFG )
    #define Slider_PWM_CH0_STBY_PWR_EN                  (Slider_MeasureCH0_FF_Window__PM_STBY_MSK )
    
    /* Raw Counter */
    #define Slider_RAW_CH0_PERIOD_PTR         ( (reg16 *) Slider_MeasureCH0_FF_Counter__PER0 )
    #define Slider_RAW_CH0_COUNTER_PTR        ( (reg16 *) Slider_MeasureCH0_FF_Counter__CNT_CMP0 )
    
    #define Slider_RAW_CH0_CONTROL_REG        (*(reg8 *) Slider_MeasureCH0_FF_Counter__CFG0 )
    #define Slider_RAW_CH0_CONTROL_PTR        ( (reg8 *) Slider_MeasureCH0_FF_Counter__CFG0 )
    

    #define Slider_RAW_CH0_CONTROL2_REG   (*(reg8 *) Slider_MeasureCH0_FF_Counter__CFG2 )
    #define Slider_RAW_CH0_CONTROL2_PTR   ( (reg8 *) Slider_MeasureCH0_FF_Counter__CFG2 )

    
    #define Slider_RAW_CH0_ACT_PWRMGR_REG     (*(reg8 *) Slider_MeasureCH0_FF_Counter__PM_ACT_CFG )
    #define Slider_RAW_CH0_ACT_PWRMGR_PTR     ( (reg8 *) Slider_MeasureCH0_FF_Counter__PM_ACT_CFG )
    #define Slider_RAW_CH0_ACT_PWR_EN                   (Slider_MeasureCH0_FF_Counter__PM_ACT_MSK )
    
    #define Slider_RAW_CH0_STBY_PWRMGR_REG    (*(reg8 *) Slider_MeasureCH0_FF_Counter__PM_STBY_CFG )
    #define Slider_RAW_CH0_STBY_PWRMGR_PTR    ( (reg8 *) Slider_MeasureCH0_FF_Counter__PM_STBY_CFG )
    #define Slider_RAW_CH0_STBY_PWR_EN                  (Slider_MeasureCH0_FF_Counter__PM_STBY_MSK )
#else
     /* Window PWM */
    #define Slider_PWM_CH0_COUNTER_LO_REG     (*(reg8 *) Slider_MeasureCH0_UDB_Window_u0__A0_REG )
    #define Slider_PWM_CH0_COUNTER_LO_PTR     ( (reg8 *) Slider_MeasureCH0_UDB_Window_u0__A0_REG )
    
    #define Slider_PWM_CH0_COUNTER_HI_REG     (*(reg8 *) Slider_MeasureCH0_UDB_Window_u0__A1_REG )
    #define Slider_PWM_CH0_COUNTER_HI_PTR     ( (reg8 *) Slider_MeasureCH0_UDB_Window_u0__A1_REG )
    
    #define Slider_PWM_CH0_PERIOD_LO_REG      (*(reg8 *) Slider_MeasureCH0_UDB_Window_u0__F0_REG )
    #define Slider_PWM_CH0_PERIOD_LO_PTR      ( (reg8 *) Slider_MeasureCH0_UDB_Window_u0__F0_REG )
    
    #define Slider_PWM_CH0_PERIOD_HI_REG      (*(reg8 *) Slider_MeasureCH0_UDB_Window_u0__F1_REG )
    #define Slider_PWM_CH0_PERIOD_HI_PTR      ( (reg8 *) Slider_MeasureCH0_UDB_Window_u0__F1_REG )
    
    #define Slider_PWM_CH0_ADD_VALUE_REG      (*(reg8 *) Slider_MeasureCH0_UDB_Window_u0__D0_REG )
    #define Slider_PWM_CH0_ADD_VALUE_PTR      ( (reg8 *) Slider_MeasureCH0_UDB_Window_u0__D0_REG )
    
    #define Slider_PWM_CH0_AUX_CONTROL_REG    (*(reg8 *) \
                                                            Slider_MeasureCH0_UDB_Window_u0__DP_AUX_CTL_REG )
    #define Slider_PWM_CH0_AUX_CONTROL_PTR    ( (reg8 *) \
                                                            Slider_MeasureCH0_UDB_Window_u0__DP_AUX_CTL_REG )
    
    /* Raw Counter */
    #define Slider_RAW_CH0_COUNTER_LO_REG      (*(reg8 *) Slider_MeasureCH0_UDB_Counter_u0__A0_REG )
    #define Slider_RAW_CH0_COUNTER_LO_PTR      ( (reg8 *) Slider_MeasureCH0_UDB_Counter_u0__A0_REG )
    
    #define Slider_RAW_CH0_COUNTER_HI_REG      (*(reg8 *) Slider_MeasureCH0_UDB_Counter_u0__A1_REG )
    #define Slider_RAW_CH0_COUNTER_HI_PTR      ( (reg8 *) Slider_MeasureCH0_UDB_Counter_u0__A1_REG )
    
    #define Slider_RAW_CH0_PERIOD_LO_REG       (*(reg8 *) Slider_MeasureCH0_UDB_Counter_u0__F0_REG )
    #define Slider_RAW_CH0_PERIOD_LO_PTR       ( (reg8 *) Slider_MeasureCH0_UDB_Counter_u0__F0_REG )
    
    #define Slider_RAW_CH0_PERIOD_HI_REG       (*(reg8 *) Slider_MeasureCH0_UDB_Counter_u0__F1_REG )
    #define Slider_RAW_CH0_PERIOD_HI_PTR       ( (reg8 *) Slider_MeasureCH0_UDB_Counter_u0__F1_REG )
    
    #define Slider_RAW_CH0_ADD_VALUE_REG       (*(reg8 *) Slider_MeasureCH0_UDB_Counter_u0__D0_REG )
    #define Slider_RAW_CH0_ADD_VALUE_PTR       ( (reg8 *) Slider_MeasureCH0_UDB_Counter_u0__D0_REG )
    
    #define Slider_RAW_CH0_AUX_CONTROL_REG     (*(reg8 *) \
                                                            Slider_MeasureCH0_UDB_Counter_u0__DP_AUX_CTL_REG )
    #define Slider_RAW_CH0_AUX_CONTROL_PTR     ( (reg8 *) \
                                                            Slider_MeasureCH0_UDB_Counter_u0__DP_AUX_CTL_REG )

#endif  /* (Slider_IMPLEMENTATION_CH0 == Slider_MEASURE_IMPLEMENTATION_FF) */
    
#if (Slider_DESIGN_TYPE == Slider_TWO_CHANNELS_DESIGN)
    #if (Slider_IMPLEMENTATION_CH1 == Slider_MEASURE_IMPLEMENTATION_FF)
        /* Window PWM */
        #define Slider_PWM_CH1_PERIOD_PTR        ( (reg16 *) Slider_MeasureCH1_FF_Window__PER0 )
        #define Slider_PWM_CH1_COUNTER_PTR       ( (reg16 *) Slider_MeasureCH1_FF_Window__CNT_CMP0 )
        
        #define Slider_PWM_CH1_CONTROL_REG        (*(reg8 *) Slider_MeasureCH1_FF_Window__CFG0 )
        #define Slider_PWM_CH1_CONTROL_PTR        ( (reg8 *) Slider_MeasureCH1_FF_Window__CFG0 )
        
        #define Slider_PWM_CH1_CONTROL2_REG   (*(reg8 *) Slider_MeasureCH1_FF_Window__CFG2 )
        #define Slider_PWM_CH1_CONTROL2_PTR   ( (reg8 *) Slider_MeasureCH1_FF_Window__CFG2 )
        
        #define Slider_PWM_CH1_ACT_PWRMGR_REG   (*(reg8 *) Slider_MeasureCH1_FF_Window__PM_ACT_CFG )
        #define Slider_PWM_CH1_ACT_PWRMGR_PTR   ( (reg8 *) Slider_MeasureCH1_FF_Window__PM_ACT_CFG )
        #define Slider_PWM_CH1_ACT_PWR_EN                 (Slider_MeasureCH1_FF_Window__PM_ACT_MSK )
        
        #define Slider_PWM_CH1_STBY_PWRMGR_REG (*(reg8 *) Slider_MeasureCH1_FF_Window__PM_STBY_CFG )
        #define Slider_PWM_CH1_STBY_PWRMGR_PTR ( (reg8 *) Slider_MeasureCH1_FF_Window__PM_STBY_CFG )
        #define Slider_PWM_CH1_STBY_PWR_EN               (Slider_MeasureCH1_FF_Window__PM_STBY_MSK )
            
        /* Raw Counter */
        #define Slider_RAW_CH1_PERIOD_PTR       ( (reg16 *) Slider_MeasureCH1_FF_Counter__PER0 )
        #define Slider_RAW_CH1_COUNTER_PTR      ( (reg16 *) Slider_MeasureCH1_FF_Counter__CNT_CMP0 )
        
        #define Slider_RAW_CH1_CONTROL_REG      (*(reg8 *) Slider_MeasureCH1_FF_Counter__CFG0 )
        #define Slider_RAW_CH1_CONTROL_PTR      ( (reg8 *) Slider_MeasureCH1_FF_Counter__CFG0 )
        
        #define Slider_RAW_CH1_CONTROL2_REG   (*(reg8 *) Slider_MeasureCH1_FF_Counter__CFG2 )
        #define Slider_RAW_CH1_CONTROL2_PTR   ( (reg8 *) Slider_MeasureCH1_FF_Counter__CFG2 )
        
        #define Slider_RAW_CH1_ACT_PWRMGR_REG  (*(reg8 *) Slider_MeasureCH1_FF_Counter__PM_ACT_CFG )
        #define Slider_RAW_CH1_ACT_PWRMGR_PTR  ( (reg8 *) Slider_MeasureCH1_FF_Counter__PM_ACT_CFG )
        #define Slider_RAW_CH1_ACT_PWR_EN                (Slider_MeasureCH1_FF_Counter__PM_ACT_MSK )
        
        #define Slider_RAW_CH1_STBY_PWRMGR_REG (*(reg8 *) Slider_MeasureCH1_FF_Counter__PM_STBY_CFG)
        #define Slider_RAW_CH1_STBY_PWRMGR_PTR ( (reg8 *) Slider_MeasureCH1_FF_Counter__PM_STBY_CFG)
        #define Slider_RAW_CH1_STBY_PWR_EN               (Slider_MeasureCH1_FF_Counter__PM_STBY_MSK)
    
    #else
        /* Window PWM */
        #define Slider_PWM_CH1_COUNTER_LO_REG   (*(reg8 *) Slider_MeasureCH1_UDB_Window_u0__A0_REG )
        #define Slider_PWM_CH1_COUNTER_LO_PTR   ( (reg8 *) Slider_MeasureCH1_UDB_Window_u0__A0_REG )
        
        #define Slider_PWM_CH1_COUNTER_HI_REG   (*(reg8 *) Slider_MeasureCH1_UDB_Window_u0__A1_REG )
        #define Slider_PWM_CH1_COUNTER_HI_PTR   ( (reg8 *) Slider_MeasureCH1_UDB_Window_u0__A1_REG )
        
        #define Slider_PWM_CH1_PERIOD_LO_REG    (*(reg8 *) Slider_MeasureCH1_UDB_Window_u0__F0_REG )
        #define Slider_PWM_CH1_PERIOD_LO_PTR    ( (reg8 *) Slider_MeasureCH1_UDB_Window_u0__F0_REG )
        
        #define Slider_PWM_CH1_PERIOD_HI_REG    (*(reg8 *) Slider_MeasureCH1_UDB_Window_u0__F1_REG )
        #define Slider_PWM_CH1_PERIOD_HI_PTR    ( (reg8 *) Slider_MeasureCH1_UDB_Window_u0__F1_REG )
        
        #define Slider_PWM_CH1_ADD_VALUE_REG    (*(reg8 *) Slider_MeasureCH1_UDB_Window_u0__D0_REG )
        #define Slider_PWM_CH1_ADD_VALUE_PTR    ( (reg8 *) Slider_MeasureCH1_UDB_Window_u0__D0_REG )
        
        #define Slider_PWM_CH1_AUX_CONTROL_REG  (*(reg8 *) \
                                                            Slider_MeasureCH1_UDB_Window_u0__DP_AUX_CTL_REG )
        #define Slider_PWM_CH1_AUX_CONTROL_PTR  ( (reg8 *) \
                                                            Slider_MeasureCH1_UDB_Window_u0__DP_AUX_CTL_REG )
        
        /* Raw Counter */
        #define Slider_RAW_CH1_COUNTER_LO_REG  (*(reg8 *) Slider_MeasureCH1_UDB_Counter_u0__A0_REG )
        #define Slider_RAW_CH1_COUNTER_LO_PTR  ( (reg8 *) Slider_MeasureCH1_UDB_Counter_u0__A0_REG )
        
        #define Slider_RAW_CH1_COUNTER_HI_REG  (*(reg8 *) Slider_MeasureCH1_UDB_Counter_u0__A1_REG )
        #define Slider_RAW_CH1_COUNTER_HI_PTR  ( (reg8 *) Slider_MeasureCH1_UDB_Counter_u0__A1_REG )
        
        #define Slider_RAW_CH1_PERIOD_LO_REG   (*(reg8 *) Slider_MeasureCH1_UDB_Counter_u0__F0_REG )
        #define Slider_RAW_CH1_PERIOD_LO_PTR   ( (reg8 *) Slider_MeasureCH1_UDB_Counter_u0__F0_REG )
        
        #define Slider_RAW_CH1_PERIOD_HI_REG   (*(reg8 *) Slider_MeasureCH1_UDB_Counter_u0__F1_REG )
        #define Slider_RAW_CH1_PERIOD_HI_PTR   ( (reg8 *) Slider_MeasureCH1_UDB_Counter_u0__F1_REG )
        
        #define Slider_RAW_CH1_ADD_VALUE_REG   (*(reg8 *) Slider_MeasureCH1_UDB_Counter_u0__D0_REG )
        #define Slider_RAW_CH1_ADD_VALUE_PTR   ( (reg8 *) Slider_MeasureCH1_UDB_Counter_u0__D0_REG )
        
        #define Slider_RAW_CH1_AUX_CONTROL_REG  (*(reg8 *) \
                                                            Slider_MeasureCH1_UDB_Counter_u0__DP_AUX_CTL_REG )
        #define Slider_RAW_CH1_AUX_CONTROL_PTR  ( (reg8 *) \
                                                            Slider_MeasureCH1_UDB_Counter_u0__DP_AUX_CTL_REG )
        
    #endif  /* Slider_DESIGN_TYPE */
    
#endif  /* Slider_DESIGN_TYPE */


/* CapSense Buffer REGs definitions */
#define Slider_BufCH0_CAPS_CFG0_REG           (*(reg8 *) Slider_BufCH0__CFG0 )
#define Slider_BufCH0_CAPS_CFG0_PTR           ( (reg8 *) Slider_BufCH0__CFG0 )

#define Slider_BufCH0_CAPS_CFG1_REG           (*(reg8 *) Slider_BufCH0__CFG1 )
#define Slider_BufCH0_CAPS_CFG1_PTR           ( (reg8 *) Slider_BufCH0__CFG1 )

#define Slider_BufCH0_ACT_PWRMGR_REG          (*(reg8 *) Slider_BufCH0__PM_ACT_CFG )
#define Slider_BufCH0_ACT_PWRMGR_PTR          ( (reg8 *) Slider_BufCH0__PM_ACT_CFG )
#define Slider_BufCH0_ACT_PWR_EN                        (Slider_BufCH0__PM_ACT_MSK )

#define Slider_BufCH0_STBY_PWRMGR_REG         (*(reg8 *) Slider_BufCH0__PM_STBY_CFG )
#define Slider_BufCH0_STBY_PWRMGR_PTR         ( (reg8 *) Slider_BufCH0__PM_STBY_CFG )
#define Slider_BufCH0_STBY_PWR_EN                       (Slider_BufCH0__PM_STBY_MSK )

#if (Slider_DESIGN_TYPE == Slider_TWO_CHANNELS_DESIGN)
    #define Slider_BufCH1_CAPS_CFG0_REG       (*(reg8 *) Slider_BufCH1__CFG0 )
    #define Slider_BufCH1_CAPS_CFG0_PTR       ( (reg8 *) Slider_BufCH1__CFG0 )
    
    #define Slider_BufCH1_CAPS_CFG1_REG       (*(reg8 *) Slider_BufCH1__CFG1 )
    #define Slider_BufCH1_CAPS_CFG1_PTR       ( (reg8 *) Slider_BufCH1__CFG1 )
    
    #define Slider_BufCH1_ACT_PWRMGR_REG      (*(reg8 *) Slider_BufCH1__PM_ACT_CFG )
    #define Slider_BufCH1_ACT_PWRMGR_PTR      ( (reg8 *) Slider_BufCH1__PM_ACT_CFG )
    #define Slider_BufCH1_ACT_PWR_EN                    (Slider_BufCH1__PM_ACT_MSK )
    
    #define Slider_BufCH1_STBY_PWRMGR_REG     (*(reg8 *) Slider_BufCH1__PM_STBY_CFG )
    #define Slider_BufCH1_STBY_PWRMGR_PTR     ( (reg8 *) Slider_BufCH1__PM_STBY_CFG )
    #define Slider_BufCH1_STBY_PWR_EN                   (Slider_BufCH1__PM_STBY_MSK )
#endif  /* Slider_DESIGN_TYPE == Slider_TWO_CHANNELS_DESIGN */

/* ISR Number and Priority to work with CyLib functions */
#define Slider_IsrCH0_ISR_NUMBER        (Slider_IsrCH0__INTC_NUMBER)
#define Slider_IsrCH0_ISR_PRIORITY      (Slider_IsrCH0__INTC_PRIOR_NUM)

#if (Slider_DESIGN_TYPE == Slider_TWO_CHANNELS_DESIGN)
    #define Slider_IsrCH1_ISR_NUMBER        (Slider_IsrCH1__INTC_NUMBER)
    #define Slider_IsrCH1_ISR_PRIORITY      (Slider_IsrCH1__INTC_PRIOR_NUM)
#endif /* Slider_DESIGN_TYPE == Slider_TWO_CHANNELS_DESIGN */


/***************************************
*       Register Constants
***************************************/

/* Control Register definitions */
#define Slider_CTRL_SYNC_EN                       (0x01u)
#define Slider_CTRL_START                         (0x02u)
#define Slider_CTRL_WINDOW_EN__CH0                (0x04u)
#define Slider_CTRL_WINDOW_EN__CH1                (0x08u)
/* Addtional bit to verify if component is enalbed */
#define Slider_CTRL_CAPSENSE_EN                   (0x80u)

#define Slider_IS_CAPSENSE_ENABLE(reg)            ( ((reg) & Slider_CTRL_CAPSENSE_EN) != 0u )

/* ClockGen defines */
/* Prescaler */
#define Slider_PRESCALER_CTRL_ENABLE          (0x01u)
#define Slider_PRESCALER_CTRL_MODE_CMP        (0x02u) 

#define Slider_PRESCALER_CTRL_CMP_MODE_SHIFT  (0x04u)

#define Slider_PRESCALER_CTRL_CMP_LESS_EQ         (0x02u << Slider_PRESCALER_CTRL_CMP_MODE_SHIFT)

/* Set PRS polynomial */
#define Slider_PRS8_DEFAULT_POLYNOM           (0xB8u)
#define Slider_PRS16_DEFAULT_POLYNOM          (0xB400u)

/* Scan Speed */
#define Slider_SCANSPEED_CTRL_ENABLE          (0x20u)

/* Measure defines */
/* FF Timers */
#define Slider_MEASURE_FULL_RANGE             (0xFFFFu)
#define Slider_MEASURE_FULL_RANGE_LOW         (0xFFu)
#define Slider_MEASURE_CTRL_ENABLE            (0x01u)

#define Slider_MEASURE_CTRL_MODE_SHIFT        (0x00u)


#define Slider_MEASURE_CTRL_PULSEWIDTH        (0x01u << Slider_MEASURE_CTRL_MODE_SHIFT)

/* UDB timers */
#define Slider_AUXCTRL_FIFO_SINGLE_REG        (0x03u)
 
/* Masks of PTR PC Register */
#define Slider_DR_MASK            (0x01u)
#define Slider_DM0_MASK           (0x02u)
#define Slider_DM1_MASK           (0x04u)
#define Slider_DM2_MASK           (0x08u)
#define Slider_BYP_MASK           (0x80u)

#define Slider_PRT_PC_GND         (Slider_DM2_MASK)
#define Slider_PRT_PC_HIGHZ       (Slider_DM2_MASK |Slider_DR_MASK)
#define Slider_PRT_PC_SHIELD      (Slider_DM2_MASK | Slider_DM1_MASK | \
                                             Slider_BYP_MASK)

/* CapSense Buffer definitions */
#define Slider_CSBUF_BOOST_ENABLE         (0x02u)
#define Slider_CSBUF_ENABLE               (0x01u)

/* Define direction of Current - Souce as Sink */
#if (Slider_CURRENT_SOURCE == Slider_IDAC_SOURCE)
    #define Slider_IdacCH0_IDIR      (Slider_IdacCH0_SOURCE)
    #define Slider_IdacCH1_IDIR      (Slider_IdacCH1_SOURCE)
#elif (Slider_CURRENT_SOURCE == Slider_IDAC_SINK)
    #define Slider_IdacCH0_IDIR      (Slider_IdacCH0_SINK)
    #define Slider_IdacCH1_IDIR      (Slider_IdacCH1_SINK)
#else
    /* No Idac - Rb selected */
#endif  /* (Slider_CURRENT_SOURCE == Slider_IDAC_SOURCE) */


/* Rb init function */
#if (Slider_CURRENT_SOURCE == Slider_EXTERNAL_RB)
    void Slider_InitRb(void);
#endif /* End Slider_CURRENT_SOURCE */ 

#if (Slider_IS_COMPLEX_SCANSLOTS)
    void Slider_EnableScanSlot(uint8 slot) CYREENTRANT;
    void Slider_DisableScanSlot(uint8 slot) CYREENTRANT;
    
#else
    #define Slider_EnableScanSlot(slot)   Slider_EnableSensor(slot)
    #define Slider_DisableScanSlot(slot)  Slider_DisableSensor(slot)

#endif  /* End Slider_IS_COMPLEX_SCANSLOTS */

/* Helper functions - do nto part of public interface*/

/* Find next sensor for One Channel design */
#if (Slider_DESIGN_TYPE == Slider_ONE_CHANNEL_DESIGN)
    uint8 Slider_FindNextSensor(uint8 snsIndex) CYREENTRANT;
#endif  /* End Slider_DESIGN_TYPE */

/* Find next pair for Two Channels design */
 #if (Slider_DESIGN_TYPE == Slider_TWO_CHANNELS_DESIGN)
    uint8 Slider_FindNextPair(uint8 snsIndex) CYREENTRANT;
#endif  /* End Slider_DESIGN_TYPE */

/* Start and Compete the scan */
void Slider_PreScan(uint8 sensor) CYREENTRANT;
#if (Slider_DESIGN_TYPE == Slider_ONE_CHANNEL_DESIGN)
    void Slider_PostScan(uint8 sensor) CYREENTRANT;
#else
    void Slider_PostScanCh0(uint8 sensor) CYREENTRANT;
    void Slider_PostScanCh1(uint8 sensor) CYREENTRANT;
#endif  /* End Slider_DESIGN_TYPE */

#if (Slider_PRESCALER_OPTIONS)
    void Slider_SetPrescaler(uint8 prescaler) CYREENTRANT;
#endif  /* End Slider_PRESCALER_OPTIONS */

void Slider_SetScanSpeed(uint8 scanSpeed) ;

/* SmartSense functions */
#if (Slider_TUNING_METHOD == Slider_AUTO_TUNING)
    extern uint8 Slider_lowLevelTuningDone;
    extern void Slider_AutoTune(void) ;
#endif /* End (Slider_TUNING_METHOD == Slider_AUTO_TUNING) */

/* Global software variables */
extern volatile uint8 Slider_csv;            /* CapSense CSD status, control variable */
extern volatile uint8 Slider_sensorIndex;    /* Index of scannig sensor */

#if (Slider_CURRENT_SOURCE == Slider_EXTERNAL_RB)
    extern uint8  Slider_extRbCh0Cur;
    #if (Slider_DESIGN_TYPE == Slider_TWO_CHANNELS_DESIGN)
        extern uint8  Slider_extRbCh1Cur;
    #endif /* (Slider_DESIGN_TYPE == Slider_TWO_CHANNELS_DESIGN)*/ 
#endif /* (Slider_CURRENT_SOURCE == Slider_EXTERNAL_RB) */ 
        
/* Global array of Raw Counts */
extern uint16 Slider_sensorRaw[Slider_TOTAL_SENSOR_COUNT];


extern Slider_BACKUP_STRUCT Slider_backup;

/****************************************************************************************
*       Obsolete definitions. Not recommended to use. Will be removed in future releases.
*****************************************************************************************/

/* Auto Tuning defualt raw counts value */
#define Slider_DEFUALT_RAW_COUNTS_VALUE    (384u)

/* IDAC_CR0 registers save/restore flags */
#define Slider_IdacCH0_RESTORE_CFG (1u)
#define Slider_IdacCH1_RESTORE_CFG (2u)
/* DAC_CR0 register value when IDAC is stopped*/
#define Slider_IDAC_STOP_CR0_VAL   (0x1Eu)

/* Idac SetRange */
#define Slider_IDAC_RANGE_MASK            (0x0Cu)
#define Slider_IDAC_RANGE_32uA            (0x00u)
#define Slider_IDAC_RANGE_255uA           (0x04u)
#define Slider_IDAC_RANGE_2mA             (0x08u)

/* CR0 IDAC Control Register 0 definitions */
/* Bit Field DAC_MODE */
#define Slider_IDAC_MODE_MASK         (0x10u)
#define Slider_IDAC_MODE_V            (0x00u)
#define Slider_IDAC_MODE_I            (0x10u)

/* CR1 Idac Control Register 1 definitions */
/* Bit Field  DAC_I_DIR */
#define Slider_IDAC_IDIR_MASK         (0x04u)
#define Slider_IDAC_IDIR_SINK         (0x04u)
#define Slider_IDAC_IDIR_SRC          (0x00u)

/* Bit Field  DAC_MX_IOFF_SRC */
#define Slider_IDAC_IDIR_CTL_MASK     (0x02u)
#define Slider_IDAC_IDIR_CTL_REG      (0x00u)
#define Slider_IDAC_IDIR_CTL_UDB      (0x02u)

/* Obsolete names of variables */
//#define Slider_SensorRaw              Slider_sensorRaw
//#define Slider_SensorEnableMask       Slider_sensorEnableMask
//#define Slider_AnalogSwitchDivider    Slider_analogSwitchDivider


#endif /* CY_CAPSENSE_CSD_Slider_H */

 /* [] END OF FILE */
