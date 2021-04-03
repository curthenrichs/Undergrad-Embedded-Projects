

#ifndef MOTOR_H
#define MOTOR_H
    
#include <project.h>
#include <stdbool.h>
    
#define MOTOR_STOP_PWM (0)
    
#define motor_stop() (PWM_WriteCompare(MOTOR_STOP_PWM))
    
void motor_init(void);

float motor_ramp(float target, float base, float step);

bool motor_get_fault_status(void);
    
#endif
    