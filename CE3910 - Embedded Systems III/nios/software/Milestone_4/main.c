/**
 * Milestone 3
 * @author Curt Henrichs
 * @version 0.2
 * @date 3-22-17
 * @file main.c
 *
 * Milestone 1: Command Entry System. Commands entered through stdio device and
 * commands are parsed to carry out various operations. Read and write to
 * memory is required.
 *
 * Milestone 2: Develop servo motor module and related commands for the command
 * structure developed in milestone 1.
 *
 * Milestone 3: Develop I2C camera control module. Develop camera control
 * module and VGA module.
 *
 * Commands:
 *    RD <address> reads contest of single location. Returns byte
 *    RD <address> <count> Reads from address to address + count.
 *    WR <address> <value> Writes data to location address. Only a byte used.
 *	  PAN <position> moves pan servo to desired absolute position.
 *	  TILT <position> moves tilt servo to desired absolute position.
 *	  RDCAMREG <Register> reads byte from the camera I2C device.
 *	  WRCAMREG <Register> <Value> writes byte to camera I2C device.
 *	  IMAGE displays the camera data onto the VGA display
 *	  TRACK <type> <value> tracks object matching filter type and threshold
 */

//=============================================================================
//                                  Libraries
//=============================================================================

#include "CommandLine.h"
#include "Hardware/LCD.h"
#include "Servo.h"
#include "Camera.h"

//=============================================================================
//                                    MAIN
//=============================================================================

/**
 * main will run the serial command line input to get information from user
 * and enact the commands.
 * @return does not actually return (infinite loop)
 */
int main(){
	//initialization
	lcd_init();
	servo_init();
	cam_init();
	cmd_init();

	//run input loop from serial input to command the controller
	lcd_printString("Starting...");
	while(1){
		cmd_getCommand();
	}

	//should never get here
	return 0;
}
