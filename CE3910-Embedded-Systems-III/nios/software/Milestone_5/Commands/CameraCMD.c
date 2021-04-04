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
#include <string.h>
#include <stdint.h>
#include "../Servo.h"
#include "../Camera.h"
#include "../Hardware/GenericIO.h"
#include "../Hardware/Pushbutton.h"
#include "../Hardware/LCD.h"

//=============================================================================
//                        Constant & Macro Declaration
//=============================================================================

//Filtering Constants
/**
 * Filter types for tracking algorithms
 */
typedef enum FILTER_TYPE{
	DARK_AUTO,    //!< DARK_AUTO automatic filtering for dark objects
	LIGHT_AUTO,   //!< LIGHT_AUTO automatic filtering for light objects
	DARK_MANUAL,  //!< DARK_MANUAL manual value for dark objects
	LIGHT_MANUAL, //!< LIGHT_MANUAL manual value for light objects
	TARGET_VALUE, //!< TARGET_VALUE manual target value filtering
	ERROR		  //!< ERROR parse failed
} FILTER_TYPE_t;

/**
 * Filter tolerance from desired value
 */
#define TOLERANCE 0x08
/**
 * Minimum pixel confidence value before pixel is counted in the boxing method
 */
#define CONFIDENCE_THRESHOLD 0xF0

//Position Control Constants
/**
 * Minimum command value sent to X servo before attenuated to prevent hunting
 */
#define MIN_SERVO_COMMAND_X 1
/**
 * Minimum command value sent to Y servo before attenuated to prevent hunting
 */
#define MIN_SERVO_COMMAND_Y 4

//PID Controller Constants
/**
 * Proportional Term for X servo divided by PID_DEM
 */
#define PX 2
/**
 * Proportional Term for Y servo divided by PID_DEM
 */
#define PY 3
/**
 * Derivative term for X servo divided by PID_DEM
 */
#define DX 0
/**
 * Derivative term for Y servo divided by PID_DEM
 */
#define DY 1
/**
 * Common division term for PID controller used for integer math
 */
#define PID_DEM 8

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
static void _track(char* arg);

/**
 * Calculates the bounding box of the target and updates through the bounding
 * box pointer.
 * @param box pointer to object that will be updated with new bounding info.
 */
static inline void _track_boundingBox(struct boundingBox* box);
/**
 * Parses user input for the tracking command such that a filtering type and
 * possible manual value is selected.
 * @param arg String to parse
 * @param min minimum value of range
 * @param max maximum value of range
 * @return filter type
 */
