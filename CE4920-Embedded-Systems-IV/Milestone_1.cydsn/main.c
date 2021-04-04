/**
 * Main
 * @author Curt Henrichs
 * @date 9-13-17
 *
 * Heartbeat compenstated treadmill, fitness application. Application uses a
 * state machine to reset program variables, wate for user input, motor 
 * rampup, heartbeat monitor plus motor compensation, and motor stop state.
 */

//=============================================================================
//                                 Library
//=============================================================================

#include <project.h>
#include <stdio.h>
#include <stdbool.h>

#include "Display.h"
#include "Heartbeat.h"
#include "Profile.h"
#include "Music.h"

//=============================================================================
//                        Constant and Macro Definitions
//=============================================================================

#define RST_ACTIVE_STATE    0       //! Logic level of active for HW_RST_BTN

#define NO_TOUCH_ON_SLIDER  0xFFFF  //! State returned when slider

/**
 * Application state machine, state enumeration
 */
typedef enum States_e {
    RESET,              //! Initalization state of application, welcome user
    WAIT_FOR_WAKE,      //! Waits for user to wakeup treadmill
    WAIT_FOR_PROFILE,   //! Waits for user to select a profile, drives menu
    RAMPUP,             //! Ramps the motor to profile setting
    MONITOR,            //! Adaptivly controls motor based on heartbeat
    STOP,               //! Stops the motor and profile operation
} States_e;

/**
 * Startup tune for welcoming user
 */
const Note_t STARTUP_TUNE[] = {\
    {NOTE_C,500},\
    {NOTE_E,1000},\
    {NOTE_C,500},\
    {NOTE_G,1000},\
    {NOTE_A,1000},\
    {NOTE_C_S,500},\
    {NOTE_B,500}};

/**
 * Enumeration of the events that the user can trigger with the slider
*/
typedef enum Slider_e {
    SLIDE_OFF,      //! User is not touching slider (default)
    SLIDE_LEFT,     //! User is moving left on slider
    SLIDE_RIGHT,    //! User is moving right on slider
    SLIDE_NO_MATCH, //! User has 
    SLIDE_NOT_DETERMINED //! Not enough data to determine state yet
}Slider_e;   

#define SLIDE_RIGHT_START   20 //! Origin requirment for right swipe
#define SLIDE_RIGHT_END     80 //! End requirement for right swipe
#define SLIDE_LEFT_START    80 //! Origin requirement for left swipe
#define SLIDE_LEFT_END      20 //! End requirement for left swipe

//=============================================================================
//                          Data Structure Declaration
//=============================================================================

/**
 * Slider processing object detects rising and falling edge of data to trigger
 * processing of user slide event as either left or right.
 */
typedef struct Slider_Event {
    uint16_t prev_val;  //! Previous value recorded from slider
    int8_t dir;         //! Current direction of swipe is either -1, 0, 1
    uint8_t origin;     //! Origin x value of swipe
} Slider_Event_t;

//=============================================================================
//                             Private Attributes
//=============================================================================

/**
 * Application state machine current state, defaults to RESET
 */
static States_e state = RESET;
/**
 * Profile Selection current page index
 */
static uint8_t profile_page_index = 0;
/**
 * Profile Page indexed is already being displayed 
 */
static bool profile_page_displayed = false;
/**
 * Profile next button has not been released yet, therefore don't consider
 * active state as second press.
 */
static bool prev_press_right = false;
/**
 * Profile selected by user to control treadmill. Defaults to NULL if no 
 * selection is made.
 */
static const Profile_t* active_profile = NULL;

static Slider_Event_t slider_event;

//=============================================================================
//                         Private Function Prototypes
//=============================================================================

/**
 * Reset state for application state machine responsible for setting 
 * application variables and sending a welcome message to the user
 * @return next state for application, WAIT_WAKE
 */
static inline States_e _reset(void);
/**
 * Wait state for waking up the treadmill. Will need user to send activation
 * sequence before a transition to waiting for profile selection
 * @return next state for application, WAIT_WAKE or WAIT_PROFILE
 */
static inline States_e _wait_for_wake(void);
/**
 * Wait state for selecting which profile to run on treadmill.
 * @return next state for application, WAIT_PROFILE, RAMPUP, or STOP
 */
