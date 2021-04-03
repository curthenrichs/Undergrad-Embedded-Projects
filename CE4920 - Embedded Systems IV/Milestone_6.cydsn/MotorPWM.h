/**
 * MotorPWM
 * @author Curt Henrichs
 * @date 10-17-17
 *
 * Motor PWM module controls a single PWM signal that is intended to control
 * a motor. This module provides a layer of decoupling between the state
 * machine and the hardware. Note that this module does not auto-update the
 * state of the motor. Instead the client software must invoke this module
 * appropriately.
 */

#ifndef MOTOR_H
#define MOTOR_H
 
//=============================================================================
//                                  Libraries
//=============================================================================
    
#include <project.h>
#include <stdbool.h>
  
//=============================================================================
//                        Constant and Macro Defintions
//=============================================================================
    
#define MOTOR_STOP_PWM (0)  //! PWM value that represents off to motor
    
/**
 * Stops the PWM pulse to the motor
 */
#define motor_stop() (PWM_WriteCompare(MOTOR_STOP_PWM))
    
//=============================================================================
//                         Public Function Prototypes
//=============================================================================
    
/**
 * Initalizes the motor software and hardware modules
 */
void motor_init(void);
/**
 * Calculates a step toward the desired target PWM and sets the motor. Requires
 * external tracking of PWM current value. This function must be called in an
 * external loop to actually ramp the motor to speed.
 * @param target is the end PWM to ramp to. Note that his value must be
 *        bounded between 0 and 255 inclusively 
 * @param base is the current PWM of the motor. Note that his value must be
 *        bounded between 0 and 255 inclusively 
 * @param step is the increment (or decrement) to reach the target. Note that
 *        this value should be positive
 * @return base + step until it reaches target, then returns target.
 */
float motor_ramp(float target, float base, float step);

#endif
    