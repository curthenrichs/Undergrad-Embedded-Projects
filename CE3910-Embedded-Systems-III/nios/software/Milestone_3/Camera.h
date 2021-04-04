/**
 * Camera Module
 * @author Curt Henrichs
 * @date 3-26-17
 * Defines the entirety of the camera module from the hardware perspective.
 * This device will implement the control loop for tracking and provide VGA
 * output.
 */

#ifndef CAMERA_H_
#define CAMERA_H_

//=============================================================================
//                                 Libraries
//=============================================================================

#include "Hardware/I2C.h"
#include "Hardware/VGA.h"

//=============================================================================
//                             I2C Register Structure
//=============================================================================

/**
 * Information returned regarding meta-data of the image frame
 */
struct frameData{
	uint8_t maxBrightness;
	uint8_t minBrightness;
};

//=============================================================================
//						 Constant and Macro Declaration
//=============================================================================

/**
 * I2C address of the camera as 7-bit value shift left 1 and append R/W bit.
 */
#define CAMERA_I2C_ADDRESS 0x60

// Registers of interest from camera
#define CAMERA_I2C_CLKRC 0x11
#define CAMERA_I2C_COMC 0x14
#define CAMERA_I2C_COML 0x39
#define CAMERA_I2C_MIDH 0x1C
#define CAMERA_I2C_MIDL 0x1D

//=============================================================================
//                         Public Function Declaration
//=============================================================================

/**
 * Initializes the camera module over I2C for slow rate and desired format.
 * Sets any necessary flags and controls for the camera and prepares all
 * data structures.
 */
void cam_init();

/**
 * Captures a frame from the camera and returns it unaltered.
 * @return meta-date of the frame as pointer to the internal structure.
 */
struct frameData* cam_imageCapture();
/**
 * Captures a frame from the camera and returns it filtered with the given value
 * @param threshold value used to filter the pixel data
 * @return meta-date of the frame as pointer to the internal structure.
 */
struct frameData* cam_track(int16_t threshold);

#endif /* CAMERA_H_ */
