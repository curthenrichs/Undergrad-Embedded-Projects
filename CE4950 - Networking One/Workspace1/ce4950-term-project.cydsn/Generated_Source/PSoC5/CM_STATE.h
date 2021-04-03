/*******************************************************************************
* File Name: CM_STATE.h  
* Version 1.80
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CONTROL_REG_CM_STATE_H) /* CY_CONTROL_REG_CM_STATE_H */
#define CY_CONTROL_REG_CM_STATE_H

#include "cyfitter.h"

#if ((CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC3) || \
     (CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC4) || \
     (CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC5))
    #include "cytypes.h"
#else
    #include "syslib/cy_syslib.h"
#endif

    
/***************************************
*     Data Struct Definitions
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 controlState;

} CM_STATE_BACKUP_STRUCT;


/***************************************
*         Function Prototypes 
***************************************/

void    CM_STATE_Write(uint8 control) ;
uint8   CM_STATE_Read(void) ;

void CM_STATE_SaveConfig(void) ;
void CM_STATE_RestoreConfig(void) ;
void CM_STATE_Sleep(void) ; 
void CM_STATE_Wakeup(void) ;


/***************************************
*            Registers        
***************************************/

/* Control Register */
#define CM_STATE_Control        (* (reg8 *) CM_STATE_Sync_ctrl_reg__CONTROL_REG )
#define CM_STATE_Control_PTR    (  (reg8 *) CM_STATE_Sync_ctrl_reg__CONTROL_REG )

#endif /* End CY_CONTROL_REG_CM_STATE_H */


/* [] END OF FILE */
