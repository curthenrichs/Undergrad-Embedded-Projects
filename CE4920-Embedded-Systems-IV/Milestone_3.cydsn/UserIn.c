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

//=============================================================================
//                                 Library
//=============================================================================

#include "UserIn.h"
#include <project.h>

//=============================================================================
//                        Constant and Macro Definitions
//=============================================================================

#define CAPSENSE_SETTLE_TIME 100

//=============================================================================
//                        Public Function Implementation
//=============================================================================

/**
 * User input initalization function will start any hardware necessary to
 * process user input events.
 */
void userIn_init(void){
    CapSense_Start();
    CyDelay(CAPSENSE_SETTLE_TIME);
    CapSense_InitializeAllBaselines();
}

/**
 * User input process slider function will process the capacitive touch slider
 * for user input.
 * @param obj is pointer to Slider_Event_t object to assist in processing of
 *        slider data.
 * @param id is slider to check
 * @return current state of the slider
 */
Slider_e userIn_check_slider_state(Slider_Event_t* obj, uint8_t id){  
    //parameter check
    if(obj == NULL){
        return SLIDE_NO_MATCH;   
    }
    
    //process slider
    Slider_e retVal;
    uint16_t pos = CapSense_GetCentroidPos(id);

    //if start of button press
    if(obj->prev_val == NO_TOUCH_ON_SLIDER 
            && pos != NO_TOUCH_ON_SLIDER){
        retVal = SLIDE_NOT_DETERMINED;
        obj->dir = 0;
        obj->origin = pos;
    }
    //else if up event
    else if(obj->prev_val != NO_TOUCH_ON_SLIDER 
            && pos == NO_TOUCH_ON_SLIDER){
        //if slide a left, right, or no match?
        if(obj->origin <= SLIDE_RIGHT_START 
                && obj->prev_val >= SLIDE_RIGHT_END){
            retVal = SLIDE_RIGHT;
        }else if(obj->origin >= SLIDE_LEFT_START 
                && obj->prev_val <= SLIDE_LEFT_END){
            retVal = SLIDE_LEFT;
        }else{
            retVal = SLIDE_OFF;
        }
    }
    //else if not touching consistent event
    else if(obj->prev_val == NO_TOUCH_ON_SLIDER 
            && pos == NO_TOUCH_ON_SLIDER){
        retVal = SLIDE_OFF;
    }
    //else currently sliding
    else{
        retVal = SLIDE_NOT_DETERMINED;
        
        //if user starts moving in the reverse direction, reset origin
        int8_t curr_dir = pos - obj->prev_val;
        curr_dir = (curr_dir > 0) ? 1 : (curr_dir < 0) ? -1 : 0;
        if(curr_dir != 0 && curr_dir != obj->dir){
            obj->dir = curr_dir;
            obj->origin = pos;
        }
    }
    obj->prev_val = pos;

    return retVal;
}

/**
 * User input process button event will check a button for state of hardware.
 * @param id is the button to check
 * @return boolean where true is active state of button, false otherwise
 */
bool userIn_check_button_state(uint8_t id){
    return CapSense_CheckIsWidgetActive(id);
}