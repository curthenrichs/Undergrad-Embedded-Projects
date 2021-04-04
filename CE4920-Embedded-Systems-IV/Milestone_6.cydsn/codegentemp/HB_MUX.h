/*******************************************************************************
* File Name: HB_MUX.h
* Version 1.80
*
*  Description:
*    This file contains the constants and function prototypes for the Analog
*    Multiplexer User Module AMux.
*
*   Note:
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_AMUX_HB_MUX_H)
#define CY_AMUX_HB_MUX_H

#include "cyfitter.h"
#include "cyfitter_cfg.h"

#if ((CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC3) || \
         (CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC4) || \
         (CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC5))    
    #include "cytypes.h"
#else
    #include "syslib/cy_syslib.h"
#endif /* ((CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC3) */


/***************************************
*        Function Prototypes
***************************************/

void HB_MUX_Start(void) ;
#define HB_MUX_Init() HB_MUX_Start()
void HB_MUX_FastSelect(uint8 channel) ;
/* The Stop, Select, Connect, Disconnect and DisconnectAll functions are declared elsewhere */
/* void HB_MUX_Stop(void); */
/* void HB_MUX_Select(uint8 channel); */
/* void HB_MUX_Connect(uint8 channel); */
/* void HB_MUX_Disconnect(uint8 channel); */
/* void HB_MUX_DisconnectAll(void) */


/***************************************
*         Parameter Constants
***************************************/

#define HB_MUX_CHANNELS  2u
#define HB_MUX_MUXTYPE   1
#define HB_MUX_ATMOSTONE 1

/***************************************
*             API Constants
***************************************/

#define HB_MUX_NULL_CHANNEL 0xFFu
#define HB_MUX_MUX_SINGLE   1
#define HB_MUX_MUX_DIFF     2


/***************************************
*        Conditional Functions
***************************************/

#if HB_MUX_MUXTYPE == HB_MUX_MUX_SINGLE
# if !HB_MUX_ATMOSTONE
#  define HB_MUX_Connect(channel) HB_MUX_Set(channel)
# endif
# define HB_MUX_Disconnect(channel) HB_MUX_Unset(channel)
#else
# if !HB_MUX_ATMOSTONE
void HB_MUX_Connect(uint8 channel) ;
# endif
void HB_MUX_Disconnect(uint8 channel) ;
#endif

#if HB_MUX_ATMOSTONE
# define HB_MUX_Stop() HB_MUX_DisconnectAll()
# define HB_MUX_Select(channel) HB_MUX_FastSelect(channel)
void HB_MUX_DisconnectAll(void) ;
#else
# define HB_MUX_Stop() HB_MUX_Start()
void HB_MUX_Select(uint8 channel) ;
# define HB_MUX_DisconnectAll() HB_MUX_Start()
#endif

#endif /* CY_AMUX_HB_MUX_H */


/* [] END OF FILE */