static inline States_e _wait_for_profile(void);
/**
 * Rampup motor to the profile setpoint regardless of heartbeat input.
 * @return next state for application, MONITOR, STOP
 */
static inline States_e _rampup(void);
/**
 * Monitor state records user's heartbeat. Compares heartbeat trend to 
 * profile response to control the speed of the motor.
 * @return next state for application, MONITOR, STOP
 */
static inline States_e _monitor(void);
/**
 * Stop state reduces the speed from current pwm speed to zero. 
 * Ends session for user and goes to sleep
 * @return WAIT_WAKE
 */
static inline States_e _stop(void);
/**
 * Gets the current state of the slider user event
 * @return enumeration of slider user events
 */
static inline Slider_e _check_slider_state(void);

//=============================================================================
//                                  Main
//=============================================================================

/**
 * Main function will implement application state machine to run heartbeat
 * tracker treadmill.
 */
int main(void){
    //hardware initialization
    CyGlobalIntEnable;
    CapSense_Start();
    music_init();
    display_init();
    profile_init();
    CapSense_InitializeAllBaselines();
    
    while(true){
        //update capacitive touch user input
        if(!CapSense_IsBusy()){
            CapSense_UpdateEnabledBaselines();
            CapSense_ScanEnabledWidgets();
        }
        
        //update reset button input
        if(RST_Read() == RST_ACTIVE_STATE){
            state = RESET;
        }
        
        //state machine update
        switch(state){
            case RESET:
                state = _reset();
                break;
            case WAIT_FOR_WAKE:
                state = _wait_for_wake();
                break;
            case WAIT_FOR_PROFILE:
                state = _wait_for_profile();
                break;
            case RAMPUP:
                state = _rampup();
                break;
            case MONITOR:
                state = _monitor();
                break;
            case STOP:
                state = _stop();
                break;
            default:
                state = RESET;
                break;
        }
    }
}

//=============================================================================
//                        Private Function Implementation
//=============================================================================

/**
 * Reset state for application state machine responsible for setting 
 * application variables and sending a welcome message to the user
 * @return next state for application, WAIT_WAKE
 */
static inline States_e _reset(void){
    //set default off states
    PWM_WriteCompare(0);
    active_profile = NULL;
    slider_event.dir = 0;
    slider_event.prev_val = NO_TOUCH_ON_SLIDER;
    slider_event.origin = 0;
    
    //welcome user
    music_set_tune(STARTUP_TUNE,(sizeof(STARTUP_TUNE)/sizeof(Note_t)));
    display_welcome();
    while(music_is_playing()){/* Block until welcome is done */};
    display_clear();
    return WAIT_FOR_WAKE;
}

/**
 * Wait state for waking up the treadmill. Will need user to send activation
 * sequence before a transition to waiting for profile selection
 * @return next state for application, WAIT_WAKE or WAIT_PROFILE
 */
static inline States_e _wait_for_wake(void){
    States_e retVal = WAIT_FOR_WAKE;
    Slider_e pos = _check_slider_state();
    if(pos == SLIDE_RIGHT){
        retVal = WAIT_FOR_PROFILE;
        profile_page_displayed = false;
        profile_page_index = 0;
    }
    return retVal;
}

/**
 * Wait state for selecting which profile to run on treadmill.
 * @return next state for application, WAIT_PROFILE, RAMPUP, or STOP
 */
