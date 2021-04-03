/*******************************************************************************
* File Name: Tone_F.h
* Version 3.30
*
* Description:
*  Contains the prototypes and constants for the functions available to the
*  PWM user module.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_PWM_Tone_F_H)
#define CY_PWM_Tone_F_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 Tone_F_initVar;


/***************************************
* Conditional Compilation Parameters
***************************************/
#define Tone_F_Resolution                     (16u)
#define Tone_F_UsingFixedFunction             (0u)
#define Tone_F_DeadBandMode                   (0u)
#define Tone_F_KillModeMinTime                (0u)
#define Tone_F_KillMode                       (0u)
#define Tone_F_PWMMode                        (0u)
#define Tone_F_PWMModeIsCenterAligned         (0u)
#define Tone_F_DeadBandUsed                   (0u)
#define Tone_F_DeadBand2_4                    (0u)

#if !defined(Tone_F_PWMUDB_genblk8_stsreg__REMOVED)
    #define Tone_F_UseStatus                  (1u)
#else
    #define Tone_F_UseStatus                  (0u)
#endif /* !defined(Tone_F_PWMUDB_genblk8_stsreg__REMOVED) */

#if !defined(Tone_F_PWMUDB_genblk1_ctrlreg__REMOVED)
    #define Tone_F_UseControl                 (1u)
#else
    #define Tone_F_UseControl                 (0u)
#endif /* !defined(Tone_F_PWMUDB_genblk1_ctrlreg__REMOVED) */

#define Tone_F_UseOneCompareMode              (1u)
#define Tone_F_MinimumKillTime                (1u)
#define Tone_F_EnableMode                     (0u)

#define Tone_F_CompareMode1SW                 (0u)
#define Tone_F_CompareMode2SW                 (0u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component PWM_v3_30 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */

/* Use Kill Mode Enumerated Types */
#define Tone_F__B_PWM__DISABLED 0
#define Tone_F__B_PWM__ASYNCHRONOUS 1
#define Tone_F__B_PWM__SINGLECYCLE 2
#define Tone_F__B_PWM__LATCHED 3
#define Tone_F__B_PWM__MINTIME 4


/* Use Dead Band Mode Enumerated Types */
#define Tone_F__B_PWM__DBMDISABLED 0
#define Tone_F__B_PWM__DBM_2_4_CLOCKS 1
#define Tone_F__B_PWM__DBM_256_CLOCKS 2


/* Used PWM Mode Enumerated Types */
#define Tone_F__B_PWM__ONE_OUTPUT 0
#define Tone_F__B_PWM__TWO_OUTPUTS 1
#define Tone_F__B_PWM__DUAL_EDGE 2
#define Tone_F__B_PWM__CENTER_ALIGN 3
#define Tone_F__B_PWM__DITHER 5
#define Tone_F__B_PWM__HARDWARESELECT 4


/* Used PWM Compare Mode Enumerated Types */
#define Tone_F__B_PWM__LESS_THAN 1
#define Tone_F__B_PWM__LESS_THAN_OR_EQUAL 2
#define Tone_F__B_PWM__GREATER_THAN 3
#define Tone_F__B_PWM__GREATER_THAN_OR_EQUAL_TO 4
#define Tone_F__B_PWM__EQUAL 0
#define Tone_F__B_PWM__FIRMWARE 5



/***************************************
* Data Struct Definition
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for PWM Component
 *************************************************************************/
