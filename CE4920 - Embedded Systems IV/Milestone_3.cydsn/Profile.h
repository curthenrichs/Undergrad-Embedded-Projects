/**
 * Profile
 * @author Curt Henrichs
 * @date 9-13-17
 *
 * Profile module defines workout profile singletons as profile objects for
 * the treadmill application. Control of motor is dependent on the profile's
 * calculation, which is based on a heartbeat compensated PWM duty cycle.
 */

#ifndef PROFILE_H
#define PROFILE_H
 
//=============================================================================
//                                  Libraries
//=============================================================================
    
#include "Display.h"
#include <stdint.h>
 
//=============================================================================
//                       Constant and Macro Definitions
//=============================================================================

//! Max string size for name parameter
#define PROFILE_NAME_STR_LENGTH (MAX_PROFILE_NAME_LENGTH+1)
//! Max string size for detail parameter
#define PROFILE_DETAIL_STR_LENGTH (MAX_PROFILE_DETAIL_LENGTH+1)
    
/**
 * Enumeration of all profile cases
 */
typedef enum Profile_e {
    PROFILE_OFF = 0,    //! Not moving state
    PROFILE_CASUAL,     //! Easy workout for relaxation
    PROFILE_MODERATE,   //! Moderate workout for fitness
    PROFILE_INTENSE,    //! Hard-core workout, for humans
    PROFILE_EXTREME,    //! Hard-core workout, with max intensity
    
    //private enum to describe end of list
    _END_OF_PROFILE_LIST //! Not selectable enumeration (used for count)
} Profile_e;

#define CASUAL_MOTOR_RATE   33 //! Casual profile PWM rate
#define MODERATE_MOTOR_RATE 64 //! Moderate profile PWM rate
#define INTENSE_MOTOR_RATE  128 //! Intense profile PWM rate
#define EXTREME_MOTOR_RATE  192 //! Extreme profile PWM rate
    
//=============================================================================
//                         Data Structure Declaration
//=============================================================================
    
/**
 * Profile object defines the characteristics of each workout profile. 
 */
typedef struct Profile {
    int8_t motor_heartbeat_shift_up;    //! Heartbeat increased by 10%
    int8_t motor_heartbeat_shift_down;  //! Heartbeat decreased by 10%
    int8_t warn_heartbeat_val;          //! Hartbeat increased past threshold
    uint16_t pwm_base;                  //! Starting PWM value pre-compensation
    char name[PROFILE_NAME_STR_LENGTH]; //! Name of profile as string
    char detail[PROFILE_DETAIL_STR_LENGTH]; //! Description of profile string
} Profile_t;

//=============================================================================
//                         Public Function Prototypes
//=============================================================================

/**
 * Profile singleton intialization function.
 */
void profile_init(void);
/**
 * Profile singleton selection function
 * @param profile enumeration of singletons
 * @return pointer to profile singleton given selection parameter. NULL if
 *         invalid.
 */
const Profile_t* profile_get(Profile_e profile);
/**
 * Profile menu display function
 * @param profile to display on LCD display
 */
void profile_menu_option(const Profile_t* profile);

#endif
    