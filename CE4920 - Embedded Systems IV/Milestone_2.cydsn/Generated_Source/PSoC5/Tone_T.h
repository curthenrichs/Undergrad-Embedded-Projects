/*******************************************************************************
* File Name: Tone_T.h
* Version 2.70
*
*  Description:
*     Contains the function prototypes and constants available to the timer
*     user module.
*
*   Note:
*     None
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_Timer_v2_60_Tone_T_H)
#define CY_Timer_v2_60_Tone_T_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 Tone_T_initVar;

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component Timer_v2_70 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */


/**************************************
*           Parameter Defaults
**************************************/

#define Tone_T_Resolution                 8u
#define Tone_T_UsingFixedFunction         1u
#define Tone_T_UsingHWCaptureCounter      0u
#define Tone_T_SoftwareCaptureMode        0u
#define Tone_T_SoftwareTriggerMode        0u
#define Tone_T_UsingHWEnable              0u
#define Tone_T_EnableTriggerMode          0u
#define Tone_T_InterruptOnCaptureCount    0u
#define Tone_T_RunModeUsed                0u
#define Tone_T_ControlRegRemoved          0u

#if defined(Tone_T_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG)
    #define Tone_T_UDB_CONTROL_REG_REMOVED            (0u)
#elif  (Tone_T_UsingFixedFunction)
    #define Tone_T_UDB_CONTROL_REG_REMOVED            (0u)
#else 
    #define Tone_T_UDB_CONTROL_REG_REMOVED            (1u)
#endif /* End Tone_T_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG */


/***************************************
*       Type defines
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for Timer Component
 *************************************************************************/
typedef struct
{
    uint8 TimerEnableState;
    #if(!Tone_T_UsingFixedFunction)

        uint8 TimerUdb;
        uint8 InterruptMaskValue;
        #if (Tone_T_UsingHWCaptureCounter)
            uint8 TimerCaptureCounter;
        #endif /* variable declarations for backing up non retention registers in CY_UDB_V1 */

        #if (!Tone_T_UDB_CONTROL_REG_REMOVED)
            uint8 TimerControlRegister;
        #endif /* variable declaration for backing up enable state of the Timer */
    #endif /* define backup variables only for UDB implementation. Fixed function registers are all retention */

}Tone_T_backupStruct;


/***************************************
*       Function Prototypes
***************************************/

void    Tone_T_Start(void) ;
void    Tone_T_Stop(void) ;

void    Tone_T_SetInterruptMode(uint8 interruptMode) ;
uint8   Tone_T_ReadStatusRegister(void) ;
/* Deprecated function. Do not use this in future. Retained for backward compatibility */
#define Tone_T_GetInterruptSource() Tone_T_ReadStatusRegister()

#if(!Tone_T_UDB_CONTROL_REG_REMOVED)
    uint8   Tone_T_ReadControlRegister(void) ;
    void    Tone_T_WriteControlRegister(uint8 control) ;
#endif /* (!Tone_T_UDB_CONTROL_REG_REMOVED) */

uint8  Tone_T_ReadPeriod(void) ;
void    Tone_T_WritePeriod(uint8 period) ;
uint8  Tone_T_ReadCounter(void) ;
void    Tone_T_WriteCounter(uint8 counter) ;
uint8  Tone_T_ReadCapture(void) ;
void    Tone_T_SoftwareCapture(void) ;

#if(!Tone_T_UsingFixedFunction) /* UDB Prototypes */
    #if (Tone_T_SoftwareCaptureMode)
        void    Tone_T_SetCaptureMode(uint8 captureMode) ;
    #endif /* (!Tone_T_UsingFixedFunction) */

    #if (Tone_T_SoftwareTriggerMode)
        void    Tone_T_SetTriggerMode(uint8 triggerMode) ;
    #endif /* (Tone_T_SoftwareTriggerMode) */

    #if (Tone_T_EnableTriggerMode)
        void    Tone_T_EnableTrigger(void) ;
        void    Tone_T_DisableTrigger(void) ;
    #endif /* (Tone_T_EnableTriggerMode) */


    #if(Tone_T_InterruptOnCaptureCount)
        void    Tone_T_SetInterruptCount(uint8 interruptCount) ;
    #endif /* (Tone_T_InterruptOnCaptureCount) */

    #if (Tone_T_UsingHWCaptureCounter)
        void    Tone_T_SetCaptureCount(uint8 captureCount) ;
        uint8   Tone_T_ReadCaptureCount(void) ;
    #endif /* (Tone_T_UsingHWCaptureCounter) */

    void Tone_T_ClearFIFO(void) ;
