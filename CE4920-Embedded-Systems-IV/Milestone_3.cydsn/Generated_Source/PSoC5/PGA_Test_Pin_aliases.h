/*******************************************************************************
* File Name: PGA_Test_Pin.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_PGA_Test_Pin_ALIASES_H) /* Pins PGA_Test_Pin_ALIASES_H */
#define CY_PINS_PGA_Test_Pin_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"


/***************************************
*              Constants        
***************************************/
#define PGA_Test_Pin_0			(PGA_Test_Pin__0__PC)
#define PGA_Test_Pin_0_INTR	((uint16)((uint16)0x0001u << PGA_Test_Pin__0__SHIFT))

#define PGA_Test_Pin_INTR_ALL	 ((uint16)(PGA_Test_Pin_0_INTR))

#endif /* End Pins PGA_Test_Pin_ALIASES_H */


/* [] END OF FILE */
