/*******************************************************************************
* File Name: CSMA_RX_int_falling.h
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
#if !defined(CY_ISR_CSMA_RX_int_falling_H)
#define CY_ISR_CSMA_RX_int_falling_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void CSMA_RX_int_falling_Start(void);
void CSMA_RX_int_falling_StartEx(cyisraddress address);
void CSMA_RX_int_falling_Stop(void);

CY_ISR_PROTO(CSMA_RX_int_falling_Interrupt);

void CSMA_RX_int_falling_SetVector(cyisraddress address);
cyisraddress CSMA_RX_int_falling_GetVector(void);

void CSMA_RX_int_falling_SetPriority(uint8 priority);
uint8 CSMA_RX_int_falling_GetPriority(void);

void CSMA_RX_int_falling_Enable(void);
uint8 CSMA_RX_int_falling_GetState(void);
void CSMA_RX_int_falling_Disable(void);

void CSMA_RX_int_falling_SetPending(void);
void CSMA_RX_int_falling_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the CSMA_RX_int_falling ISR. */
#define CSMA_RX_int_falling_INTC_VECTOR            ((reg32 *) CSMA_RX_int_falling__INTC_VECT)

/* Address of the CSMA_RX_int_falling ISR priority. */
#define CSMA_RX_int_falling_INTC_PRIOR             ((reg8 *) CSMA_RX_int_falling__INTC_PRIOR_REG)

/* Priority of the CSMA_RX_int_falling interrupt. */
#define CSMA_RX_int_falling_INTC_PRIOR_NUMBER      CSMA_RX_int_falling__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable CSMA_RX_int_falling interrupt. */
#define CSMA_RX_int_falling_INTC_SET_EN            ((reg32 *) CSMA_RX_int_falling__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the CSMA_RX_int_falling interrupt. */
#define CSMA_RX_int_falling_INTC_CLR_EN            ((reg32 *) CSMA_RX_int_falling__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the CSMA_RX_int_falling interrupt state to pending. */
#define CSMA_RX_int_falling_INTC_SET_PD            ((reg32 *) CSMA_RX_int_falling__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the CSMA_RX_int_falling interrupt. */
#define CSMA_RX_int_falling_INTC_CLR_PD            ((reg32 *) CSMA_RX_int_falling__INTC_CLR_PD_REG)


#endif /* CY_ISR_CSMA_RX_int_falling_H */


/* [] END OF FILE */
