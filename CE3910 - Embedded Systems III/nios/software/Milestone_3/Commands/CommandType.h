/**
 * CommandType Interface
 * @author Curt Henrichs
 * @date 3-31-17
 * Object definition for all commands used in the command line program.
 * Each object has a string name that will be used to polymorphically
 * match when present in the user input stream. The function pointer will be
 * invoked with the assumption that this call will return back to the
 * invocation point after some condition. Additionally, help information
 * is present in the object for internal documentation and for the menu
 * system.
 */

#ifndef COMMANDTYPE_H_
#define COMMANDTYPE_H_

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

#endif /* COMMANDTYPE_H_ */
