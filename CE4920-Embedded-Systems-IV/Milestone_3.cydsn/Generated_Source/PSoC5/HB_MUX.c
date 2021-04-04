/*******************************************************************************
* File Name: HB_MUX.c
* Version 1.80
*
*  Description:
*    This file contains all functions required for the analog multiplexer
*    AMux User Module.
*
*   Note:
*
*******************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "HB_MUX.h"

static uint8 HB_MUX_lastChannel = HB_MUX_NULL_CHANNEL;


/*******************************************************************************
* Function Name: HB_MUX_Start
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
void HB_MUX_Start(void) 
{
    uint8 chan;

    for(chan = 0u; chan < HB_MUX_CHANNELS ; chan++)
    {
#if (HB_MUX_MUXTYPE == HB_MUX_MUX_SINGLE)
        HB_MUX_Unset(chan);
#else
        HB_MUX_CYAMUXSIDE_A_Unset(chan);
        HB_MUX_CYAMUXSIDE_B_Unset(chan);
#endif
    }

    HB_MUX_lastChannel = HB_MUX_NULL_CHANNEL;
}


#if (!HB_MUX_ATMOSTONE)
/*******************************************************************************
* Function Name: HB_MUX_Select
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
void HB_MUX_Select(uint8 channel) 
{
    HB_MUX_DisconnectAll();        /* Disconnect all previous connections */
    HB_MUX_Connect(channel);       /* Make the given selection */
    HB_MUX_lastChannel = channel;  /* Update last channel */
}
#endif


/*******************************************************************************
* Function Name: HB_MUX_FastSelect
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
void HB_MUX_FastSelect(uint8 channel) 
{
    /* Disconnect the last valid channel */
    if( HB_MUX_lastChannel != HB_MUX_NULL_CHANNEL)
    {
        HB_MUX_Disconnect(HB_MUX_lastChannel);
    }

    /* Make the new channel connection */
#if (HB_MUX_MUXTYPE == HB_MUX_MUX_SINGLE)
    HB_MUX_Set(channel);
#else
    HB_MUX_CYAMUXSIDE_A_Set(channel);
    HB_MUX_CYAMUXSIDE_B_Set(channel);
#endif


    HB_MUX_lastChannel = channel;   /* Update last channel */
}


#if (HB_MUX_MUXTYPE == HB_MUX_MUX_DIFF)
#if (!HB_MUX_ATMOSTONE)
/*******************************************************************************
* Function Name: HB_MUX_Connect
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
void HB_MUX_Connect(uint8 channel) 
{
    HB_MUX_CYAMUXSIDE_A_Set(channel);
    HB_MUX_CYAMUXSIDE_B_Set(channel);
}
#endif

/*******************************************************************************
* Function Name: HB_MUX_Disconnect
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
void HB_MUX_Disconnect(uint8 channel) 
{
    HB_MUX_CYAMUXSIDE_A_Unset(channel);
    HB_MUX_CYAMUXSIDE_B_Unset(channel);
}
#endif

#if (HB_MUX_ATMOSTONE)
/*******************************************************************************
* Function Name: HB_MUX_DisconnectAll
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
void HB_MUX_DisconnectAll(void) 
{
    if(HB_MUX_lastChannel != HB_MUX_NULL_CHANNEL) 
    {
        HB_MUX_Disconnect(HB_MUX_lastChannel);
        HB_MUX_lastChannel = HB_MUX_NULL_CHANNEL;
    }
}
#endif

/* [] END OF FILE */