#endif /* UDB Prototypes */

/* Sleep Retention APIs */
void Tone_T_Init(void)          ;
void Tone_T_Enable(void)        ;
void Tone_T_SaveConfig(void)    ;
void Tone_T_RestoreConfig(void) ;
void Tone_T_Sleep(void)         ;
void Tone_T_Wakeup(void)        ;


/***************************************
*   Enumerated Types and Parameters
***************************************/

/* Enumerated Type B_Timer__CaptureModes, Used in Capture Mode */
#define Tone_T__B_TIMER__CM_NONE 0
#define Tone_T__B_TIMER__CM_RISINGEDGE 1
#define Tone_T__B_TIMER__CM_FALLINGEDGE 2
#define Tone_T__B_TIMER__CM_EITHEREDGE 3
#define Tone_T__B_TIMER__CM_SOFTWARE 4



/* Enumerated Type B_Timer__TriggerModes, Used in Trigger Mode */
#define Tone_T__B_TIMER__TM_NONE 0x00u
#define Tone_T__B_TIMER__TM_RISINGEDGE 0x04u
#define Tone_T__B_TIMER__TM_FALLINGEDGE 0x08u
#define Tone_T__B_TIMER__TM_EITHEREDGE 0x0Cu
#define Tone_T__B_TIMER__TM_SOFTWARE 0x10u


/***************************************
*    Initialial Parameter Constants
***************************************/

#define Tone_T_INIT_PERIOD             1u
#define Tone_T_INIT_CAPTURE_MODE       ((uint8)((uint8)0u << Tone_T_CTRL_CAP_MODE_SHIFT))
#define Tone_T_INIT_TRIGGER_MODE       ((uint8)((uint8)0u << Tone_T_CTRL_TRIG_MODE_SHIFT))
#if (Tone_T_UsingFixedFunction)
    #define Tone_T_INIT_INTERRUPT_MODE (((uint8)((uint8)1u << Tone_T_STATUS_TC_INT_MASK_SHIFT)) | \
                                                  ((uint8)((uint8)0 << Tone_T_STATUS_CAPTURE_INT_MASK_SHIFT)))
#else
    #define Tone_T_INIT_INTERRUPT_MODE (((uint8)((uint8)1u << Tone_T_STATUS_TC_INT_MASK_SHIFT)) | \
                                                 ((uint8)((uint8)0 << Tone_T_STATUS_CAPTURE_INT_MASK_SHIFT)) | \
                                                 ((uint8)((uint8)0 << Tone_T_STATUS_FIFOFULL_INT_MASK_SHIFT)))
#endif /* (Tone_T_UsingFixedFunction) */
#define Tone_T_INIT_CAPTURE_COUNT      (2u)
#define Tone_T_INIT_INT_CAPTURE_COUNT  ((uint8)((uint8)(1u - 1u) << Tone_T_CTRL_INTCNT_SHIFT))


/***************************************
*           Registers
***************************************/

