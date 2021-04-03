/**
 * ServoCMD Objects
 * @author Curt Henrichs
 * @date 3-31-17
 * Implementation of the servo control commands used in the command line.
 * Pan and tilt functionality exposed for the user to control the servos.
 */

//=============================================================================
//                                  Libraries
//=============================================================================

#include "ServoCMD.h"

#include <stdio.h>
#include "../Servo.h"
#include "../Hardware/LCD.h"

//=============================================================================
//                        Private Function Declaration
//=============================================================================

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

//=============================================================================
//                       Command Object Instantiation
//=============================================================================

/**
 * Pan Servo Command Object
 */
command_t PAN = {
	.name = "PAN",
	.helpInfo = "Absolute position control of the pan servo. Valid input is "
			    "between 0 and 175.\n\tForm PAN <position>\n",
	.cmdFnt = _pan
};
/**
 * Tilt Servo Command Object
 */
command_t TILT = {
	.name = "TILT",
	.helpInfo = "Absolute position control of the tilt servo. Valid input is "
			    "between 0 and 144.\n\tForm TILT <position>\n",
	.cmdFnt = _tilt
};

//=============================================================================
//                          Private Function Implementation
//=============================================================================

/**
 * Pans the servo by the input value provided
 * @param arg string to parse
 */
static void _pan(char* arg){
	lcd_printString("Panning");
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
	lcd_printString("Tilting");
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
