/*******************************************************************************
* File Name: Button_TunerHelper.c
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

#include "Button_TunerHelper.h"

#if (Button_TUNER_API_GENERATE)
    volatile Button_MAILBOXES Button_mailboxesComm;
#endif  /* (Button_TUNER_API_GENERATE) */


/*******************************************************************************
* Function Name: Button_TunerStart
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
void Button_TunerStart(void) 
{
    #if (Button_TUNER_API_GENERATE)
        
        /* Init mbx and quick check */
        Button_InitMailbox(&Button_mailboxesComm.csdMailbox);
        Button_mailboxesComm.numMailBoxes = Button_DEFAULT_MAILBOXES_NUMBER;
        
        /* Start CapSense and baselines */
        Button_Start();
        
        /* Initialize baselines */ 
        Button_InitializeAllBaselines();
        Button_InitializeAllBaselines();
        
        /* Start EzI2C, clears buf pointers */
        EZI2C_Start();
        
        /* Setup EzI2C buffers */
        EZI2C_SetBuffer1(sizeof(Button_mailboxesComm), sizeof(Button_mailboxesComm),
                                        (void *) &Button_mailboxesComm);
        
        /* Starts scan all enabled sensors */
        Button_ScanEnabledWidgets();
    
    #endif  /* (Button_TUNER_API_GENERATE) */
}


/*******************************************************************************
* Function Name: Button_TunerComm
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
void Button_TunerComm(void) 
{
    #if (Button_TUNER_API_GENERATE)
        if (0u == Button_IsBusy())
        {   
            /* Apply new settings */
            #if (Button_TUNING_METHOD == Button_MANUAL_TUNING)
                Button_ReadMessage(&Button_mailboxesComm.csdMailbox);
            #endif  /* (Button_TUNING_METHOD == Button_MANUAL_TUNING) */

            /* Update all baselines and process all widgets */
            Button_UpdateEnabledBaselines();
            Button_ProcessAllWidgets(&Button_mailboxesComm.csdMailbox.outbox);
            Button_PostMessage(&Button_mailboxesComm.csdMailbox);

            /* Enable EZI2C interrupts, after scan complete */
            EZI2C_EnableInt();

            while((Button_mailboxesComm.csdMailbox.type != Button_TYPE_ID) || \
                  ((EZI2C_GetActivity() & EZI2C_STATUS_BUSY) != 0u)){}
            
            /* Disable EZI2C interrupts, while scanning */
            EZI2C_DisableInt();
            
            /* Start scan all sensors */
            Button_ScanEnabledWidgets();
        }
    #endif /* (Button_TUNER_API_GENERATE) */
}


#if (Button_TUNER_API_GENERATE)
    /*******************************************************************************
    * Function Name: Button_ProcessAllWidgets
    ********************************************************************************
    *
    * Summary:
    *  Call required functions to update all widgets state:
    *   - Button_GetCentroidPos() - calls only if linear sliders 
    *     available.
    *   - Button_GetRadialCentroidPos() - calls only if radial slider 
    *     available.
    *   - Button_GetTouchCentroidPos() - calls only if touch pad slider 
    *     available.
    *   - Button_CheckIsAnyWidgetActive();
    *  The results of opeartions are copied to OUTBOX.
    *   
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    * Global Variables:
    *  Button_OUTBOX outbox - structure which is used as ouput 
    *  buffer for report data to Tuner GUI.
    *  Update fields:
    *    - position[];
    *    - OnMask[];
    *
    * Reentrant:
    *  No
    *
    *******************************************************************************/
    void Button_ProcessAllWidgets(volatile Button_OUTBOX *outbox)
	                                        
    {
        uint8 i = 0u;
		#if (Button_TOTAL_MATRIX_BUTTONS_COUNT)
        	volatile uint8 *mbPositionAddr;
		#endif /* (Button_TOTAL_MATRIX_BUTTONS_COUNT) */

        #if (Button_TOTAL_TOUCH_PADS_COUNT)
            uint16 pos[2];
        #endif  /* (Button_TOTAL_TOUCH_PADS_COUNT) */
        
        #if ( (Button_TOTAL_RADIAL_SLIDERS_COUNT) || (Button_TOTAL_TOUCH_PADS_COUNT) || \
              (Button_TOTAL_MATRIX_BUTTONS_COUNT) )
            uint8 widgetCnt;
        #endif  /* ((Button_TOTAL_RADIAL_SLIDERS_COUNT) || (Button_TOTAL_TOUCH_PADS_COUNT)) || 
                *   (Button_TOTAL_MATRIX_BUTTONS_COUNT)
                */
        
        /* Calculate widget with centroids */
        #if (Button_TOTAL_LINEAR_SLIDERS_COUNT)
            for(; i < Button_TOTAL_LINEAR_SLIDERS_COUNT; i++)
            {
                outbox->position[i] = Button_SWAP_ENDIAN16(Button_GetCentroidPos(i));
            }
        #endif /* (Button_TOTAL_LINEAR_SLIDERS_COUNT) */
        
        #if (Button_TOTAL_RADIAL_SLIDERS_COUNT)
            widgetCnt = i;
            for(; i < (widgetCnt + Button_TOTAL_RADIAL_SLIDERS_COUNT); i++)
            {
                outbox->position[i] = Button_SWAP_ENDIAN16(Button_GetRadialCentroidPos(i));
            }
        #endif /* (Button_TOTAL_RADIAL_SLIDERS_COUNT) */
        
        #if (Button_TOTAL_TOUCH_PADS_COUNT)
            widgetCnt = i;
            for(; i < (widgetCnt + (Button_TOTAL_TOUCH_PADS_COUNT * 2u)); i = (i+2u))
            {
                if(Button_GetTouchCentroidPos(i, pos) == 0u)
                {
                    outbox->position[i] = 0xFFFFu;
                    outbox->position[i+1u] = 0xFFFFu;
                }
                else
                {
                    outbox->position[i] = Button_SWAP_ENDIAN16( (uint16) pos[0u]);
                    outbox->position[i+1u] = Button_SWAP_ENDIAN16( (uint16) pos[1u]);
                }
            }
        #endif /* (Button_TOTAL_TOUCH_PADS_COUNT) */

        #if (Button_TOTAL_MATRIX_BUTTONS_COUNT)
            i += Button_TOTAL_BUTTONS_COUNT;
            widgetCnt = 0u;
            for(; widgetCnt < (Button_TOTAL_MATRIX_BUTTONS_COUNT * 2u); widgetCnt += 2u)
            {
                mbPositionAddr = &outbox->mbPosition[widgetCnt];
                if(Button_GetMatrixButtonPos(i, ((uint8*) mbPositionAddr)) == 0u)
                {
                    outbox->mbPosition[widgetCnt] = 0xFFu;
                    outbox->mbPosition[widgetCnt+1u] = 0xFFu;
                }
                i += 2u;
            }
        #endif /* (Button_TOTAL_MATRIX_BUTTONS_COUNT) */

        /* Update On/Off State */
        (void)Button_CheckIsAnyWidgetActive();

        /* Copy OnMask */
        for(i = 0u; i < Button_TOTAL_SENSOR_MASK_COUNT; i++)
        {
            outbox->onMask[i]  = Button_sensorOnMask[i];
        }
    }
#endif /* (Button_TUNER_API_GENERATE) */


/* [] END OF FILE */
