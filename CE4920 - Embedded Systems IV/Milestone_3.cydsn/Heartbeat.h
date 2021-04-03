/**
 * Heartbeat
 * @author Curt Henrichs
 * @date 9-17-17
 *
 * Heartbeat module is a software module tasked with collecting heartbeat
 * rate from the user for the treadmill application. Heartbeat module operates
 * via interrupts. Interrupts are on ADC (for waveform recreation), on counter
 * (for overflow, signalling no pulse), and on an analog comparator (to detect
 * heartbeat peak and therefore measure frequency).
 */

#ifndef HEARTBEAT_H
#define HEARTBEAT_H

//=============================================================================
//                                  Libraries
//=============================================================================
    
#include <stdint.h>
#include <stdbool.h>
    
//=============================================================================
//                        Constant and Macro Defintions
//=============================================================================

/**
 * Heartbeat source enumeration to direct the analog MUX channel
 */
typedef enum Heartbeat_Source {
    HEARTBEAT_SOURCE_OFF,       //! Internal with DAC off
    HEARTBEAT_SOURCE_INTERNAL,  //! Internal with DAC generating waveform
    HEARTBEAT_SOURCE_EXTERNAL   //! External pin, DAC off
} Heartbeat_Source_e;
    
//=============================================================================
//                         Public Function Prototypes
//=============================================================================
    
/**
 * Heartbeat module intialization function will start all module hardware and
 * intialize all private attributes. This module is entirely driven by
 * interrupts so it is crucial that global interrupts are enabled.
 * @param source is the heartbeat input source for analog MUX channel select
 */
void heartbeat_init(Heartbeat_Source_e source);
/**
 * @return heartbeat beats per minute as standard measure of user's heartrate
 */
uint16_t heartbeat_get_bpm(void);
/**
 * Clears the previous heartbeat data, thereby reseting the module
 * Note hardware is not modified.
 */
void heartbeat_clear(void);
/**
 * Sets the use LED flag so that heartbeat is pulsed out
 * @param state, true if activate LED heartbeat, false if disable
 */
void heartbeat_set_use_LED(bool state);
/**
 * @return gets the use LED flag state. true if using, false if disabled
 */
bool heartbeat_get_use_LED(void);

#endif