static inline FILTER_TYPE_t _track_inputParsing(char* arg, uint8_t* min, uint8_t* max);

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
				    "Displays filtered image onto the VGA display.\n"
					"Type of filter specified to be DARK, LIGHT, TARGET.\n"
					"Value sets threshold to manual, else automatic adjustment."
				    "\n\tForm TRACK <type> <value>\n",
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

	//data members
	struct frameData* fd;
	struct boundingBox box;
	int16_t errorX, prevErrorX = 0;
	int16_t errorY, prevErrorY = 0;
	uint8_t targetMin = 0, targetMax = 0;

	//parse user input
	FILTER_TYPE_t type = _track_inputParsing(arg,&targetMin,&targetMax);
	if(type == ERROR){
		return; //return since parse failed
	}

	//notify user UI of exit process
	lcd_printString("Tracking Target");
	printf("Press Button 2 to exit.\n");

	//loop the tracking until button pressed
	int16_t cogx,cogy;
	int32_t xcommand, ycommand;
	while(pushbutton_read(PUSHBUTTON_1_MASK)){
		//get frame data
		fd = cam_track(targetMin,targetMax);

		//determine location of object
		box.minr = VGA_ROW_MAX;
		box.minc = VGA_COL_MAX;
		box.maxr = 0;
		box.maxc = 0;
		_track_boundingBox(&box);

		//calculate cog of the object
		cogy = (box.maxr + box.minr)/2;
		cogx = (box.maxc + box.minc)/2;
		if(cogx < 0 || cogy < 0){
			continue; //no pixels to track
		}

		//calculate servo command
		errorX = (40 - cogx);
		errorY = (30 - cogy);
		xcommand = ((errorX*PX) + (errorX - prevErrorX)*DX)/PID_DEM;
		if(xcommand < MIN_SERVO_COMMAND_X && xcommand > -MIN_SERVO_COMMAND_X){
			xcommand = 0;
		}
		ycommand = ((errorY*PY) + (errorY - prevErrorY)*DY)/PID_DEM;
		if(ycommand < MIN_SERVO_COMMAND_Y && ycommand > -MIN_SERVO_COMMAND_Y){
			ycommand = 0;
		}
		prevErrorX = errorX;
		prevErrorY = errorY;

		//set servo motors
		setServoX(xcommand);
		setServoY(ycommand);

		//adjust target value in automatic mode
		switch(type){
			case DARK_AUTO:
				if(fd->minBrightness > (0xFF - TOLERANCE)){
					targetMax = fd->minBrightness;
				}else{
					targetMax = fd->minBrightness + TOLERANCE;
				}
				break;
			case LIGHT_AUTO:
				if(fd->maxBrightness < (TOLERANCE)){
					targetMin = fd->maxBrightness;
				}else{
					targetMin = fd->maxBrightness - TOLERANCE;
				}
				break;
			default:
				break;
		}
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
static inline void _track_boundingBox(struct boundingBox* box){
	//store data statically for reduced stack frame time
	static uint8_t row,col,pixel;
	static uint8_t* ptr;

	//iterate through buffer to determine location of object
	for(row=0;row<VGA_ROW_MAX;row++){
		ptr = (uint8_t*)(VGA_MEM_START + (row << 7));
		for(col=0;col<VGA_COL_MAX;col++){
			pixel = *(ptr + col);
			if(pixel > CONFIDENCE_THRESHOLD){
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

/**
 * Parses user input for the tracking command such that a filtering type and
 * possible manual value is selected.
 * @param arg String to parse
 * @param min minimum value of range
 * @param max maximum value of range
 * @return filter type
 */
static inline FILTER_TYPE_t _track_inputParsing(char* arg, uint8_t* min, uint8_t* max){
	// parse input for filtering method
	char cmd[10];
	char typeStr[80]; //large to prevent failure of size
	unsigned int value;
	FILTER_TYPE_t retVal;
	int matched = sscanf(arg,"%s %s %x",cmd,typeStr,&value);
	if(matched == 3){
		if(strcmp(typeStr,"TARGET") == 0){
			retVal = TARGET_VALUE;
			*min = (value < TOLERANCE)? value : value - TOLERANCE;
			*max = (value > 0xFF - TOLERANCE)? value : value + TOLERANCE;
		}else if(strcmp(typeStr,"LIGHT") == 0){
			retVal = LIGHT_MANUAL;
			*min = value;
			*max = 0xFF;
		}else if(strcmp(typeStr,"DARK") == 0){
			retVal = DARK_MANUAL;
			*min = 0x00;
			*max = value;
		}else{
			//error
			printf("Invalid TRACK Command Structure.\n");
			retVal = ERROR;
		}
	}else if(matched == 2){
		if(strcmp(typeStr,"LIGHT") == 0){
			retVal = LIGHT_AUTO;
			*min = 0x55; //default value
			*max = 0xFF;
		}else if(strcmp(typeStr,"DARK") == 0){
			retVal = DARK_AUTO;
			*min = 0x00;
			*max = 0x55;
		}else{
			//error
			printf("Invalid TRACK Command Structure.\n");
			retVal = ERROR;
		}
	}else{
		printf("Invalid TRACK Command Structure.\n");
		retVal = ERROR;
	}
	return retVal;
}
