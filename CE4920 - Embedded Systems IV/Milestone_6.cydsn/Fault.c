/**
 * Fault
 * @author Curt Henrichs
 * @date 10-17-17
 *
 * Fault detection module will determine if any component have failed. If
 * failure occured then the watchdog epxire unless system has entered a critical
 * section, where then the main loop takes control of the watchdog.
 */

//=============================================================================
//                                  Libraries
//=============================================================================
 
#include "Fault.h"
#include <project.h>

//=============================================================================
//                         Interrupt Handler Prototypes
//=============================================================================

/**
 * Fault Detection ISR triggered on watchdog feed timer overflow. ISR will 
 * check for all possible faults and if a fault occurs, it will cause the
 * system to reset.
 */
CY_ISR_PROTO(fault_detection_isr);

//=============================================================================
//                             Private Attributes
//=============================================================================

/**
 * Flag to signal main program has entered a critical section where watchdog
 * should not cause immediate reset.
 */
static volatile bool _critical_section = false;

//=============================================================================
//                         Public Function Implementation
//=============================================================================

/**
 * Initialization function for fault detection software module
 */
void fault_init(void){
    //start watchdog timer
    Watchdog_Timer_Overflow_Int_StartEx(fault_detection_isr);
    Watchdog_Timer_Start();
    
    //start watchdog
    CyWdtStart(CYWDT_128_TICKS,CYWDT_LPMODE_NOCHANGE);
}

/**
 * Checks hardware signals noting component failure.
 * @return true if a module has failed, false if module 
 */
bool fault_check_for_failure(void){
    return  !(Heartbeat_COP_Read() && Motor_COP_Read() && UserInput_COP_Read());
}

/**
 * Generates the fault code associated to the components failed.
 * result is a bit field that follows form
 * b8 : 0
 * b7 : 0
 * b6 : 0
 * b5 : 0
 * b4 : 0
 * b3 : 1 if input failed else 0
 * b2 : 1 if motor failed else 0
 * b1 : 1 if heartbeat failed else 0
 * @return error code as detailed above
 */
uint8_t fault_code(void){
    uint8_t code = 0x00;
    code += (!Heartbeat_COP_Read()) << 0;
    code += (!Motor_COP_Read()) << 1;
    code += (!UserInput_COP_Read()) << 2;
    return code;
}

/**
 * Signals to the fault module whether to supress watchdog expiration even if
 * component has failed. Ideally used when main loop is interacting with the 
 * motor, which needs to be slowly backed off.
* @param s is boolean signaling to module whether if in critical section
 */
void fault_in_critical_section(bool s){
    _critical_section = s;
}

//=============================================================================
//                       Interrupt Handler Implementation
//=============================================================================

/**
 * Fault Detection ISR triggered on watchdog feed timer overflow. ISR will 
 * check for all possible faults and if a fault occurs, it will cause the
 * system to reset.
 */
CY_ISR(fault_detection_isr){
    
    //feed watchdog only if no faults exist
    if(!fault_check_for_failure() || _critical_section){
        CyWdtClear();
    }
        
    //clear source of interrupt
    Watchdog_Timer_ReadStatusRegister();
    Watchdog_Timer_Overflow_Int_ClearPending();
}