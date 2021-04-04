/*******************************************************************************
* File Name: Slider_TunerHelper.c
* Version 3.50
*
* Description:
*  This file provides the source code of Tuner helper APIs for the CapSense CSD 
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

#include "Slider_TunerHelper.h"

#if (Slider_TUNER_API_GENERATE)
    volatile Slider_MAILBOXES Slider_mailboxesComm;
#endif  /* (Slider_TUNER_API_GENERATE) */


/*******************************************************************************
* Function Name: Slider_TunerStart
********************************************************************************
*
* Summary:
*  Initializes CapSense CSD component and EzI2C communication componenet to use
*  mailbox data structure for communication with Tuner GUI.
*  Start the scanning, after initialization complete.
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
* Note:
*  All widgets are enabled by default except proximity widgets. Proximity widgets 
*  must be manually enabled as their long scan time is incompatible 
*  with the fast response required of other widget types. 
*
*******************************************************************************/
void Slider_TunerStart(void) 
{
    #if (Slider_TUNER_API_GENERATE)
        
        /* Init mbx and quick check */
        Slider_InitMailbox(&Slider_mailboxesComm.csdMailbox);
        Slider_mailboxesComm.numMailBoxes = Slider_DEFAULT_MAILBOXES_NUMBER;
        
        /* Start CapSense and baselines */
        Slider_Start();
        
        /* Initialize baselines */ 
        Slider_InitializeAllBaselines();
        Slider_InitializeAllBaselines();
        
        /* Start EzI2C, clears buf pointers */
        EZI2C_Start();
        
        /* Setup EzI2C buffers */
        EZI2C_SetBuffer1(sizeof(Slider_mailboxesComm), sizeof(Slider_mailboxesComm),
                                        (void *) &Slider_mailboxesComm);
        
        /* Starts scan all enabled sensors */
        Slider_ScanEnabledWidgets();
    
    #endif  /* (Slider_TUNER_API_GENERATE) */
}


/*******************************************************************************
* Function Name: Slider_TunerComm
********************************************************************************
*
* Summary:
*  This function is blocking. It waits till scaning loop is completed and apply
*  new parameters from Tuner GUI if available (manual tuning mode only). Updates
*  enabled baselines and state of widgets. Waits while Tuner GUI reports that 
*  content of mailbox could be modified. Then loads the report data into outbox 
*  and sets the busy flag. Starts new scanning loop.
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
void Slider_TunerComm(void) 
{
    #if (Slider_TUNER_API_GENERATE)
        if (0u == Slider_IsBusy())
        {   
            /* Apply new settings */
            #if (Slider_TUNING_METHOD == Slider_MANUAL_TUNING)
                Slider_ReadMessage(&Slider_mailboxesComm.csdMailbox);
            #endif  /* (Slider_TUNING_METHOD == Slider_MANUAL_TUNING) */

            /* Update all baselines and process all widgets */
            Slider_UpdateEnabledBaselines();
            Slider_ProcessAllWidgets(&Slider_mailboxesComm.csdMailbox.outbox);
            Slider_PostMessage(&Slider_mailboxesComm.csdMailbox);

            /* Enable EZI2C interrupts, after scan complete */
            EZI2C_EnableInt();

            while((Slider_mailboxesComm.csdMailbox.type != Slider_TYPE_ID) || \
                  ((EZI2C_GetActivity() & EZI2C_STATUS_BUSY) != 0u)){}
            
            /* Disable EZI2C interrupts, while scanning */
            EZI2C_DisableInt();
            
            /* Start scan all sensors */
            Slider_ScanEnabledWidgets();
        }
    #endif /* (Slider_TUNER_API_GENERATE) */
}


