/**
 * Receiver
 * @author Curt Henrichs
 * @date 1-26-18
 *
 * Receiver module handles RZ encoded message clockin from network bus. This
 * module is interrupt driven up to filling buffers. The user must poll the 
 * module for new messages received and copy those messages into their
 * application space.
 *
 * Note that a finite set of buffers are instantiated to collect message data
 * from bus. If these are filled, new data will be lost. Thus message polling 
 * should be performed regularly.
 *
 * Note that this module requires global interrupts to be enabled.
 */

#ifndef RECEIVER_H
#define RECEIVER_H
    
//=============================================================================
//                                 Libraries
//=============================================================================
    
#include <stdbool.h>
#include <stdint.h>
    
//=============================================================================
//                        Constant and Macro Definition
//=============================================================================
    
#define RX_MAX_BUFFER_LENGTH (52)   //! Protocol specified message length
    
//=============================================================================
//                        Public Function Prototypes
//=============================================================================

/**
 * Receiver initialization routine sets up message buffers, receive
 * state-machine, and necessary hardware. 
 *
 * Note that this module requires an entry in the bus callback table
 */    
void rx_init(void);
/**
 * Receiver checks if a message has been retrieved
 * @return true if a message needs to be read, else false
 */
bool rx_has_data(void);
/**
 * Receiver message copy function. If there exists data to be read, the copy
 * data into provided buffer
 * @param buffer is valid pointer to character buffer to place data
 * @param length is size of buffer passed in as parameter (must be at least 
 *        protocol MAX)
 * @return number of characters copied into buffer
 */
uint8_t rx_buffer(char* buffer, uint8_t length);
    
#endif
    