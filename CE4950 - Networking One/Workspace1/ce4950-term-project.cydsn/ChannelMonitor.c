/**
 * ChannelMonitor
 * @author Curt Henrichs
 * @date 12-6-2017
 *
 * Channel monitor is intended to run above the physical layer yet have high
 * coupling as the monitor must assess the state of the physical medium as
 * IDLE, BUSY, or COLLISION. This module should be used by higher layers to
 * get this state information.
 *
 * Note at startup the bus starts in INIT state which is an undetermined state
 * on the bus until an interrupt (pin or timer) occurs. It is recommended that
 * transmission is avoided until a known bus state is reached.
 */

//=============================================================================
//                                 Libraries
//=============================================================================

#include "ChannelMonitor.h"
#include <project.h>
#include "CSMA_CD_Bus.h"

//=============================================================================
//                        Constant and Macro Definition
//=============================================================================

// 0.6 * 100 <0.1 precision on timer>
#define CM_TIMER_SHORT_PERIOD (525)     //! Time for 1/2 a bit to pass through
// 7.7 * 100 <0.01 precision on timer>   
#define CM_TIMER_LONG_PERIOD (8150)     //! Time for 7 bits to pass through

//=============================================================================
//                          Data Structure Declaration
//=============================================================================

/**
 * State machine definition for channel monitor module
 */
typedef struct CM_State_Machine {
    CM_State_e state;                   //! current state of monitor as enum
    CSMA_CD_Edge_e prev_edge;           //! last edge triggered on bus
} CM_State_Machine_t;   

/**
 * ISR callback table for collision event. Triggered on collision ISR not on
 * transition to state.
 */
typedef struct CM_Collision_Callback_Table {
    void (*tx)(void);   //! Callback function for transmitter
    void (*rx)(void);   //! Callback function for receiver
} CM_Collision_Callback_Table_t;

/**
 * ISR callback table for idle event. Triggered on idle ISR not on transition
 * to sate
 */
typedef struct CM_Idle_Callback_Table {
    void (*tx)(void);   //! Callback function for transmitter
    void (*rx)(void);   //! Callback function for receiver
} CM_Idle_Callback_Table_t;

//=============================================================================
//                             Private Attributes
//=============================================================================

/**
 * State machine for channel monitor (including interrupt ISRs)
 */
static volatile CM_State_Machine_t _cm_sm;
/**
 * Callback table for collision ISR event (not state change)
 */
static volatile CM_Collision_Callback_Table_t _collision_callback_table;
/**
 * Callback table for idle ISR event (not state change)
 */
static volatile CM_Idle_Callback_Table_t _idle_callback_table;

//=============================================================================
//                           Private ISR Prototypes
//=============================================================================

/**
 * Callback to be assigned to receive pin ISR table. This function will handle
 * monitor state change logic associated with receiving a level change
 * @param edge is current state change on bus
 */
void cm_rx_isr_callback(CSMA_CD_Edge_e edge);
/*
 * ISR for channel monitor's timer which is used to detect end of busy and 
 * code violations (collisions).
 */
CY_ISR_PROTO(cm_timer_isr);
/**
 * IDLE ISR passes interrupt to all register callbacks
 */
CY_ISR_PROTO(cm_idle_isr);
/**
 * Collision ISR passes interrupt to all register callbacks
 */
CY_ISR_PROTO(cm_collision_isr);

//=============================================================================
//                        Public Function Implementation
//=============================================================================

/**
 * Initalize channel monitor hardware and state-machine. Will set the state-
 * machine into the CD_INIT state which waits for the timer to expire before
 * transitioning into IDLE. (See header for detailed documentation)
 */
void cm_init(void){
    _cm_sm.state = CM_STATE_INIT;
    _cm_sm.prev_edge = CSMA_CD_EDGE_UNKNOWN;
    CM_State_Reg_Write(_cm_sm.state);
    
    _collision_callback_table.rx = NULL;
    _collision_callback_table.tx = NULL;
    
    _idle_callback_table.rx = NULL;
    _idle_callback_table.tx = NULL;

    CyDelay(100); //needed to let bus settle before starting network 
    
    csma_bus_assign_callback(CSMA_CD_BUS_CH_MONITOR_CALLBACK,cm_rx_isr_callback);

    CM_Timer_Start();
    CM_Timer_WritePeriod(CM_TIMER_LONG_PERIOD);
    CM_Timer_int_StartEx(cm_timer_isr);
    CM_Timer_Trigger_Write(1);
    CyDelayUs(1);
    CM_Timer_Trigger_Write(0);
    
    CM_IDLE_int_StartEx(cm_idle_isr);
    CM_COLLISION_int_StartEx(cm_collision_isr);
}

