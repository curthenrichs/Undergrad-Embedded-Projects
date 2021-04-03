/**
 * CameraCMD Object
 * @author Curt Henrichs
 * @date 3-31-17
 * Command Line objects for the camera commands (read register, write register,
 * display image, and tracking target) Commands exposed are to be treated as
 * constants.
 */

//=============================================================================
//                                  Libraries
//=============================================================================

#include "CameraCMD.h"

#include <stdio.h>
#include <stdint.h>
#include "../Servo.h"
#include "../Camera.h"
#include "../Hardware/GenericIO.h"
#include "../Hardware/Pushbutton.h"
#include "../Hardware/LCD.h"

//=============================================================================
//                             I2C Register Structure
//=============================================================================

/**
 * Bounding box of the shape being tracked
 */
struct boundingBox{
	uint8_t minr;
	uint8_t minc;
	uint8_t maxr;
	uint8_t maxc;
};

//=============================================================================
//                        Private Function Declaration
//=============================================================================

/**
 * Reads from the I2C camera.
 * @param arg string to parse
 */
static void _readCam(char* arg);
/**
 * Writes to the I2C camera.
 * @param arg string to parse.
 */
static void _writeCam(char* arg);
/**
 * Displays the camera image onto the VGA hardware. Runs in a loop until
 * pushbutton two is pressed to exit.
 * @param arg ignored
 */
static void _displayImage(char* arg);
/**
 * Track command looks for target that meets threshold requirement. Manipulates
 * servo turret to track target under closed loop control.
 * @param arg ignored
 */
//todo add parameterization
static void _track(char* arg);
/**
 * Calculates the bounding box of the target and updates through the bounding
 * box pointer.
 * @param box pointer to object that will be updated with new bounding info.
 */
static void _track_boundingBox(struct boundingBox* box);

//=============================================================================
//                       Command Object Instantiation
//=============================================================================

/**
 * Reads from the camera over I2C
 */
command_t RDCAMREG = {
	.name = "RDCAMREG",
	.helpInfo = "Reads from Camera I2C register specified as parameter.\n\t"
			    "Form RDCAMREG RegisterNumber\n",
	.cmdFnt = _readCam
};
/**
 * Writes to the camera over I2C
 */
command_t WRCAMREG = {
	.name = "WRCAMREG",
	.helpInfo = "Writes to Camera I2C register specified as parameter.\n\t"
			    "Form WRCAMREG RegisterNumber Value\n",
	.cmdFnt = _writeCam
};
/**
 * Displays video feed from camera onto VGA display.
 */
command_t IMAGE = {
		.name = "IMAGE",
		.helpInfo = "Displays image from camera onto the VGA display. \n\t"
				    "Form IMAGE\n",
		.cmdFnt = _displayImage
};
/**
 * Tracks an object in view of camera with servo turret.
 */
command_t TRACK = {
		.name = "TRACK",
		.helpInfo = "Tracks object in view of camera with servo turret.\n"
				    "Displays filtered image onto the VGA display\n\t"
				    "Form TRACK\n",
		.cmdFnt = _track
};

//=============================================================================
//                          Private Function Implementation
//=============================================================================

/**
 * Reads from the I2C camera.
 * @param arg string to parse
 */
static void _readCam(char* arg){
	lcd_printString("Reading I2C");
	char cmd[10];
	unsigned int regNumber;
	int matched = sscanf(arg,"%s %x",cmd,&regNumber);
	if(matched > 1){
		unsigned char data = I2C_read(CAMERA_I2C_ADDRESS,regNumber);
		printf("%02x: %02x\n",regNumber,data);
	}else{
		printf("Invalid read command structure\n");
	}
}

/**
 * Writes to the I2C camera.
 * @param arg string to parse.
 */
static void _writeCam(char* arg){
	lcd_printString("Writing I2C");
	char cmd[10];
	unsigned int regNumber, value;
	int matched = sscanf(arg,"%s %x %x",cmd,&regNumber,&value);
	if(matched > 2){
		I2C_write(CAMERA_I2C_ADDRESS,regNumber,value);
	}else{
		printf("Invalid write command structure\n");
	}
}

/**
 * Displays the camera image onto the VGA hardware. Runs in a loop until
 * pushbutton two is pressed to exit.
 * @param arg ignored
 */
static void _displayImage(char* arg){
	lcd_printString("Displaying\nCamera Image");
	printf("Press Button 2 to exit.\n");

	while(pushbutton_read(PUSHBUTTON_1_MASK)){
		cam_imageCapture();
	}

	//set to default
	vga_clearDisplay();
}

/**
 * Track command looks for target that meets threshold requirement. Manipulates
 * servo turret to track target under closed loop control.
 * @param arg ignored
 */
static void _track(char* arg){
	lcd_printString("Tracking Target");
	printf("Press Button 2 to exit.\n");

	//data members
	struct frameData* fd;
	struct boundingBox box;

	int16_t threshold = 0x55;
	int16_t errorX, prevErrorX = 0;
	int16_t errorY, prevErrorY = 0;

	//loop the tracking until button pressed
	while(pushbutton_read(PUSHBUTTON_1_MASK)){
		//get data
		fd = cam_track(threshold);
		box.minr = VGA_ROW_MAX;
		box.minc = VGA_COL_MAX;
		box.maxr = 0;
		box.maxc = 0;

		//filter image for abstract target
		_track_boundingBox(&box);

		//calculate cog of the object
		int16_t cogy = (box.maxr + box.minr)/2;
		int16_t cogx = (box.maxc + box.minc)/2;
		if(cogx < 0 || cogy < 0){
			continue; //no pixels to track
		}

		//control servo motors
		errorX = (40 - cogx);
		setServoX(errorX/2 + (errorX - prevErrorX)/5);
		errorY = (30 - cogy);
		setServoY(errorY*2/3 + (errorY - prevErrorY)/5);
		prevErrorX = errorX;
		prevErrorY = errorY;
	}

	//set system to default
	servo_init();
	vga_clearDisplay();
}

/**
 * Calculates the bounding box of the target and updates through the bounding
 * box pointer.
 * @param box pointer to object that will be updated with new bounding info.
 */
static void _track_boundingBox(struct boundingBox* box){
	uint8_t row,col,pixel;
	for(row=0;row<VGA_ROW_MAX;row++){
		for(col=0;col<VGA_COL_MAX;col++){
			uint8_t* ptr = (uint8_t*)(VGA_MEM_START + (row << 7) + col);
			pixel = *ptr;
			if(pixel > 0x55){
				//check within current bounding box
				if(row < box->minr)
					box->minr = row;
				if(col < box->minc)
					box->minc = col;
				if(col > box->maxc)
					box->maxc = col;
				if(row > box->maxr)
					box->maxr = row;
			}
		}
	}
}