typedef struct
{

    uint8 PWMEnableState;

    #if(!Tone_F_UsingFixedFunction)
        uint16 PWMUdb;               /* PWM Current Counter value  */
        #if(!Tone_F_PWMModeIsCenterAligned)
            uint16 PWMPeriod;
        #endif /* (!Tone_F_PWMModeIsCenterAligned) */
        #if (Tone_F_UseStatus)
            uint8 InterruptMaskValue;   /* PWM Current Interrupt Mask */
        #endif /* (Tone_F_UseStatus) */

        /* Backup for Deadband parameters */
        #if(Tone_F_DeadBandMode == Tone_F__B_PWM__DBM_256_CLOCKS || \
            Tone_F_DeadBandMode == Tone_F__B_PWM__DBM_2_4_CLOCKS)
            uint8 PWMdeadBandValue; /* Dead Band Counter Current Value */
        #endif /* deadband count is either 2-4 clocks or 256 clocks */

        /* Backup Kill Mode Counter*/
        #if(Tone_F_KillModeMinTime)
            uint8 PWMKillCounterPeriod; /* Kill Mode period value */
        #endif /* (Tone_F_KillModeMinTime) */

        /* Backup control register */
        #if(Tone_F_UseControl)
            uint8 PWMControlRegister; /* PWM Control Register value */
        #endif /* (Tone_F_UseControl) */

    #endif /* (!Tone_F_UsingFixedFunction) */

}Tone_F_backupStruct;


/***************************************
*        Function Prototypes
 **************************************/

void    Tone_F_Start(void) ;
void    Tone_F_Stop(void) ;

#if (Tone_F_UseStatus || Tone_F_UsingFixedFunction)
    void  Tone_F_SetInterruptMode(uint8 interruptMode) ;
    uint8 Tone_F_ReadStatusRegister(void) ;
#endif /* (Tone_F_UseStatus || Tone_F_UsingFixedFunction) */

#define Tone_F_GetInterruptSource() Tone_F_ReadStatusRegister()

#if (Tone_F_UseControl)
    uint8 Tone_F_ReadControlRegister(void) ;
    void  Tone_F_WriteControlRegister(uint8 control)
          ;
#endif /* (Tone_F_UseControl) */

#if (Tone_F_UseOneCompareMode)
   #if (Tone_F_CompareMode1SW)
       void    Tone_F_SetCompareMode(uint8 comparemode)
               ;
   #endif /* (Tone_F_CompareMode1SW) */
#else
    #if (Tone_F_CompareMode1SW)
        void    Tone_F_SetCompareMode1(uint8 comparemode)
                ;
    #endif /* (Tone_F_CompareMode1SW) */
    #if (Tone_F_CompareMode2SW)
        void    Tone_F_SetCompareMode2(uint8 comparemode)
                ;
    #endif /* (Tone_F_CompareMode2SW) */
#endif /* (Tone_F_UseOneCompareMode) */

#if (!Tone_F_UsingFixedFunction)
    uint16   Tone_F_ReadCounter(void) ;
    uint16 Tone_F_ReadCapture(void) ;

    #if (Tone_F_UseStatus)
            void Tone_F_ClearFIFO(void) ;
    #endif /* (Tone_F_UseStatus) */

    void    Tone_F_WriteCounter(uint16 counter)
            ;
#endif /* (!Tone_F_UsingFixedFunction) */

void    Tone_F_WritePeriod(uint16 period)
        ;
uint16 Tone_F_ReadPeriod(void) ;

#if (Tone_F_UseOneCompareMode)
    void    Tone_F_WriteCompare(uint16 compare)
            ;
    uint16 Tone_F_ReadCompare(void) ;
#else
    void    Tone_F_WriteCompare1(uint16 compare)
            ;
    uint16 Tone_F_ReadCompare1(void) ;
    void    Tone_F_WriteCompare2(uint16 compare)
            ;
    uint16 Tone_F_ReadCompare2(void) ;
#endif /* (Tone_F_UseOneCompareMode) */


#if (Tone_F_DeadBandUsed)
    void    Tone_F_WriteDeadTime(uint8 deadtime) ;
    uint8   Tone_F_ReadDeadTime(void) ;
#endif /* (Tone_F_DeadBandUsed) */

#if ( Tone_F_KillModeMinTime)
    void Tone_F_WriteKillTime(uint8 killtime) ;
    uint8 Tone_F_ReadKillTime(void) ;
#endif /* ( Tone_F_KillModeMinTime) */

void Tone_F_Init(void) ;
void Tone_F_Enable(void) ;
void Tone_F_Sleep(void) ;
void Tone_F_Wakeup(void) ;
void Tone_F_SaveConfig(void) ;
void Tone_F_RestoreConfig(void) ;


