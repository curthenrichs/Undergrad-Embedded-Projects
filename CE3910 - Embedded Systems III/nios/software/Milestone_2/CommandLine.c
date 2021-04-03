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

#include "Hardware/GenericIO.h"
#include "Servo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//=============================================================================
//                        Private Function Declaration
//=============================================================================

/**
 * Read Command Function
 * @param arg string to parse
 */
static void _read(char* arg);
/**
 * Write Command Function
 * @param arg string to parse
 */
static void _write(char* arg);
/**
 * Help Command Function
 * @param arg string to parse
 */
static void _help(char* arg);
/**
 * Pans the servo by the input value provided
 * @param arg string to parse
 */
static void _pan(char* arg);
/**
 * Tilts the servo by the input value provided
 * @param arg string to parse
 */
static void _tilt(char* arg);
/**
 * Generates the list of commands in an array of pointers so that commands
 * can be looked up based on their name when parsed.
 */
static void _generateList();
/**
 * Prints out memory starting at the address for the count amount of bytes
 * @param address the address of memory to start printing
 * @param count the number of bytes to read and print
 */
static void _printMem(unsigned int address, unsigned int count);

//=============================================================================
//                      Constant and Macro Declaration
//=============================================================================

/**
 * Defines the number of commands that this device decodes from user input.
 */
#define NUMBER_OF_COMMANDS (5)

//=============================================================================
//                            Private Data Members
//=============================================================================

/**
 * Stores all commands in a list for polymorphic invocation.
 */
static command_t* commandList[NUMBER_OF_COMMANDS];

//=============================================================================
//                       Command Objects (Use Like Constant)
//=============================================================================

/**
 * Read Command Object
 */
static command_t RR = {
	.name = "RR",
	.helpInfo = "Reads from selected memory.\n\tForm RR <address> or RR "
			    "<address> <count>.\n",
	.cmdFnt = _read
};

/**
 * Write Command Object
 */
static command_t WR = {
	.name = "WR",
	.helpInfo = "Writes to selected memory.\n\tForm WR <address> <value>.\n",
	.cmdFnt = _write
};

/**
 * Help Command Object
 */
static command_t HELP = {
	.name = "HELP",
	.helpInfo = "Displays information about a command.\n\tForm HELP or HELP "
			    "<command> or HELP ALL\n",
	.cmdFnt = _help
};

/**
 * Pan Servo Command Object
 */
static command_t PAN = {
	.name = "PAN",
	.helpInfo = "Absolute position control of the pan servo. Valid input is "
			    "between 0 and 175.\n\tForm PAN <position>\n",
	.cmdFnt = _pan
};

/**
 * Tilt Servo Command Object
 */
static command_t TILT = {
	.name = "TILT",
	.helpInfo = "Absolute position control of the tilt servo. Valid input is "
			    "between 0 and 144.\n\tForm TILT <position>\n",
	.cmdFnt = _tilt
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
 * Matches the command given by the user with the predefined list. Will run
 * any command that is matched with given.
 * @param inputStr original string from the user.
 * @param cmdStr parsed out command for matching and further parsing.
 */
void cmd_parseInput(char* inputStr, char* cmdStr){
		//search command list for match
		int commandFoundFlag = 0;
		int i; for(i = 0; i < NUMBER_OF_COMMANDS;++i){
			if(strcmp(cmdStr,commandList[i]->name)==0){
				commandList[i]->cmdFnt(inputStr);
				commandFoundFlag = 1;
			}
		}

		//if command not found then print error message
		if(!commandFoundFlag){
			printf("Invalid Command.\n");
		}
}

//=============================================================================
//                          Private Function Implementation
//=============================================================================

/**
 * Read Command Function
 * @param arg string to parse
 */
static void _read(char* arg){
	char cmd[3];
	unsigned int address, count;
	int matched = sscanf(arg,"%s %x %x",cmd,&address,&count);
	if(matched == 3){
		//Read multiple
		_printMem(address,count);
	}else if(matched == 2){
		//Read single
		_printMem(address,1);
	}else{
		printf("Invalid RR Command Structure.\n");
	}
}

/**
 * Write Command Function
 * @param arg string to parse
 */
static void _write(char* arg){
	char cmd[3];
	unsigned int address, value;
	int matched = sscanf(arg,"%s %x %x",cmd,&address,&value);
	if(matched == 3){
		//always apply the store IO version of the instruction for immediate
		// change and store in original for memory required versions
		*((volatile unsigned char*)(address | BYPASS_CACHE_FLAG)) = value;
	}else{
		printf("Invalid WR Command Structure.\n");
	}
}

/**
 * Help Command Function
 * @param arg string to parse
 */
static void _help(char* arg){
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

/**
 * Pans the servo by the input value provided
 * @param arg string to parse
 */
static void _pan(char* arg){
	char cmd[5];
	int pos;
	int matched = sscanf(arg,"%s %d",cmd,&pos);
	if(matched > 1){
		if(servo_pan(pos) == -1){
			printf("Position ");
			char temp[11]; *(temp+10) = '\0';
			sprintf(temp,"%d",pos);
			printf(temp);
			printf(" is invalid. Enter between 0 and 175.\n");
		}
	}else{
		printf("Invalid command structure.\n");
	}
}

/**
 * Tilts the servo by the input value provided
 * @param arg string to parse
 */
static void _tilt(char* arg){
	char cmd[5];
	int pos;
	int matched = sscanf(arg,"%s %d",cmd,&pos);
	if(matched > 1){
		if(servo_tilt(pos) == -1){
			printf("Position ");
			char temp[11]; *(temp+10) = '\0';
			sprintf(temp,"%d",pos);
			printf(temp);
			printf(" is invalid. Enter between 0 and 143.\n");
		}
	}else{
		printf("Invalid command structure.\n");
	}
}

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
}

/**
 * Prints out memory starting at the address for the count amount of bytes
 * @param address the address of memory to start printing
 * @param count the number of bytes to read and print
 */
static void _printMem(unsigned int address, unsigned int count){

	//sanity check count for valid specification
	if(count < 1){
		printf("Error - Invalid Number of Addresses Specified.\n");
		return; //error case so leave early
	}else if(address + count < address){
		printf("Warning - Address Overflow.\n");
	}

	//buffer allocation
	char buffer[80];
	char temp[12];

	//print header
	memset(buffer,'\0',sizeof buffer);
	strcat(buffer,"  Base   :");
	int i; for(i = 0; i < count && i < 16; ++i){
		*(temp+3) = '\0';
		sprintf(temp," +%x",i);
		strcat(buffer,temp);
	}
	strcat(buffer,"\n");
	printf(buffer);

	//print bytes as chunks of buffer
	int j; for(j = 0; j < (count - 1) / 16 + 1; ++j){
		memset(buffer,'\0',sizeof buffer);

		//print address
		*(temp+10) = '\0';
		sprintf(temp,"%08x :",address + j*16);
		strcat(buffer,temp);

		//print row or less if count
		int a; for(a = 0; a < 16 && a < (count - j * 16); ++a){
			unsigned int addr = address + j*16+a;
			unsigned char val = *((unsigned char*)(addr | BYPASS_CACHE_FLAG));
			*(temp+3) = '\0';
			sprintf(temp," %02x",val);
			strcat(buffer,temp);
		}

		//send to serial device
		strcat(buffer,"\n");
		printf(buffer);
	}

	//send final character to clean up display
	printf("\n");
}
