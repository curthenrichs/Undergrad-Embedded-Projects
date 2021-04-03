/*******************************************************************************
* File Name: Tone_Int.h
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
#if !defined(CY_ISR_Tone_Int_H)
#define CY_ISR_Tone_Int_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void Tone_Int_Start(void);
void Tone_Int_StartEx(cyisraddress address);
void Tone_Int_Stop(void);

CY_ISR_PROTO(Tone_Int_Interrupt);

void Tone_Int_SetVector(cyisraddress address);
cyisraddress Tone_Int_GetVector(void);

void Tone_Int_SetPriority(uint8 priority);
uint8 Tone_Int_GetPriority(void);

void Tone_Int_Enable(void);
uint8 Tone_Int_GetState(void);
void Tone_Int_Disable(void);

void Tone_Int_SetPending(void);
void Tone_Int_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the Tone_Int ISR. */
#define Tone_Int_INTC_VECTOR            ((reg32 *) Tone_Int__INTC_VECT)

/* Address of the Tone_Int ISR priority. */
#define Tone_Int_INTC_PRIOR             ((reg8 *) Tone_Int__INTC_PRIOR_REG)

/* Priority of the Tone_Int interrupt. */
#define Tone_Int_INTC_PRIOR_NUMBER      Tone_Int__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable Tone_Int interrupt. */
#define Tone_Int_INTC_SET_EN            ((reg32 *) Tone_Int__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the Tone_Int interrupt. */
#define Tone_Int_INTC_CLR_EN            ((reg32 *) Tone_Int__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the Tone_Int interrupt state to pending. */
#define Tone_Int_INTC_SET_PD            ((reg32 *) Tone_Int__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the Tone_Int interrupt. */
#define Tone_Int_INTC_CLR_PD            ((reg32 *) Tone_Int__INTC_CLR_PD_REG)


#endif /* CY_ISR_Tone_Int_H */


/* [] END OF FILE */