/**
 * @return current state from channel monitor, accurate to last bit received
 */
CM_State_e cm_get_bus_state(void){
    return _cm_sm.state;   
}

/**
 * Assign a callback function to collision state interrupt
 * @param name is table entry to place new callback
 * @param fnt is function pointer to callback, NULL is to be cleared
 */
void cm_assign_collision_callback(CM_Collision_Callback_e name, void (*fnt)(void)){
    switch(name){
        case CM_COLLISION_RX_CALLBACK:
            _collision_callback_table.rx = fnt;
            break;
        case CM_COLLISION_TX_CALLBACK:
            _collision_callback_table.tx = fnt;
            break;
    }
}

/**
 * Assign a callback function to idle state interrupt
 * @param name is table entry to place new callback
 * @param fnt is function pointer to callback, NULL is to be cleared
 */
void cm_assign_idle_callback(CM_Idle_Callback_e name, void (*fnt)(void)){
    switch(name){
        case CM_IDLE_RX_CALLBACK:
            _idle_callback_table.rx = fnt;
            break;
        case CM_IDLE_TX_CALLBACK:
            _idle_callback_table.tx = fnt;
            break;
    }
}

//=============================================================================
//                           Private ISR Implementation
//=============================================================================

/**
 * Callback to be assigned to receive pin ISR table. This function will handle
 * monitor state change logic associated with receiving a level change
 * @param edge is current state change on bus
 */
void cm_rx_isr_callback(CSMA_CD_Edge_e edge){
    CM_Timer_Trigger_Write(1);
    if(edge == CSMA_CD_EDGE_RISING){ //rising edge  
        if(_cm_sm.state == CM_STATE_IDLE || _cm_sm.state == CM_STATE_BUSY 
                ||_cm_sm.state == CM_STATE_INIT){
            _cm_sm.state = CM_STATE_BUSY;
            
            CM_Timer_WritePeriod(CM_TIMER_SHORT_PERIOD);
        }
    }else{ //falling edge
        if(_cm_sm.state == CM_STATE_BUSY || _cm_sm.state == CM_STATE_COLLISION 
                || _cm_sm.state == CM_STATE_INIT){
            _cm_sm.state = CM_STATE_BUSY;
                    
            CM_Timer_WritePeriod(CM_TIMER_LONG_PERIOD);
        }
    }

    _cm_sm.prev_edge = edge;
    CM_State_Reg_Write(_cm_sm.state);
    CM_Timer_ReadStatusRegister(); 
    CM_Timer_int_ClearPending();
    
    CyDelayUs(1);
    CM_Timer_Trigger_Write(0);
}

/*
 * ISR for channel monitor's timer which is used to detect end of busy and 
 * code violations (collisions).
 */
CY_ISR(cm_timer_isr){    
    static volatile uint8_t level;
    level = CSMA_Receive_Level();
    
    //next state
    switch(_cm_sm.state){
        case CM_STATE_INIT:
            _cm_sm.state = (level) ? CM_STATE_BUSY : CM_STATE_IDLE;
            _cm_sm.prev_edge = (level) ? CSMA_CD_EDGE_RISING : CSMA_CD_EDGE_FALLING; 
            if(_cm_sm.state == CM_STATE_BUSY){
                CM_Timer_WritePeriod(CM_TIMER_SHORT_PERIOD);
                CM_Timer_Trigger_Write(1);
                CyDelayUs(1);
                CM_Timer_Trigger_Write(0);
            }
            break;
        case CM_STATE_IDLE:
            _cm_sm.state = (level) ? CM_STATE_BUSY : CM_STATE_IDLE;
            break;
        case CM_STATE_BUSY:
                _cm_sm.state = (level) ? CM_STATE_COLLISION : CM_STATE_IDLE;
            break;
        case CM_STATE_COLLISION:
            _cm_sm.state = (level) ? CM_STATE_COLLISION : CM_STATE_BUSY;
            break;
    }
    CM_State_Reg_Write(_cm_sm.state);
    
    //clear interrupt
    CM_Timer_ReadStatusRegister(); 
    CM_Timer_int_ClearPending();
}

/**
 * IDLE ISR passes interrupt to all register callbacks
 */
CY_ISR(cm_idle_isr){
    if(_idle_callback_table.rx != NULL){
        _idle_callback_table.rx();
    }
    if(_idle_callback_table.tx != NULL){
        _idle_callback_table.tx();
    }
}

/**
 * Collision ISR passes interrupt to all register callbacks
 */
CY_ISR(cm_collision_isr){
    if(_collision_callback_table.rx != NULL){
        _collision_callback_table.rx();
    }
    if(_collision_callback_table.tx != NULL){
        _collision_callback_table.tx();
    }
}