/***************************************
*         Initialization Values
**************************************/
#define Tone_F_INIT_PERIOD_VALUE          (32768u)
#define Tone_F_INIT_COMPARE_VALUE1        (0u)
#define Tone_F_INIT_COMPARE_VALUE2        (63u)
#define Tone_F_INIT_INTERRUPTS_MODE       (uint8)(((uint8)(0u <<   \
                                                    Tone_F_STATUS_TC_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    Tone_F_STATUS_CMP2_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    Tone_F_STATUS_CMP1_INT_EN_MASK_SHIFT )) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    Tone_F_STATUS_KILL_INT_EN_MASK_SHIFT )))
#define Tone_F_DEFAULT_COMPARE2_MODE      (uint8)((uint8)1u <<  Tone_F_CTRL_CMPMODE2_SHIFT)
#define Tone_F_DEFAULT_COMPARE1_MODE      (uint8)((uint8)1u <<  Tone_F_CTRL_CMPMODE1_SHIFT)
#define Tone_F_INIT_DEAD_TIME             (1u)


/********************************
*         Registers
******************************** */

#if (Tone_F_UsingFixedFunction)
   #define Tone_F_PERIOD_LSB              (*(reg16 *) Tone_F_PWMHW__PER0)
   #define Tone_F_PERIOD_LSB_PTR          ( (reg16 *) Tone_F_PWMHW__PER0)
   #define Tone_F_COMPARE1_LSB            (*(reg16 *) Tone_F_PWMHW__CNT_CMP0)
   #define Tone_F_COMPARE1_LSB_PTR        ( (reg16 *) Tone_F_PWMHW__CNT_CMP0)
   #define Tone_F_COMPARE2_LSB            (0x00u)
   #define Tone_F_COMPARE2_LSB_PTR        (0x00u)
   #define Tone_F_COUNTER_LSB             (*(reg16 *) Tone_F_PWMHW__CNT_CMP0)
   #define Tone_F_COUNTER_LSB_PTR         ( (reg16 *) Tone_F_PWMHW__CNT_CMP0)
   #define Tone_F_CAPTURE_LSB             (*(reg16 *) Tone_F_PWMHW__CAP0)
   #define Tone_F_CAPTURE_LSB_PTR         ( (reg16 *) Tone_F_PWMHW__CAP0)
   #define Tone_F_RT1                     (*(reg8 *)  Tone_F_PWMHW__RT1)
   #define Tone_F_RT1_PTR                 ( (reg8 *)  Tone_F_PWMHW__RT1)

