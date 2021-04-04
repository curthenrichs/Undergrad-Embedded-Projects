/*******************************************************************************
* File Name: Slider_AMuxCH0.c
* Version 3.50
*
*  Description:
*    This file contains all functions required for the analog multiplexer
*    CapSense_CSD_AMux User Module.
*
*   Note:
*
*******************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "Slider_AMuxCH0.h"

uint8 Slider_AMuxCH0_initVar = 0u;
uint8 Slider_AMuxCH0_lastChannel = Slider_AMuxCH0_NULL_CHANNEL;


/*******************************************************************************
* Function Name: Slider_AMuxCH0_Start
********************************************************************************
* Summary:
*  Disconnect all channels.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void Slider_AMuxCH0_Start(void)
{
    Slider_AMuxCH0_DisconnectAll();
    Slider_AMuxCH0_initVar = 1u;
}


/*******************************************************************************
* Function Name: Slider_AMuxCH0_Init
********************************************************************************
* Summary:
*  Disconnect all channels.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void Slider_AMuxCH0_Init(void)
{
    Slider_AMuxCH0_DisconnectAll();
}


/*******************************************************************************
* Function Name: Slider_AMuxCH0_Stop
********************************************************************************
* Summary:
*  Disconnect all channels.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void Slider_AMuxCH0_Stop(void)
{
    Slider_AMuxCH0_DisconnectAll();
}


/*******************************************************************************
* Function Name: Slider_AMuxCH0_Select
********************************************************************************
* Summary:
*  This functions first disconnects all channels then connects the given
*  channel.
*
* Parameters:
*  channel:  The channel to connect to the common terminal.
*
* Return:
*  void
*
*******************************************************************************/
void Slider_AMuxCH0_Select(uint8 channel) CYREENTRANT
{
    Slider_AMuxCH0_DisconnectAll();        /* Disconnect all previous connections */
    Slider_AMuxCH0_Connect(channel);       /* Make the given selection */
    Slider_AMuxCH0_lastChannel = channel;  /* Update last channel */
}


/*******************************************************************************
* Function Name: Slider_AMuxCH0_FastSelect
********************************************************************************
* Summary:
*  This function first disconnects the last connection made with FastSelect or
*  Select, then connects the given channel. The FastSelect function is similar
*  to the Select function, except it is faster since it only disconnects the
*  last channel selected rather than all channels.
*
* Parameters:
*  channel:  The channel to connect to the common terminal.
*
* Return:
*  void
*
*******************************************************************************/
void Slider_AMuxCH0_FastSelect(uint8 channel) CYREENTRANT
{
    /* Disconnect the last valid channel */
    if( Slider_AMuxCH0_lastChannel != Slider_AMuxCH0_NULL_CHANNEL)   /* Update last channel */
    {
        Slider_AMuxCH0_Disconnect(Slider_AMuxCH0_lastChannel);
    }

    /* Make the new channel connection */
    Slider_AMuxCH0_Connect(channel);
    Slider_AMuxCH0_lastChannel = channel;   /* Update last channel */
}


#if(Slider_AMuxCH0_MUXTYPE == Slider_AMuxCH0_MUX_DIFF)
    /*******************************************************************************
    * Function Name: Slider_AMuxCH0_Connect
    ********************************************************************************
    * Summary:
    *  This function connects the given channel without affecting other connections.
    *
    * Parameters:
    *  channel:  The channel to connect to the common terminal.
    *
    * Return:
    *  void
    *
    *******************************************************************************/
    void Slider_AMuxCH0_Connect(uint8 channel) CYREENTRANT
    {
        Slider_AMuxCH0_CYAMUXSIDE_A_Set(channel);
        Slider_AMuxCH0_CYAMUXSIDE_B_Set(channel);
    }
    
    
    /*******************************************************************************
    * Function Name: Slider_AMuxCH0_Disconnect
    ********************************************************************************
    * Summary:
    *  This function disconnects the given channel from the common or output
    *  terminal without affecting other connections.
    *
    * Parameters:
    *  channel:  The channel to disconnect from the common terminal.
    *
    * Return:
    *  void
    *
    *******************************************************************************/
    void Slider_AMuxCH0_Disconnect(uint8 channel) CYREENTRANT
    {
        Slider_AMuxCH0_CYAMUXSIDE_A_Unset(channel);
        Slider_AMuxCH0_CYAMUXSIDE_B_Unset(channel);
    }
    
#endif  /* End (Slider_AMuxCH0_MUXTYPE == Slider_AMuxCH0_MUX_DIFF) */


/*******************************************************************************
* Function Name: Slider_AMuxCH0_DisconnectAll
********************************************************************************
* Summary:
*  This function disconnects all channels.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void Slider_AMuxCH0_DisconnectAll(void) CYREENTRANT
{
    uint8 chan;

    #if(Slider_AMuxCH0_MUXTYPE == Slider_AMuxCH0_MUX_SINGLE)
        for(chan = 0; chan < Slider_AMuxCH0_CHANNELS ; chan++)
        {
            Slider_AMuxCH0_Unset(chan);
        }
    #else
        for(chan = 0; chan < Slider_AMuxCH0_CHANNELS ; chan++)
        {
            Slider_AMuxCH0_CYAMUXSIDE_A_Unset(chan);
            Slider_AMuxCH0_CYAMUXSIDE_B_Unset(chan);
        }
    #endif  /* End (Slider_AMuxCH0_MUXTYPE == Slider_AMuxCH0_MUX_SINGLE) */
}


/* [] END OF FILE */
