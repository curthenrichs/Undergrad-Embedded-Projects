/**
 * LED Module
 * @author Curt Henrichs
 *
 * Defines the LED device module as output only general purpose output.
 */

#ifndef LED_LIB_H_
#define LED_LIB_H_

//=============================================================================
//                               Libraries
//=============================================================================

#include "GENERIC_IO.h"

#include <stdint.h>
#include <system.h>

//=============================================================================
//                      Constant and Macro Declaration
//=============================================================================

// Button Masks
#define LED_MAX_CNT 10

// Register Address
#define LEDS ((struct GEN_IO volatile *)(LEDS_BASE | BYPASS_CACHE_FLAG))

//=============================================================================
//							  Function Declaration
//=============================================================================

/**
 * Writes the value to the data register
 * @param output sets this value
 */
void LED_write(uint16_t output);

/**
 * Reads from the data register
 * @return value from register which is activity of LEDs
 */
uint16_t LED_read();

#endif
