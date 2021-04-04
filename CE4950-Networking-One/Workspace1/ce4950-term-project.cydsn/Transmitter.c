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

//=============================================================================
//                                 Libraries
//=============================================================================

#include "Transmitter.h"
#include <project.h>
#include "CSMA_CD_Bus.h"
#include "ChannelMonitor.h"
#include <stdlib.h>

//=============================================================================
//                        Constant and Macro Definitions
//=============================================================================

#define TX_START_MASK (0x80)    //! One is most significant bit of byte

#define TX_TIMER_CHAR_INTERVAL (497) //! Interval for half a bit

#define BACKOFF_CONSTANT (1000000) //! Reduce on random value to 1 second

//=============================================================================
//                          Data Structure Declaration
//=============================================================================

/**
 * Character Transmission state-machine
 */
typedef struct Tx_Char_State_Machine {
    uint8_t t_bit;  //! transmission mask (indicates which bit is active)
    bool rz_bit;    //! toggle bit for which half of bit is being sent on line 
    uint8_t c;      //! character to transmit
} Tx_Char_State_Machine_t;

/**
 * Buffer transmission state-machine
 */
typedef struct Tx_State_Machine {
    Tx_States_e state;                  //! Current state of transmission
    Tx_Char_State_Machine_t chr;        //! Sub-state machine for tx character
    char buffer[TX_MAX_BUFFER_LENGTH];  //! Current message to transmit
    uint8_t buffer_len;                 //! Current message length
    uint8_t buffer_ind;                 //! Current message indexed char
} Tx_State_Machine_t;

//=============================================================================
//                             Private Attributes
//=============================================================================

/**
 * State-machine to transmit a character on bus
 */
static volatile Tx_State_Machine_t _tx_sm;

//=============================================================================
//                           Private ISR Prototypes
//=============================================================================

/**
 * Transmission timer ISR operates on _tx_sm state_machine to transmit a single
 * character on the network bus. The encoding used is RZ where logic high is a
 * binary one and logic low is a binary zero.
 */
CY_ISR_PROTO(tx_timer_isr);
/**
 * Callback on collision state-change to move state-machine to backoff
 */
void tx_collision_isr_callback(void);
/**
 * Callback on idle state-change to move state-machine from waiting to clocking
 */
void tx_idle_isr_callback(void);

//=============================================================================
//                        Public Function Implementation
//=============================================================================

/**
 * Initialize transmitter module for data clockout
 */
void tx_init(void){
    //initialize state-machine
    _tx_sm.state = TX_DONE;
    _tx_sm.buffer_len = 0;
    _tx_sm.buffer_ind = 0;
    _tx_sm.chr.c = 0;
    _tx_sm.chr.rz_bit = true;
    _tx_sm.chr.t_bit = 0;
    
    //start hardware and assign interrupts
    Tx_Timer_Start();
    Tx_Timer_Trigger_Write(1);
    Tx_Timer_int_StartEx(tx_timer_isr);
    
    cm_assign_collision_callback(CM_COLLISION_TX_CALLBACK,tx_collision_isr_callback);
    cm_assign_idle_callback(CM_IDLE_TX_CALLBACK,tx_idle_isr_callback);
}

/**
 * Transmits a string of character over bus with precaution on bus state
 * Note that this function does use the channel monitor.
 * Note that this is a non-blocking function and thus required polling of state
 * function for completion
 * @param buff is buffer start address in memory
 * @param length is number of characters in buffer to transmit
 * @return true if starting, false is invalid parameter or message in progress
 */
bool tx_write(char* buffer, uint8_t len){
    if(buffer == NULL || len > TX_MAX_BUFFER_LENGTH){
        return false; // invalid parameters
    }else if(_tx_sm.state != TX_DONE){
       return false; // already in transmit
    }else if(len == 0){
        return true; // No message to write, pass through 
    }
    
    memcpy((char*)(_tx_sm.buffer),buffer,len);
    _tx_sm.buffer_len = len;
    _tx_sm.buffer_ind = 0;
    
    if(cm_get_bus_state() == CM_STATE_IDLE){
        _tx_sm.buffer_ind = 0;
        _tx_sm.chr.t_bit = TX_START_MASK;
        _tx_sm.chr.rz_bit = true;
        _tx_sm.chr.c = (uint8_t)(_tx_sm.buffer[_tx_sm.buffer_ind]);
        _tx_sm.state = TX_CHAR;
        
        Tx_Timer_Trigger_Write(1);
        Tx_Timer_WritePeriod(TX_TIMER_CHAR_INTERVAL);
        //CyDelayUs(1);
        Tx_Timer_Trigger_Write(0);
    }else{
        _tx_sm.state = TX_WAIT;
    }
    return true;
}

/**
 * Gets current status of the transmission state-machine. Do not call tx_write
 * until status is done.
 * @return currrent status
 */
Tx_States_e tx_get_status(void){
    return _tx_sm.state;
}

