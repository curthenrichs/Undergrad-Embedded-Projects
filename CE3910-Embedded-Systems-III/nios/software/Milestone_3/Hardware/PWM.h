/**
 * PWM Module
 * @author Curt Henrichs
 * @date 1-11-16, revised 3-10-17
 *
 * Definition of the PWM controller on the NIOS processor. This is used for the
 * servo motor system.
 */

#ifndef PWM_LIB_H_
#define PWM_LIB_H_

//=============================================================================
//                          Constant and Macro Declaration
//=============================================================================

//100% Constant
#define MAX_OCR_VALUE 20000

//=============================================================================
//                               Function Declaration
//=============================================================================

/**
 * Writes a duty cycle to the PWM register
 * @param dutyPercent Duty cycle, given as a percent from 0 to 100. (anything
 *        lower or higher will be ignored and no change will occur to register
 * @return Either a 0 if there was no error (Bad input) or a EOF (-1) if the
 *         input was out of bounds.
 */
int PWM_setDutyCycle_A(int dutyPercent);
/**
 * Writes a duty cycle to the PWM register
 * @param dutyPercent Duty cycle, given as a percent from 0 to 100. (anything
 *        lower or higher will be ignored and no change will occur to register
 * @return Either a 0 if there was no error (Bad input) or a EOF (-1) if the
 *         input was out of bounds.
 */
int PWM_setDutyCycle_B(int dutyPercent);
/**
 * Writes a value to the PWM register. Note that the function will only
 * write out valid ranged values.
 * @param value place number directly into register
 * @return -1 if invalid data, 0 otherwise
 */
int PWM_writeOCRA(unsigned int value);
/**
 * Writes a value to the PWM register. Note that the function will only
 * write out valid ranged values.
 * @param value place number directly into register
 * @return -1 if invalid data, 0 otherwise
 */
int PWM_writeOCRB(unsigned int value);
/**
 * Reads from the OCRA register in counts.
 * @return count value.
 */
int PWM_readOCRA();
/**
 * Reads from the OCRB register in counts.
 * @return count value.
 */
int PWM_readOCRB();

#endif
