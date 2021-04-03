/*******************************************************************************
* File Name: Button_TunerHelper.h
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

#if !defined(CY_CAPSENSE_CSD_TUNERHELPER_Button_H)
#define CY_CAPSENSE_CSD_TUNERHELPER_Button_H

#include "Button.h"
#include "Button_CSHL.h"
#if (Button_TUNER_API_GENERATE)
    #include "Button_MBX.h"
    #include "EZI2C.h"
#endif /* (Button_TUNER_API_GENERATE) */


/***************************************
*     Constants for mailboxes
***************************************/

#define Button_DEFAULT_MAILBOXES_NUMBER   (1u)


/***************************************
*        Function Prototypes
***************************************/

void Button_TunerStart(void) ;
void Button_TunerComm(void) ;

#if (Button_TUNER_API_GENERATE)
    Button_NO_STRICT_VOLATILE void Button_ProcessAllWidgets(volatile Button_OUTBOX *outbox)
	                                        					;

    extern volatile Button_MAILBOXES Button_mailboxesComm;
#endif /* (Button_TUNER_API_GENERATE) */

#endif  /* (CY_CAPSENSE_CSD_TUNERHELPER_Button_H)*/


/* [] END OF FILE */
