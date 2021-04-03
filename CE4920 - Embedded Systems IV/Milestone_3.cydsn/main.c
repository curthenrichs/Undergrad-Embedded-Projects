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
#include "Uart.h"
#include "UserIn.h"

//=============================================================================
//                        Constant and Macro Definitions
//=============================================================================

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
 * Heartbeat exceeded threshold warning to user
 */
const Note_t WARNING_TUNE[] = {\
    {NOTE_A,150},\
    {NOTE_PAUSE,100},\
    {NOTE_A,150},\
    {NOTE_PAUSE,75}};

#define RAMP_TIME_STEP      50   //! Delay between ramp state operations
#define MONITOR_TIME_STEP   50   //! Delay between monitor state operations
#define STOP_TIME_STEP      50   //! Delay between stop state operations

#define RAMP_LOOP_COUNT     160  //! Iterations of ramp before transition
#define STOP_LOOP_COUNT     40   //! Iterations of stop before transition

#define STOP_PWM_BASE       0    //! Motor PWM stop value
#define STOP_PWM_OFFSET     2.5f //! Fudge factor to allow for truncate to zero
#define MONITOR_DUTY_STEP   0.5f //! Iterations to ramp to desired PWM

#define HEARTBEAT_VALID_THRESHOLD 5 //! Less than this value is considred off

//=============================================================================
//                         Data Structure Declaration
//=============================================================================

/**
 * State Machine - Profile State attributes
 */
typedef struct State_Machine_Profile{
    uint8_t page_index;     //! Current page of profile menu
    bool page_displayed;    //! Flag noting that the page is rendered
    bool prev_press_right;  //! Flag noting that the next button is being held
}State_Machine_Profile_t;

/**
 * State Machine - Ramp State attributes
 */
typedef struct State_Machine_Ramp{
    uint8_t count;          //! Counter to cause transiton to next state
    float motor_increment;  //! Incrementer step to profile set point
}State_Machine_Ramp_t;

/**
 * State Machine - Stop State attributes
 */
typedef struct State_Machine_Stop{
    uint8_t count;          //! Counter to cause transition to next state
    float motor_decrement;  //! Decrement step to profile set point
}State_Machine_Stop_t;

/**
 * State Machine - Monitor State attributes
 */
typedef struct State_Machine_Monitor{
    bool startFlag;          //! Flag set before entry into state
    uint16_t init_heartbeat; //! Inital heartbeat for monitor state
}State_Machine_Monitor_t;

/**
 * State Machine attributes as a collection of all states specific attrubutes
 */
typedef struct State_Machine {
    States_e state;         //! Current state of state machine
    const Profile_t* active_profile; //! Selected profile for monitor state
    State_Machine_Profile_t profile; //! Profile Select State Attributes
    State_Machine_Monitor_t monitor; //! Monitor heartbeat State attributes
    State_Machine_Ramp_t ramp; //! Ramp State Attributes
    State_Machine_Stop_t stop; //! Stop State attributes
    float current_duty;     //! Current duty cycle applied to the motor
}State_Machine_t;

//=============================================================================
//                             Private Attributes
//=============================================================================

/**
 * Capacitive slider object
 */
static Slider_Event_t slider_event;
/**
 * USB UART transmission buffer
 */
static char txStr[25];
/**
 * State machine attributes
 */
static State_Machine_t state_machine;

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
 * Shifts a value from base to target via step value. Needs to be 
 * iterated to accomplish the ramp to target.
 * @param target is the goal value
 * @param base is the current value
 * @param step is the increment to get to target
 * @return next current value to approach target or target if reached
 */
static inline float _shift_value(float target, float base, float step);
/**
 * Stop state reduces the speed from current pwm speed to zero. 
 * Ends session for user and goes to sleep
 * @return WAIT_WAKE
 */
