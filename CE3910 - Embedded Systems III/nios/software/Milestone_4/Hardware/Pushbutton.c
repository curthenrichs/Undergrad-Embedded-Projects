/**
 * PUSHBUTTON Module
 * @author Curt Henrichs
 * @date 1-24-16, revised 2-10-16, revised 3-10-17
 *
 * Implements the pushbutton interface. The object is input only, therefore no
 * direction setup is necessary. Interrupts are disabled on this version of
 * NIOS.
 */

//=============================================================================
//						         Libraries
//=============================================================================

#include "PUSHBUTTON.h"

//=============================================================================
//							  Function Implementation
//=============================================================================

/**
 * Reads the raw data from the pushbutton. Note that the pushbutton logic is
 * inverted.
 * @param selectedButton number for the correct pushbutton. 1 or 2
 * @return  masked value returning value for register
 */
uint8_t pushbutton_read(uint8_t selectedButton){
	return (PUSHBUTTON -> DATA) & selectedButton;
}
