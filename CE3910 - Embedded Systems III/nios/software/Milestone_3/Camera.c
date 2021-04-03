/**
 * Camera Module
 * @author Curt Henrichs
 * @date 3-26-17
 * Defines the entirety of the camera module from the hardware perspective.
 * This device will implement the control loop for tracking and provide VGA
 * output.
 */

//=============================================================================
//                                 Libraries
//=============================================================================

#include "Camera.h"
#include "Hardware/Timer.h"

//=============================================================================
//						 Constant and Macro Declaration
//=============================================================================

// I2C register values
#define CAMERA_CLKRC_SCALER 0x15 //(17.7MHz)/((Scaler+1)*4) 6-bit value
#define CAMERA_COMC_QCIF_MASK 0x20
#define CAMERA_COML_PCLK_VALID_MASK 0x40

// Video format size
#define QCIF_COL_MAX 176
#define QCIF_ROW_MAX 144

// Camera clock signals for pixel synchronization
#define CAM_CNTRL_VSYNC_MASK (0x04)
#define CAM_CNTRL_HREF_MASK (0x02)
#define CAM_CNTRL_PCLK_MASK (0x01)

// Camera Control Register and Camera Pixel Register pointers
#define CAM_CNTRL_REGISTER (uint8_t volatile*)(CAM_CONTROL_BASE | BYPASS_CACHE_FLAG)
#define CAM_PIXEL_REGISTER (uint8_t volatile*)(PIXEL_PORT_BASE | BYPASS_CACHE_FLAG)

//=============================================================================
//                             Private Data Members
//=============================================================================

/**
 * Data for each frame. While private it is exposed through the captureImage
 * function for further processing.
 */
static struct frameData fd;

//=============================================================================
//                         Public Function Implementation
//=============================================================================

/**
 * Initializes the camera module over I2C for slow rate and desired format.
 * Sets any necessary flags and controls for the camera and prepares all
 * data structures.
 */
void cam_init(){

	//initialize members
	fd.maxBrightness = 0x00;
	fd.minBrightness = 0xFF;

	//initialize I2C camera hardware
	I2C_init();
	timer_delay(100000,TIMER_0);
	I2C_write(CAMERA_I2C_ADDRESS,CAMERA_I2C_CLKRC,CAMERA_CLKRC_SCALER);
	timer_delay(100000,TIMER_0);
	I2C_write(CAMERA_I2C_ADDRESS,CAMERA_I2C_COML,CAMERA_COML_PCLK_VALID_MASK);
	timer_delay(100000,TIMER_0);
	I2C_write(CAMERA_I2C_ADDRESS,CAMERA_I2C_COMC,CAMERA_COMC_QCIF_MASK);
	timer_delay(100000,TIMER_0);

	//clear VGA display
	vga_clearDisplay();
}

/**
 * Runs an infinite loop to display the data from the camera on screen. Later
 * this will be converted to the tracking function.
 */
struct frameData* cam_imageCapture(){
	int row, col;
	volatile uint8_t* vgaPtr;
	uint8_t pixel;

	//start of image
	while(!(*CAM_CNTRL_REGISTER & CAM_CNTRL_VSYNC_MASK));
	while((*CAM_CNTRL_REGISTER & CAM_CNTRL_VSYNC_MASK));
	vgaPtr = VGA_MEM_START;
	fd.maxBrightness = 0x00;
	fd.minBrightness = 0xFF;

	//gather by row
	for(row = QCIF_ROW_MAX-1;row >= 0; row--){
		//set next VGA row
		if(row < 120 && row%2){
			vgaPtr = VGA_MEM_START + (row/2 << 7);
		}
		//gather pixels in each column
		while(!(*CAM_CNTRL_REGISTER & CAM_CNTRL_HREF_MASK));
		for(col = 0; (col < QCIF_COL_MAX) && (*CAM_CNTRL_REGISTER & CAM_CNTRL_HREF_MASK); col++){
			//get single pixel
			while(!(*CAM_CNTRL_REGISTER & CAM_CNTRL_PCLK_MASK));
			pixel = *CAM_PIXEL_REGISTER;
			while((*CAM_CNTRL_REGISTER & CAM_CNTRL_PCLK_MASK));

			//display pixels to VGA
			if(row < 120 && row%2 && col < 160 && col%2){
				//write to VGA
				*vgaPtr = pixel;
				vgaPtr++;

				//update pixel information
				if(fd.maxBrightness < pixel)
					fd.maxBrightness = pixel;
				if(fd.minBrightness > pixel)
					fd.minBrightness = pixel;
			}
		}
	}
	return &fd;
}

/**
 * Captures a frame from the camera and returns it filtered with the given value
 * @param threshold value used to filter the pixel data
 * @return meta-date of the frame as pointer to the internal structure.
 */
struct frameData* cam_track(int16_t threshold){
	int row, col;
	volatile uint8_t* vgaPtr;
	uint8_t pixel;

	//start of image
	while(!(*CAM_CNTRL_REGISTER & CAM_CNTRL_VSYNC_MASK));
	while((*CAM_CNTRL_REGISTER & CAM_CNTRL_VSYNC_MASK));
	vgaPtr = VGA_MEM_START;
	fd.maxBrightness = 0x00;
	fd.minBrightness = 0xFF;

	//gather by row
	for(row = QCIF_ROW_MAX-1;row >= 0; row--){
		//set next VGA row
		if(row < 120 && row%2){
			vgaPtr = VGA_MEM_START + (row/2 << 7);
		}
		//gather pixels in each column
		while(!(*CAM_CNTRL_REGISTER & CAM_CNTRL_HREF_MASK));
		for(col = 0; (col < QCIF_COL_MAX) && (*CAM_CNTRL_REGISTER & CAM_CNTRL_HREF_MASK); col++){
			//get single pixel
			while(!(*CAM_CNTRL_REGISTER & CAM_CNTRL_PCLK_MASK));
			pixel = *CAM_PIXEL_REGISTER;
			while((*CAM_CNTRL_REGISTER & CAM_CNTRL_PCLK_MASK));

			//display pixels to VGA
			if(row < 120 && row%2 && col < 160 && col%2){
				//write to VGA
				*vgaPtr = (pixel > threshold)? 0xFF : 0x00;;
				vgaPtr++;

				//update pixel information
				if(fd.maxBrightness < pixel)
					fd.maxBrightness = pixel;
				if(fd.minBrightness > pixel)
					fd.minBrightness = pixel;
			}
		}
	}
	return &fd;
}