#else
   #if (Tone_F_Resolution == 8u) /* 8bit - PWM */

       #if(Tone_F_PWMModeIsCenterAligned)
           #define Tone_F_PERIOD_LSB      (*(reg8 *)  Tone_F_PWMUDB_sP16_pwmdp_u0__D1_REG)
           #define Tone_F_PERIOD_LSB_PTR  ((reg8 *)   Tone_F_PWMUDB_sP16_pwmdp_u0__D1_REG)
       #else
           #define Tone_F_PERIOD_LSB      (*(reg8 *)  Tone_F_PWMUDB_sP16_pwmdp_u0__F0_REG)
           #define Tone_F_PERIOD_LSB_PTR  ((reg8 *)   Tone_F_PWMUDB_sP16_pwmdp_u0__F0_REG)
       #endif /* (Tone_F_PWMModeIsCenterAligned) */

       #define Tone_F_COMPARE1_LSB        (*(reg8 *)  Tone_F_PWMUDB_sP16_pwmdp_u0__D0_REG)
       #define Tone_F_COMPARE1_LSB_PTR    ((reg8 *)   Tone_F_PWMUDB_sP16_pwmdp_u0__D0_REG)
       #define Tone_F_COMPARE2_LSB        (*(reg8 *)  Tone_F_PWMUDB_sP16_pwmdp_u0__D1_REG)
       #define Tone_F_COMPARE2_LSB_PTR    ((reg8 *)   Tone_F_PWMUDB_sP16_pwmdp_u0__D1_REG)
       #define Tone_F_COUNTERCAP_LSB      (*(reg8 *)  Tone_F_PWMUDB_sP16_pwmdp_u0__A1_REG)
       #define Tone_F_COUNTERCAP_LSB_PTR  ((reg8 *)   Tone_F_PWMUDB_sP16_pwmdp_u0__A1_REG)
       #define Tone_F_COUNTER_LSB         (*(reg8 *)  Tone_F_PWMUDB_sP16_pwmdp_u0__A0_REG)
       #define Tone_F_COUNTER_LSB_PTR     ((reg8 *)   Tone_F_PWMUDB_sP16_pwmdp_u0__A0_REG)
       #define Tone_F_CAPTURE_LSB         (*(reg8 *)  Tone_F_PWMUDB_sP16_pwmdp_u0__F1_REG)
       #define Tone_F_CAPTURE_LSB_PTR     ((reg8 *)   Tone_F_PWMUDB_sP16_pwmdp_u0__F1_REG)

   #else
        #if(CY_PSOC3) /* 8-bit address space */
            #if(Tone_F_PWMModeIsCenterAligned)
               #define Tone_F_PERIOD_LSB      (*(reg16 *) Tone_F_PWMUDB_sP16_pwmdp_u0__D1_REG)
               #define Tone_F_PERIOD_LSB_PTR  ((reg16 *)  Tone_F_PWMUDB_sP16_pwmdp_u0__D1_REG)
            #else
               #define Tone_F_PERIOD_LSB      (*(reg16 *) Tone_F_PWMUDB_sP16_pwmdp_u0__F0_REG)
               #define Tone_F_PERIOD_LSB_PTR  ((reg16 *)  Tone_F_PWMUDB_sP16_pwmdp_u0__F0_REG)
            #endif /* (Tone_F_PWMModeIsCenterAligned) */

            #define Tone_F_COMPARE1_LSB       (*(reg16 *) Tone_F_PWMUDB_sP16_pwmdp_u0__D0_REG)
            #define Tone_F_COMPARE1_LSB_PTR   ((reg16 *)  Tone_F_PWMUDB_sP16_pwmdp_u0__D0_REG)
            #define Tone_F_COMPARE2_LSB       (*(reg16 *) Tone_F_PWMUDB_sP16_pwmdp_u0__D1_REG)
            #define Tone_F_COMPARE2_LSB_PTR   ((reg16 *)  Tone_F_PWMUDB_sP16_pwmdp_u0__D1_REG)
            #define Tone_F_COUNTERCAP_LSB     (*(reg16 *) Tone_F_PWMUDB_sP16_pwmdp_u0__A1_REG)
            #define Tone_F_COUNTERCAP_LSB_PTR ((reg16 *)  Tone_F_PWMUDB_sP16_pwmdp_u0__A1_REG)
            #define Tone_F_COUNTER_LSB        (*(reg16 *) Tone_F_PWMUDB_sP16_pwmdp_u0__A0_REG)
            #define Tone_F_COUNTER_LSB_PTR    ((reg16 *)  Tone_F_PWMUDB_sP16_pwmdp_u0__A0_REG)
            #define Tone_F_CAPTURE_LSB        (*(reg16 *) Tone_F_PWMUDB_sP16_pwmdp_u0__F1_REG)
            #define Tone_F_CAPTURE_LSB_PTR    ((reg16 *)  Tone_F_PWMUDB_sP16_pwmdp_u0__F1_REG)
        #else
            #if(Tone_F_PWMModeIsCenterAligned)
               #define Tone_F_PERIOD_LSB      (*(reg16 *) Tone_F_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
               #define Tone_F_PERIOD_LSB_PTR  ((reg16 *)  Tone_F_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
            #else
               #define Tone_F_PERIOD_LSB      (*(reg16 *) Tone_F_PWMUDB_sP16_pwmdp_u0__16BIT_F0_REG)
               #define Tone_F_PERIOD_LSB_PTR  ((reg16 *)  Tone_F_PWMUDB_sP16_pwmdp_u0__16BIT_F0_REG)
            #endif /* (Tone_F_PWMModeIsCenterAligned) */

            #define Tone_F_COMPARE1_LSB       (*(reg16 *) Tone_F_PWMUDB_sP16_pwmdp_u0__16BIT_D0_REG)
            #define Tone_F_COMPARE1_LSB_PTR   ((reg16 *)  Tone_F_PWMUDB_sP16_pwmdp_u0__16BIT_D0_REG)
            #define Tone_F_COMPARE2_LSB       (*(reg16 *) Tone_F_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
            #define Tone_F_COMPARE2_LSB_PTR   ((reg16 *)  Tone_F_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
            #define Tone_F_COUNTERCAP_LSB     (*(reg16 *) Tone_F_PWMUDB_sP16_pwmdp_u0__16BIT_A1_REG)
            #define Tone_F_COUNTERCAP_LSB_PTR ((reg16 *)  Tone_F_PWMUDB_sP16_pwmdp_u0__16BIT_A1_REG)
            #define Tone_F_COUNTER_LSB        (*(reg16 *) Tone_F_PWMUDB_sP16_pwmdp_u0__16BIT_A0_REG)
            #define Tone_F_COUNTER_LSB_PTR    ((reg16 *)  Tone_F_PWMUDB_sP16_pwmdp_u0__16BIT_A0_REG)
            #define Tone_F_CAPTURE_LSB        (*(reg16 *) Tone_F_PWMUDB_sP16_pwmdp_u0__16BIT_F1_REG)
            #define Tone_F_CAPTURE_LSB_PTR    ((reg16 *)  Tone_F_PWMUDB_sP16_pwmdp_u0__16BIT_F1_REG)
        #endif /* (CY_PSOC3) */

       #define Tone_F_AUX_CONTROLDP1          (*(reg8 *)  Tone_F_PWMUDB_sP16_pwmdp_u1__DP_AUX_CTL_REG)
       #define Tone_F_AUX_CONTROLDP1_PTR      ((reg8 *)   Tone_F_PWMUDB_sP16_pwmdp_u1__DP_AUX_CTL_REG)

   #endif /* (Tone_F_Resolution == 8) */

   #define Tone_F_COUNTERCAP_LSB_PTR_8BIT ( (reg8 *)  Tone_F_PWMUDB_sP16_pwmdp_u0__A1_REG)
   #define Tone_F_AUX_CONTROLDP0          (*(reg8 *)  Tone_F_PWMUDB_sP16_pwmdp_u0__DP_AUX_CTL_REG)
   #define Tone_F_AUX_CONTROLDP0_PTR      ((reg8 *)   Tone_F_PWMUDB_sP16_pwmdp_u0__DP_AUX_CTL_REG)

