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
//                               Function Declaration
//=============================================================================

/**
 * Writes a duty cycle to the PWM register
 * @param dutyPercent Duty cycle, given as a percent from 0 to 100. (anything
 *        lower or higher will be ignored and no change will occur to register
 * @return Either a 0 if there was no error (Bad input) or a EOF (-1) if the
 *         input was out of bounds.
 */
int PWMwriteOCRA(int dutyPercent);

/**
 * Writes a duty cycle to the PWM register
 * @param dutyPercent Duty cycle, given as a percent from 0 to 100. (anything
 *        lower or higher will be ignored and no change will occur to register
 * @return Either a 0 if there was no error (Bad input) or a EOF (-1) if the
 *         input was out of bounds.
 */
int PWMwriteOCRB(int dutyPercent);

#endif