#if (Tone_T_UsingFixedFunction) /* Implementation Specific Registers and Register Constants */


    /***************************************
    *    Fixed Function Registers
    ***************************************/

    #define Tone_T_STATUS         (*(reg8 *) Tone_T_TimerHW__SR0 )
    /* In Fixed Function Block Status and Mask are the same register */
    #define Tone_T_STATUS_MASK    (*(reg8 *) Tone_T_TimerHW__SR0 )
    #define Tone_T_CONTROL        (*(reg8 *) Tone_T_TimerHW__CFG0)
    #define Tone_T_CONTROL2       (*(reg8 *) Tone_T_TimerHW__CFG1)
    #define Tone_T_CONTROL2_PTR   ( (reg8 *) Tone_T_TimerHW__CFG1)
    #define Tone_T_RT1            (*(reg8 *) Tone_T_TimerHW__RT1)
    #define Tone_T_RT1_PTR        ( (reg8 *) Tone_T_TimerHW__RT1)

    #if (CY_PSOC3 || CY_PSOC5LP)
        #define Tone_T_CONTROL3       (*(reg8 *) Tone_T_TimerHW__CFG2)
        #define Tone_T_CONTROL3_PTR   ( (reg8 *) Tone_T_TimerHW__CFG2)
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */
    #define Tone_T_GLOBAL_ENABLE  (*(reg8 *) Tone_T_TimerHW__PM_ACT_CFG)
    #define Tone_T_GLOBAL_STBY_ENABLE  (*(reg8 *) Tone_T_TimerHW__PM_STBY_CFG)

    #define Tone_T_CAPTURE_LSB         (* (reg16 *) Tone_T_TimerHW__CAP0 )
    #define Tone_T_CAPTURE_LSB_PTR       ((reg16 *) Tone_T_TimerHW__CAP0 )
    #define Tone_T_PERIOD_LSB          (* (reg16 *) Tone_T_TimerHW__PER0 )
    #define Tone_T_PERIOD_LSB_PTR        ((reg16 *) Tone_T_TimerHW__PER0 )
    #define Tone_T_COUNTER_LSB         (* (reg16 *) Tone_T_TimerHW__CNT_CMP0 )
    #define Tone_T_COUNTER_LSB_PTR       ((reg16 *) Tone_T_TimerHW__CNT_CMP0 )


    /***************************************
    *    Register Constants
    ***************************************/

    /* Fixed Function Block Chosen */
    #define Tone_T_BLOCK_EN_MASK                     Tone_T_TimerHW__PM_ACT_MSK
    #define Tone_T_BLOCK_STBY_EN_MASK                Tone_T_TimerHW__PM_STBY_MSK

    /* Control Register Bit Locations */
    /* Interrupt Count - Not valid for Fixed Function Block */
    #define Tone_T_CTRL_INTCNT_SHIFT                  0x00u
    /* Trigger Polarity - Not valid for Fixed Function Block */
    #define Tone_T_CTRL_TRIG_MODE_SHIFT               0x00u
    /* Trigger Enable - Not valid for Fixed Function Block */
    #define Tone_T_CTRL_TRIG_EN_SHIFT                 0x00u
    /* Capture Polarity - Not valid for Fixed Function Block */
    #define Tone_T_CTRL_CAP_MODE_SHIFT                0x00u
    /* Timer Enable - As defined in Register Map, part of TMRX_CFG0 register */
    #define Tone_T_CTRL_ENABLE_SHIFT                  0x00u

    /* Control Register Bit Masks */
    #define Tone_T_CTRL_ENABLE                        ((uint8)((uint8)0x01u << Tone_T_CTRL_ENABLE_SHIFT))

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define Tone_T_CTRL2_IRQ_SEL_SHIFT                 0x00u
    #define Tone_T_CTRL2_IRQ_SEL                      ((uint8)((uint8)0x01u << Tone_T_CTRL2_IRQ_SEL_SHIFT))

    #if (CY_PSOC5A)
        /* Use CFG1 Mode bits to set run mode */
        /* As defined by Verilog Implementation */
        #define Tone_T_CTRL_MODE_SHIFT                 0x01u
        #define Tone_T_CTRL_MODE_MASK                 ((uint8)((uint8)0x07u << Tone_T_CTRL_MODE_SHIFT))
    #endif /* (CY_PSOC5A) */
    #if (CY_PSOC3 || CY_PSOC5LP)
        /* Control3 Register Bit Locations */
        #define Tone_T_CTRL_RCOD_SHIFT        0x02u
        #define Tone_T_CTRL_ENBL_SHIFT        0x00u
        #define Tone_T_CTRL_MODE_SHIFT        0x00u

        /* Control3 Register Bit Masks */
        #define Tone_T_CTRL_RCOD_MASK  ((uint8)((uint8)0x03u << Tone_T_CTRL_RCOD_SHIFT)) /* ROD and COD bit masks */
        #define Tone_T_CTRL_ENBL_MASK  ((uint8)((uint8)0x80u << Tone_T_CTRL_ENBL_SHIFT)) /* HW_EN bit mask */
        #define Tone_T_CTRL_MODE_MASK  ((uint8)((uint8)0x03u << Tone_T_CTRL_MODE_SHIFT)) /* Run mode bit mask */

        #define Tone_T_CTRL_RCOD       ((uint8)((uint8)0x03u << Tone_T_CTRL_RCOD_SHIFT))
        #define Tone_T_CTRL_ENBL       ((uint8)((uint8)0x80u << Tone_T_CTRL_ENBL_SHIFT))
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */

    /*RT1 Synch Constants: Applicable for PSoC3 and PSoC5LP */
    #define Tone_T_RT1_SHIFT                       0x04u
    /* Sync TC and CMP bit masks */
    #define Tone_T_RT1_MASK                        ((uint8)((uint8)0x03u << Tone_T_RT1_SHIFT))
    #define Tone_T_SYNC                            ((uint8)((uint8)0x03u << Tone_T_RT1_SHIFT))
    #define Tone_T_SYNCDSI_SHIFT                   0x00u
    /* Sync all DSI inputs with Mask  */
    #define Tone_T_SYNCDSI_MASK                    ((uint8)((uint8)0x0Fu << Tone_T_SYNCDSI_SHIFT))
    /* Sync all DSI inputs */
    #define Tone_T_SYNCDSI_EN                      ((uint8)((uint8)0x0Fu << Tone_T_SYNCDSI_SHIFT))

    #define Tone_T_CTRL_MODE_PULSEWIDTH            ((uint8)((uint8)0x01u << Tone_T_CTRL_MODE_SHIFT))
    #define Tone_T_CTRL_MODE_PERIOD                ((uint8)((uint8)0x02u << Tone_T_CTRL_MODE_SHIFT))
    #define Tone_T_CTRL_MODE_CONTINUOUS            ((uint8)((uint8)0x00u << Tone_T_CTRL_MODE_SHIFT))

    /* Status Register Bit Locations */
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define Tone_T_STATUS_TC_SHIFT                 0x07u
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define Tone_T_STATUS_CAPTURE_SHIFT            0x06u
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define Tone_T_STATUS_TC_INT_MASK_SHIFT        (Tone_T_STATUS_TC_SHIFT - 0x04u)
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define Tone_T_STATUS_CAPTURE_INT_MASK_SHIFT   (Tone_T_STATUS_CAPTURE_SHIFT - 0x04u)

    /* Status Register Bit Masks */
    #define Tone_T_STATUS_TC                       ((uint8)((uint8)0x01u << Tone_T_STATUS_TC_SHIFT))
    #define Tone_T_STATUS_CAPTURE                  ((uint8)((uint8)0x01u << Tone_T_STATUS_CAPTURE_SHIFT))
    /* Interrupt Enable Bit-Mask for interrupt on TC */
    #define Tone_T_STATUS_TC_INT_MASK              ((uint8)((uint8)0x01u << Tone_T_STATUS_TC_INT_MASK_SHIFT))
    /* Interrupt Enable Bit-Mask for interrupt on Capture */
    #define Tone_T_STATUS_CAPTURE_INT_MASK         ((uint8)((uint8)0x01u << Tone_T_STATUS_CAPTURE_INT_MASK_SHIFT))

