/**
 * LED Module
 * @author Curt Henrichs
 * @date 1-24-16, revised 3-10-17
 *
 * Implements the LED device module as output only general purpose output.
 */

//=============================================================================
//                               Libraries
//=============================================================================

#include "LED.h"

//=============================================================================
//							  Function Implementation
//=============================================================================

/**
 * Writes the value to the data register
 * @param output sets this value
 */
void LED_write(uint16_t output){
	(LEDS -> DATA) = output;
}

/**
 * Reads from the data register
 * @return value from register which is activity of LEDs
 */
uint16_t LED_read(){
	return (LEDS -> DATA);
}
