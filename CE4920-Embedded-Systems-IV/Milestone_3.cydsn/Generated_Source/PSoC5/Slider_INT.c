/*******************************************************************************
* File Name: Slider_INT.c
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

#include "Slider.h"
#include "cyapicallbacks.h"


/*******************************************************************************
*  Place your includes, defines and code here 
********************************************************************************/
/* `#START Slider_IsrCH_ISR_intc` */

/* `#END` */


#if (Slider_DESIGN_TYPE == Slider_ONE_CHANNEL_DESIGN)
    /*******************************************************************************
    * Function Name: Slider_IsrCH0_ISR
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
    *  Slider_csv - used to provide status and mode of scanning process.
    *   Mode - single scan or scan of all enabled sensors.
    *   Status - scan in progress or ready for new scan.
    *  Slider_sensorIndex - used to store sensor scanning sensor number.
    *
    * Reentrant:
    *  No
    *
    *******************************************************************************/
    CY_ISR(Slider_IsrCH0_ISR)
    {
        #ifdef Slider_ONE_CHANNEL_CH0_ISR_ENTRY_CALLBACK
            Slider_IsrCH0_ISR_EntryCallback();
        #endif /* Slider_ONE_CHANNEL_CH0_ISR_ENTRY_CALLBACK */

        /*  Place your Interrupt code here. */
        /* `#START Slider_ONE_CHANNEL_IsrCH0_ISR_ENTER` */

        /* `#END` */

        /* Save results and disable sensor */
        Slider_PostScan(Slider_sensorIndex);
                
        if ((Slider_csv & Slider_SW_CTRL_SINGLE_SCAN) != 0u)
        {
            Slider_csv &= (uint8)~Slider_SW_STS_BUSY;
        }
        else
        {
            /* Proceed the scanning */
            Slider_sensorIndex = Slider_FindNextSensor(Slider_sensorIndex);
            
            /* Check end of scan */
            if(Slider_sensorIndex < Slider_TOTAL_SENSOR_COUNT)
            {
                Slider_PreScan(Slider_sensorIndex);
            }
            else
            {
                Slider_csv &= (uint8)~Slider_SW_STS_BUSY;
            } 
        }

        /*  Place your Interrupt code here. */
        /* `#START Slider_ONE_CHANNEL_IsrCH0_ISR_EXIT` */

        /* `#END` */

        #ifdef Slider_ONE_CHANNEL_CH0_ISR_EXIT_CALLBACK
            Slider_IsrCH0_ISR_ExitCallback();
        #endif /* Slider_ONE_CHANNEL_CH0_ISR_EXIT_CALLBACK */
    }
 #endif  /* (Slider_DESIGN_TYPE == Slider_ONE_CHANNEL_DESIGN) */


 #if (Slider_DESIGN_TYPE == Slider_TWO_CHANNELS_DESIGN)   
    /*******************************************************************************
    * Function Name: Slider_IsrCH0_ISR
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
    *  Slider_csv - used to provide status and mode of scanning process.
    *   Mode - single scan or scan of all enabled sensors.
    *   Status - scan in progress or ready for new scan.
    *  Slider_sensorIndex - used to store sensor scanning sensor number.
    *
    * Reentrant:
    *  No
    *
    *******************************************************************************/
    CY_ISR(Slider_IsrCH0_ISR)
    {
        #ifdef Slider_TWO_CHANNEL_CH0_ISR_ENTRY_CALLBACK
            Slider_IsrCH0_ISR_EntryCallback();
        #endif /* Slider_TWO_CHANNEL_CH0_ISR_ENTRY_CALLBACK */

        /*  Place your Interrupt code here. */
        /* `#START Slider_TWO_CHANNELS_IsrCH0_ISR_ENTER` */

        /* `#END` */
        
        Slider_CONTROL_REG &= ~Slider_CTRL_WINDOW_EN__CH0;
        Slider_PostScanCh0(Slider_sensorIndex);
        
        if((Slider_CONTROL_REG & Slider_CTRL_WINDOW_EN__CH1) == 0u)
        {
            if ((Slider_csv & Slider_SW_CTRL_SINGLE_SCAN) != 0u)
            {
                Slider_csv &= (uint8)~Slider_SW_STS_BUSY;
            }
            else
            {
                /* Procced the scanning */
                Slider_sensorIndex = Slider_FindNextPair(Slider_sensorIndex);
                
                /* Check end of scan conditions */
                if((Slider_sensorIndex < Slider_TOTAL_SENSOR_COUNT__CH0) || 
                   (Slider_sensorIndex < Slider_TOTAL_SENSOR_COUNT__CH1))
                {
                    Slider_PreScan(Slider_sensorIndex);
                }
                else
                {
                    Slider_csv &= (uint8)~Slider_SW_STS_BUSY;
                }
            }
        }
        else
        {
            /* Do nothing */
        }

		/*  Place your Interrupt code here. */
        /* `#START Slider_TWO_CHANNELS_IsrCH0_ISR_EXIT` */

        /* `#END` */

        #ifdef Slider_TWO_CHANNEL_CH0_ISR_EXIT_CALLBACK
            Slider_IsrCH0_ISR_ExitCallback();
        #endif /* Slider_TWO_CHANNEL_CH0_ISR_EXIT_CALLBACK */
    }

    
    /*******************************************************************************
    * Function Name: Slider_IsrCH1_ISR
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
    *  Slider_csv - used to provide status and mode of scanning process.
    *   Mode - single scan or scan of all enabled sensors.
    *   Status - scan in progress or ready for new scan.
    *  Slider_sensorIndex - used to store sensor scanning sensor number.
    *
    * Reentrant:
    *  No
    *
    *******************************************************************************/
    CY_ISR(Slider_IsrCH1_ISR)
    {
        #ifdef Slider_TWO_CHANNEL_CH1_ISR_ENTRY_CALLBACK
            Slider_IsrCH1_ISR_EntryCallback();
        #endif /* Slider_TWO_CHANNEL_CH1_ISR_ENTRY_CALLBACK */

        /*  Place your Interrupt code here */
        /* `#START Slider_TWO_CHANNELS_IsrCH1_ISR_ENTER` */

        /* `#END` */
        
        Slider_CONTROL_REG &= (uint8)~Slider_CTRL_WINDOW_EN__CH1;
        Slider_PostScanCh1(Slider_sensorIndex + Slider_TOTAL_SENSOR_COUNT__CH0);
        
        if((Slider_CONTROL_REG & Slider_CTRL_WINDOW_EN__CH0) == 0u)
        {
            if ((Slider_csv & Slider_SW_CTRL_SINGLE_SCAN) != 0u)
            {
                Slider_csv &= (uint8)~Slider_SW_STS_BUSY;
            }
            else 
            {
                /* Procced the scanning */
                Slider_sensorIndex = Slider_FindNextPair(Slider_sensorIndex);
                
                /* Check end of scan conditions */
                if((Slider_sensorIndex < Slider_TOTAL_SENSOR_COUNT__CH0) || 
                   (Slider_sensorIndex < Slider_TOTAL_SENSOR_COUNT__CH1))
                {
                    Slider_PreScan(Slider_sensorIndex);
                }
                else
                {
                    Slider_csv &= ~Slider_SW_STS_BUSY;
                }
            }
        }
        else
        {
            /* Do nothing */
        }

		/*  Place your Interrupt code here */
        /* `#START Slider_TWO_CHANNELS_IsrCH1_ISR_EXIT` */

        /* `#END` */

        #ifdef Slider_TWO_CHANNEL_CH1_ISR_EXIT_CALLBACK
            Slider_IsrCH1_ISR_ExitCallback();
        #endif /* Slider_TWO_CHANNEL_CH1_ISR_EXIT_CALLBACK */
    }
#endif  /* (Slider_DESIGN_TYPE == Slider_TWO_CHANNELS_DESIGN) */


/* [] END OF FILE */