#endif /* (Tone_F_UsingFixedFunction) */

#if(Tone_F_KillModeMinTime )
    #define Tone_F_KILLMODEMINTIME        (*(reg8 *)  Tone_F_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    #define Tone_F_KILLMODEMINTIME_PTR    ((reg8 *)   Tone_F_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    /* Fixed Function Block has no Kill Mode parameters because it is Asynchronous only */
#endif /* (Tone_F_KillModeMinTime ) */

#if(Tone_F_DeadBandMode == Tone_F__B_PWM__DBM_256_CLOCKS)
    #define Tone_F_DEADBAND_COUNT         (*(reg8 *)  Tone_F_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define Tone_F_DEADBAND_COUNT_PTR     ((reg8 *)   Tone_F_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define Tone_F_DEADBAND_LSB_PTR       ((reg8 *)   Tone_F_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
    #define Tone_F_DEADBAND_LSB           (*(reg8 *)  Tone_F_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
#elif(Tone_F_DeadBandMode == Tone_F__B_PWM__DBM_2_4_CLOCKS)
    
    /* In Fixed Function Block these bits are in the control blocks control register */
    #if (Tone_F_UsingFixedFunction)
        #define Tone_F_DEADBAND_COUNT         (*(reg8 *)  Tone_F_PWMHW__CFG0)
        #define Tone_F_DEADBAND_COUNT_PTR     ((reg8 *)   Tone_F_PWMHW__CFG0)
        #define Tone_F_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << Tone_F_DEADBAND_COUNT_SHIFT)

        /* As defined by the Register Map as DEADBAND_PERIOD[1:0] in CFG0 */
        #define Tone_F_DEADBAND_COUNT_SHIFT   (0x06u)
    #else
        /* Lower two bits of the added control register define the count 1-3 */
        #define Tone_F_DEADBAND_COUNT         (*(reg8 *)  Tone_F_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define Tone_F_DEADBAND_COUNT_PTR     ((reg8 *)   Tone_F_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define Tone_F_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << Tone_F_DEADBAND_COUNT_SHIFT)

        /* As defined by the verilog implementation of the Control Register */
        #define Tone_F_DEADBAND_COUNT_SHIFT   (0x00u)
    #endif /* (Tone_F_UsingFixedFunction) */
