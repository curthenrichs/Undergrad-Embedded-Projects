/**
 * DataLinkLayer
 * @author Curt Henrichs
 * @date 1-26-18
 *
 * Aggregates all of the low level driver components into a cohesive interface 
 * for higher level application services. Additionally, wraps low-level bus
 * read/write into a data frame containing header and routing information.
 *
 * Low-Level drivers include
 *  - CSMA_CD_Bus
 *  - ChannelMonitor
 *  - Transmitter
 *  - Receiver
 */

#ifndef DATALINKLAYER_H
#define DATALINKLAYER_H
    
//=============================================================================
//                                 Libraries
//=============================================================================
    
#include <project.h>
#include <stdbool.h>
    
#include "UserAddressAllocation.h"
#include "CSMA_CD_Bus.h"
#include "ChannelMonitor.h"
#include "Transmitter.h"
#include "Receiver.h"
    
//=============================================================================
//                        Constant and Macro Definitions
//=============================================================================

/**
 * Number of bytes allowed in a transmission packet (data, header, and crc)
 */
#define DLL_PHY_PACKET_SIZE_TX (TX_MAX_BUFFER_LENGTH)
/**
 * Number of bytes allowed in a receive packet (data, header, and crc)
 */
#define DLL_PHY_PACKET_SIZE_RX (RX_MAX_BUFFER_LENGTH)
/**
 * Message size passed to this layer. Will be wrapped around a header and crc
 */
#define DLL_PACKET_SIZE (44)
/**
 * Options to select level of error detection used for message
 */
typedef enum DLL_CRC_Use {
    DLL_NO_CRC = 0,     //! Blank header CRC, not transmit data CRC
    DLL_HEADER_CRC,     //! Send header CRC, not transmit data CRC
    DLL_MESSAGE_CRC,    //! Blank header CRC, transmit data CRC byte
    DLL_BOTH_CRC        //! Send header CRC, transmit data CRC byte
} DLL_CRC_Use_e;
/**
 * Return status for transmission in progress flags
 */
typedef Tx_States_e DLL_Tx_Status_e;
    
//=============================================================================
//                            Data Structure Declaration
//=============================================================================
    
/**
 * PDU for data-link layer
 */
typedef struct DLL_Message {
    uint8_t src;                //! Address of current user (must match logged)
    uint8_t dst;                //! Address of destination device.
    DLL_CRC_Use_e use_crc;      //! CRC options flag
    char msg[DLL_PACKET_SIZE];  //! Contents of message
    uint8_t msg_size;           //! Size of message, maximum is DLL_PACKET_SIZE
    bool failed_header_crc;     //! Failed CRC check on header, data corrupted
    bool failed_data_crc;       //! Failed CRC check on data, data corrupted
} DLL_Message_t;
    
//=============================================================================
//                               Function Mapping
//=============================================================================

/**
 * Tranmsits a string of character over bus with precaution on bus state
 * Note that this function does use the channel monitor.
 * @param buff is buffer start address in memory
 * @param length is number of characters in buffer to transmit
 * @return boolean true if starting transmission, false if error
 */    
#define dll_tx_buffer(buffer,length) (tx_write(buffer,length))
/**
 * Transmitter check if message has been transmitted
 * @return current status of transmission as DLL_Tx_Status_e
 */
#define dll_tx_check() (tx_get_status())
/**
 * Receiver checks if a message has been retrieved
 * @return true if a message needs to be read, else false
 */
#define dll_rx_check() (rx_has_data())
/**
 * Receiver message copy function. If there exists data to be read, the copy
 * data into provided buffer
 * @param buffer is valid pointer to character buffer to place data
 * @param length is size of buffer passed in as parameter (must be at least 
 *        protocol MAX)
 * @return number of characters copied into buffer
 */
#define dll_rx_buffer(buffer,length) (rx_buffer(buffer,length))
/**
 * Gets current state from channel monitor, accurate to last bit received
 * @return INIT, IDLE, BUSY, COLLISION
 */
#define dll_ch_state() (cm_get_bus_state)

//=============================================================================
//                          Public Function Prototypes
//=============================================================================
    
/**
 * Data-link layer initialization routine, will start lower-level drivers
 * to read, write on physical bus.
 *
 * Note expects that user_init() is handled outside of this scope
 */
void dll_init(void);
/**
 * Resets the layer to initializations state. 
 *
 * Call this if low-level drivers already initialized
 */
void dll_reset(void);
/**
 * Log in the supplied user as the current user for the data-link layer.
 * necessary to receive messages as per the user's allocated addresses.
 * @param user user table name so that dll has correct receive addresses
 */
void dll_set_user(UserName_e user);
/**
 * @return Get currently logged in user for data-link layer.
 */
UserName_e dll_get_user(void);
/**
 * Periodic update of the module, must be called in main regularly to capture
 * messages from low-level receive.
 * @return true if a message has been received else false
 */
bool dll_update(void);
/**
 * Write a PDU out to physical bus. Note that charaters must be 7-bit values
 * else message will generate a warning which is regarded as a message transmit
 * failure.
 * @param message is pointer to PDU to write. Must have a valid source address
 * @return true if message sent successfully, else false 
 */
bool dll_write(DLL_Message_t* message);
/**
 * Check if data-link layer has a message in buffer waiting for consumption
 * @return true if message buffered else false
 */
bool dll_has_data(void);
/**
 * Check for when transmission has completed buffer. Note that for more details
 * use the dll_tx_check()
 * @return boolean true when done, false while transmitting
 */
bool dll_tx_finished(void);
/**
 * Read message from buffer. Should first check for data using "has_data".
 * @param message PDU to store buffered message into.
 * @return size of data contents in message. 0 if no data or invalid parameters
 */
uint8_t dll_read(DLL_Message_t* message);

#endif
    