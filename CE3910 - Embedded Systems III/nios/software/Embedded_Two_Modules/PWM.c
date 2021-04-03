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
#include "GENERIC_IO.h"

//=============================================================================
//                          Constant and Macro Declaration
//=============================================================================

//100% Constant
#define MAX_OCR_VALUE 20000

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
int PWMwriteOCRA(int dutyCycle){
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
int PWMwriteOCRB(int dutyCycle){
	if((dutyCycle <= 100) && (dutyCycle >= 0)){
		//Convert from percent to the acceptable OCR value with max 20000
		int duty = dutyCycle * (MAX_OCR_VALUE / 100);
		//We can never actually get to 20000 based on my CE2800 notes
		if(duty == MAX_OCR_VALUE)
			duty--;
		//set register
		*OCRB = duty;
	} else {
		return -1; // return an error
	}
	// no error
	return 0;
}
