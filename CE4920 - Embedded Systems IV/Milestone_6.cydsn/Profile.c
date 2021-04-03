/**
 * Profile
 * @author Curt Henrichs
 * @date 9-13-17
 *
 * Profile module defines workout profile singletons as profile objects for
 * the treadmill application. Control of motor is dependent on the profile's
 * calculation, which is based on a heartbeat compensated PWM duty cycle.
 */

//=============================================================================
//                                  Libraries
//=============================================================================

#include "Profile.h"
#include <project.h>
#include <string.h>

//=============================================================================
//                             Private Attributes
//=============================================================================

/**
 * Array of singleton profiles that is selectable by the profiles enumeration
 */
static Profile_t profiles[_END_OF_PROFILE_LIST];

//=============================================================================
//                        Public Function Implementation
//=============================================================================

/**
 * Profile singleton intialization function.
 */
void profile_init(void){
    //generate off object
    Profile_t* off = &profiles[PROFILE_OFF];
    off->motor_heartbeat_shift_down = 0;
    off->motor_heartbeat_shift_up = 0;
    off->warn_heartbeat_val = 0;
    off->pwm_base = 0;
    strcpy(off->name,"Off");
    strcpy(off->detail,"motor is off");
    
    //generate casual object
    Profile_t* casual = &profiles[PROFILE_CASUAL];
    casual->motor_heartbeat_shift_down = -51;
    casual->motor_heartbeat_shift_up = 13;
    casual->warn_heartbeat_val = 25;
    casual->pwm_base = CASUAL_MOTOR_RATE;
    strcpy(casual->name,"Casual");
    strcpy(casual->detail,"leisurely stroll");
    
    //generate moderate object
    Profile_t* moderate = &profiles[PROFILE_MODERATE];
    moderate->motor_heartbeat_shift_down = -26;
    moderate->motor_heartbeat_shift_up = 26;
    moderate->warn_heartbeat_val = 25;
    moderate->pwm_base = MODERATE_MOTOR_RATE;
    strcpy(moderate->name,"Moderate");
    strcpy(moderate->detail,"fit and toned");
    
    //generate intense object
    Profile_t* intense  = &profiles[PROFILE_INTENSE];
    intense->motor_heartbeat_shift_down = -13;
    intense->motor_heartbeat_shift_up = 26;
    intense->warn_heartbeat_val = 25;
    intense->pwm_base = INTENSE_MOTOR_RATE;
    strcpy(intense->name,"Intense");
    strcpy(intense->detail,"feel the burn");
    
    //generate extreme object
    Profile_t* extreme = &profiles[PROFILE_EXTREME];
    extreme->motor_heartbeat_shift_down = 0;
    extreme->motor_heartbeat_shift_up = 51;
    extreme->warn_heartbeat_val = 25;
    extreme->pwm_base = EXTREME_MOTOR_RATE;
    strcpy(extreme->name,"Extreme");
    strcpy(extreme->detail,"why?");
}

/**
 * Profile singleton selection function
 * @param profile enumeration of singletons
 * @return pointer to profile singleton given selection parameter. NULL if
 *         invalid.
 */
const Profile_t* profile_get(Profile_e profile){
    const Profile_t* retVal = NULL;
    if(profile < _END_OF_PROFILE_LIST){
        retVal = &profiles[profile];
    }
    return retVal;
}

/**
 * Profile menu display function
 * @param profile to display on LCD display
 */
void profile_menu_option(const Profile_t* profile){
    if(profile != NULL){
        display_profile(profile->name,profile->detail);
    }
}