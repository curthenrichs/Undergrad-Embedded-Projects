/*******************************************************************************
* File Name: Button_INT.c
* Version 3.50
*
* Description:
*  This file provides the source code of Interrupt Service Routine (ISR)
*  for CapSense CSD component.
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


/*******************************************************************************
*  Place your includes, defines and code here 
********************************************************************************/
/* `#START Button_IsrCH_ISR_intc` */

/* `#END` */


#if (Button_DESIGN_TYPE == Button_ONE_CHANNEL_DESIGN)
    /*******************************************************************************
    * Function Name: Button_IsrCH0_ISR
    ********************************************************************************
    *
    * Summary:
    *  This ISR is executed when measure window is closed. The measure window 
    *  depends on scan resolution parameter of sensor. 
    *  The ISR has two modes: 
    *   1. Scans single sensor and stores measure result.
    *   2. Stores result of current measure and starts next scan till all 
    *      enabled sensors will be scanned.
    *  This interrupt handler only used in one channel designs.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    * Global Variables:
    *  Button_csv - used to provide status and mode of scanning process.
    *   Mode - single scan or scan of all enabled sensors.
    *   Status - scan in progress or ready for new scan.
    *  Button_sensorIndex - used to store sensor scanning sensor number.
    *
    * Reentrant:
    *  No
    *
    *******************************************************************************/
    CY_ISR(Button_IsrCH0_ISR)
    {
        #ifdef Button_ONE_CHANNEL_CH0_ISR_ENTRY_CALLBACK
            Button_IsrCH0_ISR_EntryCallback();
        #endif /* Button_ONE_CHANNEL_CH0_ISR_ENTRY_CALLBACK */

        /*  Place your Interrupt code here. */
        /* `#START Button_ONE_CHANNEL_IsrCH0_ISR_ENTER` */

        /* `#END` */

        /* Save results and disable sensor */
        Button_PostScan(Button_sensorIndex);
                
        if ((Button_csv & Button_SW_CTRL_SINGLE_SCAN) != 0u)
        {
            Button_csv &= (uint8)~Button_SW_STS_BUSY;
        }
        else
        {
            /* Proceed the scanning */
            Button_sensorIndex = Button_FindNextSensor(Button_sensorIndex);
            
            /* Check end of scan */
            if(Button_sensorIndex < Button_TOTAL_SENSOR_COUNT)
            {
                Button_PreScan(Button_sensorIndex);
            }
            else
            {
                Button_csv &= (uint8)~Button_SW_STS_BUSY;
            } 
        }

        /*  Place your Interrupt code here. */
        /* `#START Button_ONE_CHANNEL_IsrCH0_ISR_EXIT` */

        /* `#END` */

        #ifdef Button_ONE_CHANNEL_CH0_ISR_EXIT_CALLBACK
            Button_IsrCH0_ISR_ExitCallback();
        #endif /* Button_ONE_CHANNEL_CH0_ISR_EXIT_CALLBACK */
    }
 #endif  /* (Button_DESIGN_TYPE == Button_ONE_CHANNEL_DESIGN) */


 #if (Button_DESIGN_TYPE == Button_TWO_CHANNELS_DESIGN)   
    /*******************************************************************************
    * Function Name: Button_IsrCH0_ISR
    ********************************************************************************
    *
    * Summary:
    *  This ISR is executed when measure window is closed. The measure window 
    *  depends on scan resolution parameter of sensor. 
    *  The ISR has two modes: 
    *   1. Scans single sensor or pair of sensors and stores measure result.
    *   2. Stores results of current measure when complete scan of sensors pair or 
    *      sensor scan and starts next enabled sensors pair or sensor scan. 
    *  This interrupt handler only used in two channel designs.
    *  The synchronization is added to scanning process. The ISR channel 0 or 
    *  channel 1 stores the result and starts next scan. It depends which channel
    *  scans longer. The biggest scan resolution parameter of sensor within pair 
    *  defines scanning time of pair.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    * Global Variables:
    *  Button_csv - used to provide status and mode of scanning process.
    *   Mode - single scan or scan of all enabled sensors.
    *   Status - scan in progress or ready for new scan.
    *  Button_sensorIndex - used to store sensor scanning sensor number.
    *
    * Reentrant:
    *  No
    *
    *******************************************************************************/
    CY_ISR(Button_IsrCH0_ISR)
    {
        #ifdef Button_TWO_CHANNEL_CH0_ISR_ENTRY_CALLBACK
            Button_IsrCH0_ISR_EntryCallback();
        #endif /* Button_TWO_CHANNEL_CH0_ISR_ENTRY_CALLBACK */

        /*  Place your Interrupt code here. */
        /* `#START Button_TWO_CHANNELS_IsrCH0_ISR_ENTER` */

        /* `#END` */
        
        Button_CONTROL_REG &= ~Button_CTRL_WINDOW_EN__CH0;
        Button_PostScanCh0(Button_sensorIndex);
        
        if((Button_CONTROL_REG & Button_CTRL_WINDOW_EN__CH1) == 0u)
        {
            if ((Button_csv & Button_SW_CTRL_SINGLE_SCAN) != 0u)
            {
                Button_csv &= (uint8)~Button_SW_STS_BUSY;
            }
            else
            {
                /* Procced the scanning */
                Button_sensorIndex = Button_FindNextPair(Button_sensorIndex);
                
                /* Check end of scan conditions */
                if((Button_sensorIndex < Button_TOTAL_SENSOR_COUNT__CH0) || 
                   (Button_sensorIndex < Button_TOTAL_SENSOR_COUNT__CH1))
                {
                    Button_PreScan(Button_sensorIndex);
                }
                else
                {
                    Button_csv &= (uint8)~Button_SW_STS_BUSY;
                }
            }
        }
        else
        {
            /* Do nothing */
        }

		/*  Place your Interrupt code here. */
        /* `#START Button_TWO_CHANNELS_IsrCH0_ISR_EXIT` */

        /* `#END` */

        #ifdef Button_TWO_CHANNEL_CH0_ISR_EXIT_CALLBACK
            Button_IsrCH0_ISR_ExitCallback();
        #endif /* Button_TWO_CHANNEL_CH0_ISR_EXIT_CALLBACK */
    }

    
    /*******************************************************************************
    * Function Name: Button_IsrCH1_ISR
    ********************************************************************************
    *
    * Summary:
    *  This ISR is executed when measure window is closed. The measure window 
    *  depends on scan resolution parameter of sensor. 
    *  The ISR has two modes: 
    *   1. Scans single sensor or pair of sensors and stores measure result.
    *   2. Stores results of current measure when complete scan of sensors pair or 
    *      sensor scan and starts next enabled sensors pair or sensor scan. 
    *  This interrupt handler only used in two channel designs.
    *  The synchronization is added to scanning process. The ISR channel 0 or 
    *  channel 1 stores the result and starts next scan. It depends which channel
    *  scans longer. The biggest scan resolution parameter of sensor within pair 
    *  defines scanning time of pair.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    * Global Variables:
    *  Button_csv - used to provide status and mode of scanning process.
    *   Mode - single scan or scan of all enabled sensors.
    *   Status - scan in progress or ready for new scan.
    *  Button_sensorIndex - used to store sensor scanning sensor number.
    *
    * Reentrant:
    *  No
    *
    *******************************************************************************/
    CY_ISR(Button_IsrCH1_ISR)
    {
        #ifdef Button_TWO_CHANNEL_CH1_ISR_ENTRY_CALLBACK
            Button_IsrCH1_ISR_EntryCallback();
        #endif /* Button_TWO_CHANNEL_CH1_ISR_ENTRY_CALLBACK */

        /*  Place your Interrupt code here */
        /* `#START Button_TWO_CHANNELS_IsrCH1_ISR_ENTER` */

        /* `#END` */
        
        Button_CONTROL_REG &= (uint8)~Button_CTRL_WINDOW_EN__CH1;
        Button_PostScanCh1(Button_sensorIndex + Button_TOTAL_SENSOR_COUNT__CH0);
        
        if((Button_CONTROL_REG & Button_CTRL_WINDOW_EN__CH0) == 0u)
        {
            if ((Button_csv & Button_SW_CTRL_SINGLE_SCAN) != 0u)
            {
                Button_csv &= (uint8)~Button_SW_STS_BUSY;
            }
            else 
            {
                /* Procced the scanning */
                Button_sensorIndex = Button_FindNextPair(Button_sensorIndex);
                
                /* Check end of scan conditions */
                if((Button_sensorIndex < Button_TOTAL_SENSOR_COUNT__CH0) || 
                   (Button_sensorIndex < Button_TOTAL_SENSOR_COUNT__CH1))
                {
                    Button_PreScan(Button_sensorIndex);
                }
                else
                {
                    Button_csv &= ~Button_SW_STS_BUSY;
                }
            }
        }
        else
        {
            /* Do nothing */
        }

		/*  Place your Interrupt code here */
        /* `#START Button_TWO_CHANNELS_IsrCH1_ISR_EXIT` */

        /* `#END` */

        #ifdef Button_TWO_CHANNEL_CH1_ISR_EXIT_CALLBACK
            Button_IsrCH1_ISR_ExitCallback();
        #endif /* Button_TWO_CHANNEL_CH1_ISR_EXIT_CALLBACK */
    }
#endif  /* (Button_DESIGN_TYPE == Button_TWO_CHANNELS_DESIGN) */


/* [] END OF FILE */
