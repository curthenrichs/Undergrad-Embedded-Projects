/**
 * PWM Module
 * @author Curt Henrichs
 * @date 1-11-16, revised 3-10-17
 *
 * Implementation of the PWM controller on the NIOS processor. This is used for
 * the servo motor system.
 */

//=============================================================================
//                                   Libraries
//=============================================================================

#include "PWM.h"

#include <system.h>
#include "GenericIO.h"

//=============================================================================
//                          Constant and Macro Declaration
//=============================================================================

//Register Locations
#define OCRA ((uint16_t volatile *)(MYNEWPWMV2_0_BASE | BYPASS_CACHE_FLAG))
#define OCRB ((uint16_t volatile *)(MYNEWPWMV2_0_BASE | BYPASS_CACHE_FLAG)+1)

//=============================================================================
//							  Function Implementation
//=============================================================================

/**
 * Writes a duty cycle to the PWM register
 * @param dutyPercent Duty cycle, given as a percent from 0 to 100. (anything
 *        lower or higher will be ignored and no change will occur to register
 * @return Either a 0 if there was no error (Bad input) or a EOF (-1) if the
 *         input was out of bounds.
 */
int PWM_setDutyCycle_A(int dutyCycle){
	if((dutyCycle <= 100) && (dutyCycle >= 0)){
		//Convert from percent to the acceptable OCR value with max 20000
		int duty = dutyCycle * (MAX_OCR_VALUE / 100);
		//We can never actually get to 20000 based on my CE2800 notes
		if(duty == MAX_OCR_VALUE)
			duty--;
		//set register
		*OCRA = duty;
	} else {
		return -1; // return an error
	}
	// no error
	return 0;
}

/**
 * Writes a duty cycle to the PWM register
 * @param dutyPercent Duty cycle, given as a percent from 0 to 100. (anything
 *        lower or higher will be ignored and no change will occur to register
 * @return Either a 0 if there was no error (Bad input) or a EOF (-1) if the
 *         input was out of bounds.
 */
int PWM_setDutyCycle_B(int dutyCycle){
	if((dutyCycle <= 100) && (dutyCycle >= 0)){
		//Convert from percent to the acceptable OCR value with max 20000
		int duty = dutyCycle * (MAX_OCR_VALUE / 100);
		//We can never actually get to 20000 based on my CE2800 notes
		if(duty == MAX_OCR_VALUE)
			duty--;
		//set register
		*OCRB = duty;
	} else {
		return -1;
	}
	return 0;
}

/**
 * Writes a value to the PWM register. Note that the function will only
 * write out valid ranged values.
 * @param value place number directly into register
 * @return -1 if invalid data, 0 otherwise
 */
int PWM_writeOCRA(unsigned int value){
	if(value > MAX_OCR_VALUE){
		return -1;
	}
	*OCRA = value;
	return 0;
}

/**
 * Writes a value to the PWM register. Note that the function will only
 * write out valid ranged values.
 * @param value place number directly into register
 * @return -1 if invalid data, 0 otherwise
 */
int PWM_writeOCRB(unsigned int value){
	if(value > MAX_OCR_VALUE){
		return -1;
	}
	*OCRB = value;
	return 0;
}

/**
 * Reads from the OCRA register in counts.
 * @return count value.
 */
int PWM_readOCRA(){
	return *OCRA;
}

/**
 * Reads from the OCRB register in counts.
 * @return count value.
 */
int PWM_readOCRB(){
	return *OCRB;
}