#endif /* (Tone_F_DeadBandMode == Tone_F__B_PWM__DBM_256_CLOCKS) */



#if (Tone_F_UsingFixedFunction)
    #define Tone_F_STATUS                 (*(reg8 *) Tone_F_PWMHW__SR0)
    #define Tone_F_STATUS_PTR             ((reg8 *) Tone_F_PWMHW__SR0)
    #define Tone_F_STATUS_MASK            (*(reg8 *) Tone_F_PWMHW__SR0)
    #define Tone_F_STATUS_MASK_PTR        ((reg8 *) Tone_F_PWMHW__SR0)
    #define Tone_F_CONTROL                (*(reg8 *) Tone_F_PWMHW__CFG0)
    #define Tone_F_CONTROL_PTR            ((reg8 *) Tone_F_PWMHW__CFG0)
    #define Tone_F_CONTROL2               (*(reg8 *) Tone_F_PWMHW__CFG1)
    #define Tone_F_CONTROL3               (*(reg8 *) Tone_F_PWMHW__CFG2)
    #define Tone_F_GLOBAL_ENABLE          (*(reg8 *) Tone_F_PWMHW__PM_ACT_CFG)
    #define Tone_F_GLOBAL_ENABLE_PTR      ( (reg8 *) Tone_F_PWMHW__PM_ACT_CFG)
    #define Tone_F_GLOBAL_STBY_ENABLE     (*(reg8 *) Tone_F_PWMHW__PM_STBY_CFG)
    #define Tone_F_GLOBAL_STBY_ENABLE_PTR ( (reg8 *) Tone_F_PWMHW__PM_STBY_CFG)


    /***********************************
    *          Constants
    ***********************************/

    /* Fixed Function Block Chosen */
    #define Tone_F_BLOCK_EN_MASK          (Tone_F_PWMHW__PM_ACT_MSK)
    #define Tone_F_BLOCK_STBY_EN_MASK     (Tone_F_PWMHW__PM_STBY_MSK)
    
    /* Control Register definitions */
    #define Tone_F_CTRL_ENABLE_SHIFT      (0x00u)

    /* As defined by Register map as MODE_CFG bits in CFG2*/
    #define Tone_F_CTRL_CMPMODE1_SHIFT    (0x04u)

    /* As defined by Register map */
    #define Tone_F_CTRL_DEAD_TIME_SHIFT   (0x06u)  

    /* Fixed Function Block Only CFG register bit definitions */
    /*  Set to compare mode */
    #define Tone_F_CFG0_MODE              (0x02u)   

    /* Enable the block to run */
    #define Tone_F_CFG0_ENABLE            (0x01u)   
    
    /* As defined by Register map as DB bit in CFG0 */
    #define Tone_F_CFG0_DB                (0x20u)   

    /* Control Register Bit Masks */
    #define Tone_F_CTRL_ENABLE            (uint8)((uint8)0x01u << Tone_F_CTRL_ENABLE_SHIFT)
    #define Tone_F_CTRL_RESET             (uint8)((uint8)0x01u << Tone_F_CTRL_RESET_SHIFT)
    #define Tone_F_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << Tone_F_CTRL_CMPMODE2_SHIFT)
    #define Tone_F_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << Tone_F_CTRL_CMPMODE1_SHIFT)

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define Tone_F_CTRL2_IRQ_SEL_SHIFT    (0x00u)
    #define Tone_F_CTRL2_IRQ_SEL          (uint8)((uint8)0x01u << Tone_F_CTRL2_IRQ_SEL_SHIFT)

    /* Status Register Bit Locations */
    /* As defined by Register map as TC in SR0 */
    #define Tone_F_STATUS_TC_SHIFT        (0x07u)   
    
    /* As defined by the Register map as CAP_CMP in SR0 */
    #define Tone_F_STATUS_CMP1_SHIFT      (0x06u)   

    /* Status Register Interrupt Enable Bit Locations */
    #define Tone_F_STATUS_KILL_INT_EN_MASK_SHIFT          (0x00u)
    #define Tone_F_STATUS_TC_INT_EN_MASK_SHIFT            (Tone_F_STATUS_TC_SHIFT - 4u)
    #define Tone_F_STATUS_CMP2_INT_EN_MASK_SHIFT          (0x00u)
    #define Tone_F_STATUS_CMP1_INT_EN_MASK_SHIFT          (Tone_F_STATUS_CMP1_SHIFT - 4u)

    /* Status Register Bit Masks */
    #define Tone_F_STATUS_TC              (uint8)((uint8)0x01u << Tone_F_STATUS_TC_SHIFT)
    #define Tone_F_STATUS_CMP1            (uint8)((uint8)0x01u << Tone_F_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks */
    #define Tone_F_STATUS_TC_INT_EN_MASK              (uint8)((uint8)Tone_F_STATUS_TC >> 4u)
    #define Tone_F_STATUS_CMP1_INT_EN_MASK            (uint8)((uint8)Tone_F_STATUS_CMP1 >> 4u)

    /*RT1 Synch Constants */
    #define Tone_F_RT1_SHIFT             (0x04u)

    /* Sync TC and CMP bit masks */
    #define Tone_F_RT1_MASK              (uint8)((uint8)0x03u << Tone_F_RT1_SHIFT)
    #define Tone_F_SYNC                  (uint8)((uint8)0x03u << Tone_F_RT1_SHIFT)
    #define Tone_F_SYNCDSI_SHIFT         (0x00u)

    /* Sync all DSI inputs */
    #define Tone_F_SYNCDSI_MASK          (uint8)((uint8)0x0Fu << Tone_F_SYNCDSI_SHIFT)

    /* Sync all DSI inputs */
    #define Tone_F_SYNCDSI_EN            (uint8)((uint8)0x0Fu << Tone_F_SYNCDSI_SHIFT)


