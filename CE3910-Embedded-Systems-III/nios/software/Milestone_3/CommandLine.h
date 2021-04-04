/**
 * CommandLine Module
 * @author Curt Henrichs
 * @date 3-16-17
 *
 * Serial command line parse and function module. Called from main loop to
 * execute all commands given by the user.
 */

#ifndef COMMANDLINE_H_
#define COMMANDLINE_H_

//=============================================================================
//                          Public Function Declaration
//=============================================================================

/**
 * Initializes the command line module. Specifically the command list is
 * generated so that command parsing can occur.
 */
void cmd_init();

/**
 * Gets the command from the user and will perform that action. Blocking
 */
void cmd_getCommand();

#endif /* COMMANDLINE_H_ */
