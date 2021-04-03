/**
 * CameraCMD Object
 * @author Curt Henrichs
 * @date 3-31-17
 * Command Line objects for the camera commands (read register, write register,
 * display image, and tracking target) Commands exposed are to be treated as
 * constants.
 */

#ifndef CAMERACMD_H_
#define CAMERACMD_H_

//=============================================================================
//                                  Libraries
//=============================================================================

#include "CommandType.h"

//=============================================================================
//                       Command Objects (Use Like Constant)
//=============================================================================

/**
 * Reads from the camera over I2C
 */
extern command_t RDCAMREG;
/**
 * Writes to the camera over I2C
 */
extern command_t WRCAMREG;
/**
 * Displays video feed from camera onto VGA display.
 */
extern command_t IMAGE;
/**
 * Tracks an object in view of camera with servo turret.
 */
extern command_t TRACK;

#endif /* CAMERACMD_H_ */