#else
    #define Tone_F_STATUS                (*(reg8 *)   Tone_F_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define Tone_F_STATUS_PTR            ((reg8 *)    Tone_F_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define Tone_F_STATUS_MASK           (*(reg8 *)   Tone_F_PWMUDB_genblk8_stsreg__MASK_REG)
    #define Tone_F_STATUS_MASK_PTR       ((reg8 *)    Tone_F_PWMUDB_genblk8_stsreg__MASK_REG)
    #define Tone_F_STATUS_AUX_CTRL       (*(reg8 *)   Tone_F_PWMUDB_genblk8_stsreg__STATUS_AUX_CTL_REG)
    #define Tone_F_CONTROL               (*(reg8 *)   Tone_F_PWMUDB_genblk1_ctrlreg__CONTROL_REG)
    #define Tone_F_CONTROL_PTR           ((reg8 *)    Tone_F_PWMUDB_genblk1_ctrlreg__CONTROL_REG)


    /***********************************
    *          Constants
    ***********************************/

    /* Control Register bit definitions */
    #define Tone_F_CTRL_ENABLE_SHIFT      (0x07u)
    #define Tone_F_CTRL_RESET_SHIFT       (0x06u)
    #define Tone_F_CTRL_CMPMODE2_SHIFT    (0x03u)
    #define Tone_F_CTRL_CMPMODE1_SHIFT    (0x00u)
    #define Tone_F_CTRL_DEAD_TIME_SHIFT   (0x00u)   /* No Shift Needed for UDB block */
    
    /* Control Register Bit Masks */
    #define Tone_F_CTRL_ENABLE            (uint8)((uint8)0x01u << Tone_F_CTRL_ENABLE_SHIFT)
    #define Tone_F_CTRL_RESET             (uint8)((uint8)0x01u << Tone_F_CTRL_RESET_SHIFT)
    #define Tone_F_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << Tone_F_CTRL_CMPMODE2_SHIFT)
    #define Tone_F_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << Tone_F_CTRL_CMPMODE1_SHIFT)

    /* Status Register Bit Locations */
    #define Tone_F_STATUS_KILL_SHIFT          (0x05u)
    #define Tone_F_STATUS_FIFONEMPTY_SHIFT    (0x04u)
    #define Tone_F_STATUS_FIFOFULL_SHIFT      (0x03u)
    #define Tone_F_STATUS_TC_SHIFT            (0x02u)
    #define Tone_F_STATUS_CMP2_SHIFT          (0x01u)
    #define Tone_F_STATUS_CMP1_SHIFT          (0x00u)

    /* Status Register Interrupt Enable Bit Locations - UDB Status Interrupt Mask match Status Bit Locations*/
    #define Tone_F_STATUS_KILL_INT_EN_MASK_SHIFT          (Tone_F_STATUS_KILL_SHIFT)
    #define Tone_F_STATUS_FIFONEMPTY_INT_EN_MASK_SHIFT    (Tone_F_STATUS_FIFONEMPTY_SHIFT)
    #define Tone_F_STATUS_FIFOFULL_INT_EN_MASK_SHIFT      (Tone_F_STATUS_FIFOFULL_SHIFT)
    #define Tone_F_STATUS_TC_INT_EN_MASK_SHIFT            (Tone_F_STATUS_TC_SHIFT)
    #define Tone_F_STATUS_CMP2_INT_EN_MASK_SHIFT          (Tone_F_STATUS_CMP2_SHIFT)
    #define Tone_F_STATUS_CMP1_INT_EN_MASK_SHIFT          (Tone_F_STATUS_CMP1_SHIFT)

    /* Status Register Bit Masks */
    #define Tone_F_STATUS_KILL            (uint8)((uint8)0x00u << Tone_F_STATUS_KILL_SHIFT )
    #define Tone_F_STATUS_FIFOFULL        (uint8)((uint8)0x01u << Tone_F_STATUS_FIFOFULL_SHIFT)
    #define Tone_F_STATUS_FIFONEMPTY      (uint8)((uint8)0x01u << Tone_F_STATUS_FIFONEMPTY_SHIFT)
    #define Tone_F_STATUS_TC              (uint8)((uint8)0x01u << Tone_F_STATUS_TC_SHIFT)
    #define Tone_F_STATUS_CMP2            (uint8)((uint8)0x01u << Tone_F_STATUS_CMP2_SHIFT)
    #define Tone_F_STATUS_CMP1            (uint8)((uint8)0x01u << Tone_F_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks  - UDB Status Interrupt Mask match Status Bit Locations */
    #define Tone_F_STATUS_KILL_INT_EN_MASK            (Tone_F_STATUS_KILL)
    #define Tone_F_STATUS_FIFOFULL_INT_EN_MASK        (Tone_F_STATUS_FIFOFULL)
    #define Tone_F_STATUS_FIFONEMPTY_INT_EN_MASK      (Tone_F_STATUS_FIFONEMPTY)
    #define Tone_F_STATUS_TC_INT_EN_MASK              (Tone_F_STATUS_TC)
    #define Tone_F_STATUS_CMP2_INT_EN_MASK            (Tone_F_STATUS_CMP2)
    #define Tone_F_STATUS_CMP1_INT_EN_MASK            (Tone_F_STATUS_CMP1)

    /* Datapath Auxillary Control Register bit definitions */
    #define Tone_F_AUX_CTRL_FIFO0_CLR         (0x01u)
    #define Tone_F_AUX_CTRL_FIFO1_CLR         (0x02u)
    #define Tone_F_AUX_CTRL_FIFO0_LVL         (0x04u)
    #define Tone_F_AUX_CTRL_FIFO1_LVL         (0x08u)
    #define Tone_F_STATUS_ACTL_INT_EN_MASK    (0x10u) /* As defined for the ACTL Register */
#endif /* Tone_F_UsingFixedFunction */

#endif  /* CY_PWM_Tone_F_H */


/* [] END OF FILE */
