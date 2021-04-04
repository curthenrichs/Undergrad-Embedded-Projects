/*******************************************************************************
* File Name: Slider_TunerHelper.h
* Version 3.50
*
* Description:
*  This file provides constants and structure declarations for the tunner hepl
*  APIs for CapSense CSD component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CAPSENSE_CSD_TUNERHELPER_Slider_H)
#define CY_CAPSENSE_CSD_TUNERHELPER_Slider_H

#include "Slider.h"
#include "Slider_CSHL.h"
#if (Slider_TUNER_API_GENERATE)
    #include "Slider_MBX.h"
    #include "EZI2C.h"
#endif /* (Slider_TUNER_API_GENERATE) */


/***************************************
*     Constants for mailboxes
***************************************/

#define Slider_DEFAULT_MAILBOXES_NUMBER   (1u)


/***************************************
*        Function Prototypes
***************************************/

void Slider_TunerStart(void) ;
void Slider_TunerComm(void) ;

#if (Slider_TUNER_API_GENERATE)
    Slider_NO_STRICT_VOLATILE void Slider_ProcessAllWidgets(volatile Slider_OUTBOX *outbox)
	                                        					;

    extern volatile Slider_MAILBOXES Slider_mailboxesComm;
#endif /* (Slider_TUNER_API_GENERATE) */

#endif  /* (CY_CAPSENSE_CSD_TUNERHELPER_Slider_H)*/


/* [] END OF FILE */
