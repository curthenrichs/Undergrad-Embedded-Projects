/**
 * Application
 * @author Curt Henrichs
 * @date 1-11-18
 *
 * Application layer demo program for term project.
 *
 * Demo project consists of a serial terminal, command-line interface to 
 *  - log into device as a user (thereby allocating a set of addresses)
 *  - send short messages (44 characters or less) accross the network bus
 *  - receive short messages on logged in addresses and broadcast address
 *  - generate help menu for explaining commands
 *  - logout of device
 */

#ifndef APPLICATION_H
#define APPLICATION_H
   
//=============================================================================
//                           Public Function Prototypes
//=============================================================================

/**
 * Application main function will run demo program for term project
 * @return --ignore
 */
int app_main(void);
    
#endif
    