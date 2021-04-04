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
//                                Data Structures
//=============================================================================

/**
 * Defines a command object that has a name, help information, and commanded
 * function pointer.
 */
typedef struct command{
	char* name;
	char* helpInfo;
	void (*cmdFnt)(char*);
}command_t;

//=============================================================================
//                          Public Function Declaration
//=============================================================================

/**
 * Initializes the command line module. Specifically the command list is
 * generated so that command parsing can occur.
 */
void cmd_init();
/**
 * Matches the command given by the user with the predefined list. Will run
 * any command that is matched with given.
 * @param inputStr original string from the user.
 * @param cmdStr parsed out command for matching and further parsing.
 */
void cmd_parseInput(char* inputStr, char* cmdStr);

#endif /* COMMANDLINE_H_ */