#if (Slider_TUNER_API_GENERATE)
    /*******************************************************************************
    * Function Name: Slider_ProcessAllWidgets
    ********************************************************************************
    *
    * Summary:
    *  Call required functions to update all widgets state:
    *   - Slider_GetCentroidPos() - calls only if linear sliders 
    *     available.
    *   - Slider_GetRadialCentroidPos() - calls only if radial slider 
    *     available.
    *   - Slider_GetTouchCentroidPos() - calls only if touch pad slider 
    *     available.
    *   - Slider_CheckIsAnyWidgetActive();
    *  The results of opeartions are copied to OUTBOX.
    *   
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    * Global Variables:
    *  Slider_OUTBOX outbox - structure which is used as ouput 
    *  buffer for report data to Tuner GUI.
    *  Update fields:
    *    - position[];
    *    - OnMask[];
    *
    * Reentrant:
    *  No
    *
    *******************************************************************************/
    void Slider_ProcessAllWidgets(volatile Slider_OUTBOX *outbox)
	                                        
    {
        uint8 i = 0u;
		#if (Slider_TOTAL_MATRIX_BUTTONS_COUNT)
        	volatile uint8 *mbPositionAddr;
		#endif /* (Slider_TOTAL_MATRIX_BUTTONS_COUNT) */

        #if (Slider_TOTAL_TOUCH_PADS_COUNT)
            uint16 pos[2];
        #endif  /* (Slider_TOTAL_TOUCH_PADS_COUNT) */
        
        #if ( (Slider_TOTAL_RADIAL_SLIDERS_COUNT) || (Slider_TOTAL_TOUCH_PADS_COUNT) || \
              (Slider_TOTAL_MATRIX_BUTTONS_COUNT) )
            uint8 widgetCnt;
        #endif  /* ((Slider_TOTAL_RADIAL_SLIDERS_COUNT) || (Slider_TOTAL_TOUCH_PADS_COUNT)) || 
                *   (Slider_TOTAL_MATRIX_BUTTONS_COUNT)
                */
        
        /* Calculate widget with centroids */
        #if (Slider_TOTAL_LINEAR_SLIDERS_COUNT)
            for(; i < Slider_TOTAL_LINEAR_SLIDERS_COUNT; i++)
            {
                outbox->position[i] = Slider_SWAP_ENDIAN16(Slider_GetCentroidPos(i));
            }
        #endif /* (Slider_TOTAL_LINEAR_SLIDERS_COUNT) */
        
        #if (Slider_TOTAL_RADIAL_SLIDERS_COUNT)
            widgetCnt = i;
            for(; i < (widgetCnt + Slider_TOTAL_RADIAL_SLIDERS_COUNT); i++)
            {
                outbox->position[i] = Slider_SWAP_ENDIAN16(Slider_GetRadialCentroidPos(i));
            }
        #endif /* (Slider_TOTAL_RADIAL_SLIDERS_COUNT) */
        
        #if (Slider_TOTAL_TOUCH_PADS_COUNT)
            widgetCnt = i;
            for(; i < (widgetCnt + (Slider_TOTAL_TOUCH_PADS_COUNT * 2u)); i = (i+2u))
            {
                if(Slider_GetTouchCentroidPos(i, pos) == 0u)
                {
                    outbox->position[i] = 0xFFFFu;
                    outbox->position[i+1u] = 0xFFFFu;
                }
                else
                {
                    outbox->position[i] = Slider_SWAP_ENDIAN16( (uint16) pos[0u]);
                    outbox->position[i+1u] = Slider_SWAP_ENDIAN16( (uint16) pos[1u]);
                }
            }
        #endif /* (Slider_TOTAL_TOUCH_PADS_COUNT) */

        #if (Slider_TOTAL_MATRIX_BUTTONS_COUNT)
            i += Slider_TOTAL_BUTTONS_COUNT;
            widgetCnt = 0u;
            for(; widgetCnt < (Slider_TOTAL_MATRIX_BUTTONS_COUNT * 2u); widgetCnt += 2u)
            {
                mbPositionAddr = &outbox->mbPosition[widgetCnt];
                if(Slider_GetMatrixButtonPos(i, ((uint8*) mbPositionAddr)) == 0u)
                {
                    outbox->mbPosition[widgetCnt] = 0xFFu;
                    outbox->mbPosition[widgetCnt+1u] = 0xFFu;
                }
                i += 2u;
            }
        #endif /* (Slider_TOTAL_MATRIX_BUTTONS_COUNT) */

        /* Update On/Off State */
        (void)Slider_CheckIsAnyWidgetActive();

        /* Copy OnMask */
        for(i = 0u; i < Slider_TOTAL_SENSOR_MASK_COUNT; i++)
        {
            outbox->onMask[i]  = Slider_sensorOnMask[i];
        }
    }
#endif /* (Slider_TUNER_API_GENERATE) */


/* [] END OF FILE */
