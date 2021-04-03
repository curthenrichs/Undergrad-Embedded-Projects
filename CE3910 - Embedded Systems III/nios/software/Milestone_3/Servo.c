/**
 * Servo Module
 * @author Curt Henrichs
 * @date 3-15-17
 *
 * Servo module is a pan/tilt abstraction for the camera base. The module will
 * rotate x and y by the limits specified. Absolute position control is used
 * with respect of the column and row position of the camera.
 *
 * Servo Motor Tuning
 * 	PAN SERVO:
 * 		MIN OCR 525
 * 		MAX OCR 2400
 * 		CTR OCR 1425
 *
 * 	TILT SERVO:
 * 		MIN OCR 1100
 * 		MAX OCR 2100
 * 		CTR OCR 1775
 *
 * Tuned approximate equations
 *  PAN_OCR = 107 * pos / 10 + 510
 *  TILT_OCR = 69 * pos / 10 + 1015
 */

//=============================================================================
//                                 Libraries
//=============================================================================

#include "Servo.h"
#include "Hardware/PWM.h"

//=============================================================================
//                       Constants and Macro Declaration
//=============================================================================

/**
 * Converts between pixel position to OCRA register
 * @param column position of pixel
 * @return OCR PWM value
 */
#define PAN_POS_TO_OCR_VALUE(pos) ((107 * pos)/10 + 510)
/**
 * Converts between pixel position to OCRB register
 * @param column position of pixel
 * @return OCR PWM value
 */
#define TILT_POS_TO_OCR_VALUE(pos) ((69 * pos)/10 + 1015)
/**
 * Converts between OCRA register value to pixel position
 * @param value is OCR register value
 * @return pixel position column
 */
#define OCR_VALUE_TO_PAN_POS(value) (((value - 510)*10)/107)
/**
 * Converts between OCRB register value to pixel position
 * @param value in OCR register value
 * @return pixel position row
 */
#define OCR_VALUE_TO_TILT_POS(value) (((value - 1015)*10)/69)
/**
 * Maximum step size of the relative pan position
 */
#define PAN_STEP 20
/**
 * Maximum step size of the relative tilt position
 */
#define TILT_STEP 20

//=============================================================================
//                         Public Function Implementation
//=============================================================================

/**
 * Sets up the PWM to move the servos to center position
 */
void servo_init(){
	servo_pan(PAN_MAX/2);
	servo_tilt(TILT_MAX/2+5);
}

/**
 * Used to provide an absolute x-axis servo position. The number provided is to
 * be a number between 0 and 175. This corresponds to the horizontal resolution
 * of the camera. These values should cause a full range of travel from far
 * left to far right.
 * @param col the x tracking position
 * @return -1 if invalid input, else 0
 */
int servo_pan(int col){
	if(col < PAN_MIN || col > PAN_MAX){
		return -1;
	}
	PWM_writeOCRA(PAN_POS_TO_OCR_VALUE(col));
	return 0;
}

/**
 * Used to provide absolute y-axis servo position. the number provided is to be
 * a number between 0 and 144. This corresponds to the vertical resolution for
 * the camera. These values should cause a full range of travel from top to
 * bottom.
 * @param row the y tracking position
 * @return -1 if invalid input, else 0
 */
int servo_tilt(int row){
	if(row < TILT_MIN || row > TILT_MAX){
		return -1;
	}
	PWM_writeOCRB(TILT_POS_TO_OCR_VALUE(row));
	return 0;
}

/**
 * Gets the current pan location
 * @return absolute position of servo.
 */
int getServoX(){
	return OCR_VALUE_TO_PAN_POS(PWM_readOCRA());
}

/**
 * Sets the relative pan location. Will stop at limit of range.
 * @param xpos position to add to current position.
 */
void setServoX(int xpos){
	xpos = (xpos > PAN_STEP)? PAN_STEP : (xpos < -PAN_STEP)? -PAN_STEP : xpos;
	int pos = getServoX() + xpos;
	pos = (pos > PAN_MAX) ? (PAN_MAX) : ((pos < PAN_MIN) ? PAN_MIN : pos);
	PWM_writeOCRA(PAN_POS_TO_OCR_VALUE(pos));
}

/**
 * Gets the current tilt location.
 * @return absolute position of servo.
 */
int getServoY(){
	return OCR_VALUE_TO_TILT_POS(PWM_readOCRB());
}

/**
 * Sets the relative tilt location. Will stop at limit of range.
 * @param ypos position to add to current position.
 */
void setServoY(int ypos){
	ypos = (ypos > TILT_STEP)? TILT_STEP : (ypos < -TILT_STEP)? -TILT_STEP : ypos;
	int pos = getServoY() + ypos;
	pos = (pos > TILT_MAX) ? (TILT_MAX) : ((pos < TILT_MIN) ? TILT_MIN : pos);
	PWM_writeOCRB(TILT_POS_TO_OCR_VALUE(pos));
}
