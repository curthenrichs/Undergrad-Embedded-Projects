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

//=============================================================================
//                                 Libraries
//=============================================================================
    
#include "DataLinkLayer.h"
    
//=============================================================================
//                        Constant and Macro Definitions
//=============================================================================

#define HEADER_START_BYTE   (0x00)  //! Header Start expected value
#define HEADER_VERSION_BYTE (0x01)  //! Header Version expected value
#define CRC_NOT_USED_VALUE  (0x77)  //! Header CRC value if not used
#define HEADER_CRC_GEN      (0x13)  //! Header CRC generator 5-bit value
#define DATA_CRC_GEN        (0x13)  //! Data CRC generator 5-bit value

//=============================================================================
//                            Data Structure Declaration
//=============================================================================
    
/**
 * Header structure is a mask over larger byte buffer so that header can be
 * accessed by element name.
 */
typedef struct DLL_Header {
    uint8_t start;      //! Start bit is constant expected for valid message 
    uint8_t version;    //! Version bit is constant used to interpret standard
    uint8_t src;        //! Source address to forward source to application
    uint8_t dst;        //! Destination address to check against current user
    uint8_t msg_len;    //! Length of data section of message
    uint8_t crc_usage;  //! Flag noting if CRC used and what type(s)
    uint8_t crc;        //! Header CRC regardless of Flag is sent 
} DLL_Header_t;

//=============================================================================
//                              Private Attributes
//=============================================================================

/**
 * Associate current user into data-link layer for address allocation
 */
static UserName_e _current_user;
/**
 * Dll-receive buffer is single element, must be cleared with read
 */
static DLL_Message_t _received_buffer;
/**
 * Flag noting when dll-receive buffer is full, must be cleared with read
 */
static bool _received_buffer_has_data;

//=============================================================================
//                           Private Function Prototypes
//=============================================================================

/**
 * Generate Cyclic Redundancy Check FCS byte per Networking protocol
 * @param buffer is bytes in frame to reference
 * @param len is number of bytes in frame
 * @param gen is 5-bit generator for CRC
 * @return FCS byte formated as 4-bit CRC in LSB of byte (MSB first)
 */
static uint8_t dll_generate_crc(uint8_t* buffer, uint8_t len, uint8_t gen);

//=============================================================================
//                         Public Function Implementation
//=============================================================================
    
/**
 * Data-link layer initialization routine, will start lower-level drivers
 * to read, write on physical bus.
 *
 * Note expects that user_init() is handled outside of this scope
 */
void dll_init(void){
    csma_bus_init();
    cm_init();
    tx_init();
    rx_init();
    dll_reset();
}

/**
 * Resets the layer to initializations state. 
 *
 * Call this if low-level drivers already initialized
 */
void dll_reset(void){
    //disable user as default
    _current_user = _INVALID_NAME;
    
    //initialize read buffer
    _received_buffer_has_data = false;
}

/**
 * Log in the supplied user as the current user for the data-link layer.
 * necessary to receive messages as per the user's allocated addresses.
 * @param user user table name so that dll has correct receive addresses
 */
void dll_set_user(UserName_e user){
    _current_user = user;
}

/**
 * @return Get currently logged in user for data-link layer.
 */
UserName_e dll_get_user(void){
    return _current_user;
}

/**
 * Periodic update of the module, must be called in main regularly to capture
 * messages from low-level receive.
 * @return true if a message has been received else false
 */
bool dll_update(void){
    bool retval = false;
    
    //copy data from receive buffer into data-link buffer after validation
    if(dll_rx_check() && !_received_buffer_has_data){
        
        char buffer[DLL_PHY_PACKET_SIZE_RX];
        uint8_t count = dll_rx_buffer(buffer,sizeof(buffer)); 
        DLL_Header_t* header = (DLL_Header_t*)(buffer);
        
        //validate general characteristics
        if(header->start != HEADER_START_BYTE){
            return false; //failed
        }else if(header->version != HEADER_VERSION_BYTE){
            return false; //failed   
        }else if(header->msg_len > DLL_PACKET_SIZE){
            return false; //invalid size   
        }else if(header->msg_len + 8 != count){
            return false; //bytes captured does not match expected   
        }
        
        //validate user
        UserName_e user = user_address_to_id(header->dst);
        if(user != _current_user && user != NETWORK_BROADCAST){
            return false; //ignore message as not for this user   
        }
        
        //validate crc
        uint8_t* crcPtr = (uint8_t*)(&buffer[sizeof(DLL_Header_t) + header->msg_len]);
        _received_buffer.use_crc = (DLL_CRC_Use_e)(header->crc_usage);
        switch(header->crc_usage){
            case DLL_NO_CRC:
                _received_buffer.failed_header_crc = 
                        (header->crc != CRC_NOT_USED_VALUE); 
                _received_buffer.failed_data_crc = 
                        (*crcPtr != CRC_NOT_USED_VALUE);
                break;
            case DLL_HEADER_CRC:
                _received_buffer.failed_header_crc = 
                        (header->crc != dll_generate_crc((uint8_t*)(header),
                        6,HEADER_CRC_GEN)); 
                _received_buffer.failed_data_crc = 
                        (*crcPtr != CRC_NOT_USED_VALUE);
                break;
            case DLL_MESSAGE_CRC:
                _received_buffer.failed_header_crc = 
                        (header->crc != CRC_NOT_USED_VALUE); 
                _received_buffer.failed_data_crc = 
                        (*crcPtr != dll_generate_crc((uint8_t*)
                        (&buffer[sizeof(DLL_Header_t)]),header->msg_len,
                        DATA_CRC_GEN));
                break;
            case DLL_BOTH_CRC:
                _received_buffer.failed_header_crc = 
                        (header->crc != dll_generate_crc((uint8_t*)(header),
                        6,HEADER_CRC_GEN)); 
                _received_buffer.failed_data_crc = 
                        (*crcPtr != dll_generate_crc((uint8_t*)
                        (&buffer[sizeof(DLL_Header_t)]),header->msg_len,
                        DATA_CRC_GEN));
                break;
            default:
                return false; //invalid CRC setting
        }
        
        //copy data into dll frame for application consuption
        _received_buffer.dst = header->dst;
        _received_buffer.src = header->src;
        _received_buffer.msg_size = header->msg_len;
        _received_buffer.use_crc = header->crc_usage;
        
        memset(_received_buffer.msg,0,sizeof(_received_buffer.msg));
        memcpy(_received_buffer.msg,&buffer[sizeof(DLL_Header_t)],
                _received_buffer.msg_size);
        retval = _received_buffer_has_data = true;
    }
    
    return retval;
}

