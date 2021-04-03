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

//=============================================================================
//                                 Libraries
//=============================================================================

#include "Receiver.h"
#include <project.h>
#include "ChannelMonitor.h"
#include "CSMA_CD_Bus.h"

#include <string.h>

//=============================================================================
//                        Constant and Macro Definition
//=============================================================================

#define RX_TIMER_SHORT_PERIOD   (250)   //! 1/4 of bit period for sync clock
#define RX_TIMER_LONG_PERIOD    (1000)  //! 1 bit period for data clock

/**
 * States for interrupt driven state machine that clocks data messages from bus
 */
typedef enum Rx_States {
    RX_WAIT_MSG,    //! Wait until rising edge to start a new message
    RX_WAIT_CHAR,   //! Wait until rising edge to start next character
    RX_CLK_CHAR     //! Clocking data using timer interrupt
} Rx_States_e;

//=============================================================================
//                          Data Structure Declaration
//=============================================================================

/**
 * Message Buffer Element which captures the stream of characters from the
 * bus. Notes if the data is valid/finished or not yet complete/empty.
 */
typedef struct Rx_Message {
    char buffer[RX_MAX_BUFFER_LENGTH];  //! Message buffer
    uint8_t size;   //! Number of characters received
    bool valid;     //! Data has finished physical transaction (readable)
} Rx_Message_t;

/**
 * Character Buffer Element which is captured from the stream of bits 
 * received from physical layer. Note that this is the character clock
 * state-machine as well as the final result.
 */
typedef struct Rx_Char {
    uint8_t value;  //! Working value being clocked in from bus
    uint8_t shift;  //! State and amount to shift bit into place
} Rx_Char_t;

/**
 * Interrupt driven state-machine that clocks data messages from physical layer
 * into a message buffer.
 */
typedef struct Rx_State_Machine {
    Rx_States_e state;  //! Current high-level state of the receiver
    Rx_Message_t* msg;  //! Current message buffer being filled
    Rx_Char_t data;     //! Bit to character sub-state machine
} Rx_State_Machine_t;

//=============================================================================
//                             Private Attributes
//=============================================================================

/**
 * Receiver state-machine use to clock messages from bus
 */
static volatile Rx_State_Machine_t _rx_sm;
/**
 * Message buffer used to temporarily store message until copied to user code
 */
static Rx_Message_t message_temp_0;
/**
 * Message buffer used to temporarily store message until copied to user code
 */
static Rx_Message_t message_temp_1;

//=============================================================================
//                           Private ISR Prototypes
//=============================================================================

/**
 * Callback to link receive pin state-change to this module. This is effectivly
 * the rising edge interrupt for the state-machine. 
 *
 * When in waiting for message start, check if a buffer is free
 *      Then start character capture process
 * When in waiting for next character to start
 *      Start character capture process
 * When in character clocking state
 *      Sync clocks with rising edge to prevent drift of sample
 *
 * @param edge is either RISING or FALLING
 */
void rx_rx_isr_callback(CSMA_CD_Edge_e edge);
/**
 * Overflow interrupt on timer used to clock bits in for charater recovery
 * 
 * When in clocking character state
 *      If shift is zero then
 *          store character into buffer
 *          if buffer is full then transition to waiting for next message
 *          else wait for next character to start
 *      If shift is seven then
 *          do not copy bit as this bit is always leading 1 bit of clock sync
 *          stay in character clocking state
 *      Else
 *          copy shifted bit into value, stay in character clocking state
 * Else other states
 *      Disable timer and do nothing
 */
CY_ISR_PROTO(rx_timer_isr);
/**
 * Channel Monitor triggers IDLE state
 *
 * When state is waiting for character
 *      Store buffer for user consuption
 * When state is clocking characters
 *      An error occured somewhere, discard message
 * When state is waiting for message
 *      Do nothing
 */
void rx_idle_isr_callback(void);
/**
 * Channel Monitor triggers COLLISION state
 *
 * If not in waiting for message state
 *      discard current message as data is invalid
 */
