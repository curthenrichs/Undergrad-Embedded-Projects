/*******************************************************************************
* File Name: Tx_Timer_int_1.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_Tx_Timer_int_1_H)
#define CY_ISR_Tx_Timer_int_1_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void Tx_Timer_int_1_Start(void);
void Tx_Timer_int_1_StartEx(cyisraddress address);
void Tx_Timer_int_1_Stop(void);

CY_ISR_PROTO(Tx_Timer_int_1_Interrupt);

void Tx_Timer_int_1_SetVector(cyisraddress address);
cyisraddress Tx_Timer_int_1_GetVector(void);

void Tx_Timer_int_1_SetPriority(uint8 priority);
uint8 Tx_Timer_int_1_GetPriority(void);

void Tx_Timer_int_1_Enable(void);
uint8 Tx_Timer_int_1_GetState(void);
void Tx_Timer_int_1_Disable(void);

void Tx_Timer_int_1_SetPending(void);
void Tx_Timer_int_1_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the Tx_Timer_int_1 ISR. */
#define Tx_Timer_int_1_INTC_VECTOR            ((reg32 *) Tx_Timer_int_1__INTC_VECT)

/* Address of the Tx_Timer_int_1 ISR priority. */
#define Tx_Timer_int_1_INTC_PRIOR             ((reg8 *) Tx_Timer_int_1__INTC_PRIOR_REG)

/* Priority of the Tx_Timer_int_1 interrupt. */
#define Tx_Timer_int_1_INTC_PRIOR_NUMBER      Tx_Timer_int_1__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable Tx_Timer_int_1 interrupt. */
#define Tx_Timer_int_1_INTC_SET_EN            ((reg32 *) Tx_Timer_int_1__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the Tx_Timer_int_1 interrupt. */
#define Tx_Timer_int_1_INTC_CLR_EN            ((reg32 *) Tx_Timer_int_1__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the Tx_Timer_int_1 interrupt state to pending. */
#define Tx_Timer_int_1_INTC_SET_PD            ((reg32 *) Tx_Timer_int_1__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the Tx_Timer_int_1 interrupt. */
#define Tx_Timer_int_1_INTC_CLR_PD            ((reg32 *) Tx_Timer_int_1__INTC_CLR_PD_REG)


#endif /* CY_ISR_Tx_Timer_int_1_H */


/* [] END OF FILE */
