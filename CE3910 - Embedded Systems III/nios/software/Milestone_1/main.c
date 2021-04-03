/**
 * Milestone 1
 * @author Curt Henrichs
 * @version 0.1
 * @date 3-8-17
 * @file main.c
 *
 * Milestone 1: Command Entry System. Commands entered through stdio device and
 * commands are parsed to carry out various operations. Read and write to
 * memory is required.
 *
 * Commands:
 *    RD <address> reads contest of single location. Returns byte
 *    RD <address> <count> Reads from address to address + count.
 *    WR <address> <value> Writes data to location address. Only a byte used.
 */

//=============================================================================
//                                  Libraries
//=============================================================================

#include <system.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
//                              Constants & Macros
//=============================================================================

/**
 * Defines the Flag placed on an address to notify the program to bypass
 * the cache and directly update memory.
 */
#define BYPASS_CACHE_FLAG 0x80000000

/**
 * Defines the number of commands that this device decodes from user input.
 */
#define NUMBER_OF_COMMANDS 3

//=============================================================================
//            Approximations of Future Module Function Declaration
//=============================================================================

/**
 * Temporary serial function using the jtag_uart for debugging.
 * This function signature will be migrated to the UART module
 * in a later milestone.
 * @param str passes a string through the UART
 */
void serial_printStr(char* str);
/**
 * Temporary serial function using the jtag_uart for debugging.
 * This function signature will be migrated to the UART module
 * in a later milestone.
 * @param inputStr String buffer that will be filled by the
 *        function until either a new line character is
 *        received or length is reached.
 * @param length size of the buffer.
 */
void serial_readStr(char* inputStr, unsigned int length);

//=============================================================================
//                        Private Function Declaration
//=============================================================================

/**
 * Read Command Function
 * @param arg string to parse
 */
void cmd_read(char* arg);
/**
 * Write Command Function
 * @param arg string to parse
 */
void cmd_write(char* arg);
/**
 * Help Command Function
 * @param arg string to parse
 */
void cmd_help(char* arg);
/**
 * Generates the list of commands in an array of pointers so that commands
 * can be looked up based on their name when parsed.
 */
void cmd_generateList();
/**
 * Prints out memory starting at the address for the count amount of bytes
 * @param address the address of memory to start printing
 * @param count the number of bytes to read and print
 */
void memmgr_printMem(unsigned int address, unsigned int count);
/**
 * Writes a byte of data to the address specified. Bypasses the cache.
 * @param address Address of memory to write to
 * @param value byte of information to store
 */
void memmgr_writeByte(unsigned int address, unsigned char value);

//=============================================================================
//                            Command Declaration
//=============================================================================
/**
 * Read Command Object
 */
static const command_t RR = {
	.name = "RR",
	.helpInfo = "Reads from selected memory.\n\tForm RR <address> or RR "
			    "<address> <count>.\n",
	.cmdFnt = cmd_read
};

/**
 * Write Command Object
 */
static const command_t WR = {
	.name = "WR",
	.helpInfo = "Writes to selected memory.\n\tForm WR <address> <value>.\n",
	.cmdFnt = cmd_write
};

/**
 * Help Command Object
 */
static const command_t HELP = {
	.name = "HELP",
	.helpInfo = "Displays information about a command.\n\tForm HELP or HELP "
			    "<command> or HELP ALL\n",
	.cmdFnt = cmd_help
};

/**
 * Stores all commands in a list for polymorphic invocation.
 */
static command_t* commandList[NUMBER_OF_COMMANDS];

//=============================================================================
//                                    MAIN
//=============================================================================

/**
 * main will run the serial command line input to get information from user
 * and enact the commands.
 * @return does not actually return (infinite loop)
 */
int main(){
	//setup command list
	cmd_generateList();

	//input string buffers
	char inputStr[80];
	char cmdStr[80]; //must be same size for worst case parsing

	//run input loop from serial input to command the controller
	while(1){

		//print prompt to the user for next command
		serial_printStr("\nEnter Command:");

		//block until string entered
		serial_readStr(inputStr, sizeof inputStr);

		//parse for command
		int matched = sscanf(inputStr,"%s",cmdStr);
		if(matched != EOF){
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
				serial_printStr("Invalid Command.\n");
			}
		}
	}

	//should never get here
	return 0;
}