static inline States_e _stop(void);

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
    heartbeat_init(HEARTBEAT_SOURCE_EXTERNAL);
    music_init();
    display_init();
    profile_init();
    uart_init();
    userIn_init();
    PWM_Start();
    
    //software initalization
    state_machine.state = RESET;
    
    //run state machine loop indefinitly
    while(true){
        //update capacitive touch user input
        if(!CapSense_IsBusy()){
            CapSense_UpdateEnabledBaselines();
            CapSense_ScanEnabledWidgets();
        }
        
        //update usb interface
        uart_update();            
        
        //state machine update
        switch(state_machine.state){
            case RESET:
                state_machine.state = _reset();
                break;
            case WAIT_FOR_WAKE:
                state_machine.state = _wait_for_wake();
                break;
            case WAIT_FOR_PROFILE:
                state_machine.state = _wait_for_profile();
                break;
            case RAMPUP:
                state_machine.state = _rampup();
                break;
            case MONITOR:
                state_machine.state = _monitor();
                break;
            case STOP:
                state_machine.state = _stop();
                break;
            default:
                state_machine.state = RESET;
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
    PWM_WriteCompare(STOP_PWM_BASE);
    state_machine.active_profile = NULL;
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
    Slider_e pos = userIn_check_slider_state(&slider_event,CapSense_SLIDER__LS);
    if(pos == SLIDE_RIGHT){
        retVal = WAIT_FOR_PROFILE;
        state_machine.profile.page_displayed = false;
        state_machine.profile.page_index = 0;
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
    if(!state_machine.profile.page_displayed){
        switch(state_machine.profile.page_index){
            case 0:
                display_profile_title();
                break;
            case 1:
            case 2:
            case 3:
            case 4:
                profile_menu_option(profile_get(state_machine.profile.page_index));
                break;
        }
        state_machine.profile.page_displayed = true;
    }
    
    //check user input for selection
    if(userIn_check_button_state(CapSense_LEFT__BTN) 
            && state_machine.profile.page_index != 0 
            && !userIn_check_button_state(CapSense_RIGHT__BTN)){
        retVal = RAMPUP;
        state_machine.active_profile = profile_get(state_machine.profile.page_index);
        display_clear();
        
        //flash profile for user to see selection
        char temp[LCD_WIDTH+1];
        char padding[LCD_WIDTH+1];
        uint8_t name_size = strlen(state_machine.active_profile->name);
        int8_t pad_count = (LCD_WIDTH - name_size)/2;
        strcpy(padding,"");
        for(int8_t i=0; i<pad_count; ++i){
            strcat(padding," ");   
        }
        sprintf(temp,"%s%s",padding,state_machine.active_profile->name);
        display_flash_text(temp,"",4,250,250);
        
        //setup for ramp state
        display_clear();
        heartbeat_set_use_LED(true);
        state_machine.ramp.count = 0;
        state_machine.ramp.motor_increment = 0;
        state_machine.current_duty = 0;
    }   
    //check user input for toggle
    else if(userIn_check_button_state(CapSense_RIGHT__BTN) 
            && !state_machine.profile.prev_press_right){
        state_machine.profile.prev_press_right = true;
        state_machine.profile.page_displayed = 0;
        state_machine.profile.page_index = (state_machine.profile.page_index + 1) % 5;
    }else if(!userIn_check_button_state(CapSense_RIGHT__BTN)){
        state_machine.profile.prev_press_right = false;
    }
    
    //check for stop slide event which cancels selection
    Slider_e pos = userIn_check_slider_state(&slider_event,CapSense_SLIDER__LS);
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
    
    //on inital rampup call, set LCD information, calculate motor increment
    if(0 == state_machine.ramp.count){
        display_clear();
        display_ramp_text();
        uint8_t target_pwm = state_machine.active_profile->pwm_base;
        state_machine.ramp.motor_increment = (target_pwm) 
                                            / (RAMP_LOOP_COUNT * 1.0f);
    }
    
    //increment counter, if end of count then transition to next state
    state_machine.ramp.count++;
    if(state_machine.ramp.count >= RAMP_LOOP_COUNT){
        //setup for monitor state
        retVal = MONITOR;
        state_machine.monitor.init_heartbeat = heartbeat_get_bpm();
        state_machine.monitor.startFlag = true;
    }
    
    //adjust motor speed
    state_machine.current_duty += state_machine.ramp.motor_increment;
    if(state_machine.current_duty > state_machine.active_profile->pwm_base){
        state_machine.current_duty = state_machine.active_profile->pwm_base;
    }
    PWM_WriteCompare((uint8_t)(state_machine.current_duty));
    
    //check for stop slide event
    Slider_e pos = userIn_check_slider_state(&slider_event,CapSense_SLIDER__LS);
    if(pos == SLIDE_LEFT){
        retVal = STOP;   
        //prepare state machine for stop state
        state_machine.stop.count = 0;
        state_machine.stop.motor_decrement = 0;
    }
    
    CyDelay(RAMP_TIME_STEP);
    return retVal;
}

/**
 * Monitor state records user's heartbeat. Compares heartbeat trend to 
 * profile response to control the speed of the motor.
 * @return next state for application, MONITOR, STOP
 */
static inline States_e _monitor(void){
    States_e retVal = MONITOR;

    uint16_t current_heartbeat = heartbeat_get_bpm();
    sprintf(txStr,"%3d ",current_heartbeat);
    uart_write(txStr,strlen(txStr));
    
    //display heartbeat on LCD
    if(state_machine.monitor.startFlag){
        state_machine.monitor.startFlag = false;
        display_profile(state_machine.active_profile->name,"Heartbeat:");
    }
    display_print_string(1,11,txStr);
    
    //calculate error from inital to current heartbeat
    float heartbeat_diff = (((float)(current_heartbeat) 
                            / (float)(state_machine.monitor.init_heartbeat)) 
                            * 100.0f) - 100;
    
    //compensate motor state  
    int pwm_steps = heartbeat_diff / 10 * ((heartbeat_diff < 0) ? -1 : 1);
    float pwm_step_size = 0;
    if(heartbeat_diff > 0){
        pwm_step_size = state_machine.active_profile->motor_heartbeat_shift_down;
    }else if(heartbeat_diff < 0){
        pwm_step_size = state_machine.active_profile->motor_heartbeat_shift_up;
    }
    float pwm_target = state_machine.active_profile->pwm_base 
            + pwm_steps * pwm_step_size;
    if(pwm_target > 255){
        pwm_target = 255;   
    }else if(pwm_target < 0){
        pwm_target = 0;   
    }
    state_machine.current_duty = _shift_value(pwm_target,
            state_machine.current_duty,MONITOR_DUTY_STEP);
    PWM_WriteCompare((uint8_t)(state_machine.current_duty));
    
    //check if alarm needs to be set
    if((heartbeat_diff >= 25) && !music_is_playing()){
        music_set_tune(WARNING_TUNE,sizeof(WARNING_TUNE)/sizeof(Note_t));
    }else if(heartbeat_diff < 25){
        music_stop_playing();      
    }
    
    //check for stop slide event
    Slider_e pos = userIn_check_slider_state(&slider_event,CapSense_SLIDER__LS);
    if(pos == SLIDE_LEFT || current_heartbeat < HEARTBEAT_VALID_THRESHOLD){
        retVal = STOP;   
        //prepare state machine for stop state
        state_machine.stop.count = 0;
        state_machine.stop.motor_decrement = 0;
    }
    
    CyDelay(MONITOR_TIME_STEP);
    return retVal;
}

/**
 * Shifts a value from base to target via step value. Needs to be 
 * iterated to accomplish the ramp to target.
 * @param target is the goal value
 * @param base is the current value
 * @param step is the increment to get to target
 * @return next current value to approach target or target if reached
 */
static inline float _shift_value(float target, float base, float step){
    float retVal = 0;
    step = step * ((target < base) ? -1 : 1);
    if(step >= 0 && (base + step) > target){
        retVal = target;   
    }else if(step < 0 && (base + step) < target){
        retVal = target;   
    }else {
        retVal = base + step;
    }
    return retVal;
}

/**
 * Stop state reduces the speed from current pwm speed to zero. 
 * Ends session for user and goes to sleep
 * @return WAIT_WAKE
 */
static inline States_e _stop(void){
    States_e retVal = STOP;
    
    //on initial runthrough, display message and calculate motor step
    if(state_machine.stop.count == 0){
        heartbeat_set_use_LED(false);
        display_clear();
        display_stop_text();
        
        uint8_t target_pwm = state_machine.current_duty;
        state_machine.stop.motor_decrement = -1 * (target_pwm) 
                                            / (STOP_LOOP_COUNT * 1.0f);
    }
    
    //increment the counter, if end of count then transition to next state
    state_machine.stop.count++;
    if(state_machine.stop.count >= STOP_LOOP_COUNT){
        display_clear(); //clear message before entring wait state
        PWM_WriteCompare(0); //make sure zero is not just approached
        retVal = WAIT_FOR_WAKE;
    }
    
    //adjust motor speed
    state_machine.current_duty += state_machine.stop.motor_decrement;
    uint8_t target_pwm = 0;
    if(state_machine.current_duty < (STOP_PWM_BASE + STOP_PWM_OFFSET)){
        target_pwm = STOP_PWM_BASE;
    }else{
        target_pwm = (uint8_t)(state_machine.current_duty);
    }
    PWM_WriteCompare(target_pwm);
    
    CyDelay(STOP_TIME_STEP);
    return retVal;
}