/**
 * Write a PDU out to physical bus. Note that charaters must be 7-bit values
 * else message will generate a warning which is regarded as a message transmit
 * failure.
 * @param message is pointer to PDU to write. Must have a valid source address
 * @return true if message sent successfully, else false 
 */
bool dll_write(DLL_Message_t* message){
    char buffer[DLL_PHY_PACKET_SIZE_TX];
    uint8_t buffer_size;
    
    //validate message
    if(message == NULL){
        return false; //invalid message   
    }else if(message->msg_size > sizeof(message->msg)){
        return false; //invalid message size   
    }else if(user_address_to_id(message->src) != _current_user){
        return false; //user does not match dll logged in user   
    }
    
    //package message into a network frame
    DLL_Header_t* header = (DLL_Header_t*)(buffer);
    header->start = HEADER_START_BYTE;
    header->version = HEADER_VERSION_BYTE;
    header->src = message->src;
    header->dst = message->dst;
    header->msg_len = message->msg_size;
    
    switch(message->use_crc){
        case DLL_NO_CRC:
            header->crc_usage = DLL_NO_CRC;
            header->crc = CRC_NOT_USED_VALUE;
            break;
        case DLL_HEADER_CRC:
            header->crc_usage = DLL_HEADER_CRC;
            header->crc = dll_generate_crc((uint8_t*)(header),6,HEADER_CRC_GEN);
            break;
        case DLL_MESSAGE_CRC:
            header->crc_usage = DLL_MESSAGE_CRC;
            header->crc = CRC_NOT_USED_VALUE;
            break;
        case DLL_BOTH_CRC:
            header->crc_usage = DLL_BOTH_CRC;
            header->crc = dll_generate_crc((uint8_t*)(header),6,HEADER_CRC_GEN);
            break;
        default:
            return false; //invalid CRC option
    }
    
    //copy message contents
    memcpy(&buffer[sizeof(DLL_Header_t)],message->msg,header->msg_len);
    
    //if crc, then compute and store
    if(header->crc_usage == DLL_MESSAGE_CRC 
                || header->crc_usage == DLL_BOTH_CRC){
        buffer[sizeof(DLL_Header_t) + header->msg_len] = 
                dll_generate_crc((uint8_t*)(&buffer[sizeof(DLL_Header_t)]),
                header->msg_len,DATA_CRC_GEN);
    }else{
        buffer[sizeof(DLL_Header_t) + header->msg_len] = CRC_NOT_USED_VALUE;
    }
    
    //send message to lower level driver to send physical message
    buffer_size = sizeof(DLL_Header_t) + header->msg_len + 1;
    return dll_tx_buffer(buffer,buffer_size);
}

/**
 * Check if data-link layer has a message in buffer waiting for consumption
 * @return true if message buffered else false
 */
bool dll_has_data(void){
    return _received_buffer_has_data;
}

/**
 * Check for when transmission has completed buffer. Note that for more details
 * use the dll_tx_check()
 * @return boolean true when done, false while transmitting
 */
bool dll_tx_finished(void){
    return dll_tx_check() == TX_DONE;   
}

/**
 * Read message from buffer. Should first check for data using "has_data".
 * @param message PDU to store buffered message into.
 * @return size of data contents in message. 0 if no data or invalid parameters
 */
uint8_t dll_read(DLL_Message_t* message){
    
    //validate parameter
    if(message == NULL){
        return 0; // invalid parameter
    }else if(!_received_buffer_has_data){
        return 0; //no data buffered
    }
    
    //copy from received
    memcpy(message,&_received_buffer,sizeof(DLL_Message_t));
    _received_buffer_has_data = false;
    
    return message->msg_size;
}

//=============================================================================
//                        Private Function Implementation
//=============================================================================

/**
 * Generate Cyclic Redundancy Check FCS byte per Networking protocol
 * @param buffer is bytes in frame to reference
 * @param len is number of bytes in frame
 * @param gen is 5-bit generator for CRC
 * @return FCS byte formated as 4-bit CRC in LSB of byte (MSB first)
 */
static uint8_t dll_generate_crc(uint8_t* buffer, uint8_t len, uint8_t gen){
    uint8_t i,k,b,msb;
    uint8_t x = 0;
    
    //shift buffer in
    for(i=0; i<len; i++){
        b = buffer[i];
        for(k=0; k<8; k++){
            if(k == 0){
                msb = 1; //MSB is always one during transmission
            }else{
                msb = (b & (1 << (7 - k))) >> (7 - k);
            }
            x = (x << 1) | msb;
            x = x ^ (gen & ((x & 0x10) ? (0x0F) : (0x00)));
        }
    }
    
    //shift zero bits to find FCS
    for(k=0; k<4; k++){
        x = (x << 1) | 0;
        x = x ^ (gen & ((x & 0x10) ? (0x0F) : (0x00)));
    }
    
    return x & 0x0F;
}