//=============================================================================
//                          Private Function Implementation
//=============================================================================

/**
 * Read Command Function
 * @param arg string to parse
 */
void cmd_read(char* arg){
	char cmd[3];
	unsigned int address, count;
	int matched = sscanf(arg,"%s %x %x",cmd,&address,&count);
	if(matched == 3){
		//Read multiple
		memmgr_printMem(address,count);
	}else if(matched == 2){
		//Read single
		memmgr_printMem(address,1);
	}else{
		serial_printStr("Invalid RR Command Structure.\n");
	}
}

/**
 * Write Command Function
 * @param arg string to parse
 */
void cmd_write(char* arg){
	char cmd[3];
	unsigned int address, value;
	int matched = sscanf(arg,"%s %x %x",cmd,&address,&value);
	if(matched == 3){
		//Write Byte
		memmgr_writeByte(address,value);
	}else{
		serial_printStr("Invalid WR Command Structure.\n");
	}
}

/**
 * Help Command Function
 * @param arg string to parse
 */
void cmd_help(char* arg){
	char cmd[5];
	char otherStr[80]; //large to prevent failure of size

	//parse string for HELP <command> or HELP
	int matched = sscanf(arg,"%s %s",cmd,otherStr);
	if(matched == 2){
		//check for all parameter
		if(strcmp(otherStr,"ALL") == 0){
			int i; for(i = 0; i < NUMBER_OF_COMMANDS; ++i){
				serial_printStr(commandList[i]->name);
				serial_printStr(":\n");
				serial_printStr(commandList[i]->helpInfo);
				serial_printStr("\n");
			}
			return;
		}else{
			//search list of commands for helpInfo and print
			int i; for(i = 0; i < NUMBER_OF_COMMANDS; ++i){
				if(strcmp(otherStr,commandList[i]->name)==0){
					serial_printStr(commandList[i]->helpInfo);
					return; //ends early as the item was found
				}
			}
		}

		//did not find command.
		serial_printStr("Command ");
		serial_printStr(otherStr);
		serial_printStr(" is not valid.\n");
	}

	//print its own help info
	serial_printStr(HELP.helpInfo);
}

/**
 * Generates the list of commands in an array of pointers so that commands
 * can be looked up based on their name when parsed.
 */
void cmd_generateList(){
	//setup command list
	commandList[0] = &RR;
	commandList[1] = &WR;
	commandList[2] = &HELP;
}

/**
 * Prints out memory starting at the address for the count amount of bytes
 * @param address the address of memory to start printing
 * @param count the number of bytes to read and print
 */
void memmgr_printMem(unsigned int address, unsigned int count){

	//sanity check count for valid specification
	if(count < 1){
		serial_printStr("Error - Invalid Number of Addresses Specified.\n");
		return; //error case so leave early
	}else if(address + count < address){
		serial_printStr("Warning - Address Overflow.\n");
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
	serial_printStr(buffer);

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
		serial_printStr(buffer);
	}

	//send final character to clean up display
	serial_printStr("\n");
}

/**
 * Writes a byte of data to the address specified. Bypasses the cache.
 * @param address Address of memory to write to
 * @param value byte of information to store
 */
void memmgr_writeByte(unsigned int address, unsigned char value){
	//always apply the store IO version of the instruction for immediate change
	// and store in original for memory required versions
	*((volatile unsigned char*)(address | BYPASS_CACHE_FLAG)) = value;
}

//=============================================================================
//            Approximations of Future Module Function Implementation
//=============================================================================

/**
 * Temporary serial function using the jtag_uart for debugging.
 * This function signature will be migrated to the UART module
 * in a later milestone.
 * @param str passes a string through the UART
 */
void serial_printStr(char* str){
	//todo replace with UART in future lab
	printf("%s",str);
}

/**
 * Temporary serial function using the jtag_uart for debugging.
 * This function signature will be migrated to the UART module
 * in a later milestone.
 * @param inputStr String buffer that will be filled by the
 *        function until either a new line character is
 *        received or length is reached.
 * @param length size of the buffer.
 */
void serial_readStr(char* inputStr, unsigned int length){
	//todo replace with UART in future lab
	if(fgets(inputStr,length, stdin) != NULL){
		char* newline = strchr(inputStr,'\n');
		if(newline != NULL){
			*newline = '\0';
		}
	}
}


