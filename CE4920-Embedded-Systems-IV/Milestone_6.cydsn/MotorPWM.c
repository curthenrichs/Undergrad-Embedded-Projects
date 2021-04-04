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

//=============================================================================
//                                  Libraries
//=============================================================================

#include "MotorPWM.h"

//=============================================================================
//                         Public Function Implementation
//=============================================================================

/**
 * Initalizes the motor software and hardware modules
 */
void motor_init(void){
    PWM_Start();   
}

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
float motor_ramp(float target, float base, float step){
    float duty = 0;
    step = step * ((target < base) ? -1 : 1);
    if(step >= 0 && (base + step) > target){
        duty = target;   
    }else if(step < 0 && (base + step) < target){
        duty = target;   
    }else {
        duty = base + step;
    }
    PWM_WriteCompare((uint8_t)(duty));
    return duty;
}