#else   /* UDB Registers and Register Constants */


    /***************************************
    *           UDB Registers
    ***************************************/

    #define Tone_T_STATUS              (* (reg8 *) Tone_T_TimerUDB_rstSts_stsreg__STATUS_REG )
    #define Tone_T_STATUS_MASK         (* (reg8 *) Tone_T_TimerUDB_rstSts_stsreg__MASK_REG)
    #define Tone_T_STATUS_AUX_CTRL     (* (reg8 *) Tone_T_TimerUDB_rstSts_stsreg__STATUS_AUX_CTL_REG)
    #define Tone_T_CONTROL             (* (reg8 *) Tone_T_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG )
    
    #if(Tone_T_Resolution <= 8u) /* 8-bit Timer */
        #define Tone_T_CAPTURE_LSB         (* (reg8 *) Tone_T_TimerUDB_sT8_timerdp_u0__F0_REG )
        #define Tone_T_CAPTURE_LSB_PTR       ((reg8 *) Tone_T_TimerUDB_sT8_timerdp_u0__F0_REG )
        #define Tone_T_PERIOD_LSB          (* (reg8 *) Tone_T_TimerUDB_sT8_timerdp_u0__D0_REG )
        #define Tone_T_PERIOD_LSB_PTR        ((reg8 *) Tone_T_TimerUDB_sT8_timerdp_u0__D0_REG )
        #define Tone_T_COUNTER_LSB         (* (reg8 *) Tone_T_TimerUDB_sT8_timerdp_u0__A0_REG )
        #define Tone_T_COUNTER_LSB_PTR       ((reg8 *) Tone_T_TimerUDB_sT8_timerdp_u0__A0_REG )
    #elif(Tone_T_Resolution <= 16u) /* 8-bit Timer */
        #if(CY_PSOC3) /* 8-bit addres space */
            #define Tone_T_CAPTURE_LSB         (* (reg16 *) Tone_T_TimerUDB_sT8_timerdp_u0__F0_REG )
            #define Tone_T_CAPTURE_LSB_PTR       ((reg16 *) Tone_T_TimerUDB_sT8_timerdp_u0__F0_REG )
            #define Tone_T_PERIOD_LSB          (* (reg16 *) Tone_T_TimerUDB_sT8_timerdp_u0__D0_REG )
            #define Tone_T_PERIOD_LSB_PTR        ((reg16 *) Tone_T_TimerUDB_sT8_timerdp_u0__D0_REG )
            #define Tone_T_COUNTER_LSB         (* (reg16 *) Tone_T_TimerUDB_sT8_timerdp_u0__A0_REG )
            #define Tone_T_COUNTER_LSB_PTR       ((reg16 *) Tone_T_TimerUDB_sT8_timerdp_u0__A0_REG )
        #else /* 16-bit address space */
            #define Tone_T_CAPTURE_LSB         (* (reg16 *) Tone_T_TimerUDB_sT8_timerdp_u0__16BIT_F0_REG )
            #define Tone_T_CAPTURE_LSB_PTR       ((reg16 *) Tone_T_TimerUDB_sT8_timerdp_u0__16BIT_F0_REG )
            #define Tone_T_PERIOD_LSB          (* (reg16 *) Tone_T_TimerUDB_sT8_timerdp_u0__16BIT_D0_REG )
            #define Tone_T_PERIOD_LSB_PTR        ((reg16 *) Tone_T_TimerUDB_sT8_timerdp_u0__16BIT_D0_REG )
            #define Tone_T_COUNTER_LSB         (* (reg16 *) Tone_T_TimerUDB_sT8_timerdp_u0__16BIT_A0_REG )
            #define Tone_T_COUNTER_LSB_PTR       ((reg16 *) Tone_T_TimerUDB_sT8_timerdp_u0__16BIT_A0_REG )
        #endif /* CY_PSOC3 */
    #elif(Tone_T_Resolution <= 24u)/* 24-bit Timer */
        #define Tone_T_CAPTURE_LSB         (* (reg32 *) Tone_T_TimerUDB_sT8_timerdp_u0__F0_REG )
        #define Tone_T_CAPTURE_LSB_PTR       ((reg32 *) Tone_T_TimerUDB_sT8_timerdp_u0__F0_REG )
        #define Tone_T_PERIOD_LSB          (* (reg32 *) Tone_T_TimerUDB_sT8_timerdp_u0__D0_REG )
        #define Tone_T_PERIOD_LSB_PTR        ((reg32 *) Tone_T_TimerUDB_sT8_timerdp_u0__D0_REG )
        #define Tone_T_COUNTER_LSB         (* (reg32 *) Tone_T_TimerUDB_sT8_timerdp_u0__A0_REG )
        #define Tone_T_COUNTER_LSB_PTR       ((reg32 *) Tone_T_TimerUDB_sT8_timerdp_u0__A0_REG )
    #else /* 32-bit Timer */
        #if(CY_PSOC3 || CY_PSOC5) /* 8-bit address space */
            #define Tone_T_CAPTURE_LSB         (* (reg32 *) Tone_T_TimerUDB_sT8_timerdp_u0__F0_REG )
            #define Tone_T_CAPTURE_LSB_PTR       ((reg32 *) Tone_T_TimerUDB_sT8_timerdp_u0__F0_REG )
            #define Tone_T_PERIOD_LSB          (* (reg32 *) Tone_T_TimerUDB_sT8_timerdp_u0__D0_REG )
            #define Tone_T_PERIOD_LSB_PTR        ((reg32 *) Tone_T_TimerUDB_sT8_timerdp_u0__D0_REG )
            #define Tone_T_COUNTER_LSB         (* (reg32 *) Tone_T_TimerUDB_sT8_timerdp_u0__A0_REG )
            #define Tone_T_COUNTER_LSB_PTR       ((reg32 *) Tone_T_TimerUDB_sT8_timerdp_u0__A0_REG )
        #else /* 32-bit address space */
            #define Tone_T_CAPTURE_LSB         (* (reg32 *) Tone_T_TimerUDB_sT8_timerdp_u0__32BIT_F0_REG )
            #define Tone_T_CAPTURE_LSB_PTR       ((reg32 *) Tone_T_TimerUDB_sT8_timerdp_u0__32BIT_F0_REG )
            #define Tone_T_PERIOD_LSB          (* (reg32 *) Tone_T_TimerUDB_sT8_timerdp_u0__32BIT_D0_REG )
            #define Tone_T_PERIOD_LSB_PTR        ((reg32 *) Tone_T_TimerUDB_sT8_timerdp_u0__32BIT_D0_REG )
            #define Tone_T_COUNTER_LSB         (* (reg32 *) Tone_T_TimerUDB_sT8_timerdp_u0__32BIT_A0_REG )
            #define Tone_T_COUNTER_LSB_PTR       ((reg32 *) Tone_T_TimerUDB_sT8_timerdp_u0__32BIT_A0_REG )
        #endif /* CY_PSOC3 || CY_PSOC5 */ 
    #endif

    #define Tone_T_COUNTER_LSB_PTR_8BIT       ((reg8 *) Tone_T_TimerUDB_sT8_timerdp_u0__A0_REG )
    
    #if (Tone_T_UsingHWCaptureCounter)
        #define Tone_T_CAP_COUNT              (*(reg8 *) Tone_T_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define Tone_T_CAP_COUNT_PTR          ( (reg8 *) Tone_T_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define Tone_T_CAPTURE_COUNT_CTRL     (*(reg8 *) Tone_T_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
        #define Tone_T_CAPTURE_COUNT_CTRL_PTR ( (reg8 *) Tone_T_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
    #endif /* (Tone_T_UsingHWCaptureCounter) */


    /***************************************
    *       Register Constants
    ***************************************/

    /* Control Register Bit Locations */
    #define Tone_T_CTRL_INTCNT_SHIFT              0x00u       /* As defined by Verilog Implementation */
    #define Tone_T_CTRL_TRIG_MODE_SHIFT           0x02u       /* As defined by Verilog Implementation */
    #define Tone_T_CTRL_TRIG_EN_SHIFT             0x04u       /* As defined by Verilog Implementation */
    #define Tone_T_CTRL_CAP_MODE_SHIFT            0x05u       /* As defined by Verilog Implementation */
    #define Tone_T_CTRL_ENABLE_SHIFT              0x07u       /* As defined by Verilog Implementation */

    /* Control Register Bit Masks */
    #define Tone_T_CTRL_INTCNT_MASK               ((uint8)((uint8)0x03u << Tone_T_CTRL_INTCNT_SHIFT))
    #define Tone_T_CTRL_TRIG_MODE_MASK            ((uint8)((uint8)0x03u << Tone_T_CTRL_TRIG_MODE_SHIFT))
    #define Tone_T_CTRL_TRIG_EN                   ((uint8)((uint8)0x01u << Tone_T_CTRL_TRIG_EN_SHIFT))
    #define Tone_T_CTRL_CAP_MODE_MASK             ((uint8)((uint8)0x03u << Tone_T_CTRL_CAP_MODE_SHIFT))
    #define Tone_T_CTRL_ENABLE                    ((uint8)((uint8)0x01u << Tone_T_CTRL_ENABLE_SHIFT))

    /* Bit Counter (7-bit) Control Register Bit Definitions */
    /* As defined by the Register map for the AUX Control Register */
    #define Tone_T_CNTR_ENABLE                    0x20u

    /* Status Register Bit Locations */
    #define Tone_T_STATUS_TC_SHIFT                0x00u  /* As defined by Verilog Implementation */
    #define Tone_T_STATUS_CAPTURE_SHIFT           0x01u  /* As defined by Verilog Implementation */
    #define Tone_T_STATUS_TC_INT_MASK_SHIFT       Tone_T_STATUS_TC_SHIFT
    #define Tone_T_STATUS_CAPTURE_INT_MASK_SHIFT  Tone_T_STATUS_CAPTURE_SHIFT
    #define Tone_T_STATUS_FIFOFULL_SHIFT          0x02u  /* As defined by Verilog Implementation */
    #define Tone_T_STATUS_FIFONEMP_SHIFT          0x03u  /* As defined by Verilog Implementation */
    #define Tone_T_STATUS_FIFOFULL_INT_MASK_SHIFT Tone_T_STATUS_FIFOFULL_SHIFT

    /* Status Register Bit Masks */
    /* Sticky TC Event Bit-Mask */
    #define Tone_T_STATUS_TC                      ((uint8)((uint8)0x01u << Tone_T_STATUS_TC_SHIFT))
    /* Sticky Capture Event Bit-Mask */
    #define Tone_T_STATUS_CAPTURE                 ((uint8)((uint8)0x01u << Tone_T_STATUS_CAPTURE_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define Tone_T_STATUS_TC_INT_MASK             ((uint8)((uint8)0x01u << Tone_T_STATUS_TC_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define Tone_T_STATUS_CAPTURE_INT_MASK        ((uint8)((uint8)0x01u << Tone_T_STATUS_CAPTURE_SHIFT))
    /* NOT-Sticky FIFO Full Bit-Mask */
    #define Tone_T_STATUS_FIFOFULL                ((uint8)((uint8)0x01u << Tone_T_STATUS_FIFOFULL_SHIFT))
    /* NOT-Sticky FIFO Not Empty Bit-Mask */
    #define Tone_T_STATUS_FIFONEMP                ((uint8)((uint8)0x01u << Tone_T_STATUS_FIFONEMP_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define Tone_T_STATUS_FIFOFULL_INT_MASK       ((uint8)((uint8)0x01u << Tone_T_STATUS_FIFOFULL_SHIFT))

    #define Tone_T_STATUS_ACTL_INT_EN             0x10u   /* As defined for the ACTL Register */

    /* Datapath Auxillary Control Register definitions */
    #define Tone_T_AUX_CTRL_FIFO0_CLR             0x01u   /* As defined by Register map */
    #define Tone_T_AUX_CTRL_FIFO1_CLR             0x02u   /* As defined by Register map */
    #define Tone_T_AUX_CTRL_FIFO0_LVL             0x04u   /* As defined by Register map */
    #define Tone_T_AUX_CTRL_FIFO1_LVL             0x08u   /* As defined by Register map */
    #define Tone_T_STATUS_ACTL_INT_EN_MASK        0x10u   /* As defined for the ACTL Register */

#endif /* Implementation Specific Registers and Register Constants */

#endif  /* CY_Timer_v2_30_Tone_T_H */


/* [] END OF FILE */
