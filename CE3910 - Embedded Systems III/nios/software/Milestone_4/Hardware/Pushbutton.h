/**
 * PUSHBUTTON Module
 * @author Curt Henrichs
 * @date 1-24-16, revised 2-10-16, revised 3-10-17
 *
 * Defines the pushbutton interface. The object is input only, therefore no
 * direction setup is necessary. Interrupts are disabled on this version of
 * NIOS.
 */

#ifndef PUSHBUTTON_LIB_H_
#define PUSHBUTTON_LIB_H_

//=============================================================================
//						         Libraries
//=============================================================================

#include "GenericIO.h"
#include <stdint.h>
#include <system.h>

//=============================================================================
//                      Constant and Macro Declaration
//=============================================================================

// Button Masks
#define PUSHBUTTON_1_MASK 0x1
#define PUSHBUTTON_2_MASK 0x2

//Registers
#define PUSHBUTTON ((struct GEN_IO volatile *)(PUSHBUTTONS_BASE | BYPASS_CACHE_FLAG))

//=============================================================================
//							  Function Declaration
//=============================================================================

/**
 * Reads the raw data from the pushbutton. Note that the pushbutton logic is
 * inverted.
 * @param selectedButton number for the correct pushbutton. 1 or 2
 * @return  masked value returning value for register
 */
uint8_t pushbutton_read(uint8_t selectedButton);

#endif
