/**
 * CommandLine Module
 * @author Curt Henrichs
 * @date 3-16-17
 *
 * Serial command line parse and function module. Called from main loop to
 * execute all commands given by the user.
 */

//=============================================================================
//                                  Libraries
//=============================================================================

#include "CommandLine.h"

#include "Hardware/LCD.h"

#include "Commands/CommandType.h"
#include "Commands/CameraCMD.h"
#include "Commands/MemoryCMD.h"
#include "Commands/ServoCMD.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//=============================================================================
//                        Private Function Declaration
//=============================================================================

/**
 * Generates the list of commands in an array of pointers so that commands
 * can be looked up based on their name when parsed.
 */
static void _generateList();

/**
 * Matches the command given by the user with the predefined list. Will run
 * any command that is matched with given.
 * @param inputStr original string from the user.
 * @param cmdStr parsed out command for matching and further parsing.
 */
static void cmd_parseInput(char* inputStr, char* cmdStr);

/**
 * Help Command Function
 * @param arg string to parse
 */
static void _help(char* arg);

//=============================================================================
//                      Constant and Macro Declaration
//=============================================================================

/**
 * Defines the number of commands that this device decodes from user input.
 */
#define NUMBER_OF_COMMANDS (9)

//=============================================================================
//                            Private Data Members
//=============================================================================

/**
 * Stores all commands in a list for polymorphic invocation.
 */
static command_t* commandList[NUMBER_OF_COMMANDS];

//input string buffers
static char inputStr[80];
static char cmdStr[80]; //must be same size for worst case parsing

//=============================================================================
//                       Command Objects (Use Like Constant)
//=============================================================================

/**
 * Help Command Object
 */
static command_t HELP = {
	.name = "HELP",
	.helpInfo = "Displays information about a command.\n\tForm HELP or HELP "
			    "<command> or HELP ALL\n",
	.cmdFnt = _help
};

//=============================================================================
//                         Public Function Implementation
//=============================================================================

/**
 * Initializes the command line module. Specifically the command list is
 * generated so that command parsing can occur.
 */
void cmd_init(){
	_generateList();
}

/**
 * Gets the command from the user and will perform that action. Blocking
 */
void cmd_getCommand(){
	//print prompt to the user for next command
	printf("\nEnter Command:");

	//block until string entered
	fgets(inputStr, sizeof inputStr,stdin);
	char* str = inputStr;
	while(*str){
		*str = toupper(*str);
		str++;
	}

	//parse for command
	int matched = sscanf(inputStr,"%s",cmdStr);
	if(matched != EOF){
		cmd_parseInput(inputStr,cmdStr);
	}
}

//=============================================================================
//                          Private Function Implementation
//=============================================================================

/**
 * Generates the list of commands in an array of pointers so that commands
 * can be looked up based on their name when parsed.
 */
static void _generateList(){
	//setup command list
	commandList[0] = &RR;
	commandList[1] = &WR;
	commandList[2] = &HELP;
	commandList[3] = &PAN;
	commandList[4] = &TILT;
	commandList[5] = &WRCAMREG;
	commandList[6] = &RDCAMREG;
	commandList[7] = &IMAGE;
	commandList[8] = &TRACK;
}

/**
 * Matches the command given by the user with the predefined list. Will run
 * any command that is matched with given.
 * @param inputStr original string from the user.
 * @param cmdStr parsed out command for matching and further parsing.
 */
static void cmd_parseInput(char* inputStr, char* cmdStr){
		//search command list for match
		int commandFoundFlag = 0;
		int i; for(i = 0; i < NUMBER_OF_COMMANDS;++i){
			if(strcmp(cmdStr,commandList[i]->name)==0){
				lcd_clr();
				commandList[i]->cmdFnt(inputStr);
				commandFoundFlag = 1;
			}
		}

		//if command not found then print error message
		if(!commandFoundFlag){
			printf("Invalid Command.\n");
		}
}

/**
 * Help Command Function
 * @param arg string to parse
 */
static void _help(char* arg){
	lcd_printString("Help Menu");
	char cmd[5];
	char otherStr[80]; //large to prevent failure of size

	//parse string for HELP <command> or HELP
	int matched = sscanf(arg,"%s %s",cmd,otherStr);
	if(matched == 2){
		//check for all parameter
		if(strcmp(otherStr,"ALL") == 0){
			int i; for(i = 0; i < NUMBER_OF_COMMANDS; ++i){
				printf("%s:\n%s\n",commandList[i]->name,commandList[i]->helpInfo);
			}
			return;
		}else{
			//search list of commands for helpInfo and print
			int i; for(i = 0; i < NUMBER_OF_COMMANDS; ++i){
				if(strcmp(otherStr,commandList[i]->name)==0){
					printf(commandList[i]->helpInfo);
					return; //ends early as the item was found
				}
			}
		}

		//did not find command.
		printf("Command %s is not valid.\n",otherStr);
	}

	//print its own help info
	printf(HELP.helpInfo);
}
