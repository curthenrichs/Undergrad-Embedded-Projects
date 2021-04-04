/**
 * Fault
 * @author Curt Henrichs
 * @date 10-17-17
 *
 * Fault detection module will determine if any component have failed. If
 * failure occured then the watchdog epxire unless system has entered a critical
 * section, where then the main loop takes control of the watchdog.
 */

#ifndef FAULT_H
#define FAULT_H

//=============================================================================
//                                  Libraries
//=============================================================================
    
#include <stdint.h>
#include <stdbool.h>
    
//=============================================================================
//                         Public Function Prototypes
//=============================================================================
    
/**
 * Initialization function for fault detection software module
 */
void fault_init(void);
/**
 * Checks hardware signals noting component failure.
 * @return true if a module has failed, false if module 
 */
bool fault_check_for_failure(void);
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
uint8_t fault_code(void);
/**
 * Signals to the fault module whether to supress watchdog expiration even if
 * component has failed. Ideally used when main loop is interacting with the 
 * motor, which needs to be slowly backed off.
* @param s is boolean signaling to module whether if in critical section
 */
void fault_in_critical_section(bool s);
    
#endif
