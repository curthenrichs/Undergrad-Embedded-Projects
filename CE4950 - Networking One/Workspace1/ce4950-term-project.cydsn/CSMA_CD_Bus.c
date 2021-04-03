/**
 * CSMA_CD_Bus
 * @author Curt Henrichs
 * @date 12-6-2017
 *
 * CSMA/CD Bus abstracts the physical hardware pins and interrupts. The
 * interrupt is attached to a callback table that higher physical layer
 * modules can interract with. 
 */

//=============================================================================
//                                 Libraries
//=============================================================================

#include "CSMA_CD_Bus.h"

//=============================================================================
//                          Data Structure Declaration
//=============================================================================

/**
 * Callback table maps types defined in header to function pointers for ISR
 */
typedef struct CSMA_CD_Bus_Callback_Table {
    void (*ch_monitor)(CSMA_CD_Edge_e);    //! Callback for channel monitor
    void (*receive)(CSMA_CD_Edge_e);       //! Callback for data collection
} CSMA_CD_Bus_Callback_Table_t;

//=============================================================================
//                             Private Attributes
//=============================================================================

static volatile CSMA_CD_Bus_Callback_Table_t _callback_table; //! ISR table

//=============================================================================
//                           Private ISR Prototypes
//=============================================================================

/**
 * ISR for receive channel level change. Data line for the networking protocol
 */
CY_ISR_PROTO(csma_cd_rx_isr_rising);
/**
 * ISR for receive channel level change. Data line for the networking protocol
 */
CY_ISR_PROTO(csma_cd_rx_isr_falling);

//=============================================================================
//                          Private Function Prototypes
//=============================================================================

/**
 * Runs all valid entires in table according to their priority
 * @param edge is edge ISR triggering table
 */
static inline void csma_bus_run_table(CSMA_CD_Edge_e edge);

//=============================================================================
//                          Public Function Implementation
//=============================================================================

/**
 * Initialization function for physical bus 
 */
void csma_bus_init(void){
    _callback_table.ch_monitor = NULL;
    _callback_table.receive = NULL;
    
    CSMA_RX_int_rising_StartEx(csma_cd_rx_isr_rising);
    CSMA_RX_int_falling_StartEx(csma_cd_rx_isr_falling);
}

/**
 * Assign a callback function to interrupt
 * @param name is table entry to place new callback
 * @param fnt is function pointer to callback, NULL if entry is to be cleared
 */
void csma_bus_assign_callback(CSMA_CD_Callback_e name, void (*fnt)(CSMA_CD_Edge_e)){
    switch(name){
        case CSMA_CD_BUS_CH_MONITOR_CALLBACK:
            _callback_table.ch_monitor = fnt;
            break;
        case CSMA_CD_BUS_RECEIVE_CALLBACK:
            _callback_table.receive = fnt;
            break;
    }  
}

//=============================================================================
//                           Private ISR Implementation
//=============================================================================

/**
 * ISR for receive channel level change. Data line for the networking protocol
 */
CY_ISR(csma_cd_rx_isr_rising){
    csma_bus_run_table(CSMA_CD_EDGE_RISING);
    CSMA_RX_int_rising_ClearPending();
}

/**
 * ISR for receive channel level change. Data line for the networking protocol
 */
CY_ISR_PROTO(csma_cd_rx_isr_falling){
    csma_bus_run_table(CSMA_CD_EDGE_FALLING);
    CSMA_RX_int_falling_ClearPending();
}

/**
 * Runs all valid entires in table according to their priority
 * @param edge is edge ISR triggering table
 */
static inline void csma_bus_run_table(CSMA_CD_Edge_e edge){
    //run each function in structure according to priority
    if(_callback_table.ch_monitor != NULL){
        _callback_table.ch_monitor(edge);
    }
    if(_callback_table.receive != NULL){
        _callback_table.receive(edge);
    }
}