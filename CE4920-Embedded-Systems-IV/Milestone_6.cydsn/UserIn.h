/**
 * UserIn
 * @author Curt Henrichs
 * @date 9-23-17
 * 
 * User Input module abstracts all user input hardware into a common module.
 * The goal is to decouple the hardware API from the software application.
 * Currently only a CapSense device is used with two buttons and a slider.
 * Future input would need to extend this module.
 */

#ifndef USERIN_H
#define USERIN_H
    
//=============================================================================
//                                 Library
//=============================================================================

#include <stdint.h>
#include <stdbool.h>
    
//=============================================================================
//                        Constant and Macro Definitions
//=============================================================================

#define NO_TOUCH_ON_SLIDER  0xFFFF  //! State returned when slider

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

#define SLIDE_RIGHT_START   25 //! Origin requirment for right swipe
#define SLIDE_RIGHT_END     75 //! End requirement for right swipe
#define SLIDE_LEFT_START    75 //! Origin requirement for left swipe
#define SLIDE_LEFT_END      25 //! End requirement for left swipe

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
//                         Public Function Prototypes
//=============================================================================

/**
 * User input initalization function will start any hardware necessary to
 * process user input events.
 */
void userIn_init(void);
/**
 * User input process slider function will process the capacitive touch slider
 * for user input.
 * @param obj is pointer to Slider_Event_t object to assist in processing of
 *        slider data.
 * @param id is slider to check
 * @return current state of the slider
 */
Slider_e userIn_check_slider_state(Slider_Event_t* obj, uint8_t id);
/**
 * User input process button event will check a button for state of hardware.
 * @param id is the button to check
 * @return boolean where true is active state of button, false otherwise
 */
bool userIn_check_button_state(uint8_t id);

#endif
    