/**
 * Abort the current transmission regardless of state. (Does nothing if done)
 */
void tx_abort(void){
    Tx_Timer_Trigger_Write(1);
    _tx_sm.state = TX_DONE;
    _tx_sm.buffer_len = 0;
    _tx_sm.buffer_ind = 0;
    CSMA_TX_Write(0);
}

//=============================================================================
//                           Private ISR Implementation
//=============================================================================

/**
 * Transmission timer ISR operates on _tx_sm state_machine to transmit a single
 * character on the network bus. The encoding used is RZ where logic high is a
 * binary one and logic low is a binary zero.
 */
CY_ISR(tx_timer_isr){
    Tx_Timer_Trigger_Write(1);
    
    if(_tx_sm.state == TX_CHAR){
        
        // Send data bit
        if(_tx_sm.chr.rz_bit){
            if(_tx_sm.chr.t_bit == TX_START_MASK){
                CSMA_TX_Write(1);
            }else{
                CSMA_TX_Write((_tx_sm.chr.c & _tx_sm.chr.t_bit) != 0);
            }
            
            _tx_sm.chr.t_bit >>= 1;
            _tx_sm.chr.rz_bit = false;
            
            //next single-shot of timer
            Tx_Timer_WritePeriod(TX_TIMER_CHAR_INTERVAL);
            CyDelayUs(1);
            Tx_Timer_Trigger_Write(0);
        }
        // Send ground reference
        else{
            CSMA_TX_Write(0);
            _tx_sm.chr.rz_bit = true;
            
            //check for end state
            if(_tx_sm.chr.t_bit == 0){
                _tx_sm.buffer_ind++;
                if(_tx_sm.buffer_ind >= _tx_sm.buffer_len){
                    //finished transmitting buffer
                    _tx_sm.state = TX_DONE;
                    _tx_sm.buffer_len = 0;
                    _tx_sm.buffer_ind = 0;
                }else{
                    //fetch next character
                    _tx_sm.chr.t_bit = TX_START_MASK;
                    _tx_sm.chr.rz_bit = true;
                    _tx_sm.chr.c = (uint8_t)(_tx_sm.buffer[_tx_sm.buffer_ind]);
                    
                    //next single-shot of timer
                    Tx_Timer_WritePeriod(TX_TIMER_CHAR_INTERVAL);
                    CyDelayUs(1);
                    Tx_Timer_Trigger_Write(0);
                }
            }else{
                //next single-shot of timer
                Tx_Timer_WritePeriod(TX_TIMER_CHAR_INTERVAL);
                CyDelayUs(1);
                Tx_Timer_Trigger_Write(0);
            }
        }
    }else if(_tx_sm.state == TX_BACKOFF){   
        _tx_sm.buffer_ind = 0;
        if(cm_get_bus_state() == CM_STATE_IDLE){
            _tx_sm.buffer_ind = 0;
            _tx_sm.chr.t_bit = TX_START_MASK;
            _tx_sm.chr.rz_bit = true;
            _tx_sm.chr.c = (uint8_t)(_tx_sm.buffer[_tx_sm.buffer_ind]);
            _tx_sm.state = TX_CHAR;
            
            Tx_Timer_Trigger_Write(1);
            Tx_Timer_WritePeriod(TX_TIMER_CHAR_INTERVAL);
            CyDelayUs(1);
            Tx_Timer_Trigger_Write(0);
        }else{
            _tx_sm.state = TX_WAIT;
        }
    }
    
    //clear interrupt
    Tx_Timer_ReadStatusRegister();
}

/**
 * Callback on collision state-change to move state-machine to backoff
 */
void tx_collision_isr_callback(void){
    if(_tx_sm.state == TX_CHAR){
        //stop transmitting character
        CSMA_TX_Write(0);
        
        //move to backoff state
        Tx_Timer_Trigger_Write(1);
        Tx_Timer_WritePeriod(((uint32_t)(rand())) % BACKOFF_CONSTANT);
        CyDelayUs(1);
        Tx_Timer_Trigger_Write(0);
        _tx_sm.state = TX_BACKOFF;
    }
}

/**
 * Callback on idle state-change to move state-machine from waiting to clocking
 */
void tx_idle_isr_callback(void){
    if(_tx_sm.state == TX_WAIT){
        //configure for first character to clock
        _tx_sm.buffer_ind = 0;
        _tx_sm.chr.t_bit = TX_START_MASK;
        _tx_sm.chr.rz_bit = true;
        _tx_sm.chr.c = (uint8_t)(_tx_sm.buffer[_tx_sm.buffer_ind]);
        _tx_sm.state = TX_CHAR;
        
        Tx_Timer_Trigger_Write(1);
        Tx_Timer_WritePeriod(TX_TIMER_CHAR_INTERVAL);
        CyDelayUs(1);
        Tx_Timer_Trigger_Write(0);
    }
}