void rx_collision_isr_callback(void);

//=============================================================================
//                        Public Function Implementation
//=============================================================================

/**
 * Receiver initialization routine sets up message buffers, receive
 * state-machine, and necessary hardware. 
 *
 * Note that this module requires an entry in the bus callback table
 */
void rx_init(void){
    
    //initialize message buffer
    message_temp_0.size = 0;
    message_temp_0.valid = 0;
    message_temp_1.size = 0;
    message_temp_1.valid = 0;
    
    //initialize state machine
    _rx_sm.state = RX_WAIT_MSG;
    _rx_sm.data.value = 0;
    _rx_sm.data.shift = 7;
    _rx_sm.msg = (&message_temp_0);
    
    //initialize hardware
    csma_bus_assign_callback(CSMA_CD_BUS_RECEIVE_CALLBACK,rx_rx_isr_callback);
    
    Rx_Timer_Start();
    Rx_Timer_Trigger_Write(1);

    Rx_Timer_int_StartEx(rx_timer_isr);
    cm_assign_collision_callback(CM_COLLISION_RX_CALLBACK,rx_collision_isr_callback);
    cm_assign_idle_callback(CM_IDLE_RX_CALLBACK,rx_idle_isr_callback); 
}

/**
 * Receiver checks if a message has been retrieved
 * @return true if a message needs to be read, else false
 */
bool rx_has_data(void){
    return message_temp_0.valid || message_temp_1.valid;
}

/**
 * Receiver message copy function. If there exists data to be read, the copy
 * data into provided buffer
 * @param buffer is valid pointer to character buffer to place data
 * @param length is size of buffer passed in as parameter (must be at least 
 *        protocol MAX)
 * @return number of characters copied into buffer
 */
uint8_t rx_buffer(char* buffer, uint8_t length){
    
    //parameter check
    if(buffer == NULL || length < RX_MAX_BUFFER_LENGTH){
        return 0;
    }
    
    // copy data out of message block
    uint8_t retval = 0;
    if(message_temp_0.valid){
        retval = message_temp_0.size;
        memcpy(buffer,message_temp_0.buffer,message_temp_0.size);
        message_temp_0.size = 0;
        message_temp_0.valid = false;
    }else if(message_temp_1.valid){
        retval = message_temp_1.size;
        memcpy(buffer,message_temp_1.buffer,message_temp_1.size);
        message_temp_1.size = 0;
        message_temp_1.valid = false;
    }
    
    return retval;
}

//=============================================================================
//                           Private ISR Implementation
//=============================================================================

/**
 * Callback to link receive pin state-change to this module. This is effectivly
 * the rising edge interrupt for the state-machine. 
 *
 * When in waiting for message start, check if a buffer is free
 *      Then start character capture process
 * When in waiting for next character to start
 *      Start character capture process
 * When in character clocking state
 *      Sync clocks with rising edge to prevent drift of sample
 *
 * @param edge is either RISING or FALLING
 */
void rx_rx_isr_callback(CSMA_CD_Edge_e edge){
    if(edge == CSMA_CD_EDGE_RISING){
        bool triggerTimer = false;
        if(_rx_sm.state == RX_WAIT_MSG){
            //assign a free memory block
            if(!message_temp_0.valid){
                _rx_sm.msg = &message_temp_0;
                triggerTimer = true;
            }else if(!message_temp_1.valid){
                triggerTimer = true;
                _rx_sm.msg = &message_temp_1;
            }
            
            //start clocking data
            if(triggerTimer){
                _rx_sm.data.shift = 7;
                _rx_sm.data.value = 0;
                _rx_sm.state = RX_CLK_CHAR;
            }
        }else if(_rx_sm.state == RX_WAIT_CHAR){
            //start clocking data
            triggerTimer = true;
            _rx_sm.data.shift = 7;
            _rx_sm.data.value = 0;
            _rx_sm.state = RX_CLK_CHAR;
        }else if(_rx_sm.state == RX_CLK_CHAR){
            // sync bit clock
            triggerTimer = true; 
        }
        
        if(triggerTimer){
            Rx_Timer_Trigger_Write(1);
            Rx_Timer_WritePeriod(RX_TIMER_SHORT_PERIOD);
            
            Rx_Timer_ReadStatusRegister();
            Rx_Timer_int_ClearPending();
            
            CyDelayUs(1);
            Rx_Timer_Trigger_Write(0);
        }
    }  
}

