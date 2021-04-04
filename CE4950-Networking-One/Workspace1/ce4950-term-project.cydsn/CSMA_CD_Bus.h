/**
 * CSMA_CD_Bus
 * @author Curt Henrichs
 * @date 12-6-2017
 *
 * CSMA/CD Bus abstracts the physical hardware pins and interrupts. The
 * interrupt is attached to a callback table that higher physical layer
 * modules can interract with. 
 */

#ifndef CSMA_CD_BUS_H
#define CSMA_CD_BUS_H
    
//=============================================================================
//                                 Libraries
//=============================================================================

#include <project.h>
  
//=============================================================================
//                        Constant and Macro Definition
//=============================================================================

/**
 * Enumeration of callbacks to assign for the bus callback table. Each is an
 * index to the table.
 */
typedef enum CSMA_CD_Callback {
    CSMA_CD_BUS_CH_MONITOR_CALLBACK,    //! Callback for channel monitor
    CSMA_CD_BUS_RECEIVE_CALLBACK        //! Callback for data collection
} CSMA_CD_Callback_e;
    
/**
 * @return level of the CSMA_RX pin on bus
 */
#define CSMA_Receive_Level() (CSMA_RX_Read())
/**
 * @return level of the CSMA_TX pin on bus
 */
#define CSMA_Transmit_Level() (CSMA_TX_Read())
/**
 * @return write logic level of CSMA_TX pin on bus
 */
#define CSMA_Transmit_Write(lvl) (CSMA_TX_Write(lvl))

/**
 * Enumration of channel edges which trigger the callback table.
 */
typedef enum CSMA_CD_Edge {
    CSMA_CD_EDGE_RISING,                //! Detected Rising Edge
    CSMA_CD_EDGE_FALLING,               //! Detected Falling Edge
    CSMA_CD_EDGE_UNKNOWN                //! Initial state of bus
} CSMA_CD_Edge_e;

//=============================================================================
//                          Public Function Prototypes
//=============================================================================

/**
 * Initialization function for physical bus 
 */
void csma_bus_init(void);
/**
 * Assign a callback function to interrupt
 * @param name is table entry to place new callback
 * @param fnt is function pointer to callback, NULL if entry is to be cleared
 */
void csma_bus_assign_callback(CSMA_CD_Callback_e name, void (*fnt)(CSMA_CD_Edge_e));
    
#endif
