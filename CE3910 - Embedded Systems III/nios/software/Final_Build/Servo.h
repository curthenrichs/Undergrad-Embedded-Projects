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

#ifndef SERVO_H_
#define SERVO_H_

//=============================================================================
//                       Constants and Macro Declaration
//=============================================================================

#define PAN_MAX (175) 	/** Maximum Pan in pixel cols */
#define PAN_MIN (0) 	/** Minimum Pan in pixel cols */
#define TILT_MAX (143) 	/** Maximum Tilt in pixel rows */
#define TILT_MIN (0) 	/** Minimum Tilt in pixel rows */

//=============================================================================
//                         Public Function Declaration
//=============================================================================

/**
 * Sets up the PWM to move the servos to center position
 */
void servo_init();
/**
 * Used to provide an absolute x-axis servo position. The number provided is to
 * be a number between 0 and 175. This corresponds to the horizontal resolution
 * of the camera. These values should cause a full range of travel from far
 * left to far right.
 * @param col the x tracking position
 * @return -1 if invalid input, else 0
 */
int servo_pan(int col);
/**
 * Used to provide absolute y-axis servo position. the number provided is to be
 * a number between 0 and 144. This corresponds to the vertical resolution for
 * the camera. These values should cause a full range of travel from top to
 * bottom.
 * @param row the y tracking position
 * @return -1 if invalid input, else 0
 */
int servo_tilt(int row);
/**
 * Gets the current pan location
 * @return absolute position of servo.
 */
int getServoX();
/**
 * Sets the relative pan location. Will stop at limit of range.
 * @param xpos position to add to current position.
 */
void setServoX(int xpos);
/**
 * Gets the current tilt location.
 * @return absolute position of servo.
 */
int getServoY();
/**
 * Sets the relative tilt location. Will stop at limit of range.
 * @param ypos position to add to current position.
 */
void setServoY(int ypos);

#endif /* SERVO_H_ */
