/**
 * ServoCMD Objects
 * @author Curt Henrichs
 * @date 3-31-17
 * Implementation of the servo control commands used in the command line.
 * Pan and tilt functionality exposed for the user to control the servos.
 */

#ifndef SERVOCMD_H_
#define SERVOCMD_H_

//=============================================================================
//                                  Libraries
//=============================================================================

#include "CommandType.h"

//=============================================================================
//                       Command Objects (Use Like Constant)
//=============================================================================

/**
 * Pan Servo Command Object
 */
extern command_t PAN;

/**
 * Tilt Servo Command Object
 */
extern command_t TILT;

#endif /* SERVOCMD_H_ */