/**
 * Overflow interrupt on timer used to clock bits in for charater recovery
 * 
 * When in clocking character state
 *      If shift is zero then
 *          store character into buffer
 *          if buffer is full then transition to waiting for next message
 *          else wait for next character to start
 *      If shift is seven then
 *          do not copy bit as this bit is always leading 1 bit of clock sync
 *          stay in character clocking state
 *      Else
 *          copy shifted bit into value, stay in character clocking state
 * Else other states
 *      Disable timer and do nothing
 */
CY_ISR(rx_timer_isr){

    Rx_Timer_ReadStatusRegister();
    Rx_Timer_int_ClearPending();
    
    if(_rx_sm.state == RX_CLK_CHAR){
        
        uint8_t lvl = CSMA_Receive_Level();
        
        if(_rx_sm.data.shift == 7){
            _rx_sm.data.shift--;
        }else if(_rx_sm.data.shift > 0){
            _rx_sm.data.value |= (lvl << _rx_sm.data.shift); 
            _rx_sm.data.shift--;
        }else{
            _rx_sm.data.value |= lvl;
            
            //store value
            _rx_sm.msg->buffer[_rx_sm.msg->size] = (char)(_rx_sm.data.value);
            _rx_sm.msg->size++;
            
            if(_rx_sm.msg->size >= RX_MAX_BUFFER_LENGTH){
                //store buffer
                _rx_sm.state = RX_WAIT_MSG;
                _rx_sm.msg->valid = true;
            }else{
                //wait for next character
                _rx_sm.state = RX_WAIT_CHAR;
            }
        }

        Rx_Timer_Trigger_Write(1);
        Rx_Timer_WritePeriod(RX_TIMER_LONG_PERIOD);
        CyDelayUs(1);
        Rx_Timer_Trigger_Write(0);
    }else{
        // should not be clocking data, disable timer
        Rx_Timer_Trigger_Write(1);
    }
}

/**
 * Channel Monitor triggers IDLE state
 *
 * When state is waiting for character
 *      Store buffer for user consuption
 * When state is clocking characters
 *      An error occured somewhere, discard message
 * When state is waiting for message
 *      Do nothing
 */
void rx_idle_isr_callback(void){
    
    if(_rx_sm.state == RX_WAIT_CHAR){
        // store data for application consuption
        _rx_sm.state = RX_WAIT_MSG;
        _rx_sm.msg->valid = true;
    }else if(_rx_sm.state == RX_CLK_CHAR){
        // invalid IDLE event, discard message
        _rx_sm.state = RX_WAIT_MSG;
        _rx_sm.msg->valid = false;
        _rx_sm.msg->size = 0;
        _rx_sm.data.shift = 7;
        _rx_sm.data.value = 0;
    }
    
    // disable timer, until next rising edge
    Rx_Timer_Trigger_Write(1);
    Rx_Timer_ReadStatusRegister();
    Rx_Timer_int_ClearPending();
}

/**
 * Channel Monitor triggers COLLISION state
 *
 * If not in waiting for message state
 *      discard current message as data is invalid
 */
void rx_collision_isr_callback(void){
    
    if(_rx_sm.state != RX_WAIT_MSG){
        // COLLISION event, discard message
        _rx_sm.state = RX_WAIT_MSG;
        _rx_sm.msg->valid = false;
        _rx_sm.msg->size = 0;
        _rx_sm.data.shift = 7;
        _rx_sm.data.value = 0;
    }
    
    // disable timer, until next rising edge
    Rx_Timer_Trigger_Write(1);
    Rx_Timer_ReadStatusRegister();
    Rx_Timer_int_ClearPending();
}