static inline States_e _wait_for_profile(void){
    States_e retVal = WAIT_FOR_PROFILE;
    //display page information on LCD
    if(!profile_page_displayed){
        switch(profile_page_index){
            case 0:
                display_profile_title();
                break;
            case 1:
            case 2:
            case 3:
            case 4:
                profile_menu_option(profile_get(profile_page_index));
                break;
        }
        profile_page_displayed = true;
    }
    
    //check user input for selection
    if(CapSense_CheckIsWidgetActive(CapSense_LEFT__BTN) 
            && profile_page_index != 0 
            && !CapSense_CheckIsWidgetActive(CapSense_RIGHT__BTN)){
        retVal = RAMPUP;
        active_profile = profile_get(profile_page_index);
        display_clear();
        
        //flash profile for user to see selection
        char temp[LCD_WIDTH+1];
        char padding[LCD_WIDTH+1];
        uint8_t name_size = strlen(active_profile->name);
        int8_t pad_count = (LCD_WIDTH - name_size)/2;
        strcpy(padding,"");
        for(int8_t i=0; i<pad_count; ++i){
            strcat(padding," ");   
        }
        sprintf(temp,"%s%s",padding,active_profile->name);
        display_flash_text(temp,"",4,500,500);
    }   
    //check user input for toggle
    else if(CapSense_CheckIsWidgetActive(CapSense_RIGHT__BTN) && !prev_press_right){
        prev_press_right = true;
        profile_page_displayed = 0;
        profile_page_index = (profile_page_index + 1) % 5;
    }else if(!CapSense_CheckIsWidgetActive(CapSense_RIGHT__BTN)){
        prev_press_right = false;
    }
    
    //check for stop slide event which cancels selection
    Slider_e pos = _check_slider_state();
    if(pos == SLIDE_LEFT){
        display_clear();
        retVal = WAIT_FOR_WAKE;   
    }

    return retVal;
}

/**
 * Rampup motor to the profile setpoint regardless of heartbeat input.
 * @return next state for application, MONITOR, STOP
 */
static inline States_e _rampup(void){
    States_e retVal = RAMPUP;
    display_clear();
    LCD_Position(0,0);
    LCD_PrintString("Ramp");
    CyDelay(2000);
    
    //TODO write this function
    retVal = MONITOR;
    
    /*
    //check for stop slide event
    Slider_e pos = _check_slider_state();
    if(pos == SLIDE_LEFT){
        retVal = STOP;   
    }
    */
    
    return retVal;
}

/**
 * Monitor state records user's heartbeat. Compares heartbeat trend to 
 * profile response to control the speed of the motor.
 * @return next state for application, MONITOR, STOP
 */
static inline States_e _monitor(void){
    display_clear();
    LCD_Position(0,0);
    LCD_PrintString("Monitor");
    CyDelay(2000);
    
    //TODO write this function
    
    return STOP;
}

/**
 * Stop state reduces the speed from current pwm speed to zero. 
 * Ends session for user and goes to sleep
 * @return WAIT_WAKE
 */
static inline States_e _stop(void){
    display_clear();
    LCD_Position(0,0);
    LCD_PrintString("Stop");
    CyDelay(2000);
    display_clear();
    
    //TODO write this function
    
    return WAIT_FOR_WAKE;
}

/**
 * Gets the current state of the slider user event
 * @return enumeration of slider user events
 */
static inline Slider_e _check_slider_state(void){
    Slider_e retVal;
    uint16_t pos = CapSense_GetCentroidPos(CapSense_SLIDER__LS);

    //if start of button press
    if(slider_event.prev_val == NO_TOUCH_ON_SLIDER && pos != NO_TOUCH_ON_SLIDER){
        retVal = SLIDE_NOT_DETERMINED;
        slider_event.dir = 0;
        slider_event.origin = pos;
    }
    //else if up event
    else if(slider_event.prev_val != NO_TOUCH_ON_SLIDER && pos == NO_TOUCH_ON_SLIDER){
        //if slide a left, right, or no match?
        if(slider_event.origin <= SLIDE_RIGHT_START && slider_event.prev_val >= SLIDE_RIGHT_END){
            retVal = SLIDE_RIGHT;
        }else if(slider_event.origin >= SLIDE_LEFT_START && slider_event.prev_val <= SLIDE_LEFT_END){
            retVal = SLIDE_LEFT;
        }else{
            retVal = SLIDE_OFF;
        }
    }
    //else if not touching consistent event
    else if(slider_event.prev_val == NO_TOUCH_ON_SLIDER && pos == NO_TOUCH_ON_SLIDER){
        retVal = SLIDE_OFF;
    }
    //else currently sliding
    else{
        retVal = SLIDE_NOT_DETERMINED;
        
        //if user starts moving in the reverse direction, reset origin
        int8_t curr_dir = pos - slider_event.prev_val;
        curr_dir = (curr_dir > 0) ? 1 : (curr_dir < 0) ? -1 : 0;
        if(curr_dir != 0 && curr_dir != slider_event.dir){
            slider_event.dir = curr_dir;
            slider_event.origin = pos;
        }
    }
    slider_event.prev_val = pos;

    return retVal;
}
