/**
 * Transmitter
 * @author Curt Henrichs
 * @date 1-1-2018
 *
 * Transmitter module handles RZ encoded message clockout to network bus. This
 * module has an optional mode to enable/disbale stop on collision behavior. 
 * (Normally enabled). Note that this module requires global interrupts to be
 * enabled. 
 */

#ifndef TRANSMITTER_H
#define TRANSMITTER_H
    
//=============================================================================
//                                 Libraries
//=============================================================================
    
#include <stdbool.h>
#include <stdint.h>
    
//=============================================================================
//                        Constant and Macro Definitions
//=============================================================================

/**
 * Transmission states for state machine
 */
typedef enum Tx_States {
    TX_WAIT,    //! Transmit waiting for IDLE before transmission
    TX_CHAR,    //! Transmiting character on bus
    TX_BACKOFF, //! COLLISION detected, waiting before retransmission
    TX_DONE     //! Transmission completed. Can place a new message
} Tx_States_e;

#define TX_MAX_BUFFER_LENGTH (52)   //! Maximum message size allowed on bus
    
//=============================================================================
//                          Public Function Prototypes
//=============================================================================
    
/**
 * Initialize transmitter module for data clockout
 */
void tx_init(void);
/**
 * Transmits a string of character over bus with precaution on bus state
 * Note that this function does use the channel monitor.
 * Note that this is a non-blocking function and thus required polling of state
 * function for completion
 * @param buff is buffer start address in memory
 * @param length is number of characters in buffer to transmit
 * @return true if starting, false is invalid parameter or message in progress
 */
bool tx_write(char* buffer, uint8_t len);
/**
 * Gets current status of the transmission state-machine. Do not call tx_write
 * until status is done.
 * @return currrent status
 */
Tx_States_e tx_get_status(void);
/**
 * Abort the current transmission regardless of state. (Does nothing if done)
 */
void tx_abort(void);
   
#endif
    