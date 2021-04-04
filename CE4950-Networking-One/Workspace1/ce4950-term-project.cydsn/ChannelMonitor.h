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

#ifndef CHANNEL_MONITOR_H
#define CHANNEL_MONITOR_H
    
//=============================================================================
//                        Constant and Macro Definitions
//=============================================================================
 
/**
 * Channel monitor finite state-machine states view data on CSMA_RX pin
 */
typedef enum CM_State {
    CM_STATE_INIT,        //! Initial state required to acquaint oneself with bus
    CM_STATE_IDLE,        //! No data is being transmitted on bus
    CM_STATE_BUSY,        //! A node is transmitting on bus
    CM_STATE_COLLISION    //! Multiple nodes transmitting on bus
} CM_State_e;

/**
 * Channel monitor collision event callback table enteries
 */
typedef enum CM_Collision_Callback {
    CM_COLLISION_TX_CALLBACK,   //! Entry for transmit
    CM_COLLISION_RX_CALLBACK    //! Entry for receive
} CM_Collision_Callback_e;

/**
 * Channel Monitor idle event callback table enteries
 */
typedef enum CM_Idle_Callback {
    CM_IDLE_TX_CALLBACK,        //! Entry for transmit
    CM_IDLE_RX_CALLBACK         //! Entry for receive
} CM_Idle_Callback_e;

//=============================================================================
//                          Public Function Prototypes
//=============================================================================

/**
 * Initalize channel monitor hardware and state-machine. Will set the state-
 * machine into the CM_STATE_INIT state which waits for the timer to expire 
 * before transitioning into IDLE. (See header for detailed documentation)
 */
void cm_init(void);
/**
 * @return current state from channel monitor, accurate to last bit received
 */
CM_State_e cm_get_bus_state(void);    
/**
 * Assign a callback function to collision state interrupt
 * @param name is table entry to place new callback
 * @param fnt is function pointer to callback, NULL is to be cleared
 */
void cm_assign_collision_callback(CM_Collision_Callback_e name, void (*fnt)(void));
/**
 * Assign a callback function to idle state interrupt
 * @param name is table entry to place new callback
 * @param fnt is function pointer to callback, NULL is to be cleared
 */
void cm_assign_idle_callback(CM_Idle_Callback_e name, void (*fnt)(void));

#endif
    