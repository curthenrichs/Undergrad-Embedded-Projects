/**
 * Milestone 2
 * @author Curt Henrichs
 * @version 0.2
 * @date 3-15-17
 * @file main.c
 *
 * Milestone 1: Command Entry System. Commands entered through stdio device and
 * commands are parsed to carry out various operations. Read and write to
 * memory is required.
 *
 * Milestone 2: Develop servo motor module and related commands for the command
 * structure developed in milestone 1.
 *
 * Commands:
 *    RD <address> reads contest of single location. Returns byte
 *    RD <address> <count> Reads from address to address + count.
 *    WR <address> <value> Writes data to location address. Only a byte used.
 *	  PAN <position> moves pan servo to desired absolute position.
 *	  TILT <position> moves tilt servo to desired absolute position.
 */

//=============================================================================
//                                  Libraries
//=============================================================================

#include "Servo.h"
#include "CommandLine.h"

#include <system.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

//=============================================================================
//                                    MAIN
//=============================================================================

/**
 * main will run the serial command line input to get information from user
 * and enact the commands.
 * @return does not actually return (infinite loop)
 */
int main(){

	//module initialization
	servo_init();
	cmd_init();

	//input string buffers
	char inputStr[80];
	char cmdStr[80]; //must be same size for worst case parsing

	//run input loop from serial input to command the controller
	while(1){
		//print prompt to the user for next command
		uart_printStr("\nEnter Command:");

		//block until string entered
		uart_readStr(inputStr, sizeof inputStr);
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

	//should never get here
	return 0;
}
